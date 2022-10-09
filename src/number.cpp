//
// Created by winnie泰 on 2022/10/6.
//

#include "iostream"
#include "vector"
#include "number.h"
#include<math.h>
#include <unordered_map>

using namespace std;
typedef unordered_map<string, number> fuZhi;
fuZhi MyFuZhi;
unordered_map<string, number>::iterator it;
number FuZhiNumber[3];
number finalResult;


number::number(bool positive, int decimals, vector<int> integers) {
    this->positive = positive;
    this->decimals = decimals;
    this->integers = integers;
}

bool number::convertNumToInts(number num, int cal[]) {
    cal = new int[1000];//这样对吗
    for (int i = 500 + num.decimals; i > 0; --i) {
        if (num.integers.empty()) {
            cal[0] = i + 1;
            break;
        }
        cal[i] = num.integers.back();
        num.integers.pop_back();
    }
    return true;
};

int beginIndexFinding(int ints[]) {
    for (int i = 999; i >= ints[0]; --i) {
        if (ints[i] != 0)return i;
    }
    return -1;
}

bool number::convertIntsToNum(number &num, int ans[]) {
    int beginIndex = -1;
    int stopIndex = ans[0];
    for (int i = 999; i > 0; --i) {
        if (ans[i] != 0) beginIndex = i;
        break;
    }
    if (beginIndex == -1)cerr << "convert mistake!" << endl;
    for (int i = stopIndex; i <= beginIndex; --i) {
        num.integers.push_back(ans[i]);
    }
}//注意检查，ans[0]是否都存入了有效index的开始

long long convertIntsToLongLong(int ints[]) {
    long long result = 0;
    int beginIndex = 0;
    for (int i = 999; i >= ints[0]; --i) {
        if (ints[i] != 0) {
            beginIndex = i;
            break;
        }
    }
    for (int i = ints[0]; i <= beginIndex; ++i) {
        result = result * 10 + ints[i];
    }//没有添加小数点奥
}

number number::add(number num1, number num2) {//可再优化，判断一下 要是数小，就只开100位的数组计算，这样子
    if (num1.integers.size() + abs(num1.decimals) > 499 || num2.integers.size() + abs(num2.decimals) > 499) {
        cerr << "invalid input for add , we only can provide numbers less than e500" << endl;
    }
    number result;
    if (num1.positive == num2.positive) {
        int cal1[1000] = {0};//cal【500】存个位；decimal若为正，意味着将小数点左移，
        int cal2[1000] = {0};//cal【500】存个位；decimal若为正，意味着将小数点左移，
        int ans[1000] = {0};
        for (int i = 500 + num1.decimals; i > 0; --i) {
            if (num1.integers.empty()) {
                cal1[0] = i + 1;
                break;
            }
            cal1[i] = num1.integers.back();
            num1.integers.pop_back();
        }
        for (int i = 500 + num2.decimals; i > 0; --i) {
            if (num2.integers.empty()) {
                cal2[0] = i + 1;//记录最小的一位有有效数字的index
                break;
            }
            cal2[i] = num2.integers.back();
            num2.integers.pop_back();
        }
        int beginIndex = 500 + std::max(num1.decimals, num2.decimals);
        int stopIndex = min(cal1[0], cal2[0]);
        for (int i = beginIndex; i >= stopIndex; --i) {
            ans[i] = cal1[i] + cal2[i];
            ans[i - 1] = ans[i] % 10;
            ans[i] = ans[i] / 10;
        }
        if (ans[stopIndex - 1] != 0)ans[0] = stopIndex - 1;
        else ans[0] = stopIndex;

        //数据已经都拿到了 开始复原给result
        result.positive = num1.positive;//因为两个同号的数相加

        for (int i = 999; i > 0; --i) {
            if (ans[i] != 0) {
                result.decimals = i - 500;
                break;
            }
        }
        for (int i = stopIndex; i <= beginIndex; ++i) {
            result.integers.push_back(ans[i]);
        }
        return result;

    } else {
        if (abs_com(num1, num2)) {//假如num1的绝对值大，那么式子的符号与num1相同
            num2.positive = !num2.positive;
            number temp = minus(num1, num2);
            temp.positive = num1.positive;
            return temp;
        } else {
            num1.positive = !num1.positive;
            number temp = minus(num1, num2);
            temp.positive = num2.positive;
            return temp;
        }
    }
}

