#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <string.h>
#include <math.h>
#include <time.h>
#include <cmath>
#include <string>
#include <algorithm>
#include <cstdlib>

#define abs(x) ((x) >= 0 ? (x) : (-x))

using namespace std;
template< class T >
class Matrix
{
public:
    int h, w;
    T **p;

    void allocArrays()
    {
        p = new T*[h];
        for (int i = 0; i < h; i++)
            p[i] = new T[w];
    }
 ///    inline T &at( int i, int j ) { return p[i * w + j]; }

    Matrix();
    Matrix(int Height, int Width);
    Matrix(const Matrix &t);
    ~Matrix();
    void _rand();
    Matrix< T > &operator+=(const Matrix<T> &t);
    Matrix< T > operator+(const Matrix<T> &t);
    Matrix< T > operator-(const Matrix<T> &t);
    Matrix< T > operator*(const Matrix<T> &t);

    T det() const;
    T track();
    Matrix< T > tran();
    Matrix< T > inversion();
    void _cin();
    void _cout();


};
template <class T>
Matrix<T>::Matrix()
{
    h = 0;
    w = 0;
    allocArrays();
}
template <class T>
Matrix<T>::Matrix(int Height, int Width)
{
    h = Height;
    w = Width;
    allocArrays();
    for (int i = 0; i < h; i++)
        for (int j = 0;j < w; j++)
            p[i][j] = static_cast<T>(0);
}
template <class T>
Matrix<T>::Matrix(const Matrix<T> &t)//:h(t.h), w(t.w)
{
    h = t.h;
    w = t.w;
    allocArrays();
    for (int i = 0;i < h; i++)
        for (int j = 0; j < w; j++)
            p[i][j] = t.p[i][j];
}
template <class T>
Matrix<T>::~Matrix()
{
    for(int i = 0; i < h; i++)
        delete[] p[i];
    delete[] p;
}
template <class T>
void Matrix<T>::_rand()
{
    srand(time(NULL));
    for(int i = 0; i < h; i++ )
        for(int j = 0; j < w; j++ )
            {
                p[i][j]=(rand()%10);
                p[i][j] = (T)p[i][j];
            }
}
template< class T >
Matrix< T > &Matrix< T >::operator+=(const Matrix< T > &t)
{
    for (int i = 0; i < h; ++i)
            for (int j = 0;j < w; ++j)
                p[i][j] += t.p[i][j];
    return *this;
}
template<class T>
Matrix< T > Matrix< T >::operator+( const Matrix< T > &t )
{
    Matrix< T >  temp(*this);
    temp._cout();
    cout << "\n";
    if ((h!=t.h) || (w != t.w))
    {
        cout << "Error";
        return t;
    }
    else
    {
        return(temp +=t);
    }
}
template< class T >
Matrix< T > Matrix< T >::operator-( const Matrix< T > &t )
{
    Matrix< T > t1 = *this;
    Matrix< T > t2 = t;
    Matrix< T >  temp = Matrix(t1.h, t1.w);
    if (t1.h!=t2.h || t1.w != t2.w)
    {
        cout << "Error";
        return t;
    }
    else
    {

        for (int i = 0; i < t1.h;i++)
            for (int j = 0;j < t1.w; j++)
                temp.p[i][j] = t1.p[i][j] - t2.p[i][j];
        return temp;
    }
}
template< class T>
Matrix< T > Matrix< T >::operator*( const Matrix< T > &t)
{
    Matrix t1 = *this;
    Matrix t2 = t;

    if (t1.w != t2.h)
    {
         cout << "Impossible to calculate this operation";
         return t1;
    }
    else
    {
        Matrix M = Matrix(t1.h, t2.w);
        for(int i = 0; i < M.h; i++)
            for (int j = 0; j < M.w; j++)
                for (int k = 0; k < t1.w; k++)
                    M.p[i][j] += t1.p[i][k] * t2.p[k][j];
        return M;
    }
}
template<class T>
T Matrix<T>::det() const
{
        Matrix< T > M1(*this);
        //create Matrix< double > M;
        int height = M1.h;
        int width = M1.w;
        double** arr = new double*[h];
        for (int i = 0; i < h; i++)
            arr[i] = new double[w];
        for (int i = 0;i < height; i++)
            for (int j = 0; j < width; j++)
                arr[i][j] = double(M1.p[i][j]);


        const double EPS = 1E-9;
        double d = 1.0;
        for (int i=0; i<height; ++i)
        {
            int k = i;
            for (int j=i+1; j<height; ++j)
                if (abs (arr[j][i]) > abs (arr[k][i]))
                    k = j;
            if (abs (arr[k][i]) < EPS)
            {
                d = 0.0;
                break;
            }
            swap (arr[i], arr[k]);
            if (i != k)
                d = -d;
            d *= arr[i][i];
            for (int j=i+1; j<height; ++j)
                arr[i][j] = (arr[i][j]) / (arr[i][i]);
            for (int j=0; j<height; ++j)
                if ((j != i) && (abs(arr[j][i]) > EPS))
                    for (int k=i+1; k<height; ++k)
                        {
                            arr[j][k] -= arr[i][k] * arr[j][i];
                        }
        }
    for(int i = 0; i < height; i++)
        delete[] arr[i];
    delete[] arr;
    return (static_cast<T>(d));
}
template<class T>
T Matrix< T >::track()
{
        T tr = static_cast<T>(0);
        for (int i = 0; i < h; ++i)
            tr += p[i][i];

    return tr;
}
template<class T>
Matrix <T> Matrix< T >::tran()
{
    for(int i = 0; i < h; i++)
        for(int j = i; j < h; j++)
            swap(p[i][j], p[j][i]);
    return *this;
}
template<class T>
Matrix< T > Matrix< T >::inversion()
{
    Matrix< T > M1(*this);
        //create Matrix< double > M;
    int height = M1.h;
    int width = M1.w;
    double** A = new double*[h];
    for (int i = 0; i < h; i++)
        A[i] = new double[w];
    for (int i = 0;i < height; i++)
        for (int j = 0; j < width; j++)
            A[i][j] = double(M1.p[i][j]);

    double temp;

    double **E = new double*[height];

    for (int i = 0; i < height; i++)
        E[i] = new double [height];

    for (int i = 0; i < height; i++)
        for (int j = 0; j < height; j++)
        {
            E[i][j] = 0.0;

            if (i == j)
                E[i][j] = 1.0;
        }

    for (int k = 0; k < height; k++)
    {
        temp = A[k][k];

        for (int j = 0; j < height; j++)
        {
            A[k][j] /= temp;
            E[k][j] /= temp;
        }

        for (int i = k + 1; i < height; i++)
        {
            temp = A[i][k];

            for (int j = 0; j < height; j++)
            {
                A[i][j] -= A[k][j] * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }

    for (int k = height - 1; k > 0; k--)
    {
        for (int i = k - 1; i >= 0; i--)
        {
            temp = A[i][k];

            for (int j = 0; j < height; j++)
            {
                A[i][j] -= A[k][j] * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }

    for(int i = 0; i < height; i++)
        delete[] A[i];
    delete[] A;

    Matrix< T > M(height, height);
    for (int i = 0;i < height; i++)
        for (int j = 0; j < height; j++)
            M.p[i][j] = T(E[i][j]);

    for (int i = 0; i < height; i++)
        delete [] E[i];
    delete [] E;

    return M;

}

template< class T >
void Matrix< T >::_cin()
{
    for( int i = 0; i < h; i++ )
        for (int j = 0; j < w; i++)
            cin >> p[i][j];
}
template< class T >
void Matrix< T >::_cout()
{
    for( int i = 0; i < h; i++ )
    {
        for( int j = 0; j < w; j++ )
            cout << p[i][j] << " ";
        cout << endl;
    }
}

int main()
{
/**
    int x, y;
    cin >> x >> y;
    Matrix < int > M( x, y );
    M._rand();
    M._cout(); cout << "\n";


    Matrix < int > N( x, y );
    N._rand();
    N._cout(); cout << "\n";


    Matrix < int > Z = M + N;
    Z._cout(); cout << "\n";
    Z = M - N;
    Z._cout(); cout << "\n";
    Z = M * N;
    Z._cout(); cout << "\n";

*/

    int x, y;
    cin >> x >> y;
    Matrix < double > M( x, y );
    M._rand();
    M._cout();
    cout << "\n";


    if (x == y)
    {
        double tr = M.track();
        cout << "track = " << tr << endl;
        Matrix <double> K(M);
        double det = K.det();
        cout << "det(M) = " << det << endl;

        Matrix <double> N(M);
        N.tran();
        N._cout();

        Matrix <double> L(M);
        Matrix <double> B = L.inversion();
        B._cout();
    }
    else
        cout << "This matrix is not square";

/** M.p[0][0] = 9;
    M.p[0][1] = 0;
    M.p[1][0] = 10;
    M.p[0][0] = 0;
    int det = M.det();
    cout << "det(M) = " << det << endl;
*/



    return 0;
}
