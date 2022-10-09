//
// Created by winnie泰 on 2022/10/6.
//
#include "iostream"
#include "vector"
#include "stdio.h"
#include "algorithm"
#include "number.cpp"
#include "map"
//#include "omp.h"
using namespace std;
//#pragma omp parallel for



int main() {
    string input;
    scanf("%s", &input);
    number ans = number::calculate(input);
    if (ans.decimals <= 0) {
        int temp = abs(ans.decimals);
        for (int i = 0; i < temp; ++i) {
            ans.integers.push_back(0);
        }
        for (int i = 0; i < ans.integers.size(); ++i) {
            cout << ans.integers[i];
        }
    } else {
        if (ans.decimals <= 6) {
            int index = ans.integers.size() - ans.decimals;
            for (int i = 0; i < index; ++i) {
                cout << ans.integers[i];
            }
            cout << ".";
            for (int i = index; i < ans.integers.size(); ++i) {
                cout << ans.integers[i];
            }
        } else {
            int index = ans.integers.size() - 6;
            for (int i = 0; i < index; ++i) {
                cout << ans.integers[i];
            }
            cout << ".";
            for (int i = index; i < index + 6; ++i) {
                cout << ans.integers[i];
            }
        }
    }
}
