#include <cmath>
#include <cstdio>
#include <iostream>
using namespace std;

// f(x) = Sqrt[x^2 + 9] - 3

float F(float x)
{
    float result_square = pow(x, 2);
    float result_plus = result_square + 9.0f;
    float result_sqrt = sqrt(result_plus);
    float result_minus = result_sqrt - 3.0f;
    return result_minus;
}

// g(x) := x^2 / (Sqrt[x^2 + 9] + 3)

float G(float x)
{
    float result_square = pow(x, 2);
    float result_plus_1 = result_square + 9.0f;
    float result_sqrt = sqrt(result_plus_1);
    float result_plus_2 = result_sqrt + 3.0f;
    float result_div = result_square / result_plus_2;
    return result_div;
}

int main()
{
    float x[10 + 1];
    float f[10 + 1];
    float g[10 + 1];
    for (int i = 1; i <= 10; i++)
    {
        x[i] = pow(8, -i);
        f[i] = F(x[i]);
        g[i] = G(x[i]);
        printf("%.12E & %.12E & %.12E \\\\ \\hline \n", x[i], f[i], g[i]);
    }
    return 0;
}