number number::minus(number num1, number num2) {
    int ans[1000] = {0};
    if (num1.positive == num2.positive) {
        number result;
        bool flag = abs_com(num1, num2);
        if (equal(num1, num2)) {
            number temp;
            temp.positive = 1;
            temp.decimals = 0;
            temp.integers = vector<int>{0};
            return temp;
        }
        if (flag)result.positive = num1.positive;
        else result.positive = !num1.positive;
        int beginIndex = std::min(num1.integers[0], num2.integers[0]);
        int stopIndex = std::max(num1.decimals, num2.decimals) + 500;
        if (flag) {//namely num1 is larger than num2
            for (int i = stopIndex; i >= beginIndex; --i) {
                if (num1.integers[i] < 0) {
                    num1.integers[i] += 10;
                    num1.integers[i - 1]--;
                }
                int i1 = num1.integers[i];
                int i2 = num2.integers[i];
                num1.integers[i] = i1 - i2;
                if (num1.integers[i] < 0) {
                    num1.integers[i] += 10;
                    num1.integers[i - 1]--;
                }
            }
            for (int i = 999; i > 0; --i) {
                if (num1.integers[i] != 0) {
                    result.decimals = i - 500;
                    break;
                }
            }
            for (int i = 1; i < 999; ++i) {
                if (num1.integers[i] != 0) {
                    ans[0] = i;
                    break;
                }
            }
            for (int i = result.integers[0]; i <= result.decimals + 500; ++i) {
                ans[i] = num1.integers[i];
            }
            convertIntsToNum(result, ans);
            return result;
        } else {
            return minus(num2, num1);
        }

    } else {
        num2.positive = !num2.positive;
        number temp = add(num1, num2);
        temp.positive = num1.positive;
        return temp;
    }
}

number number::mul(number num1, number num2) {
    bool positiveness = num1.positive == num2.positive;
    number result;
    int ans[1000] = {0};
    result.positive = positiveness;
    for (int i = 500 + num1.decimals; i >= 500 - (num1.integers.size() - num1.decimals); --i) {
        for (int j = 500 + num2.decimals; j >= 500 - (num2.integers.size() - num2.decimals); --j) {
            ans[i + j - 500] = num1.integers[i] * num2.integers[j];
            ans[i + j - 501] = ans[i + j - 500] % 10;
            ans[i + j - 500] = ans[i + j - 500] / 10;
        }
    }
    for (int i = 500 - (num1.integers.size() - num1.decimals)
                 + (500 - (num2.integers.size() - num2.decimals)) - 5//多减几个 没事
            ; i < 1000; ++i) {
        if (ans[i] != 0) {
            ans[0] = i;
            break;
        }
    }
    int d = -1000;
    for (int i = 999; i >= ans[0]; --i) {
        if (ans[i] != 0) {
            d = i;
            break;
        }
    }
    if (d != -1000)result.decimals = d - 500;//注意decimal可能是负的；
    else cerr << "decimal mistake!" << endl;
    convertIntsToNum(result, ans);

    return result;
}

