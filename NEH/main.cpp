#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>
using namespace std;

#define DATA_LENGTH 121

int cmax(int N, int M, int *P, int *X) {    // funkcja licząca długość wykonywania zadań
    int T[100];
    for (int m = 0; m <= M; m++) {
        T[m] = 0;
    }
    for (int n = 0; n < N; n++) {
        for (int m = 1; m <= M; m++) {
            T[m] = max(T[m],T[m-1]) + P[(m-1)+X[n]*M];
        }
    }
    return T[M];
}

void sort_by_time(int N, int M, int* P, int* X) {
    int W[1000] = {};                       // inicjalizacja tablicy czasów zerami
    for (int c = 0; c < N; c++) {           // przypisanie wagi każdemu zadaniu
        for (int d = 0; d < M; d++) {       // dla kazdej maszyny
            W[c] += P[c * M + d];           // dla zadania * maszyny
        }
    }
    for (int b = 0; b < N - 1; b++)	{        // sortowanie zadań wagami
        for (int a = 0; a < N - 1; a++) {
            if (W[a] < W[a + 1]) {           // sortowanie bąbelkowe
                swap(W[a], W[a + 1]);
                swap(X[a], X[a + 1]);
            }
        }
    }
}

int NEH(int N, int M, int* P, int* X) {
    sort_by_time(N,M,P,X);
    for (int n = 0; n < N; n++)	{	         // wstawianie na odpowiednią pozycję
        int bestP = -1, bestCmax = INT32_MAX;
        for (int p = n; p >= 0; p--) {
            int tmp = cmax(n+1, M, P, X); // liczenie cmax dla określonej kombinacji
            if (bestCmax >= tmp) {           // jeżeli znalazł się lepszy
                bestCmax = tmp;              // zapamiętaj wartość
                bestP = p;                   // zapamiętaj indeks
            }
            if (p) {                         // dopóki nie wszystkie kombinacje zostały sprawdzone
                swap(X[p], X[p - 1]); // zamień poprzedni z kolejnym
            }
        }
        for (int i = 0; i < bestP; i++) {    // zamień tyle razy ile było przy najlepszym wyniku
            swap(X[i], X[i + 1]);
        }
    }
    return cmax(N, M, P, X);
}

int main() {
    int N, M, P[10000],X[1000];
    int x;
    string s, s2;
    ifstream f("neh_data.txt");          // otworzenie strumienia wczytywania danych z pliku
    clock_t start = clock();
    clock_t end;
    for(int i = 0; i < DATA_LENGTH; i++) {
        s = "data.";
        if (i < 10) {                       // budowanie stringa tytułowego
            s += "00" + to_string(i) + ":";
        }
        else if (i < 100) {
            s += "0" + to_string(i) + ":";
        }
        else {
            s += to_string(i) + ":";
        }
        while (s2 != s)	{		            // szukanie odpowiedniego zbioru danych
            f >> s2;
        }
        f >> N >> M;						// wczytanie ilości zadań oraz ilości maszyn
        for (int j = 0; j < N * M; j++)	{	// wczytywanie parametrów kolejnych zadań
            f >> P[j];
        }
        for (int j = 0; j < N; j++) {
            X[j] = j;                       // wypełnienie wektora numerów zadań
        }
		 auto start1 = chrono::high_resolution_clock::now();
         x = NEH(N, M, P, X);
		 auto end1 = chrono::high_resolution_clock::now();
         auto duration1 = chrono::duration_cast<chrono::nanoseconds>(end1 - start1);
		 cout << s <<" cmax: "<< x << " czas: "<< duration1.count() * 1e-9 << "s" <<endl;
         for(int i = 0; i < N; i++) {       // wyświetlenie kolejności zadań po ułożeniu
             cout << X[i] + 1 << " ";
         }
         cout << endl;
    }
    end = clock();
    double duration = (end - start)/CLOCKS_PER_SEC;
    cout << "Czas dzialania programu: "<< duration << "s" << endl;
    f.close();			                    // zamknięcie strumienia wczytywania danych z pliku
}