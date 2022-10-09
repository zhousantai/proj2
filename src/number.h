//
// Created by winnie泰 on 2022/10/7.
//

#ifndef PROJ2_NUMBER_H
#define PROJ2_NUMBER_H

#endif //PROJ2_NUMBER_H


#include "iostream"

using namespace std;

class number {

public:
    number() {
        this->positive = 1;
        this->decimals = 0;
        this->integers = vector<int>();
    }

    bool positive = true;
    int decimals;
    vector<int> integers;

    number(bool positive, int decimals, vector<int> integers);

    number static add(number num1, number num2);

    static number function(const number &num1, char a);

    number static minus(const number num1, number num2, int i);

    number static mul(const number &num1, const number &num2);

    number static div(const number &num1, number num2);

    number static sin(const number &num1);

    number static cos(const number &num1);

    number static tan(const number &num1);

    number static sqrt(const number &num1);

    static void clear(vector<number> &numbers, vector<char> &operators, char a);

    bool static equal(const number &num1, number num2);

    number static exp(const number &num1);

    number static ln(const number &num1);

    bool static larger(const number &num1, number num2);

    bool static smaller(const number &num1, const number &num2);

    bool static abs_com(const number &num1, const number &num2);

    bool static convertNumToInts(number num, int cal[]);

    bool static convertIntsToNum(number &num, int ans[]);

    bool static convertStringToNum(string str, number &number);

    static number calculate(string input);

    static number calculate1(string input);

    static void calculate2(string input);

    static bool vectorCalculate(vector<number> &, vector<char> &, int i);

    static number tempMinus(const number &num1, const number &num2);

};