number number::div(number num1, number num2, vector<int> &answer) {
    number result;
//    if (num2.integers.size() - num2.decimals >= 10 || num2.integers.size() >= 12) {
//        cerr << "we can not div such a complex number";
//    }
    vector<int> vec = {0};
    int ans[1000] = {};
    number check(1, 0, vec);
    if (abs_com(check, num2)) {//如果num2的绝对值为0，就是非法输入
        cerr << "invalid input for div. we can not divide zero." << endl;
    }
    int ints1[1000] = {0};
    convertNumToInts(num1, ints1);
    int ints2[1000] = {0};
    convertNumToInts(num2, ints2);

//    long long divisor = 0;
    int beginIndexForNum2 = beginIndexFinding(ints2);
    int count = 1;
    int tempForNum2[1000] = {0};//第0位存有效数字位数，等于length，从第一位开始存有效数字
    int length = num2.integers.size();
    while (count < length) {
        tempForNum2[0]++;
        tempForNum2[count] = num2.integers[count];
        count++;
    }
    for (int i = ints1[0]; i <= 520; ++i) {
        bool flag = false;
        count = 1;
        for (int j = i; j < i + length; ++j) {

            if (ints1[j] > tempForNum2[count]) {
                flag = true;
                break;
            } else if (ints1[j] == tempForNum2[count]) {
                count++;
                continue;
            } else {
                flag = false;
                break;
            }
        }
        count = 1;
        if (!flag) {
            ints1[i + 1] = ints1[i] * 10;
            ints1[i] = 0;
        } else {
            while (flag) {
                count = 1;
                for (int j = i; j < i + length; ++j) {
                    if (ints1[j] > tempForNum2[count]) {
                        flag = true;
                        break;
                    } else if (ints1[j] == tempForNum2[count]) {
                        count++;
                        continue;
                    } else {
                        flag = false;
                        break;
                    }
                }
                count = 1;//judge 是否还大于该数
                if (!flag)break;
                for (int j = i; j < i + length; ++j) {

                    ints1[j] -= ints2[count];
                    if (ints1[j] < 0) {
                        ints1[j] = ints1[j - 1] * 10;
                        ints1[j - 1] = 0;
                    }
                }
                ans[i + length - 1]++;
            }
            if (i + length - 1 == 500) {
                //在算完最后一波后，把数字截出来
                for (int k = i; k < i + length; ++k) {
                    if (k == i) { if (ans[k] != 0)answer.push_back(ans[k]); }
                    else answer.push_back(ans[k]);
                }
            }//趁还没往后匀数；
            ints1[i + 1] = ints1[i] * 10;
            ints1[i] = 0;//结束这个i以后要把剩的换到下一位去
        }
    }

    for (int i = 1; i < 999; ++i) {
        if (ans[i] != 0)ans[0] = i;
    }
    for (int i = ans[0]; i <= 520; ++i) {
        result.integers.push_back(ans[i]);
    }
    result.decimals = 20;
//    for (int i = ints2[0] + 11; i >= ints2[0]; --i) {
//        if (ints2[0] != 0) {
//            beginIndex = i;
//            break;
//        }
//    }
//    for (int i = ints2[0]; i <= beginIndex; ++i) {
//        divisor = divisor * 10 + ints2[i];
//    }
//    int length = beginIndex - ints2[0];
//    int temp[1000] = {0};
//    for (int i = ints1[0]; i <= 515 - length; ++i) {//只计算到小数点后一定位数
//        string str = "";
//        for (int j = ints1[0]; j < i + length; ++j) {
//            str += (char) ints1[j];
//        }
//        bool flag = false;
//        long double dividend = 0;
//        for (int j = 0; j < str.length(); ++j) {
//            if (str[j] != 0)flag = true;
//            if (!flag)dividend = dividend * 10 + str[j];
//        }
//        if (dividend < divisor) { continue; }
//        else if (dividend == divisor) {
//            temp[i + length - 1] += 1;
//        } else //namely dividend>divisor
//        {
//            while (dividend >= divisor) {
//                dividend -= divisor;
//                temp[i + length - 1]++;
//            }
//        }
//    }
//    int beginIndexForTemp = 0;
//    int stopIndexForTemp = 0;
//    for (int i = 999; i > 0; --i) {
//        if (temp[i] != 0) {
//            beginIndexForTemp = i;
//            break;
//        }
//    }
//    bool flag = false;
//    for (int i = 1; i <= beginIndexForTemp; ++i) {
//        if (temp[i] != 0) {
//            temp[0] = i;
//            result.decimals = 500 - beginIndexForTemp;
//            flag = true;
//        }
//        if (flag) {
//            result.integers.push_back(temp[i]);
//        }
//    }
    bool positiveness = num1.positive == num2.positive;
    result.positive = positiveness;
    return result;
}

