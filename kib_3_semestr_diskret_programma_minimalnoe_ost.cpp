#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <queue>

using namespace std;

vector<int> path;
vector<int> superpath;
vector<int> result;
vector<bool> visited;
vector< vector< int > > matrix;

int N = 0;
const int inf = 0x7fffffff;
int node = 0;
int Temp = 0;
int prov = 1;
int x = inf;
int y = inf;

void load_data()
{
	ifstream fin("mod.txt");

	if(!fin)
		throw runtime_error("Can't open file");

	fin >> N;

    superpath.resize(N);
    result.resize(N);
    path.resize(N);
	visited.resize(N);  //обработанные вершины
	matrix.resize(N);   //матрица

	for(int i = 0; i < N; ++i)  //устанавливаем в каждой вершине цену inf
		path[i] = inf;

	for(int i = 0; i < N; ++i)
		for(int k = 0; k < N; ++k)
			matrix[i].push_back(inf); //по каждому ребру стоимость проезда составляет inf

	int from, to, cost;
	while(fin >> from >> to >> cost)    //а может и не по каждому
		matrix[from][to] = cost;
}


void run_mod()
{
    while(prov == 1)
    {
        prov = 0;	//если все вершины посетили, то этой переменной присвоим 1 и выйдем
        int mn = inf;
        for (int i = 0; i < N; i++)	//пробегаемся по всем вершинкам, которые посетили
        {
            if(visited[i])	//если она посещена
            {
                for (int k = 0; k < N; k++)	//то
                {
                    if(mn > matrix[i][k] && visited[k] != 1)	//если следующая (каждая) не посещена
                    {	//и эта вершина стоит меньше значения в нашей перемнной
                        x = i;	//запоминаем этот путь
                        y = k;
                        mn = matrix[i][k];	//и самая минимальная цена теперь равна этому
                    }
                }
            }
        }
        matrix[x][y] = inf;
        visited[y] = 1;	//куда пришли, отмечаем что мы там были
        path[Temp] = x;	//записываем откуда пришли
        superpath[Temp] = y;	//записываем куда пришли

        for (int i = 0; i < N; ++i)
        {
            if (visited[i] == 0) prov = 1;
        }
        ++Temp;
    }
}

int main()
{
	load_data();

    visited[0] = 1;
	run_mod();
    cout << "Минимальное остовное дерево: " << endl;
    for (int i = 0; i < N-1; ++i)
    cout << path[i] << "-" << superpath[i] << ";" << endl;
}

