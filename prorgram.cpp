#include <iostream>
#include <fstream>
#include <queue>
#include <Windows.h>
#include <chrono>
#include "PointCollection.h"
#include "geometry.h"
#include "geometryio.h"

const bool WRITE_TO_FILE = false;
const int THREAD_COUNT = 2;
const int POINTS_COUNT = 300;

struct ThreadParam {
	std::atomic_flag* flag;
	std::vector<Triangle>* triangles;
	PointCollection* points;
	int iStart, iEnd;
};

void OneThread(bool write = false) {
	std::cout << "One thread" << std::endl;
	PointCollection* points = readPoints("points.txt");

	std::vector<Triangle> Triangles = std::vector<Triangle>();
	std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();

	int n = points->size();
	for (int i = 0; i < n - 2; i++) {
		for (int j = i + 1; j < n - 1; j++) {
			for (int k = j + 1; k < n; k++) {
				Triangle tr{ points->getPoint(i), points->getPoint(j), points->getPoint(k) };
				if (isTriangleObtuse(&tr)) {
					setTriangleValues(&tr);
					Triangles.push_back(tr);
				}
			}
		}
	}

	std::cout << "Combinations founded" << std::endl;

	std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	
	std::cout << "Total time is " << duration.count() << " milliseconds" << std::endl;
	if (write) {
		writeTriangles("one thread.txt", Triangles.data(), Triangles.size());
	}
}

DWORD threadFunc(LPVOID lpParam) {
	ThreadParam* param = (ThreadParam*)lpParam;
	auto flag = param->flag;
	auto points = param->points;
	auto triangles = param->triangles;
	int n = param->points->size();

	int step = 0;
	for (int i = param->iStart; i < param->iEnd; i++) {
		for (int j = i + 1; j < n - 1; j++) {
			for (int k = j + 1; k < n; k++) {
				Triangle tr{ points->getPoint(i), points->getPoint(j), points->getPoint(k) };
				if (isTriangleObtuse(&tr)) {
					setTriangleValues(&tr);
					while (flag->test_and_set(std::memory_order_acquire));
					triangles->push_back(tr);
					flag->clear();
				}
			}
		}
	}

	return 0;
}

void ManyThreads(int threadsCount, bool write = false) {
	std::cout << "Many Threads with " << threadsCount << std::endl;
	auto points = readPoints("points.txt");
	int n = points->size();

	std::atomic_flag flag = ATOMIC_FLAG_INIT;

	ThreadParam* params = new ThreadParam[threadsCount];
	HANDLE* threads = new HANDLE[threadsCount];
	DWORD* threadIds = new DWORD[threadsCount];

	std::vector<Triangle> Triangles = std::vector<Triangle>();
	int iStart = 0;
	int iEnd;

	double trianglesAll = (n - 2) * (n - 2) * (n - 2);
	double trianglesToThread = trianglesAll / threadsCount;

	std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < threadsCount; i++)
	{
		params[i].flag = &flag;
		params[i].points = points;
		params[i].triangles = &Triangles;

		params[i].iStart = iStart;
		iEnd = ceil(n - pow(trianglesAll - trianglesToThread * (i + 1), 1 / 3.));
		params[i].iEnd = iEnd;
		iStart = iEnd;
		threads[i] = CreateThread(NULL, 0, threadFunc, &params[i], 0, &(threadIds[i]));
	}
	WaitForMultipleObjects(threadsCount, threads, true, INFINITE);
	for (int i = 0; i < threadsCount; i++) {
		CloseHandle(threads[i]);
	}
	std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	std::cout << "Total time is " << duration.count() << " milliseconds" << std::endl;
	std::cout << "Points: " << points->size() << " Triangles: " << Triangles.size() << std::endl;
	if (write) {
		writeTriangles("many thread " + std::to_string(threadsCount) + ".txt", Triangles.data(), Triangles.size());
	}

	delete[] threadIds;
	delete[] threads;
	delete[] params;
}

int main() {
	generateWritePoints("points.txt", POINTS_COUNT);
	OneThread(WRITE_TO_FILE);
	std::cout << "-----------------------------------" << std::endl;
	ManyThreads(THREAD_COUNT, WRITE_TO_FILE);
	std::cout << "Press any key to continue...";
	getchar();
}