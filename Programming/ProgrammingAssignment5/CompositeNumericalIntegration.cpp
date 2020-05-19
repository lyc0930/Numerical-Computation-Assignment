#include <cmath>
#include <cstdio>
#include <float.h>
#include <functional>
#include <iostream>
#include <vector>

using namespace std;

// 使用复化 Simpson 积分公式计算在区间 [a, b] 上函数 f 的积分，区间等距分为 n 份
double SimpsonsRule(function<double(double)> f, double a, double b, int n)
{
    int m = n / 2;
    double h = (b - a) / n;
    double S = 0.0;
    S += f(a);
    for (int i = 1; i < n; i += 2)
        S += 4 * f(a + i * h);
    for (int i = 2; i < n; i += 2)
        S += 2 * f(a + i * h);
    S += f(b);
    S *= h / 3;
    return S;
}

// 使用复化梯形积分公式计算在区间 [a, b] 上函数 f 的积分，区间等距分为 n 份
double TrapezoidalRule(function<double(double)> f, double a, double b, int n)
{
    double h = (b - a) / n;
    double T = 0.0;
    T += 0.5 * f(a);
    for (int i = 1; i <= n - 1; i++)
        T += f(a + i * h);
    T += 0.5 * f(b);
    T *= h;
    return T;
}

int main()
{
    auto Sin = static_cast<double (*)(double)>(&sin); // 使用强制类型转换消除cannot resolve overloaded function 'sin' based on conversion to type 'std::function<double(double)>' 错误
    int N = 10;                                       // 数据组数
    double a = 0, b = 8;                              // 区间左右端点
    double accuracy = 2 * pow(sin(4), 2);             // 精确值

    { // 复化梯形积分
        cout << "复化梯形积分，误差和误差阶为：" << endl;
        vector<double>
            Error = {abs(accuracy - TrapezoidalRule(Sin, a, b, pow(2, 0)))}; // 误差
        vector<double> Order;                                                // 误差阶
        printf("k = %2d , e = %.12E\n", 0, Error.back());
        for (int k = 1; k <= N; k++)
        {
            Error.push_back(abs(accuracy - TrapezoidalRule(Sin, a, b, pow(2, k))));
            Order.push_back(-log(Error[k] / Error[k - 1]) / log(2));
            printf("k = %2d , e = %.12E , d = %f\n", k, Error.back(), Order.back());
        }
    }

    { // 复化 Simpson 积分
        cout << "复化 Simpson 积分，误差和误差阶为：" << endl;
        vector<double> Error = {abs(accuracy - SimpsonsRule(Sin, a, b, pow(2, 0)))}; // 误差
        vector<double> Order;                                                        // 误差阶
        printf("k = %2d , e = %.12E\n", 0, Error.back());
        for (int k = 1; k <= N; k++)
        {
            Error.push_back(abs(accuracy - SimpsonsRule(Sin, a, b, pow(2, k))));
            Order.push_back(-log(Error[k] / Error[k - 1]) / log(2));
            printf("k = %2d , e = %.12E , d = %f\n", k, Error.back(), Order.back());
        }
    }
    return 0;
}
