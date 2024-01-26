#include <iostream>
#include <atomic>
#include <chrono>
#include <Windows.h>

const int THREAD_COUNT = 4;
const double accuracy = 0.00001;
const double A = -1;
const double B = 5;

std::atomic_flag flag = ATOMIC_FLAG_INIT;
double square = 0;

struct ThreadParam {
	double A, B, accuracy;
};

double func(double x) {
	return exp(-x) * atan(x);
}

DWORD calculate(LPVOID lpVoid) {
	ThreadParam* param = (ThreadParam*)lpVoid;
	double prev, curr = 0;
	int stepsCount = 2;
	curr = 0;
	do {
		prev = curr;
		curr = 0;
		double step = (param->B - param->A) / stepsCount;

		double x = param->A;
		while (x < param->B) {
			curr += func(x);
			x += step;
		}
		curr *= step;

		stepsCount *= 2;
	} while (abs(curr - prev) > param->accuracy / 2 / THREAD_COUNT);
	
	while (flag.test_and_set(std::memory_order_acquire));
	square += curr;
	flag.clear();

	return 0;
}

int main() {
	HANDLE* threads = new HANDLE[THREAD_COUNT];

	int i = 0;
	double step = (B - A) / THREAD_COUNT;
	for (double x = A; x < B; x += step) {
		ThreadParam* param = new ThreadParam{ x, x + step, accuracy };
		threads[i] = CreateThread(NULL, 0, calculate, param, CREATE_SUSPENDED, 0);
		i++;
	}

	std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();

	for (i = 0; i < THREAD_COUNT; i++) {
		ResumeThread(threads[i]);
	}
	WaitForMultipleObjects(THREAD_COUNT, threads, true, INFINITE);

	std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	for (i = 0; i < THREAD_COUNT; i++) {
		CloseHandle(threads[i]);
	}

	std::cout << "Threads: " << THREAD_COUNT << "; Square: " << square << "; Time: " << time.count() << " milliseconds";

	delete[] threads;
	return 0;
}