#include <iostream>
#include <fstream>
#include <iomanip>
#include <deque>
#include <deque>
#include <ctime>

using namespace std;


int** readFile(int&);

void printMatrix(int**, int, int);

int algorithm_(int**, int);

int** randGen() {
    int** matrix = new int* [100];
    for (int i = 0;i < 100;++i) {
        matrix[i] = new int[100];
    }
    for (int i = 0;i < 100;++i) {
        for (int j = 0;j < 100;++j) {
            if (i == j) { matrix[i][j] = 0; continue; }
            matrix[i][j] = rand() % 10000 + 1;
        }
    }
    return matrix;
}

int main() {
    setlocale(LC_ALL, "rus");
    int columns = 0;
    int** matrix_ = readFile(columns);
    //int** matrix_ = randGen();
    //columns=100;
    //cout << "Исходная матрица" << endl;
    //printMatrix(matrix_, columns, columns);
    cout << endl;
    cout << "Ответ: "<<algorithm_(matrix_, columns) << endl;
    return 0;
}

int** readFile(int& col) {
    ifstream fin("text1.txt");

    if (fin.is_open())
        cout << "Файл открыт!\n\n" << endl;
    else
    {
        cout << "Файл не открыт!\n\n" << endl;
        return NULL;
    }

    char buffer[60];
    int vertex_count, edges_count;

    fin >> vertex_count;
    vertex_count;

    fin >> edges_count;

    col = vertex_count;

    int** matrix = new int* [vertex_count];
    for (int i = 0;i < vertex_count;++i) {
        matrix[i] = new int[vertex_count];
    }

    for (int i = 0;i < vertex_count;++i) {
        for (int j = 0;j < vertex_count;++j) {
            matrix[i][j] = 0;
        }
    }

    int i, j, flow;
    for (int z = 0;z < edges_count;++z) {
        fin >> i; fin >> j; fin >> flow;
        matrix[i - 1][j - 1] = flow;
    }
    fin.close();

    return matrix;
}

void printMatrix(int** table, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            cout << setw(8)<< table[i][j] << " ";
        }
        cout << endl;
    }
}

int* BFS(int** matrix, int col, int* d, bool type = 1) {
    deque<int> dec;
    if (type == 1) {
        d[col - 1] = 0;
        dec.push_back(col - 1);
    }
    else if (type == 0) {
        d[0] = 0;
        dec.push_back(0);
    }
    int max_value = 2 * col;


    while (!dec.empty()) {
        int i = dec.front();
        dec.pop_front();

        for (int j = 0;j < col;++j) {
            if (matrix[j][i] > 0) {
                if (d[j] == max_value) {
                    d[j] = d[i] + 1;
                    dec.push_back(j);
                }
            }
        }
    }
    return d;
}

void Global_Relabel(int** matrix, int col, int* h) {
    int* d1 = new int[col];
    int* d2 = new int[col];

    int max_value = 2 * col;

    for (int i = 0;i < col;++i) {
        d1[i] = max_value;
        d2[i] = max_value;
    }

    d1 = BFS(matrix, col, d1, 1);
    d2 = BFS(matrix, col, d2, 0);
    for (int i = 0; i < col; ++i) {
        if (d1[i] == max_value) {
            h[i] = d2[i];
        }
        else
            h[i] = d1[i];
    }
}

int HLO(int* h, int* e,int col) {
    int max_h=0, max_e=0, vertex=0;
    for (int i = 1;i < col;++i) {
        if ((h[i] > max_h) && (e[i] > 0)) {
            vertex = i;
            max_e = e[i];
            max_h = h[i];
        }
        if ((h[i] == max_h)&&(e[i]>0)) {
            if (e[i] > max_e) {
                vertex = i;
                max_e = e[i];
            }
        }
    }
    return vertex;
}

void Push(int** matrix, int i, int j, int value, int col, int* e) {
    e[j] += value;
    e[i] -= value;
    matrix[i][j] -= value;
    matrix[j][i] += value;
}


int algorithm_(int** matrix, int col) {
    int** remain_matrix = new int* [col];
    for (int i = 0;i < col;++i) {
        remain_matrix[i] = new int[col];
    }
    for (int i = 0;i < col;++i) {
        for (int j = 0;j < col;++j) {
            remain_matrix[i][j] = matrix[i][j];
        }
    }

    int* h = new int[col];
    int* e = new int[col];

    unsigned int start_time = clock();
    Global_Relabel(remain_matrix, col, h);

    h[0] = col;

    for (int i = 0;i < col;++i) {
        if (remain_matrix[0][i] > 0) {
            e[i] = remain_matrix[0][i];
            remain_matrix[i][0] += remain_matrix[0][i];
            remain_matrix[0][i] = 0;
        }
        else {
            e[i] = 0;
        }
    }

    int operations,chosen_vertex,count=0,j;

    do{
        chosen_vertex = HLO(h, e, col);
        if (chosen_vertex == col-1) break;
        j = 0;
        operations = 1;
        for (int i = 0;i < col;++i) {
            if ((remain_matrix[chosen_vertex][i] > 0) && (h[chosen_vertex] - 1 == h[i])) {
                Push(remain_matrix,chosen_vertex,i,min(remain_matrix[chosen_vertex][i],e[chosen_vertex]),col,e);
                count++;
                operations--;
                break;
            }
        }
        if (operations==1) {
            h[chosen_vertex] += 1;
            count++;
        }

        if (count == 30) {
            Global_Relabel(remain_matrix, col, h);
        }

    } while (chosen_vertex != col-1);
    unsigned int end_time = clock();
    unsigned int search_time = end_time - start_time;
    cout << "Время работы: " << search_time << endl;
    //cout << "Остаточная сеть" << endl;
    //printMatrix(remain_matrix, col, col);
    return e[col - 1];
}

