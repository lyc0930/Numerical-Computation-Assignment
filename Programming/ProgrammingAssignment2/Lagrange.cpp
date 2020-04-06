#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdio>
#include <float.h>
#include <iostream>

using namespace std;

class point
{
public:
    double x, y;

public:
    point()
    {
        x = 0;
        y = 0;
    }
    point(double a, double b)
    {
        x = a;
        y = b;
    }
    void showpoint()
    {
        cout << "(" << x << "," << y << ")" << endl;
    }
};

double f(double x)
{
    return 1.0 / (2.0 + 2.0 * x + pow(x, 2));
}

point *Linear(int n)
{
    point *Nodes;
    Nodes = new point[n + 1];
    for (int i = 0; i <= n; i++)
    {
        Nodes[i].x = -5.0 + (10.0 * i) / n;
        Nodes[i].y = f(Nodes[i].x);
    }
    return Nodes;
}

point *Chebyshev(int n)
{
    point *Nodes;
    Nodes = new point[n + 1];
    for (int i = 0; i <= n; i++)
    {
        Nodes[i].x = -5.0 * cos((2.0 * i + 1.0) * M_PI / (2.0 * n + 2.0));
        Nodes[i].y = f(Nodes[i].x);
    }
    return Nodes;
}

double Lagrange(point Nodes[], int n, double x)
{
    double estimator = 0;
    for (int i = 0; i <= n; i++)
    {
        double l = 1.0;
        for (int j = 0; j <= n; j++)
        {
            if (j != i)
                l *= (x - Nodes[j].x) / (Nodes[i].x - Nodes[j].x);
        }
        estimator += Nodes[i].y * l;
    }
    return estimator;
}

int main()
{
    int N[] = {4, 8, 16};
    point *(*InterpolationNodeGenerator[])(int) = {Linear, Chebyshev};
    for (point *(*NodeGenerator)(int) : InterpolationNodeGenerator)
    {
        for (int n : N)
        {
            double error = 0.0;
            double maximum = 0.0;
            for (int i = 0; i <= 500; i++)
            {
                double y = -5.0 + (double)i / 50.0;
                error = fabs(f(y) - Lagrange(NodeGenerator(n), n, y));
                if (error > maximum)
                    maximum = error;
            }
            printf("n = %2d , %.12E\n", n, maximum);
        }
    }
    return 0;
}