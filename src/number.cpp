//
// Created by winnie泰 on 2022/10/6.
//

#include "iostream"
#include "vector"
#include "number.h"
#include<cmath>
#include <unordered_map>
#include <iomanip>
#include <utility>
#include "sstream"

using namespace std;
typedef unordered_map<string, number> fuZhi;
fuZhi MyFuZhi;
unordered_map<string, number>::iterator it;
number FuZhiNumber[3];

//declaration:
number static add(number num1, number num2);

static number function(const number &num1, char a);

number minus(const number num1, number num2, int i);

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

bool static convertNumToInts(number num, int cal[]);

bool static convertIntsToNum(number &num, int ans[]);

bool static convertStringToNum(string str, number &number);

static number calculate(string input);

static number calculate1(string input);

static void calculate2(string input);

static bool vectorCalculate(vector<number> &, vector<char> &, int i);

static number tempMinus(const number &num1, const number &num2);

int beginIndexFinding(const int ints[]);

int stopIndexFinding(const int ints[]);

number::number(bool positive, int decimals, vector<int> integers) {
    this->positive = positive;
    this->decimals = decimals;
    this->integers = std::move(integers);
}

bool number::convertNumToInts(number num, int cal[]) {

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


bool number::convertIntsToNum(number &num, int ans[]) {
    int beginIndex = -1;
    int stopIndex = ans[0];
    for (int i = 999; i >= stopIndex; i--) {
        if (ans[i] != 0) {
            beginIndex = i;
            num.decimals = i - 500;
            break;
        }
    }
    if (beginIndex == -1)cerr << "convert mistake!" << endl;
    for (int i = stopIndex; i <= beginIndex; ++i) {
        num.integers.push_back(ans[i]);
    }
    return true;
}//注意检查，ans[0]是否都存入了有效index的开始

number number::add(number num1, number num2) {//可再优化，判断一下 要是数小，就只开100位的数组计算，这样子
    if (num1.integers.size() + abs(num1.decimals) > 499 || num2.integers.size() + abs(num2.decimals) > 499) {
        cerr << "invalid input for add , we only can provide numbers less than e500" << endl;
    }
    number result;
    if (num1.positive == num2.positive) {
        int cal1[1000] = {0};//cal【500】存个位；decimal若为正，意味着将小数点左移，
        int cal2[1000] = {0};//cal【500】存个位；decimal若为正，意味着将小数点左移，
        int ans[1000] = {0};
        convertNumToInts(num1, cal1);
        convertNumToInts(num2, cal2);
        int beginIndex = 500 + std::max(num1.decimals, num2.decimals);
        int stopIndex = min(cal1[0], cal2[0]);
        for (int i = beginIndex; i >= stopIndex; --i) {
            ans[i] += (cal1[i] + cal2[i]);
            ans[i - 1] += ans[i] / 10;
            ans[i] = ans[i] % 10;
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
        for (int i = beginIndex; i >= stopIndex; --i) {
            if (ans[i] == 0)beginIndex--;
            else break;
        }
        if (ans[stopIndex - 1] != 0) { ans[0] = --stopIndex; }
        //////更新beginIndex 差点忘了！
        for (int i = stopIndex; i <= beginIndex; ++i) {
            result.integers.push_back(ans[i]);
        }
        return result;

    } else {
        if (abs_com(num1, num2)) {//假如num1的绝对值大，那么式子的符号与num1相同
            num2.positive = !num2.positive;
            number temp = minus(num1, num2, 0);
            temp.positive = num1.positive;
            return temp;
        } else {
            num1.positive = !num1.positive;
            number temp = minus(num1, num2, 0);
            temp.positive = num2.positive;
            return temp;
        }
    }
}

number number::tempMinus(const number &num1, const number &num2) {
    int ans[1000] = {0};
    int cal1[1000] = {0};//cal【500】存个位；decimal若为正，意味着将小数点左移，
    int cal2[1000] = {0};//cal【500】存个位；decimal若为正，意味着将小数点左移，
    number::convertNumToInts(num1, cal1);
    number::convertNumToInts(num2, cal2);
    number result;
    result.positive = num1.positive;
    int stopIndex = std::min(cal1[0], cal2[0]);//认为stopIndex是最大的那个数所在的index beginIndex是最小的位数在的index；
    int beginIndex = std::max(num1.decimals, num2.decimals) + 500;
    for (int i = beginIndex; i >= stopIndex; --i) {
        if (cal1[i] < 0) {
            cal1[i] += 10;
            cal1[i - 1]--;
        }
        int i1 = cal1[i];
        int i2 = cal2[i];
        cal1[i] = i1 - i2;
        if (cal1[i] < 0) {
            cal1[i] += 10;
            cal1[i - 1]--;
        }
    }
    for (int i = 999; i > 0; --i) {
        if (cal1[i] != 0) {
            result.decimals = i - 500;
            break;
        }
    }
    for (int i = 1; i < 999; ++i) {
        if (cal1[i] != 0) {
            ans[0] = i;
            break;
        }
    }


    for (int i = ans[0]; i <= result.decimals + 500; ++i) {
        ans[i] = cal1[i];
    }
    convertIntsToNum(result, ans);//result的positive和decimal已经定好了，这里只是完善result的integers
    return result;
}

number number::minus(const number num1, number num2, int i) {//正常顺序是0，反着输入过是1

    if (num1.positive == num2.positive) {
        bool flag = abs_com(num1, num2);
        if (equal(num1, num2)) {
            number temp;
            temp.positive = 1;
            temp.decimals = 0;
            temp.integers = vector<int>{0};
            return temp;
        }

        //    if (flag)result.positive = num1.positive;
        //     else result.positive = !num1.positive;
        number result;
        if (flag) {//namely num1 is larger than num2
            result = tempMinus(num1, num2);
        } else {
            //result = minus(num2, num1,1);
            result = tempMinus(num2, num1);
            result.positive = !result.positive;
        }
        return result;
    } else {
        num2.positive = !num2.positive;
        number temp = add(num1, num2);
        temp.positive = num1.positive;
        return temp;
    }
}

number number::mul(const number &num1, const number &num2) {
    bool positiveness = num1.positive == num2.positive;
    number result;
    int ans[1000] = {0};
    result.positive = positiveness;
    int cal1[1000] = {0};//cal【500】存个位；decimal若为正，意味着将小数点左移，
    int cal2[1000] = {0};//cal【500】存个位；decimal若为正，意味着将小数点左移，
    number::convertNumToInts(num1, cal1);
    number::convertNumToInts(num2, cal2);
    for (int i = 500 + num1.decimals; i >= 500 - (num1.integers.size() - num1.decimals) + 1; --i) {
        for (int j = 500 + num2.decimals; j >= 500 - (num2.integers.size() - num2.decimals) + 1; --j) {
            ans[i + j - 500] += cal1[i] * cal2[j];
            ans[i + j - 501] += ans[i + j - 500] / 10;
            ans[i + j - 500] = ans[i + j - 500] % 10;
        }
    }
    for (int i = 500 - (num1.integers.size() - num1.decimals)
                 - ((num2.integers.size() - num2.decimals)) - 5//多减几个 没事
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
    if (result.decimals>0)result.decimals = num1.decimals + num2.decimals;
    return result;
}

number number::div(const number &num1, number num2) {
    number result;
    if (num2.integers.size() - num2.decimals >= 10 || num2.integers.size() >= 12) {
        cerr << "we can not div such a complex number";
    }
    if ((num2.integers.size() == 1 && num2.integers.back() == 0) || num2.integers.empty())
        cerr << "invalid input for div. we can not divide zero." << endl;

    int cal1[1000] = {0};
    convertNumToInts(num1, cal1);
    int cal2[1000] = {0};
    convertNumToInts(num2, cal2);

    int stopIndexForNum1 = stopIndexFinding(cal1);
    int stopIndexForNum2 = stopIndexFinding(cal2);
    int length = num2.integers.size();
    int indexDifference = stopIndexForNum2 - stopIndexForNum1;
    int caltemp[1000] = {0};
    caltemp[0] = cal1[0];
    int ans[1000] = {0};
    for (int i = stopIndexForNum2; i < length + stopIndexForNum2; ++i) {
        caltemp[i - indexDifference] = cal2[i];//对齐
    }
    int deviation = 0;
    for (int i = cal1[0]; i < cal1[0] + num1.integers.size() + num2.integers.size() + 10; ++i) {
        while (true) {
            bool flag = false;
            int count = 0;
            for (int j = i; j < i + length; ++j) {
                if (cal1[j] > caltemp[j - deviation]) {
                    flag = true;
                    break;
                } else if (cal1[j] == caltemp[j - deviation])count++;
                else flag = false;
            }//比出是可以减的，就继续
            if (flag || count == length) {//可以减
                for (int j = i; j < i + length; ++j) {
                    cal1[j] -= caltemp[j - deviation];
                    if (cal1[j] < 0) {
                        cal1[j] += 10;
                        int k = j;
                        while (cal1[--k] <= 0) {}
                        cal1[k]--;
                        for (int l = k + 1; l < j; ++l) {
                            cal1[l] += 9;
                        }
                    }
                }
                ans[i + length - 1]++;
            } else break;//假如不可以减了，就跳到下一位了
        }
        cal1[i + 1] += cal1[i] * 10;
        cal1[i] = 0;//到下一位了，那个首位数就要归0 给到后面
        deviation++;
    }
    for (int i = cal1[0]; i < 999; ++i) {
        if (ans[i] != 0) {
            ans[0] = i;
            break;
        }
    }
    int beginIndex = 0;
    for (int i = 999; i > 0; --i) {
        if (ans[i] != 0) {
            beginIndex = i;
            break;
        }

    }

    for (int i = ans[0]; i <= beginIndex; i++) {
        result.integers.push_back(ans[i]);
    }
    result.decimals = beginIndex - 500 - num2.decimals;

    bool positiveness = num1.positive == num2.positive;
    result.positive = positiveness;
    return result;
}


number number::sin(const number &num1) {

    number result;
    if (num1.integers.empty()) {
        cerr << "invalid input for sin" << endl;
    }
    result = function(num1, 's');
    return result;


}

number number::cos(const number &num1) {
    number result;
    if (num1.integers.empty()) {
        cerr << "invalid input for cos" << endl;
    }
    result = function(num1, 'c');
    return result;
}

number number::tan(const number &num1) {
    if (num1.integers.empty()) {
        cerr << "invalid input for tan" << endl;
    }
    number number1 = sin(num1);
    number number2 = cos(num1);
    if (number2.integers.empty()) { cerr << "invalid input for tan ,since cos equals zero" << endl; }
    else {
        return div(number1, number2);
    }
}

number number::function(const number &num1, char a) {

    number result;
    if (num1.integers.empty()) {
        cerr << "invalid input for cos" << endl;
    }
    int ints[1000] = {};
    convertNumToInts(num1, ints);
    int k = beginIndexFinding(ints);
    long long integer_part = 0;
    long double fraction_part = 0;
    for (int i = ints[0]; i <= 500; ++i) {
        integer_part = integer_part * 10 + ints[i];
        if (integer_part > 10000000000000)cerr << "we can not calculate such a huge number" << endl;
    }

    for (int i = k; i >= 501; --i) {
        fraction_part = fraction_part * 0.1 + 0.1 * ints[i];
    }
    long double num = integer_part + fraction_part;


    long double answer;
    switch (a) {
        case 's': {
            answer = sinl(num);
            break;
        }
        case 'c': {
            answer = cosl(num);
            break;
        }
        case 'l': {
            answer = log(num);
            break;
        }
        case 'e': {
            answer = expl(num);
            break;
        }
        case 'S': {
            answer = sqrtl(num);
            break;
        }
    }


    if (answer < 0)result.positive = 0;
    else result.positive = true;
    answer = abs(answer);

    std::stringstream ss;
    ss << std::setprecision(15) << answer;
    string str = ss.str();


    int j = 0;
    if (answer < 1)j += 2;//把 0. 跳过

    result.decimals = 9 + j;
    if (ceil(answer) == floor(answer))
        for (int i = 0; i < answer; ++i) {
            result.integers.push_back(str[i] - '0');
            result.decimals = 0;
            return result;
        }//把整数的直接返回了，有小数的再取9位小数
    for (int i = j; i < j + 500 - ints[0] + 11; ++i) {
        if (str[i] == '.')continue;
        result.integers.push_back((str[i] - '0'));
    }
    return result;
}

number number::sqrt(const number &num1) {

    number result;
    result = function(num1, 'S');
    return result;
}

bool number::equal(const number &num1, number num2) {//==
    if (!larger(num1, num2) && !smaller(num1, num2))return true;
    return false;
}

number number::exp(const number &num1) {
    number result;

    result = function(num1, 'e');
    return result;
}

number number::ln(const number &num1) {
    vector<int> vec = {0};
    number check(1, 0, vec);
    if (abs_com(check, num1) || !num1.positive) {
        cerr << "invalid input for ln. we can not ln non-positive number." << endl;
    }
    number result;
    result = function(num1, 'l');

    return result;
}

bool number::larger(const number &num1, number num2) {
    if (num1.positive && !num2.positive)return true;
    if (!num1.positive && num2.positive)return false;
    if (num1.positive && num2.positive)return abs_com(num1, num2);
    if (!num1.positive && !num2.positive)return !abs_com(num1, num2);
}

bool number::smaller(const number &num1, const number &num2) {
    return larger(num2, num1);
}

bool number::abs_com(const number &num1, const number &num2) {
    int cal1[1000];
    number::convertNumToInts(num1, cal1);
    int cal2[1000];
    number::convertNumToInts(num2, cal2);
    if (cal1[0] < cal2[0])return true;
    if (cal1[0] > cal2[0])return false;
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
            if (str[i] == '.')num.decimals = str.length() - i - 1;
            else num.integers.push_back((str[i] - '0'));
        }
    } else {
        num.positive = 1;
        for (int i = 0; i < str.length(); ++i) {
            if (str[i] == '.')num.decimals = str.length() - i - 1;
            else num.integers.push_back((str[i] - '0'));
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
    if (leftBracket != rightBracket || flagForEqual[0] > 1)cerr << "invalid input here1";

    if (flagForEqual[0] == 0) {
        result = number::calculate1(input);
    } else//namely there is an equal sign
    {
        number::calculate2(input);
    }

//    pBoolean=&flagForMinus;
//    numbers_temp=numbers;
//    operators_temp=operators;
    return result;
}

void number::clear(vector<number> &numbers, vector<char> &operators, char a) {
    number temp;
    number number1 = numbers.back();
    numbers.pop_back();
    switch (a) {
        case '+': {
            number number2 = numbers.back();
            numbers.pop_back();
            temp = add(number2, number1);
            break;
        }
        case '-': {
            number number2 = numbers.back();
            numbers.pop_back();
            temp = minus(number2, number1, 0);
            break;
        }
        case '*': {
            number number2 = numbers.back();
            numbers.pop_back();
            temp = mul(number2, number1);
            break;
        }
        case '/': {
            number number2 = numbers.back();
            numbers.pop_back();
            temp = div(number2, number1);
            break;
        }
        case 's': {
            temp = sin(number1);
            break;
        }
        case 'c': {

            temp = cos(number1);
            break;
        }
        case 't': {
            temp = tan(number1);
            break;
        }
        case 'e': {
            temp = exp(number1);
            break;
        }
        case 'l': {
            temp = ln(number1);
            break;
        }
        case 'S': {
            temp = sqrt(number1);
            break;
        }
        default: {
            cerr << "invalid operators!";
            numbers.push_back(number1);
        }
    }
    numbers.push_back(
            temp);//No worries for vector 'numbers<number>' is empty since it is checked before enter this method.
}

bool number::vectorCalculate(vector<number> &numbers, vector<char> &operators, int i) {

    if (operators.empty()) {
        if (numbers.size() == 1)return true;
            //如果没有operators，也就是numbers里就一个数字
        else return false;
    }

    if (i == 1) {//operators.back() == ')'优先级最高 不用读入右括号
        if (!operators.empty()) {
            while (operators.back() != '(') {
//            number number1 = numbers.back();
//            numbers.pop_back();
//            number number2 = numbers.back();
//            numbers.pop_back();

                char a = operators.back();
                operators.pop_back();
                clear(numbers, operators, a);
            }//这样如果是sin（1）的话 不进行处理 直接把左括号pop掉了 合理
            operators.pop_back();//把左括号pop掉

        }
    } else if (i == 2)//denote 遇到了 函数 优先级第二高
    {
//不用操作，函数在我这是优先级最高的
    } else if (i == 3)//denote 遇到了 乘除        本身在这一步的处理只需要把前面的操作符处理掉就行了，不需要处理别的，符号等出了方法再push进去
    {
        if (!operators.empty()) {
            char a = operators.back();
            if (a == '*' || a == '/') {
                operators.pop_back();
                number num1 = numbers.back();
                numbers.pop_back();
                number num2 = numbers.back();
                numbers.pop_back();
                number temp;
                if (a == '*')temp = mul(num1, num2);
                else temp = div(num2, num1);
                numbers.push_back(temp);
            }
            if (a == 's' || a == 'c' || a == 't' || a == 'S') {
                operators.pop_back();
                number num1 = numbers.back();
                numbers.pop_back();
                number temp;
                if (a == 's')temp = sin(num1);
                else if (a == 'c')temp = cos(num1);
                else if (a == 't')temp = tan(num1);
                else if (a == 'S')temp = sqrt(num1);
                numbers.push_back(temp);
            }
            if (a == 'e' || a == 'l') {
                operators.pop_back();
                number num1 = numbers.back();
                numbers.pop_back();
                number temp;
                if (a == 'e')temp = exp(num1);
                else if (a == 'l')temp = ln(num1);
                numbers.push_back(temp);
            }
        }
    } else if (i == 4)//denote 到了结尾了，全部清空所有栈
    {
        while (!operators.empty()) {
            char a = operators.back();
            operators.pop_back();
            clear(numbers, operators, a);
        }
    } else if (i == 5) {//denote 遇到了加减号 这时候还没有把加号放进来
        if (!operators.empty()) {
            char a = operators.back();
            if (a == '+' || a == '-') {
                operators.pop_back();
                number num1 = numbers.back();
                numbers.pop_back();
                number num2 = numbers.back();
                numbers.pop_back();
                number temp;
                if (a == '+')temp = add(num1, num2);
                else temp = minus(num2, num1, 0);
                numbers.push_back(temp);
            }
            if (a == '*' || a == '/') {
                operators.pop_back();
                number num1 = numbers.back();
                numbers.pop_back();
                number num2 = numbers.back();
                numbers.pop_back();
                number temp;
                if (a == '*')temp = mul(num1, num2);
                else temp = div(num2, num1);
                numbers.push_back(temp);
            }
            if (a == 's' || a == 'c' || a == 't' || a == 'S') {
                operators.pop_back();
                number num1 = numbers.back();
                numbers.pop_back();
                number temp;
                if (a == 's')temp = sin(num1);
                if (a == 'c')temp = cos(num1);
                if (a == 't')temp = tan(num1);
                if (a == 'S')temp = sqrt(num1);
                numbers.push_back(temp);
            }
            if (a == 'e' || a == 'l') {
                operators.pop_back();
                number num1 = numbers.back();
                numbers.pop_back();
                number temp;
                if (a == 'e')temp = exp(num1);
                if (a == 'l')temp = ln(num1);
                numbers.push_back(temp);
            }
        }
    }
    return true;
}

number number::calculate1(string input) {//计算非等式
    //  number temp_result;
    vector<number> numbers = {};
    vector<char> operators = {};
    bool flagForMinus = false;


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
                if (++i >= input.length()) {
                    --i;
                    break;
                }
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
        } else if (a == '(')operators.push_back('(');
        else if (a == 'x')numbers.push_back(FuZhiNumber[0]);
        else if (a == 'y')numbers.push_back(FuZhiNumber[1]);
        else if (a == 'z')numbers.push_back(FuZhiNumber[2]);
        else if (a == '-') {
            if (i == 0 || input[i - 1] == '(') {
                flagForMinus = true;
            } else {
                vectorCalculate(numbers, operators, 5);
                operators.push_back(a);
            }
        } else if (a == '+') {
            vectorCalculate(numbers, operators, 5);
            operators.push_back(a);
        } else if (a == '*' || a == '/') {
            vectorCalculate(numbers, operators, 3);
            operators.push_back(a);
        } else if (a == 's') {
            if (input[i + 1] == 'i' && input[i + 2] == 'n' && input[i + 3] == '(') {
                i += 3;
                vectorCalculate(numbers, operators, 2);
                operators.push_back('s');
                operators.push_back('(');
            }//就以s记为sin
            else if (input[i + 1] == 'q' && input[i + 2] == 'r' && input[i + 3] == 't' && input[i + 4] == '(') {
                i += 4;
                vectorCalculate(numbers, operators, 2);
                operators.push_back('S');//就以S记为sqrt
                operators.push_back('(');
            } else { cerr << "invalid input here2"; }
        } else if (a == 'c') {
            if (input[i + 1] == 'o' && input[i + 2] == 's' && input[i + 3] == '(') {
                i += 3;
                vectorCalculate(numbers, operators, 2);
                operators.push_back('c');
                operators.push_back('(');
            }//就以c记为cos
            else { cerr << "invalid input here3"; }
        } else if (a == 't') {
            if (input[i + 1] == 'a' && input[i + 2] == 'n' && input[i + 3] == '(') {
                i += 3;
                vectorCalculate(numbers, operators, 2);
                operators.push_back('t');
                operators.push_back('(');
            }//就以t记为tan
            else { cerr << "invalid input here4"; }
        } else if (a == 'e') {
            if (input[i + 1] == 'x' && input[i + 2] == 'p' && input[i + 3] == '(') {
                i += 3;
                vectorCalculate(numbers, operators, 2);
                operators.push_back('e');
                operators.push_back('(');
            }//就以e记为exp
            else { cerr << "invalid input here5"; }
        } else if (a == 'l') {
            if (input[i + 1] == 'n' && input[i + 2] == '(') {
                i += 2;
                vectorCalculate(numbers, operators, 2);
                operators.push_back('l');
                operators.push_back('(');
            }//就以l记为ln
            else { cerr << "invalid input here6"; }
        } else if (a == ')') {
            vectorCalculate(numbers, operators, 1);
            //不用pop进去了，栈里是没有右括号的，出现右括号就清空到左括号
        } else { cerr << "invalid input here7"; }
    }
    vectorCalculate(numbers, operators, 4);//结束
    return numbers.back();//最后占里会剩一个number
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
    for (int i = 0; i <= flagForEqual[1] - 1; ++i) {
        str1 += input[i];
    }
    for (int i = flagForEqual[1] + 1; i < input.length(); ++i) {
        str2 += input[i];
    }
    if (input[0] == 'x') {
        FuZhiNumber[0] = calculate1(str2);
    } else if (input[0] == 'y') {
        FuZhiNumber[1] = calculate1(str2);
    } else if (input[0] == 'z') {
        FuZhiNumber[2] = calculate1(str2);
    }
}

int beginIndexFinding(const int ints[]) {
    for (int i = 999; i >= ints[0]; --i) {
        if (ints[i] != 0)return i;
    }
    return -1;
}

int stopIndexFinding(const int ints[]) {
    for (int i = 1; i < 999; ++i) {
        if (ints[i] != 0)return i;
    }
    return -1;
}

static void test(number number) {
    cout << "number's positiveness is " << number.positive << endl;
    cout << "number's decimals is " << number.decimals << endl;
    for (int i = 0; i < number.integers.size(); ++i) {
        cout << "number'integers " << i << "th :" << number.integers[i] << endl;
    }
    cout << "end of this number." << endl;
}