number number::mod(number num1, number num2) {
    number result;

    vector<int> vec = {0};
    number check(1, 0, vec);
    if (abs_com(check, num2)) {//如果num2的绝对值为0，就是非法输入
        cerr << "invalid input for mod. we can not mod zero." << endl;
    }
    if (num2.decimals > 0)cerr << "invalid input for mod, we can not mod a fraction" << endl;//这个方法只做检查，mod的结果去div里调
//    int ints1[1000];
//    convertNumToInts(num1, ints1);
//    int ints2[1000];
//    convertNumToInts(num2, ints2);
//    number num = div(num1, num2);

//    int ints[1000] = {};
//    convertNumToInts(num, ints);
//    int beginIndex = 0;
//    long double m = 0;
//    for (int i = 999; i >= ints[0]; --i) {
//        if (ints[i] != 0) {
//            beginIndex = i;
//            break;
//        }
//    }
//    for (int i = ints[0]; i <= beginIndex; ++i) {
//        m = m * 10 + ints[i];
//    }
//    if (beginIndex > 500) m /= (-500 + beginIndex);
//    if (beginIndex < 500) m *= (500 - beginIndex);
//
//    long long long1 = convertIntsToLongLong(ints1);
//    if (num1.decimals > 0)long1 /= num1.decimals;
//    if (num1.decimals < 0)long1 *= (-num1.decimals);
//    long long long2 = convertIntsToLongLong(ints2);
//    if (num2.decimals > 0)long2 /= num2.decimals;
//    if (num2.decimals < 0)long2 *= (-num2.decimals);
//    long double ans = long1 - long2 * m;
//    int count = 0;
//    long double ans_ = ans;
//
//
//    while (ans_ > 0) {
//        ans /= 10;
//        count++;
//    }
//    for (int i = 500 - count + 1; i < 999; ++i) {
//
//    }

    vector<int> answer;
    div(num1, num2, answer);
    result.integers = answer;
    result.decimals = 0;
    bool positiveness = num1.positive == num2.positive;
    result.positive = positiveness;
    return result;

}

number number::sin(number num1) {
    bool flag = true;
    number result;
    if (num1.integers.empty()) {
        cerr << "invalid input for sin" << endl;
    }
    number number6;
    number6.positive = true;
    number6.decimals = 0;
    number6.integers = vector<int>{6};

    number number120;
    number120.positive = true;
    number120.decimals = 0;
    number120.integers = vector<int>{1, 2, 0};

    number number5040;
    number5040.positive = true;
    number5040.decimals = 0;
    number5040.integers = vector<int>{5, 0, 4, 0};

    number number362880;
    number362880.positive = true;
    number362880.decimals = 0;
    number362880.integers = vector<int>{362880};


    number temp1 = result;
    number temp2 = mul(result, temp1);
    number temp3 = mul(temp2, temp1);
    number temp4 = mul(temp3, temp1);
    number temp5 = mul(temp4, temp1);
    number temp6 = mul(temp5, temp1);
    number temp7 = mul(temp6, temp1);
    number temp8 = mul(temp7, temp1);
    number temp9 = mul(temp8, temp1);
    vector<int> ints;

    result = minus(result, div(temp3, number6, ints));
    result = add(result, div(temp5, number120,
                             ints));
    result = minus(result, div(temp7, number5040,
                               ints));
    result = add(result, div(temp9, number362880,
                             ints));
    return result;

//    int ints[1000]={};
//    convertNumToInts(num1,ints);
//    int k = beginIndexFinding(ints);
//    long long integer_part= 0;
//    long long fraction_part = 0;
//    for (int i = ints[0]; i <=500 ; ++i) {
//        integer_part=integer_part*10+ints[i];
//        if (integer_part>10000000000000)cerr<<"we can not calculate such a huge number"<<endl;
//    }
//
//    for (int i =k; i >=501; --i) {
//        fraction_part=fraction_part*0.1+0.1*ints[i];
//    }
//    long double num = integer_part+fraction_part;
//    long double answer = sinl(num);
//    if (answer<0)result.positive=0;
//    else result.positive=1;
//    answer= abs(answer);
//    string str =to_string(answer);
//    result.decimals=6;//取6位小数
//    for (int i = 0; i < 500-ints[0]+7 ;++i) {
//        result.integers.push_back((str[i]-'0'));
//    }
    return result;
}

number number::cos(number num1) {
    number result;
    if (num1.integers.empty()) {
        cerr << "invalid input for cos" << endl;
    }
    int ints[1000] = {};
    convertNumToInts(num1, ints);
    int k = beginIndexFinding(ints);
    long long integer_part = 0;
    long long fraction_part = 0;
    for (int i = ints[0]; i <= 500; ++i) {
        integer_part = integer_part * 10 + ints[i];
        if (integer_part > 10000000000000)cerr << "we can not calculate such a huge number" << endl;
    }

    for (int i = k; i >= 501; --i) {
        fraction_part = fraction_part * 0.1 + 0.1 * ints[i];
    }
    long double num = integer_part + fraction_part;
    long double answer = cosl(num);
    if (answer < 0)result.positive = 0;
    else result.positive = 1;
    answer = abs(answer);
    string str = to_string(answer);
    result.decimals = 6;//取6位小数
    for (int i = 0; i < 500 - ints[0] + 7; ++i) {
        result.integers.push_back((str[i] - '0'));
    }
}

