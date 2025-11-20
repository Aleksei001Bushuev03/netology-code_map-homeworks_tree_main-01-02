#include<iostream>	
#include<vector>
#include<thread>
#include<chrono>
#include<string>
#include<iomanip>
#include <sstream>

using namespace std;
using namespace std::chrono;

void partial_sum(const vector<int>& a, const vector<int>& b, vector<int>& result,
	size_t start, size_t end, bool first_thread = false)
{
	
	for (size_t i = start; i < end; ++i)
	{
		result[i] = a[i] + b[i];
	}
}

double parallel_sum(size_t size, int num_threads)
{
	vector<int> a(size, 1);
	vector<int> b(size, 2);
	vector<int> result(size, 0);

	vector<thread>threads;
	threads.reserve(num_threads);

	size_t chunk = size / num_threads;
	auto start_time = high_resolution_clock::now();

	for (int i = 0; i < num_threads; ++i)
	{
		size_t begin = i * chunk;
		size_t end = (i == num_threads - 1) ? size : (i + 1) * chunk;
		threads.emplace_back(partial_sum, cref(a), cref(b), ref(result), begin, end, i == 0);
	}
	for (auto& th : threads)
	{
		th.join();

	}
	auto end_time = high_resolution_clock::now();
	duration<double, milli>elapsed = end_time - start_time;
	return elapsed.count();
}

int main()
{
	setlocale(LC_ALL, "ru");

	cout << "Доступно аппаратных ядер: " << thread::hardware_concurrency() << endl<<endl;


	vector<size_t> sizes = { 1000, 10000, 100000, 1000000 };
	vector<int> threads = { 2, 4, 8, 16 };

	cout << "\nТаблица времени выполнения (мс):\n";
	cout << left << setw(15) << "Размер массива";

	cout << setw(10) << "";
	for (size_t size : sizes)
	{
		cout << setw(12) << size;
	}
	cout << endl;

	cout << string(10 + sizes.size() * 12, '-') << endl;

	for (int t : threads)
	{
		cout << setw(10) << (to_string(t) + "потоков");
		for (size_t size : sizes)
		{
			double time_s = parallel_sum(size, t);

			std::ostringstream oss;
			oss  << fixed << setprecision(7) << time_s << "s";

			cout << setw(12) << oss.str();
		}
		cout << endl;
	}

	cout << "\nРабота завершена." << endl;
	return 0;
}