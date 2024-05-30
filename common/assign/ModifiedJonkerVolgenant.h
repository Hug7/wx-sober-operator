#ifndef WX_SOBER_OPERATOR_MODIFIEDJONKERVOLGENANT_H
#define WX_SOBER_OPERATOR_MODIFIEDJONKERVOLGENANT_H

#include "vector"
#include "memory"

using namespace std;


class ModifiedJonkerVolgenant {
private:
    unsigned int rowCount;  // 行数
    unsigned int colCount;  // 列数
    int *col4row;  // row是索引，col是索引对应的值，且值不为-1
    int *row4col;  // col是索引，row是索引对应的值，且值不为-1
    double *rowDual;  // row的对偶变量
    double *colDual;  // col的对偶变量
    double gain;
    vector<vector<double>> cost;  // 成本矩阵
    bool maximize;  // 最大化
    bool didFlip;  // 是否行列转换
public:
    ModifiedJonkerVolgenant(vector<vector<double>> &cost);

    ModifiedJonkerVolgenant(vector<vector<double>> &cost, bool maximize);

    // 指派
    void assign();

    int *getCol4Row() const;

    int *getRow4Col() const;

    double *getRowDual() const;

    double *getColDual() const;

    double getGain() const;

    ~ModifiedJonkerVolgenant();

private:
    // 修正成本，保证成本>=0(target=min)
    double amendCost();

    // 增广路径
    int shortestPath(int curUnassignedCol, shared_ptr<int[]> &pred);
};


#endif //WX_SOBER_OPERATOR_MODIFIEDJONKERVOLGENANT_H
