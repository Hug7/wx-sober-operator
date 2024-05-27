//
// Created by ROG on 2024/5/27.
//

#include "ModifiedJonkerVolgenant.h"


ModifiedJonkerVolgenant::ModifiedJonkerVolgenant(vector<vector<double>> cost, bool maximize) {
    this->cost = cost;
    this->maximize = maximize;
    this->gain = 0;
    this->col4row = new int[cost[0].size()];
    this->row4col = new int[cost.size()];
    this->u = new double[cost[0].size()];
    this->v = new double[cost.size()];
}

ModifiedJonkerVolgenant::ModifiedJonkerVolgenant(vector<vector<double>> cost) {
    ModifiedJonkerVolgenant(cost, true);
}