number number::tan(number num1) {
    if (num1.integers.empty()) {
        cerr << "invalid input for tan" << endl;
    }
    number number1 = sin(num1);
    number number2 = cos(num1);
    if (number2.integers.size() == 0) { cerr << "invalid input for tan ,since cos equals zero" << endl; }
    else {
        vector<int> ints;
        return div(number1, number2, ints);
    }
}


number number::sqrt(number num1) {
    number result;
    if (num1.integers.empty()) {
        cerr << "invalid input for cos" << endl;
    }
    int ints[1000] = {};
    convertNumToInts(num1, ints);
    int k = beginIndexFinding(ints);
    long long integer_part = 0;
    long long fraction_part = 0;
    for (int i = ints[0]; i <= 500; ++i) {
        integer_part = integer_part * 10 + ints[i];
        if (integer_part > 10000000000000)cerr << "we can not calculate such a huge number" << endl;
    }

    for (int i = k; i >= 501; --i) {
        fraction_part = fraction_part * 0.1 + 0.1 * ints[i];
    }
    long double num = integer_part + fraction_part;
    long double answer = sqrtl(num);
    if (answer < 0)result.positive = 0;
    else result.positive = 1;
    answer = abs(answer);
    string str = to_string(answer);
    result.decimals = 6;//取6位小数
    for (int i = 0; i < 500 - ints[0] + 7; ++i) {
        result.integers.push_back((str[i] - '0'));
    }
}

//number number::pow(number num1, number num2)//幂运算
//{
//    number result;
//    if (num1.integers.empty()||num2.integers.empty()) {
//        cerr << "invalid input for cos" << endl;
//    }
//    int ints1[1000] = {};
//    convertNumToInts(num1, ints1);
//    int k1 = beginIndexFinding(ints1);
//    long long integer_part1 = 0;
//    long long fraction_part1 = 0;
//    for (int i = ints1[0]; i <= 500; ++i) {
//        integer_part1 = integer_part1 * 10 + ints1[i];
//        if (integer_part1 > 10000000000000)cerr << "we can not calculate such a huge number" << endl;
//    }
//
//    for (int i = k1; i >= 501; --i) {
//        fraction_part1 = fraction_part1 * 0.1 + 0.1 * ints1[i];
//    }int ints2[1000] = {};
//    convertNumToInts(num2, ints2);
//    int k2 = beginIndexFinding(ints2);
//    long long integer_part2 = 0;
//    long long fraction_part2 = 0;
//    for (int i = ints2[0]; i <= 500; ++i) {
//        integer_part2 = integer_part2 * 10 + ints2[i];
//        if (integer_part2 > 10000000000000)cerr << "we can not calculate such a huge number" << endl;
//    }
//
//    for (int i = k1; i >= 501; --i) {
//        fraction_part2 = fraction_part2 * 0.1 + 0.1 * ints2[i];
//    }
//
//    long double answer = powl(integer_part1 + fraction_part1,integer_part2 + fraction_part2);
//    if (answer < 0)result.positive = 0;
//    else result.positive = 1;
//    answer = abs(answer);
//    string str = to_string(answer);
//    result.decimals = 6;//取6位小数
//    for (int i = 0; i < 500 - ints[0] + 7; ++i) {
//        result.integers.push_back((str[i] - '0'));
//    }
//}

bool number::equal(number num1, number num2) {//==
    if (!larger(num1, num2) && !smaller(num1, num2))return true;
    return false;
}

