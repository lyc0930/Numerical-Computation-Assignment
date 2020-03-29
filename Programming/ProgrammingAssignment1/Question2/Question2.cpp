#include <cmath>
#include <cstdio>
#include <iostream>
using namespace std;

double Data[5] = {4040.045551380452, -2759471.276702747, -31.64291531266504, 2755462.874010974, 0.0000557052996742893};

int main()
{
    double sum_in_order = 0;
    double sum_in_reverse_order = 0;
    double sum_in_bitonic_order = 0;
    for (int i = 0; i < 5; i++)
    {
        sum_in_order += Data[i];
        printf("%.17E\n", sum_in_order);
    }
    for (int i = 4; i >= 0; i--)
        sum_in_reverse_order += Data[i];
    // Data[3] > Data[0] > Data[4] > 0 > Data[2] > Data[1]
    int PositiveIndex[3] = {3, 0, 4};
    int NegativeIndex[2] = {1, 2};
    double subsum_in_ascending_order = 0;
    double subsum_in_descending_order = 0;
    for (int i = 0; i < 3; i++)
        subsum_in_descending_order += Data[PositiveIndex[i]];
    for (int i = 0; i < 2; i++)
        subsum_in_ascending_order += Data[NegativeIndex[i]];
    sum_in_bitonic_order = subsum_in_descending_order + subsum_in_ascending_order;
    printf("%.12E, %.12E, %.12E", sum_in_order, sum_in_reverse_order, sum_in_bitonic_order);
    return 0;
}