//
// Created by ROG on 2024/5/27.
//

#ifndef WX_SOBER_OPERATOR_MODIFIEDJONKERVOLGENANT_H
#define WX_SOBER_OPERATOR_MODIFIEDJONKERVOLGENANT_H

#include "vector"

using namespace std;


class ModifiedJonkerVolgenant {
private:
    int *col4row;  // row是索引，col是索引对应的值，且值不为-1
    int *row4col;  // col是索引，row是索引对应的值，且值不为-1
    double *v;  // row的对偶变量
    double *u;  // col的对偶变量
    double gain;
    vector<vector<double>> cost;  // 成本
    bool maximize;  // 最大化
public:
    ModifiedJonkerVolgenant();

    ModifiedJonkerVolgenant(vector<vector<double>> &cost);

    ModifiedJonkerVolgenant(vector<vector<double>> &cost, bool maximize);

    void assign();

    ~ModifiedJonkerVolgenant();
};


#endif //WX_SOBER_OPERATOR_MODIFIEDJONKERVOLGENANT_H
