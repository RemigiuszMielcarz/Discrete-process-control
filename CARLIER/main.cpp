#include <iostream>
#include<fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
struct task
{
    int r;
    int p;
    int q;
};

istream& operator >> (istream& enter, task& work)
{
    enter >> work.r >> work.p >> work.q;
    return enter;
}

int schr(int n, task *T, int *O)
{
    vector<int> UnavailableTasks;
    vector<int> AvailableTasks;

    int ut = n,     // number of unavailable tasks
    a = 0,     // number of available tasks
    d = 0,	    // number of done tasksw
    t = 0,     // current unit of time
    cmax = 0;	// the length of the longest row

    for (int i = 0; i < n; i++)
    {
        UnavailableTasks.push_back(i); // initialization with job numbers
        AvailableTasks.push_back(0);   // empty vector initialization
    }

    stable_sort(UnavailableTasks.begin(), UnavailableTasks.end(), [&](const int & a, const int & b) { return (T[a].r > T[b].r); });

    while (d != n)	                                       // if there are any incomplete tasks
    {
        if (ut != 0 && T[UnavailableTasks[ut - 1]].r <= t) // if there are still tasks unavailable and the time is right
        {
            AvailableTasks[a++] = UnavailableTasks[--ut];

            for (int i = a - 1; i > 0; i--)			       // sort the array of available tasks in ascending order of q
                if (T[AvailableTasks[i]].q < T[AvailableTasks[i - 1]].q) swap(AvailableTasks[i], AvailableTasks[i - 1]);
        }
        else if (a != 0)			                       // if there are jobs available
        {
            O[d] = AvailableTasks[--a];                    // assign the job number to the output vector
            t += T[O[d]].p;
            cmax = max(cmax, t + T[O[d++]].q);
        }

        else if (a == 0 && T[UnavailableTasks[ut - 1]].r > t) t = T[UnavailableTasks[ut - 1]].r; // if there is a break, skip it
    }
    return cmax;
}

int schr_div(int n, task* T)
{
    vector<int> UnavailableTasks;
    vector<int> AvailableTasks;
    vector<int> tmp;

    int ut = n,
            a = 0,
            t = 0,
            position = 50,
            cmax = 0,
            done = 0;

    for (int i = 0; i < n; i++)
    {
        tmp.push_back(T[i].p);
        UnavailableTasks.push_back(i);
        AvailableTasks.push_back(0);
    }

    stable_sort(UnavailableTasks.begin(), UnavailableTasks.end(), [&](const int & a, const int & b) { return (T[a].r > T[b].r); });

    while (ut != 0 || a!= 0)
    {
        if (ut != 0 && T[UnavailableTasks[ut-1]].r <= t)
        {
            AvailableTasks[a++] = UnavailableTasks[--ut];

            for (int i = a - 1; i > 0; i--)
            {
                if (T[AvailableTasks[i]].q < T[AvailableTasks[i - 1]].q) swap(AvailableTasks[i], AvailableTasks[i - 1]);
            }
            if (position != 50 && T[AvailableTasks[a - 1]].q > T[position].q)
            {
                AvailableTasks[a++] = position;
                swap(AvailableTasks[a - 1], AvailableTasks[a - 2]);
                position = 50;
            }
        }
        else if (a != 0)
        {
            if (position == 50) position = AvailableTasks[--a];
            if (ut != 0) done = min(tmp[position], T[UnavailableTasks[ut - 1]].r - t);
            else done = tmp[position];

            t += done;

            if((tmp[position] -= done) == 0)
            {
                cmax = max(cmax, t + T[position].q);
                position = 50;
            }
        }
        else if (T[UnavailableTasks[ut - 1]].r > t) t = T[UnavailableTasks[ut - 1]].r;
    }
    return cmax;
}

void block(int n, task *T, int* O, int& ic, int& rc, int& qc)
{
    vector<int> tmp;
           int  pB = -1,
                k = 0,
                cmax = 0;

    for (int i = 0; i < n; i++)
    {
        int j = O[i];
        tmp.push_back(k >= T[j].r);
        k = max(k, T[j].r) + T[j].p;
        if (cmax < k + T[j].q)
        {
            cmax = k + T[j].q;
            pB = i;
        }
    }
    int i = pB,
        j = -1,
        qb = T[O[pB]].q,
        rb = T[O[pB]].r,
        pb = T[O[pB]].p;

    while (tmp[i])
    {
        if (T[O[--i]].q < qb)
        {
            j = O[i];
            break;
        }
        rb = min(rb, T[O[i]].r);
        pb += T[O[i]].p;
    }
    ic = j;
    rc = rb + pb;
    qc = qb + pb;
}

void carlier(int n, task *T, int* O, int& UB)
{
    if (schr_div(n, T) >= UB) return;

    int cmax = schr(n, T, O); if (cmax < UB) UB = cmax;

    int j,
        rj,
        qj;

    block(n, T, O, j, rj, qj);

    if (j < 0) return;

    int tmp_r = T[j].r,
        tmp_q = T[j].q;

    T[j].r = rj;

    carlier(n, T, O, UB);

    T[j].r = tmp_r;
    T[j].q = qj;

    carlier(n, T, O, UB);

    T[j].q = tmp_q;
}

int main()
{
    string names[9] = {"data.000:",
                       "data.001:",
                       "data.002:",
                       "data.003:",
                       "data.004:",
                       "data.005:",
                       "data.006:",
                       "data.007:",
                       "data.008:"};

    ifstream data("carlier.data.txt");
    string buf;

    task T[50];
    int O[50];
    int n;
    for (int i = 0; i < 9; i++)
    {
        while (buf!=names[i]) data>>buf;

        data>>n;

        for (int k = 0; k < n; k++) data >> T[k]; // loading the parameters of subsequent tasks

        int UB = schr(n, T, O);
        carlier(n, T, O, UB);

        cout << names[i] << endl  << "schrpmtn: " << schr_div(n, T) << endl << "schr: " << schr(n, T, O) << endl << "carlier: " << UB << endl << "Order: ";
        for (int i = 0; i < n; i++) cout << O[i] << " ";
        cout << endl << endl;
    }
}