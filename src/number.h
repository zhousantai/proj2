//
// Created by winnie泰 on 2022/10/7.
//

#ifndef PROJ2_NUMBER_H
#define PROJ2_NUMBER_H

#endif //PROJ2_NUMBER_H


#include "iostream"

using namespace std;

class number {
//private:
//    bool positive = true;
//    int decimals;
//    vector<int *> integers;

public:
    number() {
        this->positive = 1;
        this->decimals = 0;
        this->integers = vector<int>{0};


    }

    bool positive = true;
    int decimals;
    vector<int>integers;

    number(bool positive, int decimals, vector<int> integers);

    number static add(number num1, number num2);

    number static minus(number num1, number num2);

    number static mul(number num1, number num2);

    number static div(number num1, number num2,vector<int>& answer);

    number static mod(number num1, number num2);

    number static sin(number num1);

    number static cos(number num1);

    number static tan(number num1);

    number static sqrt(number num1);


    bool static equal(number num1, number num2);

    number static exp(number num1);

    number static ln(number num1);

    bool static larger(number num1, number num2);

    bool static smaller(number num1, number num2);

    bool static abs_com(number num1, number num2);

    bool static convertNumToInts(number num, int cal[]);

    bool static convertIntsToNum(number &num, int ans[]);

    bool static convertStringToNum(string str,number& number);

    static number calculate(string input);
    static number calculate1(string input);
    static void calculate2(string input);

    static number vectorCalculate(vector<number>,vector<char>);

//    bool getPositive() {
//        return positive;
//    }
//
//    bool isPositive() const {
//        return positive;
//    }
//
//    void setPositive(bool positive) {
//        number::positive = positive;
//    }
//
//    int getDecimals() const {
//        return decimals;
//    }
//
//    void setDecimals(int decimals) {
//        number::decimals = decimals;
//    }
//
//    const vector<int *> &getIntegers() const {
//        return integers;
//    }
//
//    void setIntegers(const vector<int *> &integers) {
//        number::integers = integers;
//    }
//
//    virtual ~number() {
//        delete integers[integers.size()];
//    }
};