number number::exp(number num1) {
    number result;
    if (num1.integers.empty()) {
        cerr << "invalid input for cos" << endl;
    }
    int ints[1000] = {};
    convertNumToInts(num1, ints);
    int k = beginIndexFinding(ints);
    long long integer_part = 0;
    long long fraction_part = 0;
    for (int i = ints[0]; i <= 500; ++i) {
        integer_part = integer_part * 10 + ints[i];
        if (integer_part > 10000000000000)cerr << "we can not calculate such a huge number" << endl;
    }

    for (int i = k; i >= 501; --i) {
        fraction_part = fraction_part * 0.1 + 0.1 * ints[i];
    }
    long double num = integer_part + fraction_part;
    long double answer = expl(num);
    if (answer < 0)result.positive = 0;
    else result.positive = 1;
    answer = abs(answer);
    string str = to_string(answer);
    result.decimals = 6;//取6位小数
    for (int i = 0; i < 500 - ints[0] + 7; ++i) {
        result.integers.push_back((str[i] - '0'));
    }
}

number number::ln(number num1) {
    vector<int> vec = {0};
    number check(1, 0, vec);
    if (abs_com(check, num1) || !num1.positive) {
        cerr << "invalid input for ln. we can not ln non-positive number." << endl;
    }
    number result;
    if (num1.integers.empty()) {
        cerr << "invalid input for cos" << endl;
    }
    int ints[1000] = {};
    convertNumToInts(num1, ints);
    int k = beginIndexFinding(ints);
    long long integer_part = 0;
    long long fraction_part = 0;
    for (int i = ints[0]; i <= 500; ++i) {
        integer_part = integer_part * 10 + ints[i];
        if (integer_part > 10000000000000)cerr << "we can not calculate such a huge number" << endl;
    }

    for (int i = k; i >= 501; --i) {
        fraction_part = fraction_part * 0.1 + 0.1 * ints[i];
    }
    long double num = integer_part + fraction_part;
    long double answer = logl(num);
    if (answer < 0)result.positive = 0;
    else result.positive = 1;
    answer = abs(answer);
    string str = to_string(answer);
    result.decimals = 6;//取6位小数
    for (int i = 0; i < 500 - ints[0] + 7; ++i) {
        result.integers.push_back((str[i] - '0'));
    }

}

bool number::larger(number num1, number num2) {
    if (num1.positive && !num2.positive)return true;
    if (!num1.positive && num2.positive)return false;
    if (num1.positive && num2.positive)return abs_com(num1, num2);
    if (!num1.positive && !num2.positive)return !abs_com(num1, num2);
}

bool number::smaller(number num1, number num2) {
    return larger(num2, num1);
}

bool number::abs_com(number num1, number num2) {
    int cal1[1000];
    number::convertNumToInts(num1, cal1);
    int cal2[1000];
    number::convertNumToInts(num2, cal2);
    if (cal1[0] > cal2[0])return true;
    if (cal1[0] < cal2[0])return false;
    int beginIndex = cal1[0];
    for (int i = beginIndex; i <= 999; ++i) {
        if (cal1[i] > cal2[i])return true;
        else if (cal1[i] < cal2[i])return false;
    }
    return false;//此处认为两个数相等，是返回false的
}

bool number::convertStringToNum(string str, number &num) {
    if (str[0] == '-') {
        num.positive = 0;
        for (int i = 1; i < str.length(); ++i) {
            if (str[i] != '.')num.decimals = str.length() - i - 1;
            else num.integers.push_back(str[i]);
        }
    } else {
        num.positive = 1;
        for (int i = 0; i < str.length(); ++i) {
            if (str[i] != '.')num.decimals = str.length() - i - 1;
            else num.integers.push_back(str[i]);
        }
    }
    return true;
}

number number::calculate(string input) {
    number result;

    vector<number> numbers = {};
    vector<char> operators = {};
    bool flagForMinus = false;
    int flagForEqual[2] = {0, 0};
    int leftBracket = 0;
    int rightBracket = 0;
    for (int i = 0; i < input.length(); ++i) {
        if (input[i] == '(')leftBracket++;
        if (input[i] == ')')rightBracket++;
        if (input[i] == '=') {
            flagForEqual[0]++;
            flagForEqual[1] = i;
        }
    }
    if (leftBracket != rightBracket || flagForEqual[0] > 1)cerr << "invalid input";

    if (flagForEqual == 0) {
        result= number::calculate1(input);
    } else//namely there is an equal sign
    {
        number::calculate2(input);
    }

//    pBoolean=&flagForMinus;
//    numbers_temp=numbers;
//    operators_temp=operators;
    return result;
}

