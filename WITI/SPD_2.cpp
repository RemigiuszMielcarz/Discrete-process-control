#include <iostream>
#include <fstream>

using namespace std;

struct task
{
    int time;
    int weight;   // penalty factor for late task
    int deadline; // requested date for completion of the task
};

int main()
{
    task array[100];
    ifstream load("witi.data.txt");
    int n, c;
    string s;
    string data_names[11] = {"data.10:", "data.11:", "data.12:", "data.13:", "data.14:", "data.15:", "data.16:", "data.17:", "data.18:", "data.19:", "data.20:"};

    for (int iter = 0; iter < 11; iter++)
    {
        while (s != data_names[iter])
        {
            load >> s;
        }

        cout << "Dataset: " << s << endl;

        load >> n;

        for (int i = 0; i < n; i++)
        {
            load >> array[i].time >> array[i].weight >> array[i].deadline;
        }

        int N = 1 << n;
        int *F = new int[N];
        F[0] = 0;
        for (int set = 1; set < N; set++)
        {
            c = 0;
            for (int i = 0, b = 1; i < n; i++, b *= 2)
            {
                if (set & b)
                {
                    c += array[i].time;
                }
            }
            F[set] = 999999;
            for (int k = 0, b = 1; k < n; k++, b *= 2)
            {
                if (set & b)
                {
                    F[set] = min(F[set], F[set - b] + array[k].weight * max(c - array[k].deadline, 0));
                }
            }
        }

        std::cout << "Optimal solution: " << F[N - 1] << endl;
        delete[] F;
    }

    load.close();
    std::cout << endl
              << "The end of program." << endl;
}