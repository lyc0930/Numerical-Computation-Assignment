#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <valarray>
#include <vector>
using namespace std;

// 矩阵类
template <class T>
class Matrix
{
protected:
    valarray<T> data; // matrix data
    int row;
    int column;

public:
    // 构造 r * c 的矩阵
    Matrix(int r, int c) : data(r * c), row(r), column(c) {}

    // 构造 n * n 的方阵
    Matrix(int n) : data(n * n), row(n), column(n) {}

    // 根据双重{{}}表示的 A 构造矩阵
    Matrix(vector<valarray<T>> A)
    {
        row = A.size(); // 行数
        column = 0;     // 获得列数
        for (valarray<T> rows : A)
            if (column < rows.size())
                column = rows.size();
        data = valarray<T>(row * column);
        int i = 0;
        for (valarray<T> rows : A)
        {
            int j = 0;
            for (T elements : rows)
            {
                data[i * column + j] = elements;
                j++;
            }
            i++;
        }
    }

    // 返回矩阵的宽
    int width() { return column; }

    // 矩阵的高
    int height() { return row; }

    // 位于(r, c)的元素
    T &operator()(int r, int c)
    {
        return data[(r - 1) * column + (c - 1)];
    }

    // 第 i 个元素
    T operator()(int i) { return data[i - 1]; }

    // 以切片返回第 r 行作为左值
    slice_array<T> Row(int r) { return data[slice((r - 1) * column, column, 1)]; }

    // 以切片返回第 c 列作为左值
    slice_array<T> Column(int c) { return data[slice((c - 1), row, column)]; }

    // 返回第 r 行作为右值
    valarray<T> Row(int r) const { return data[slice((r - 1) * column, column, 1)]; }

    // 返回第 c 列作为右值
    valarray<T> Column(int c) const { return data[slice((c - 1), row, column)]; }

    // 输出矩阵，使用一个参数 width 指定每个元素的位宽，默认为 5
    void print(int width = 5)
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < column; j++)
                cout << setw(width) << data[i * column + j];
            cout << endl;
        }
        return;
    }
};

// 线性方程组对应矩阵类
template <class T>
class Coefficient_Matrix : public Matrix<T>
{
private:
    bool _isAugmented;

public:
    // 构造 n * n 的方阵
    Coefficient_Matrix(int n) : Matrix<T>(n), _isAugmented(false) {}

    // 根据双重{{}}表示的 A 构造矩阵
    Coefficient_Matrix(vector<valarray<T>> A) : Matrix<T>(A), _isAugmented(false) {}

    // 构造以 A 为系数矩阵且以 b 为常数矩阵的増广矩阵
    Coefficient_Matrix(vector<valarray<T>> A, valarray<T> b) : Matrix<T>(A)
    {
        Matrix<T>::row = A.size(); // 行数
        Matrix<T>::column = 0;     // 获得列数
        for (valarray<T> rows : A)
            if (Matrix<T>::column < rows.size())
                Matrix<T>::column = rows.size();
        Matrix<T>::column += 1; // 常数矩阵
        Matrix<T>::data = valarray<T>(Matrix<T>::row * Matrix<T>::column);
        int i = 0;
        for (valarray<T> rows : A)
        {
            int j = 0;
            for (T elements : rows)
            {
                Matrix<T>::data[i * Matrix<T>::column + j] = elements;
                j++;
            }
            i++;
        }
        Matrix<T>::data[slice(Matrix<T>::column - 1, Matrix<T>::row, Matrix<T>::column)] = b;
        _isAugmented = true;
    }

    // 是否是増广矩阵
    bool isAugmented()
    {
        return _isAugmented;
    }

    // 附加常数矩阵，变为増广矩阵
    void append(valarray<T> b)
    {
        if (_isAugmented == true)
            throw "Matrix is already augmented. ";
        else
        {
            if (b.size() != Matrix<T>::row)
                throw "Size not match. ";
            else
            {
                valarray<T> originData = Matrix<T>::data;
                Matrix<T>::column += 1;
                Matrix<T>::data.resize(Matrix<T>::row * Matrix<T>::column);
                for (int i = 0; i < Matrix<T>::row; i++)
                    Matrix<T>::data[slice(i * Matrix<T>::column, Matrix<T>::column - 1, 1)] = originData[slice(i * (Matrix<T>::column - 1), Matrix<T>::column - 1, 1)];
                Matrix<T>::data[slice(Matrix<T>::column - 1, Matrix<T>::row, Matrix<T>::column)] = b;
                _isAugmented = true;
            }
        }
        return;
    }

    // 系数矩阵元素A_ij
    T &A(int i, int j)
    {
        if (i <= 0 || i > Matrix<T>::row || j <= 0 || j > Matrix<T>::column)
            throw "Oversize index. ";
        else
            return Matrix<T>::operator()(i, j);
    }

    // 常数矩阵元素b_i
    T &b(int i)
    {
        if (i <= 0 || i > Matrix<T>::row)
            throw "Oversize index. ";
        else
            return Matrix<T>::operator()(i, Matrix<T>::column);
    }

    // 输出矩阵，使用一个参数 width 指定每个元素的位宽，默认为 5
    void print(int width = 5)
    {
        if (_isAugmented == false)
            Matrix<T>::print(width); // 默认形式输出矩阵
        else
        {
            for (int i = 0; i < Matrix<T>::row; i++)
            {
                for (int j = 0; j < Matrix<T>::column - 1; j++)
                    cout << setw(width) << Matrix<T>::data[i * Matrix<T>::column + j];
                cout << " |" << setw(width) << Matrix<T>::data[i * Matrix<T>::column + Matrix<T>::column - 1];
                cout << endl;
            }
            return;
        }
        return;
    }
};

valarray<double> JacobiMethod(Coefficient_Matrix<double> M, valarray<double> x_0, double epsilon)
{
    valarray<double> x_1(x_0), x_2(x_0);
    do
    {
        x_1 = x_2;
        for (int i = 1; i <= M.height(); i++)
        {
            double s = 0;
            for (int j = 1; j <= M.height(); j++)
                s += M.A(i, j) * x_1[j - 1];
            x_2[i - 1] = (M.b(i) - s + M.A(i, i) * x_1[i - 1]) / M.A(i, i);
        }
        for (auto x : x_2)
            cout << x << ' ';
        cout << endl;
    } while (abs(x_1 - x_2).max() > epsilon);
    return x_2;
}

int main()
{
    Coefficient_Matrix<double> M({{2, -1, -1}, {1, 5, -1}, {1, 1, 10}}, {-5, 8, 11});
    M.print();
    cout << endl;
    JacobiMethod(N, {1, 1, 1}, 1E-5);
}