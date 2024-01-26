#include <string>
#include <chrono>
#include <cmath>
#include <vector>
#include <atomic>
#include <mutex>
#include <fstream>
#include <Windows.h>

double func(double x);
DWORD WINAPI calculate(LPVOID lpParam);
double* launch();

struct ThreadParam {
	double a, b;
};

int numResults = 10;

double start = -2, end = 5;
double acc = 0.00001;

std::atomic_flag flag = ATOMIC_FLAG_INIT;

double square = 0; 
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 700;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void DrawGraph(HDC hdc, int width, int height, double* values, int size);
HBRUSH hBrushEllipse;
HWND hwnd;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	const wchar_t* className = L"GraphWindowClass";

	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = className;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	if (!RegisterClass(&wc)) {
		MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
		0,
		className,
		L"Лабораторная работа 3",
		WS_OVERLAPPEDWINDOW,
		30, 30, 
		WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL, NULL, hInstance, NULL
	);

	if (hwnd == NULL) {
		MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

bool needRedraw = true;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_CREATE:
	{
		LONG style = GetWindowLong(hwnd, GWL_STYLE);
		SetWindowLong(hwnd, GWL_STYLE, style & ~WS_SIZEBOX);
		hBrushEllipse = CreateSolidBrush(RGB(255, 0, 0));

	}
	break;
	case WM_DESTROY:
	{
		DeleteObject(hBrushEllipse);
		PostQuitMessage(0);
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		RECT clientRect;
		GetClientRect(hwnd, &clientRect);

		double* values = launch();

		DrawGraph(hdc, clientRect.right, clientRect.bottom, values, numResults);

		EndPaint(hwnd, &ps);
	}
	break;
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* minMaxInfo = reinterpret_cast<MINMAXINFO*>(lParam);
		minMaxInfo->ptMinTrackSize.x = minMaxInfo->ptMaxTrackSize.x = WINDOW_WIDTH;;
		minMaxInfo->ptMinTrackSize.y = minMaxInfo->ptMaxTrackSize.y = WINDOW_HEIGHT;
	}
	break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}
void DrawGraph(HDC hdc, int width, int height, double* values, int size) {
	RECT rect = { 0, 0, width, height };
	FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

	int margin = 40;

	double minValue = 0;
	double maxValue = *std::max_element(values, values + size);

	int graphWidth = width - 2 * margin;
	int graphHeight = height - 2 * margin;

	MoveToEx(hdc, 0, graphHeight + margin, NULL);
	LineTo(hdc, 2 * margin + graphWidth, graphHeight + margin);
	MoveToEx(hdc, margin, 0, NULL);
	LineTo(hdc, margin, 2 * margin + graphHeight);

	// Добавление меток на оси X
	for (int i = 0; i <= size; ++i) {
		int x = margin + static_cast<int>(graphWidth * i / size) + 5;

		int y = graphHeight + margin;

		std::wstring label = std::to_wstring(i);
		TextOut(hdc, x - 10, y + 10, label.c_str(), label.length());
	}

	// Добавление меток на оси Y
	for (int i = 0; i <= 10; ++i) {
		double yValue = minValue + (maxValue - minValue) * i / 10;

		int x = margin - 10;

		int y = margin + static_cast<int>(graphHeight * (1 - (yValue - minValue) / (maxValue - minValue)));
		std::wstring label = std::to_wstring(static_cast<int>(yValue));
		TextOut(hdc, x - 20, y - 10, label.c_str(), label.length());
	}

	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrushEllipse);

	for (int i = 0; i < size; ++i) {
		double normalizedValue = (values[i] - minValue) / (maxValue - minValue);
		int x = margin + static_cast<int>(graphWidth * static_cast<double>(i + 1) / size);
		int y = margin + static_cast<int>(graphHeight - normalizedValue * graphHeight);

		Rectangle(hdc, x - 5, y, x + 5, WINDOW_HEIGHT - 2 * margin);
	}
	SelectObject(hdc, hOldBrush);
}

double func(double x) {
	return exp(-x) * atan(x);
}

DWORD WINAPI calculate(LPVOID lpParam) {
	ThreadParam* td = (ThreadParam*)lpParam;
	double prev_sum, new_sum = 0;
	int n = 2;
	double h;
	do {
		prev_sum = new_sum;
		new_sum = 0;
		h = (td->b - td->a) / n;
		for (double x = td->a; x < td->b; x += h) {
			new_sum += func(x);
		}
		new_sum *= h;
		n *= 2;
	} while (abs(new_sum - prev_sum) > acc / 2 / numResults);

	while (flag.test_and_set(std::memory_order_acquire));
	square += new_sum;
	flag.clear();

	return 0;
}

double* launch() {
	double* results = new double[numResults];
	std::ofstream on("results.txt");
	for (int i = 1; i <= numResults; ++i) {
		int numThreads = i;
		square = 0;
		double step = (end - start) / numThreads;
		HANDLE* threads = new HANDLE[numThreads];
		for (int i = 0; i < numThreads; ++i) {
			double a = start + i * step, b = a + step;
			ThreadParam* td = new ThreadParam{ a, b };
			threads[i] = CreateThread(NULL, 0, calculate, td, CREATE_SUSPENDED, NULL);
		}

		std::chrono::system_clock::time_point tp1 = std::chrono::system_clock::now();
		for (int i = 0; i < numThreads; i++) {
			ResumeThread(threads[i]);
		}
		WaitForMultipleObjects(numThreads, threads, true, INFINITE);
		/*for (int i = 0; i < numThreads; i++) {
			WaitForSingleObject(threads[i], INFINITE);
		}*/
		int time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - tp1).count();
		results[i - 1] = time;

		for (int i = 0; i < numThreads; i++) {
			CloseHandle(threads[i]);
		}
		delete[] threads;

		on << "Number of threads: " << i << "; Time: "  << time << "; Square: " << square << std::endl;
	}

	return results;
}