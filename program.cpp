#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <chrono>
#include <Windows.h>
#include "LinearSystemContext.h"
#include "iomatrix.h"

// структура для передачи параметра в поток
struct ComputeParam {
    // номер СЛАУ
    int ordernum;
    // указатель на массив выполнения
    bool* finished;
    // контекст системы линейных уравнений
    LinearSystemContext* context;
    // общее время
    std::chrono::duration<double>* total;
};

// реализация планировщика с алгоритмом fcfs
double fcfs(int nmatrices) {
    if (!std::filesystem::exists("fcfs")) {
        std::filesystem::create_directory("fcfs");
    }
    
    std::cout << "First Come, First Served" << std::endl;

    std::chrono::duration<double> duration{ 0 };
    for (int i = 0; i < nmatrices; i++) {
        LinearSystemContext* context = readmatrix("matrices/" + std::to_string(i) + ".txt");
        readarr("values/" + std::to_string(i) + ".txt", context);

        std::chrono::time_point start = std::chrono::high_resolution_clock::now();
        context->solveLD();
        std::chrono::time_point end = std::chrono::high_resolution_clock::now();
        writearr("fcfs/" + std::to_string(i) + ".txt", context, true);

        std::chrono::duration<double> localduration = end - start;
        duration += localduration;
    }
    std::cout << "Total time is " << duration.count() << " seconds" << std::endl;
    return duration.count();
}

// функция для решения матрицы в потоке
DWORD WINAPI compute_ld(LPVOID ldParams) {
    ComputeParam* param = (ComputeParam*)ldParams;

    std::chrono::time_point start = std::chrono::high_resolution_clock::now();
    param->context->solveLD();
    std::chrono::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> localduration = end - start;

    *param->total += localduration;
    param->finished[param->ordernum] = true;
    return 0;
}

bool alltrue(bool* arr, int n) {
    bool result = true;
    for (int i = 0; i < n && result; i++) {
        result &= arr[i];
    }
    return result;
}

// реализация планировщика гарантированного планирования
void guaranted(int nmatrices) {
    if (!std::filesystem::exists("guaranted")) {
        std::filesystem::create_directory("guaranted");
    }

    std::cout << "Guaranted planning" << std::endl;

    LinearSystemContext** linearSystems = new LinearSystemContext*[nmatrices];
    ComputeParam** params = new ComputeParam*[nmatrices];

    HANDLE* threads = new HANDLE[nmatrices];
    DWORD* threadIds = new DWORD[nmatrices];
    bool* finished = new bool[nmatrices];
    for (int i = 0; i < nmatrices; i++) {
        finished[i] = false;
    }

    std::chrono::duration<double> total{ 0 };
    for (int i = 0; i < nmatrices; i++) {
        linearSystems[i] = readmatrix("matrices/" + std::to_string(i) + ".txt");
        readarr("values/" + std::to_string(i) + ".txt", linearSystems[i]);

        params[i] = new ComputeParam();
        params[i]->ordernum = i;
        params[i]->finished = finished;
        params[i]->context = linearSystems[i];
        params[i]->total = &total;

        threads[i] = CreateThread(NULL, 0, compute_ld, params[i], CREATE_SUSPENDED, &(threadIds[i]));
    }

    int quant = 30;
    int next = 0;
    while (!alltrue(finished, nmatrices)) {
        ResumeThread(threads[next]);
        WaitForSingleObject(threads[next], quant);
        SuspendThread(threads[next]);
        if (alltrue(finished, nmatrices)) {
            break;
        }

        if (next + 1 == nmatrices) {
            next = 0;
        }
        while (finished[next]) {
            next++;
        }
    }

    for (int k = 0; k < nmatrices; k++) {
        CloseHandle(threads[k]);
    }

    std::cout << "Total time is " << total.count() << " seconds" << std::endl;

    for (int i = 0; i < nmatrices; i++) {
        writearr("guaranted/" + std::to_string(i) + ".txt", linearSystems[i], true);
    }

    delete[] finished;
    delete[] threads;
    delete[] threadIds;
    delete[] params;
    delete[] linearSystems;
}

int main() {
    int n = 10;
    genwritematrices(n);
    fcfs(n);
    guaranted(n);
    std::cout << "Press enter to continue..." << std::endl;
    getchar();
    return 0;
}
