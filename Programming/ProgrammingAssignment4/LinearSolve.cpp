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

    // 根据valarray表示的 A 构造方阵
    Matrix(valarray<T> A) : data(A), row(sqrt(A.size())), column(sqrt(A.size()))
    {
        if (pow(sqrt(A.size()), 2) != A.size()) // 大小为平方数
            throw "Unprocessable Matrix";
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
    Coefficient_Matrix(vector<valarray<T>> A, valarray<T> b) : Matrix<T>(A), _isAugmented(true)
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
    }

    // 根据valarray表示的 A 构造方阵
    Coefficient_Matrix(valarray<T> A) : Matrix<T>(A), _isAugmented(false) {}

    // 根据valarray表示的 A 和常数矩阵 b 构造増广矩阵
    Coefficient_Matrix(valarray<T> A, valarray<T> b) : Matrix<T>(A), _isAugmented(true)
    {
        if (pow(sqrt(A.size()), 2) != A.size()) // 大小为平方数
            throw "Unprocessable Matrix";
        else
        {
            Matrix<T>::row = sqrt(A.size()); // 行数与列数
            Matrix<T>::column = Matrix<T>::row + 1;
            Matrix<T>::data = valarray<T>(Matrix<T>::row * Matrix<T>::column);
            for (int i = 0; i < Matrix<T>::row; i++)
            {
                Matrix<T>::data[slice(i * Matrix<T>::column, Matrix<T>::column - 1, 1)] = A[slice(i * (Matrix<T>::column - 1), Matrix<T>::column - 1, 1)];
            }
            Matrix<T>::data[slice(Matrix<T>::column - 1, Matrix<T>::row, Matrix<T>::column)] = b;
        }
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
    void print(int width = 5, int width_b = 2)
    {
        if (_isAugmented == false)
            Matrix<T>::print(width); // 默认形式输出矩阵
        else
        {
            for (int i = 0; i < Matrix<T>::row; i++)
            {
                for (int j = 0; j < Matrix<T>::column - 1; j++)
                    cout << setw(width) << Matrix<T>::data[i * Matrix<T>::column + j];
                cout << " |" << setw(width_b) << Matrix<T>::data[i * Matrix<T>::column + Matrix<T>::column - 1];
                cout << endl;
            }
            return;
        }
        return;
    }
};

// 使用 Jacobi 迭代法，解线性方程组 M = (A | b)
vector<valarray<double>> JacobiMethod(Coefficient_Matrix<double> M, valarray<double> x0, double epsilon)
{

    vector<valarray<double>> x = {};
    valarray<double> x_next(x0);
    do
    {
        x.push_back(x_next);
        for (int i = 1; i <= M.height(); i++)
        {
            double s = 0;
            for (int j = 1; j <= M.height(); j++)
                s += M.A(i, j) * x.back()[j - 1];
            x_next[i - 1] = (M.b(i) - s + M.A(i, i) * x.back()[i - 1]) / M.A(i, i);
        }
        // for (auto x : x_next)
        //     printf("%.10E ", x);
        // cout << endl;
    } while (abs(x_next - x.back()).max() > epsilon);
    return x;
}

// 使用 GaussSeidel 迭代法，解线性方程组 M = (A | b)
vector<valarray<double>> GaussSeidelMethod(Coefficient_Matrix<double> M, valarray<double> x0, double epsilon)
{

    vector<valarray<double>> x = {};
    valarray<double> x_next(x0);
    do
    {
        x.push_back(x_next);
        for (int i = 1; i <= M.height(); i++)
        {
            double s = 0;
            for (int j = 1; j <= M.height(); j++)
                s += M.A(i, j) * x_next[j - 1];
            x_next[i - 1] = (M.b(i) - s + M.A(i, i) * x.back()[i - 1]) / M.A(i, i);
        }
        // for (auto x : x_next)
        //     printf("%.10E ", x);
        // cout << endl;
    } while (abs(x_next - x.back()).max() > epsilon);
    return x;
}

int main()
{
    valarray<double> A(10 * 10);
    for (int i = 0; i < 10; i++)
    {
        A[i * 10 + i - 1] = -1;
        A[i * 10 + i] = 2;
        A[i * 10 + i + 1] = -1;
    }
    valarray<double> b(2, 10);
    valarray<double> x0(10);
    Coefficient_Matrix<double> M(A, b);
    M.print(3);
    cout << endl;
    vector<valarray<double>> Recurrence_Jacobi = JacobiMethod(M, x0, 1E-5);
    cout << "Jacobi 迭代, k = " << Recurrence_Jacobi.size() - 1 << endl;
    for (auto x : Recurrence_Jacobi.back())
        printf("%.10E\n", x);
    vector<valarray<double>> Recurrence_GaussSeidel = GaussSeidelMethod(M, x0, 1E-5);
    cout << "Gauss-Seidel 迭代, k = " << Recurrence_GaussSeidel.size() - 1 << endl;
    for (auto x : Recurrence_GaussSeidel.back())
        printf("%.10E\n", x);
    // Coefficient_Matrix<double> M({{2, -1, -1}, {1, 5, -1}, {1, 1, 10}}, {-5, 8, 11});
    // M.print();
    // cout << endl;
    // vector<valarray<double>> Recurrence_Jacobi = JacobiMethod(M, {1, 1, 1}, 1E-2);
    // cout << "k = " << Recurrence_Jacobi.size() << endl;
    // for (auto x : Recurrence_Jacobi.back())
    //     printf("%.10E\n", x);
    return 0;
}
