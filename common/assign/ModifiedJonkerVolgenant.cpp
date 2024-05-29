#include "ModifiedJonkerVolgenant.h"
#include "cstdio"
#include "memory"
#include "limits"
#include "algorithm"

using namespace std;


ModifiedJonkerVolgenant::ModifiedJonkerVolgenant(vector<vector<double>> &inputCost, bool maximize) {
    this->rowCount = inputCost.size();
    this->colCount = inputCost[0].size();
    // 如果列数大于行数，则行列转换
    if (this->colCount > this->rowCount) {
        this->didFlip = true;
        this->cost.resize(this->colCount);
        for (int u = 0; u < this->colCount; ++u) {
            this->cost[u].resize(this->rowCount);
        }
        for (int u = 0; u < this->rowCount; ++u) {
            for (int m = 0; m < this->colCount; ++m) {
                this->cost[m][u] = inputCost[u][m];
            }
        }
        swap(this->rowCount, this->colCount);
    } else {
        this->didFlip = false;
        this->cost.resize(this->rowCount);
        for (int u = 0; u < this->rowCount; ++u) {
            this->cost[u].resize(this->colCount);
        }
        for (int u = 0; u < this->rowCount; ++u) {
            for (int m = 0; m < this->colCount; ++m) {
                this->cost[u][m] = inputCost[u][m];
            }
        }
    }
    this->maximize = maximize;
    this->gain = 0;
    this->col4row = new int[this->rowCount];
    this->row4col = new int[this->colCount];
    // 初始化行列索引
    fill(this->col4row, this->col4row + this->rowCount, -1);
    fill(this->row4col, this->row4col + this->colCount, -1);

    this->colDual = new double[this->colCount];
    this->rowDual = new double[this->rowCount];
}

ModifiedJonkerVolgenant::ModifiedJonkerVolgenant(vector<vector<double>> &cost) {
    ModifiedJonkerVolgenant(cost, true);
}

ModifiedJonkerVolgenant::~ModifiedJonkerVolgenant() {
    delete[] this->row4col;
    delete[] this->col4row;
    delete[] this->colDual;
    delete[] this->rowDual;
    this->row4col = nullptr;
    this->col4row = nullptr;
    this->colDual = nullptr;
    this->rowDual = nullptr;
}

void ModifiedJonkerVolgenant::assign() {
    if (this->maximize) {
        printf("maximize \n");
    } else {
        printf("minimize \n");
    }
    // 指派算法的 cost 必须全部为正数，修正成本
    double costShift = this->amendCost();
    //
    for (int curUnassignedCol = 0; curUnassignedCol < this->colCount; ++curUnassignedCol) {
        // 找到从k开始的最短增广路径，并返回路径中的最后一个节点
        shared_ptr<int> pred(new int[this->rowCount]);
        int sink = shortestPath(curUnassignedCol, pred);
        // 如果问题不可行，标记并返回
        if (sink == -1) {
            this->col4row = nullptr;
            this->row4col = nullptr;
            this->gain = -1;
            return;
        }
        int m = sink;
        while (true) {
            int u = pred.get()[m];
            this->col4row[m] = u;
            int v = this->row4col[u];
            this->row4col[u] = m;
            m = v;
            if (u == curUnassignedCol) {
                break;
            }
        }
    }
    // Calculate the gain that should be returned.
    this->gain = 0;
    for (int curCol = 0; curCol < this->colCount; ++curCol) {
        this->gain += this->cost[this->row4col[curCol]][curCol];
    }
    // Adjust the gain for the initial offset of the cost matrix.
    if (this->maximize) {
        this->gain = -this->gain + costShift * this->colCount;
    } else {
        this->gain = this->gain + costShift * this->colCount;
    }
}

double ModifiedJonkerVolgenant::amendCost() {
    double costShift = 0; // 成本偏移量
    if (this->maximize) {
        costShift = -numeric_limits<double>::max();
        for (int u = 0; u < this->rowCount; u++) {
            for (int m = 0; m < this->colCount; m++) {
                if (this->cost[u][m] > costShift) {
                    costShift = this->cost[u][m];
                }
            }
        }
        for (int u = 0; u < this->rowCount; u++) {
            for (int m = 0; m < this->colCount; m++) {
                this->cost[u][m] = -this->cost[u][m] + costShift;
            }
        }
    } else {
        costShift = numeric_limits<double>::max();
        for (int u = 0; u < this->rowCount; u++) {
            for (int m = 0; m < this->colCount; m++) {
                if (this->cost[u][m] < costShift) {
                    costShift = this->cost[u][m];
                }
            }
        }
        for (int u = 0; u < this->rowCount; u++) {
            for (int m = 0; m < this->colCount; m++) {
                this->cost[u][m] = this->cost[u][m] - costShift;
            }
        }
    }
    return costShift;
}