number number::vectorCalculate(vector<number>, vector<char>) {
//

}

number number::calculate1(string input) {//
    number temp_result;
    vector<number> numbers = {};
    vector<char> operators = {};
    bool flagForMinus = false;
    int flagForEqual[2] = {0, 0};
    int leftBracket = 0;
    int rightBracket = 0;
    for (int i = 0; i < input.length(); ++i) {
        if (input[i] == '(')leftBracket++;
        if (input[i] == ')')rightBracket++;
        if (input[i] == '=') {
            flagForEqual[0]++;
            flagForEqual[1] = i;
        }
    }
    if (leftBracket != rightBracket || flagForEqual[0] > 1)cerr << "invalid input";
    for (int i = 0; i < input.length(); ++i) {
        string temp;
        char a = input[i];
        if ('0' <= a && a <= '9') {
            if (flagForMinus) {
                temp += '-';
                flagForMinus = false;
            }
            temp += a;
            while (true) {
                i++;
                if ('0' <= input[i] && input[i] <= '9' || input[i] == '.') {
                    temp += input[i];

                } else {
                    i--;
                    break;
                }
            }
            number num;
            number::convertStringToNum(temp, num);
            numbers.push_back(num);
        } else if (a == '-') {
            if (i == 0 || input[i - 1] == '(') {
                flagForMinus = true;
            } else operators.push_back(a);
        } else if (a == '+' || a == '*' || a == '/' || a == '%') {
            operators.push_back(a);
        } else if (a == 's') {
            if (input[i + 1] == 'i' && input[i + 2] == 'n' && input[i + 3] == '(')
                operators.push_back('s');//就以s记为sin
            else if (input[i + 1] == 'q' && input[i + 2] == 'r' && input[i + 3] == 't' && input[i + 3] == '(')
                operators.push_back('S');//就以S记为sqrt
            else { cerr << "invalid input"; }
        } else if (a == 'c') {
            if (input[i + 1] == 'o' && input[i + 2] == 's' && input[i + 3] == '(')
                operators.push_back('c');//就以c记为cos
            else { cerr << "invalid input"; }
        } else if (a == 't') {
            if (input[i + 1] == 'a' && input[i + 2] == 'n' && input[i + 3] == '(')
                operators.push_back('t');//就以t记为tan
            else { cerr << "invalid input"; }
        } else if (a == 'e') {
            if (input[i + 1] == 'x' && input[i + 2] == 'p' && input[i + 3] == '(')
                operators.push_back('e');//就以e记为exp
            else { cerr << "invalid input"; }
        } else if (a == 'l') {
            if (input[i + 1] == 'n' && input[i + 2] == '(')
                operators.push_back('l');//就以l记为ln
            else { cerr << "invalid input"; }
        } else if (a == 'x') {
            numbers.push_back(FuZhiNumber[0]);
        } else if (a == 'y') {
            numbers.push_back(FuZhiNumber[1]);
        } else if (a == 'z') {
            numbers.push_back(FuZhiNumber[2]);
        } else { cerr << "invalid input"; }
    }
    return temp_result;
}

void number::calculate2(string input) {//计算含等号的赋值式

    number temp_result;
    vector<number> numbers = {};
    vector<char> operators = {};
    bool flagForMinus = false;
    int flagForEqual[2] = {0, 0};
    int leftBracket = 0;
    int rightBracket = 0;
    for (int i = 0; i < input.length(); ++i) {
        if (input[i] == '(')leftBracket++;
        if (input[i] == ')')rightBracket++;
        if (input[i] == '=') {
            flagForEqual[0]++;
            flagForEqual[1] = i;
        }
    }
    //we only allow use "x,y,z" as a variable
    string str1;
    string str2;
    for (int i = 0; i < flagForEqual[1] - 1; ++i) {
        str1 += input[i];
    }
    for (int i = flagForEqual[1]; i < input.length(); ++i) {
        str2 += input[i];
    }
    if (input[0] == 'x') {
        FuZhiNumber[0] = calculate1(str2);
    }
    else if (input[0] == 'y') {
        FuZhiNumber[1] = calculate1(str2);
    }
    else if (input[0] == 'z') {
        FuZhiNumber[2] = calculate1(str2);
    }
}

