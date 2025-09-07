#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <queue>
#include <math.h>

using namespace std;

vector<bool> visited;
vector< vector< double > > matrix;
vector< vector< double > > transport;
vector< vector< double> > M;

int L = 0;
int N = 0;

void load_data()
{
	ifstream fin("kod2.txt");

	if(!fin)
		throw runtime_error("не открывается");

	fin >> L >> N; //размер матрицы
	//--L;
	matrix.resize(L);   //изменяем размер матрицы по строчкам
	M.resize(L);

	for(int x = 0; x < L; ++x)  //X = 1..N
		matrix[x].resize(N);    //изменяем размер матрицы по столбцам


    for(int x = 0; x < L; ++x)
    {
        M[x].resize(L);
    }

    for(int x = 0; x < L; ++x)
    for(int y = 0; y < L; ++y)
        M[x][y] = 0;

	for(int x = 0; x < L; ++x)  //X = 1..N
		for(int y = 0; y < N; ++y)  //Y = 1..N
		{
			fin >> matrix[x][y];    //заполняем
        }

    cout << '\n'<< '\n' << "Матрица:";
    for (int x = 0; x < L; ++x)
    {
        cout << endl;
        for (int y = 0; y < N; ++y)
            cout << " " << matrix[x][y] << " ";
    }
    --L;
}

void kira(vector< vector< double > > &matrix)  //Сводим к матрице Кирхгофа
{
  /*  cout << "Изначальная матрица:";
    for (int x = 0; x < N; ++x)
    {
        cout << endl;
        for (int y = 0; y < M; ++y)
            cout << matrix[x][y] << " ";
    }*/
    /* Меняем знак */
    /*    cout << '\n'<< '\n' << "Start:";
    for (int x = 0; x < L; ++x)
    {
        cout << endl;
        for (int y = 0; y < N; ++y)
            cout << matrix[x][y] << " ";
    }*/

    for (int y = 0; y < N; ++y)
    {
        for (int x = 0; x < L; ++x)
            if (matrix[x][y] == 1)
            {
                matrix[x][y] = -1;
                break;
            }
    }

    /*cout << '\n'<< '\n' << "Finish:";
    for (int x = 0; x < L; ++x)
    {
        cout << endl;
        for (int y = 0; y < N; ++y)
            cout << matrix[x][y] << " ";
    }*/
    /*.............*/
    /* Перемножаем */
    for (int x = 0; x < L; ++x)
    for (int y = 0; y < L; ++y)
    {
        for (int i = 0; i < N; ++i)
            M[x][y] += matrix[x][i] * matrix[y][i];
    }
    /*............*/
	/*int temp = 0;
	for(int x = 0; x < N; ++x)
	{
		for(int y = 0; y < N; ++y)
		{
			temp += matrix[x][y];
			if (y != x) matrix[x][y] = -matrix[x][y];
			break;
		}

		for(int y = 0; y < N; ++y)
			if(x == y)
			{
				matrix[x][y] = temp;
				temp = 0;
			}
	}*/

    cout << '\n'<< '\n' << "Матрица Киргофа:";
    for (int x = 0; x < L; ++x)
    {
        cout << endl;
        for (int y = 0; y < L; ++y)
            cout << M[x][y] << " ";
    }
   // N = N - 1;
}

double opr(vector< vector< double > > &matrix)
{
    double det = 1;
    N = L;
    for (int j = 0; j < N; ++j)
    {
        for (int i = j+1; i < N; ++i)
        {
            if (matrix[j][j] == 0)
            {
                det = 0;
                break;
            }
            double temp = matrix[i][j] / matrix[j][j];

            for (int k = 0; k < N; ++k)
            {
                matrix[i][k] -= temp*matrix[j][k];
            }
        }
        det *= matrix[j][j];
    }

    return det;
}

int main()
{
	load_data();

	kira(matrix);

    cout << '\n' << '\n' << "Количество остовных деревьев: " << opr(M) << endl;
}