int ModifiedJonkerVolgenant::shortestPath(int curUnassignedCol, shared_ptr<int> &pred) {
    // 初始化，没有行、列被扫描
    // 这将在已扫描的每一 列 中存储一个1
    unique_ptr<bool> scannedCols(new bool[this->colCount]);
    // 这将在已扫描的每一 行 中存储一个1
    unique_ptr<bool> scannedRows(new bool[this->rowCount]);
    // 待扫描的列
    unique_ptr<int> row2Scan(new int[this->rowCount]);
    for (int u = 0; u < this->rowCount; ++u) {
        row2Scan.get()[u] = u;
    }
    int sink = -1;
    double delta = 0.;
    int curCol = curUnassignedCol;
    unique_ptr<double> shortestPathCost(new double[this->rowCount]);
    fill(shortestPathCost.get(), shortestPathCost.get() + this->rowCount, numeric_limits<double>::max());

    while (sink == -1) {
        // 标记当前 行 被访问
        scannedCols.get()[curCol] = true;
        // 扫描所有没有被扫描的列
        double minVal = numeric_limits<double>::min();
        int closestRowScan = -1;
        for (int curRowScan = 0; curRowScan < this->rowCount; ++curRowScan) {
            int curRow = row2Scan.get()[curRowScan];
            if (curRow == -1) {
                continue;
            }
            double reducedCost = delta + this->cost[curRow][curCol] - this->colDual[curCol] - this->rowDual[curRow];
            if (reducedCost < shortestPathCost.get()[curRow]) {
                pred.get()[curRow] = curCol;
                shortestPathCost.get()[curRow] = reducedCost;
            }
            // 找到扫描的最小未分配列
            if (shortestPathCost.get()[curRow] < minVal) {
                minVal = shortestPathCost.get()[curRow];
                closestRowScan = curRowScan;
            }
        }
        if (minVal >= numeric_limits<double>::max()) {
            // 如果最小成本列不是有限的，这个问题无解
            return 0;
        }
        int closestRow = row2Scan.get()[closestRowScan];
        // 将该列添加到扫描列的列表中，并从要扫描的列的列表中删除该列
        scannedRows.get()[closestRow] = true;
        row2Scan.get()[closestRowScan] = -1;
        delta = shortestPathCost.get()[closestRow];
        // 如果我们到达了一个未分配的行
        if (this->col4row[closestRow] == -1) {
            sink = closestRow;
        } else {
            curCol = this->col4row[closestRow];
        }
    }
    // 更新 dual
    // 在增广路径中更新一行
    this->colDual[curUnassignedCol] += delta;

    for (int u = 0; u < this->colCount; ++u) {
        // 在增广路径中更新 行 的松弛
        if (scannedCols.get()[u] && u != curUnassignedCol) {
            this->colDual[u] += delta - shortestPathCost.get()[this->row4col[u]];
        }
    }
    for (int u = 0; u < this->rowCount; ++u) {
        // 在增广路径中更新 列 的松弛
        if (scannedRows.get()[u]) {
            this->rowDual[u] += -delta + shortestPathCost.get()[u];
        }
    }

    return -1;
}

int *ModifiedJonkerVolgenant::getCol4Row() const {
    if (this->didFlip) {
        return this->row4col;
    }
    return this->col4row;
}

int *ModifiedJonkerVolgenant::getRow4Col() const {
    if (this->didFlip) {
        return this->col4row;
    }
    return this->row4col;
}

double *ModifiedJonkerVolgenant::getRowDual() const {
    if (this->didFlip) {
        return this->colDual;
    }
    return this->rowDual;
}

double *ModifiedJonkerVolgenant::getColDual() const {
    if (this->didFlip) {
        return this->rowDual;
    }
    return this->colDual;
}

double ModifiedJonkerVolgenant::getGain() const {
    return this->gain;
}
