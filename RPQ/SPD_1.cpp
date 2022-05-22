#include <iostream>
#include <fstream>

using namespace std;

struct task
{
	int id;
	int r;
	int p;
	int q;
};

void sequence(task array[], int size)
{
	cout << "Sequence of tasks:" << endl;
	for (int i = 0; i < size; i++)
	{
		cout << array[i].id << " ";
	}
	cout << endl;
}

int length(task array[], int size)
{
	int m = 0, c = 0;
	for (int i = 0; i < size; i++)
	{
		m = max(m, array[i].r) + array[i].p;                        
		c = max(c, m + array[i].q); 
	}
	return c;
}

void change(task arr[], int a, int b)
{
	task tmp;
	tmp.id = arr[a].id;
	tmp.r = arr[a].r;
	tmp.q = arr[a].q;
	tmp.p = arr[a].p;
	arr[a].id = arr[b].id;
	arr[a].r = arr[b].r;
	arr[a].p = arr[b].p;
	arr[a].q = arr[b].q;
	arr[b].id = tmp.id;
	arr[b].r = tmp.r;
	arr[b].p = tmp.p;
	arr[b].q = tmp.q;
}

void setting(task array[], int size)
{
	int length1 = length(array, size);
	cout << "(123):   " << length1 << endl;
	int length2;
	for (int k = 0; k < 10; k++)
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				change(array, i, j);
				length2 = length(array, size);
				if (length2 > length1)
				{
					change(array, i, j);
				}
				else
				{
					length1 = length2;
				}
			}
		}
	}
}

int main()
{
	string s;
	string data_names[4] = {"data.1", "data.2", "data.3", "data.4"};
	int n, total_time = 0;
	int arr[4];
	task array[100];
	ifstream load("data.txt");

	for (int i = 0; i < 4; i++)
	{
		cout << "Dataset: " << i + 1 << endl;

		while (s != data_names[i])
		{
			load >> s;
		}

		load >> n;

		for (int i = 0; i < n; i++)
		{
			array[i].id = i + 1;
			load >> array[i].r;
			load >> array[i].p;
			load >> array[i].q;
		}

		setting(array, n);
		arr[i] = length(array, n);
		cout << "Carlier: " << arr[i] << endl;
		total_time += length(array, n);
		sequence(array, n);
		cout << endl;
	}

	cout << "Printing length of each dataset: " << endl;
	for (int i = 0; i < 4; i++)
	{
		cout << "Length of " << i + 1 << " dataset: " << arr[i] << endl;
	}

	cout << endl;
	cout << "Overall duration: " << total_time << endl;
	load.close();
}