
#include "iostream"
#include "algorithm"
#include "number.cpp"
#include<string>
//#include "omp.h"
using namespace std;
//#pragma omp parallel for



int main() {
    cout << R"(please enter your equation, type "-h" for help and restriction and "quit" to quit )" << endl;
    while (true) {
        string input;
        getline(cin, input);
        bool help = false;
        if (input == "-h") {
            cout << "The calculator can calculate operators as follows: + - * / sin() cos() tan() sqrt() exp() ln()\n"
                    "we allow use functions to calculate number whose absolute value is smaller than e13.\n"
                    "you are allowed to give value to variable using name as x,y,z.\n"
                    "Remark: pay attention to not divide zero or tan(0),which will be detected by the program." << endl;
            help = true;
        } else if (input == "quit") {
            cout << "Program over" << endl;
            break;
        }
        if (help)continue;
        bool flag = false;
        for (int i = 0; i < input.size(); ++i) {
            if (input[i] == '=') {
                flag = true;//赋值语句
                number::calculate(input);
                break;
            }
        }
        number ans;
        if (!flag) {
            ans = number::calculate(input);
        }
        if (!ans.positive)cout << '-';
        if (ans.decimals <= 0) {
            int temp = abs(ans.decimals);
            for (int i = 0; i < temp; ++i) {
                ans.integers.push_back(0);
            }
            for (int i = 0; i < ans.integers.size(); ++i) {
                cout << ans.integers[i];
            }
        } else {
            if (ans.decimals <= 15) {
                int index = ans.integers.size() - ans.decimals;
                for (int i = 0; i < index; ++i) {
                    cout << ans.integers[i];
                }
                if (index <= 0)cout << "0";
                cout << ".";
                for (int i = index; i < ans.integers.size(); ++i) {
                    cout << ans.integers[i];
                }
            } else {
                int index = ans.integers.size() - 15;
                for (int i = 0; i < index; ++i) {
                    cout << ans.integers[i];
                }
                cout << ".";
                for (int i = index; i < index + 15; ++i) {
                    cout << ans.integers[i];
                }
            }
        }
        cout<<endl;
    }
}
