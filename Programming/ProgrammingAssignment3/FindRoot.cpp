#include <cmath>
#include <cstdio>
#include <float.h>
#include <functional>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

// f(x) = 2x^4 + 24x^3 + 61x^2 - 16x + 1
double f(double x)
{
    return 2.0 * pow(x, 4) + 24.0 * pow(x, 3) + 61.0 * pow(x, 2) - 16.0 * x + 1.0;
}

// f'(x) = 8x^3 + 72x^2 + 122x - 16
double f_Derivative(double x)
{
    return 8.0 * pow(x, 3) + 72.0 * pow(x, 2) + 122.0 * x - 16;
}

// Newton 法在给定初始值 x0 下求方程 f(x) = 0 的近似解，返回迭代序列 x0, x1, ...
vector<double> NewtonsMethod(function<double(double)> f, function<double(double)> f_Derivative, double x0, double epsilon)
{
    auto phi = [f, f_Derivative](double x) -> double { return (x - f(x) / f_Derivative(x)); }; // phi(x) = x - f(x) / f'(x)
    vector<double> x = {x0};                                                                   // 迭代序列
    while (fabs(f(x.back())) >= epsilon)
        x.push_back(phi(x.back()));
    return x;
}

// 弦截法在给定初始值 x0 x1 下求方程 f(x) = 0 的近似解，返回迭代序列 x0, x1, ...
vector<double> SecantMethod(function<double(double)> f, function<double(double)> f_Derivative, double x0, double x1, double epsilon)
{
    auto phi = [f, f_Derivative](double xk_1, double xk) -> double { return (xk - (f(xk) * (xk - xk_1)) / (f(xk) - f(xk_1))); }; // phi(x) = x - (f(x_k)(x_k - x_{k - 1}) / (f(x_k) - f(x_{k-1}))
    vector<double> x = {x0, x1};                                                                                                 // 迭代序列
    while (fabs(f(x.back())) >= epsilon)
        x.push_back(phi(x.at(x.size() - 2), x.back()));
    return x;
}

int main()
{
    double epsilon = 1.0E-9;
    // // Newton 法
    // {
    //     cout << "Newton 法" << endl;
    //     vector<double> InitialValues = {0.0, 3.0}; // 初始值 x0
    //     for (double x0 : InitialValues)
    //     {
    //         vector<double> Recurrence = NewtonsMethod(f, f_Derivative, x0, epsilon);
    //         printf("迭代步数：%d\n", Recurrence.size() - 1);
    //         for (double x : Recurrence)
    //             printf("%.10E\n", x);
    //     }
    // }
    // // 弦截法
    // {
    //     cout << "弦截法" << endl;
    //     vector<pair<double, double>> InitialValues = {{0.0, 0.5}, {0.1, 1.5}}; // 初始值 (x0, x1)
    //     for (pair<double, double> x_ : InitialValues)
    //     {
    //         vector<double> Recurrence = SecantMethod(f, f_Derivative, x_.first, x_.second, epsilon);
    //         printf("迭代步数：%d\n", Recurrence.size() - 1);
    //         for (double x : Recurrence)
    //             printf("%.10E\n", x);
    //     }
    // }
    // Output to LaTeX Table
    // \hline $k = 0$  & $0.0000000000\text{E}{+}000$    & $1.0000000000\text{E}{+}000$ \\
    // Newton 法
    {
        cout << "Newton 法" << endl;
        vector<double> InitialValues = {0.0, 3.0}; // 初始值 x0
        for (double x0 : InitialValues)
        {
            vector<double> Recurrence = NewtonsMethod(f, f_Derivative, x0, epsilon);
            int k = 0;
            for (double x : Recurrence)
            {
                printf("\\hline $k = %d$ & $%.10E$ & $%.10E$ \\\\ \n", k++, x, f(x));
            }
        }
    }
    // 弦截法
    {
        cout << "弦截法" << endl;
        vector<pair<double, double>> InitialValues = {{0.0, 0.5}, {0.1, 1.5}}; // 初始值 (x0, x1)
        for (pair<double, double> x_ : InitialValues)
        {
            vector<double> Recurrence = SecantMethod(f, f_Derivative, x_.first, x_.second, epsilon);
            int k = 0;
            for (double x : Recurrence)
            {
                printf("\\hline $k = %d$ & $%.10E$ & $%.10E$ \\\\ \n", k++, x, f(x));
            }
        }
    }
}
