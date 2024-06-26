//
// Created by lq on 2024/5/27.
//
#include <cstdio>
#include "common/assign/ModifiedJonkerVolgenant.cpp"
#include "vector"

using namespace std;

int main() {

    printf("你好xx \n");
//    vector<vector<double>> *cost = new vector<vector<double>>;
    int rowCount = 3;
    int colCount = 3;
    vector<vector<double>> cost;
    cost.resize(rowCount);
    for (auto &item: cost) {
        item.resize(colCount);
    }
    for (int u = 0; u < rowCount; ++u) {
        for (int m = 0; m < colCount; ++m) {
            cost[u][m] = (u + 1) * (m + 1);
        }
    }


    ModifiedJonkerVolgenant *modifiedJonkerVolgenant = new ModifiedJonkerVolgenant(cost, true);
//    ModifiedJonkerVolgenant * modifiedJonkerVolgenant = new ModifiedJonkerVolgenant();
    modifiedJonkerVolgenant->assign();
    printf("cost = %f", modifiedJonkerVolgenant->getGain());
    return 0;
}