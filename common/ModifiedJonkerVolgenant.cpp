//
// Created by ROG on 2024/5/27.
//

#include "ModifiedJonkerVolgenant.h"
#include "cstdio"
using namespace std;


ModifiedJonkerVolgenant::ModifiedJonkerVolgenant(vector<vector<double>> &cost, bool maximize) {
    this->cost = cost;
    this->maximize = maximize;
    this->gain = 0;
    this->col4row = new int[cost[0].size()];
    this->row4col = new int[cost.size()];
    this->u = new double[cost[0].size()];
    this->v = new double[cost.size()];
}

ModifiedJonkerVolgenant::ModifiedJonkerVolgenant(vector<vector<double>> &cost) {
    ModifiedJonkerVolgenant(cost, true);
}

ModifiedJonkerVolgenant::ModifiedJonkerVolgenant() {
}

void ModifiedJonkerVolgenant::assign() {
    printf("assign");
}

ModifiedJonkerVolgenant::~ModifiedJonkerVolgenant() {
    delete[] this->row4col;
    delete[] this->col4row;
    delete[] this->u;
    delete[] this->v;
    this->row4col = nullptr;
    this->col4row = nullptr;
    this->u = nullptr;
    this->v = nullptr;

}