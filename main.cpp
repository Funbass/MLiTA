#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

vector<int> T;    // Суммы сегментов дерева
vector<int> Add;  // Ленивая модификация

// Построение дерева отрезков
void Build(int i, int L, int R) {
    if (L == R) {
        T[i] = 0; // Изначально сугробов нет
    } else {
        int m = (L + R) / 2;
        Build(2 * i, L, m);
        Build(2 * i + 1, m + 1, R);
        T[i] = T[2 * i] + T[2 * i + 1];
    }
}

// Применение отложенных операций
void ApplyLazy(int i, int Tl, int Tr) {
    if (Add[i] != 0) {
        T[i] += Add[i] * (Tr - Tl + 1);
        if (Tl != Tr) {
            Add[2 * i] += Add[i];
            Add[2 * i + 1] += Add[i];
        }
        Add[i] = 0;
    }
}

// Обновление интервала
void Plus(int L, int R, int X, int i, int Tl, int Tr) {
    ApplyLazy(i, Tl, Tr);

    if (L > Tr || R < Tl) return;
    if (L <= Tl && Tr <= R) {
        Add[i] += X;
        ApplyLazy(i, Tl, Tr);
    } else {
        int m = (Tl + Tr) / 2;
        Plus(L, R, X, 2 * i, Tl, m);
        Plus(L, R, X, 2 * i + 1, m + 1, Tr);
        T[i] = T[2 * i] + T[2 * i + 1];
    }

    // Убедиться, что высота не отрицательна
    T[i] = max(0, T[i]);
}

// Вычисление суммы на заданном интервале
int Sum(int L, int R, int i, int Tl, int Tr) {
    ApplyLazy(i, Tl, Tr);

    if (L > Tr || R < Tl) return 0;
    if (L <= Tl && Tr <= R) return T[i];

    int m = (Tl + Tr) / 2;
    return Sum(L, R, 2 * i, Tl, m) + Sum(L, R, 2 * i + 1, m + 1, Tr);
}

int main() {
    std::ifstream inputFile("../INPUT.TXT");
    std::ofstream outputFile("../OUTPUT.TXT");

    if (!inputFile.is_open() || !outputFile.is_open()) {
        cout << "File could not be opened" << endl;
        exit(1);
    }
    int N, M;
    inputFile >> N >> M;

    if (N < 1 || N > 100000) {
        outputFile << "Error: the number of supports N should be in the range from 1 to 100000." << endl;
        exit(2);
    }

    if (M < 1 || M > 100000) {
       outputFile << "Error: The number of M commands should be in the range from 1 to 100000." << endl;
        exit(2);
    }


    // Инициализация дерева отрезков для N-1 интервалов
    T.resize(4 * (N - 1), 0);
    Add.resize(4 * (N - 1), 0);
    Build(1, 1, N - 1);

    for (int q = 0; q < M; ++q) {
        int type;
        inputFile >> type;

        if (type == 1) {
            int L, R, S;
            inputFile >> L >> R >> S;
            if (L < R && R <= N && 1 <= L && -100 <= S && 100 >= S) {
                Plus(L, R - 1, S, 1, 1, N - 1); // Применяем изменение для интервалов
            } else {
                outputFile << "Incorrect input." << endl;
                exit(3);
            }
        } else if (type == 2) {
            int L, R;
            inputFile >> L >> R;
            outputFile << Sum(L, R - 1, 1, 1, N - 1) << endl;
        }
    }

    return 0;
}
