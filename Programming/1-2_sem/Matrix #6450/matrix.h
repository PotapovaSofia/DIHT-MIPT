template <typename T> class Matrix
{
public:
	Matrix(int size1, int size2);
	~Matrix();
	Matrix& operator +(const Matrix &other);
	Matrix& operator -(const Matrix &other);
	Matrix& operator *(const Matrix &other);
	long long track(const Matrix &a);
	long long det(const Matrix &a);
	void trans(Matrix &a);
	void inverse(Matrix &a);
private:
	T** data;
	size_t size1, size2;
};

template <typename T>
Matrix<T>::Matrix(int size1, int size2)
	: size1(size1), size2(size2)
{
	T** data = new T*[size1];
	for(int i = 0; i < size1; ++i)
		data[i] = new T[size2];
	for(int i = 0; i < size1; ++i)
		for(int j = 0; j < size2; ++j)
			data[i][j] = 0;
}

template <typename T>
Matrix<T>::~Matrix()
{
	for(int i = 0; i < size1; ++i)
		delete[] Matrix::data[i];
}

template <typename T>
Matrix<T>& Matrix<T>::operator + (const Matrix<T> &other)
{
	Matrix<T> result(size1, size2);
	if( (other.size1 == size1) || (other.size2 == size2) )
	{
		for(int i = 0; i < size1; ++i)
			for(int j = 0; j < size2; ++j)
				result.data[i][j] = other.data[i][j] + data[i][j];

	} else
		throw cout << "Error in size" << endl;
	return result;
}

template <typename T>
Matrix<T>& Matrix<T>::operator - (const Matrix<T> &other)
{
	Matrix<T> result(size1, size2);
	if( (other.size1 == size1) || (other.size1 == size1) )
	{
		for(int i = 0; i < size1; ++i)
			for(int j = 0; j < size2; ++j)
				result.data[i][j] = data[i][j] - other.data[i][j];

	} else
		throw cout << "Error in size" << endl;
	return result;
}

template <typename T>
Matrix<T>& Matrix<T>::operator * (const Matrix<T> &other)
{
	Matrix<T> result(size1, other.size2);
	if( other.size1 == size2 )
	{
		int i, k, j;
		for(i = 0; i < other.size1; ++i)
			for(j = 0; j < size2; ++j)
			{
				for(k = 0; k < size2; ++k)
					result.data[i][j] = result.data[i][j] + data[i][k] + other.data[k][j];
			}

	} else
		throw cout << "Error in size" << endl;
	return result;
}

template <typename T>
long long track(const Matrix<T> &a)
{
	long long tr = 1;
	if( a.size1 == a.size2 )
	{	
		for(int i = 0; i < a.size1; ++i)
			tr += a.data[i][i];
	} else 
		tr = 0;
	return tr;
}

template <typename T>
long long det(const Matrix<T> &a)
{
	float kst;
	if ( a.size1 == a.size2 )
	{
		if (a.size1 == 1)
			return a.data[0][0];
		int p = 0;
		int n = a.size1;
		int t, i, j;
		for (i = 0; i < n - 1; ++i)
		{
			t = 1;
			while(a.data[i][i] == 0)
			{
				for(j = 0; j < n; ++j)
				{
					a.data[i][j] = kst;
					a.data[i][j] = a.data[i+t][j];
					a.data[i+t][j] = kst;
				}
				++p;
				++t;
			}
 
			for (int k = i + 1; k < n; ++k)
			{
				kst = a.data[k][i] / a.data[i][i];
				for(j = 0; j < n; ++j)
					a.data[k][j] -= a.data[i][j]*kst;
			}
		}
 
			kst = pow(-1.0, p);
			for(i = 0; i < n; ++i)
			kst *= a.data[i][i];
	} else
		{	cout << "Wrong size to calculate determinant" << endl;
			kst = 0; 
		};
	return kst;

}

template <typename T>
void trans(Matrix<T> &a)
{
	Matrix<T> b(a.size1, a.size2);
		for(int i = 0; i < a.size1; ++i)
			for(int j = 0; j < a.size2; ++j)
				b.data[i][j] = a.data[i][j]; // b = a;

	for(int i = 0; i < a.size1; ++i)
			for(int j = 0; j < a.size2; ++j)
				a.data[i][j] = b.data[j][i];
	a.size1 = b.size2;
	a.size2 = b.size1;
}

template <typename T>
Matrix<T>& inverse(Matrix<T> &a)
{
	float d = det(a);
	Matrix<T> Rez(a.size1, a.size2);
	Matrix<T> minor(a.size1-1, a.size2-1);

	for (int i = 0; i < a.size1; ++i) {        // Строки
		for (int j = 0; j < a.size2; ++j) {	 // Столбцы
			for (int k = 0, m = 0; m < a.size1-1; ++k, ++m) {  // Строки минора в инвертируемой матрице и матрице минора
				if (k == i)
					++k;
				for (int l = 0, n = 0; n < a.size2-1; ++l, ++n) {   // Столбцы минора в инвертируемой матрице и матрице минора
					if (l == j) 
						++l;
					minor.data[m][n] = a.data[k][l];
				}
			}
			if ((i+j)%2 == 0)
				Rez.data[j][i] = det(minor)/d;
			else
				Rez.data[j][i] = -det(minor)/d;
		}
	}
	return Rez;
}