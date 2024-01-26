#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <chrono>
#include <random>
#include <mpi.h>

int equations_count, col_count;
double* ab, * coefficients_buffer;
double* x;

int process_col_count, *col_indices;
double *column, **columns;

MPI_Status status;

void set_column(int column_index) {
    for (int i = 0; i < equations_count; i++) {
        column[i] = ab[i * col_count + column_index];
    }
}

void replace_with_column(int column_index) {
    for (int i = 0; i < equations_count; i++) {
        ab[i * col_count + column_index] = column[i];
    }
}

void calculate1(double* column, int base_element_index) {
    for (int i = base_element_index + 1; i < equations_count; i++) {
       column[i] -= column[base_element_index] * coefficients_buffer[i];
    }
}

void calculate2(double* column, int base_element_index) {
    for (int i = base_element_index - 1; i >= 0; --i) {
        column[i] -= column[base_element_index] * coefficients_buffer[i];
    }
}

void generate(int eqs = 2000) {
    std::random_device seed;
    std::mt19937 rand(seed());
    std::ofstream out("matrix.txt");
    out << eqs << std::endl;
    for (int i = 0; i < eqs; i++) {
        for (int j = 0; j < eqs + 1; j++) {
            int value = -1000 + (rand() % (2000));
            out << value << " ";
        }
        out << std::endl;
    }
    out.close();
}

void read() {
    std::ifstream in("matrix.txt");
    std::string line, word;
    std::getline(in, line);
    equations_count = std::stoi(line);
    col_count = equations_count + 1;
    ab = new double[equations_count * col_count];
    x = new double[equations_count];
    for (int i = 0; i < equations_count; i++) {
        std::getline(in, line);
        std::stringstream str(line);
        for(int j = 0; j < col_count; j++)
        {
            str >> word;
            ab[i * col_count + j] = std::stod(word);
        }
    }
    in.close();
}

void write() {
    std::ofstream out("solution.txt");
    out << equations_count << std::endl;
    for (int i = 0; i < equations_count; i++) {
        out << x[i] << " ";
    }
    out.close();
}

void write_to_console() {
    for (int i = 0; i < equations_count; i++) {
        for (int j = 0; j < col_count; j++) {
            std::cout << ab[i * col_count + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-----------" << std::endl;
}

void elimination(int size, int rank) {
    double divider;
    for (int base_element_index = 0; base_element_index <= equations_count - 1; ++base_element_index) {
        if (rank == 0) {
            write_to_console();
            divider = ab[base_element_index * col_count + base_element_index];

            for (int j = 0; j <= equations_count; ++j) {
                ab[base_element_index * col_count + j] /= divider;
            }

            if (equations_count - 1 == base_element_index) {
                break;
            }

            for (int i = base_element_index + 1; i <= equations_count - 1; ++i) {
                coefficients_buffer[i] = ab[i * col_count + base_element_index];
            }
        }
        else if (equations_count - 1 == base_element_index) {
            break;
        }

        MPI_Bcast(coefficients_buffer, equations_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            int col_index = base_element_index;
            int nextProcess = 1, columns_per_process = (equations_count - base_element_index) / (size - 1) + 1;
            if (columns_per_process == 0) {
                columns_per_process++;
            }

            while (col_index < equations_count) {
                if (col_index + columns_per_process <= equations_count) {
                    process_col_count = columns_per_process;
                }
                else {
                    process_col_count = equations_count - col_index + 1;
                }

                col_indices[nextProcess] = process_col_count;
                MPI_Send(&process_col_count, 1, MPI_INT, nextProcess, 0, MPI_COMM_WORLD);

                for (int s = 0; s < process_col_count; s++, col_index++) {
                    set_column(col_index);
                    MPI_Send(column, equations_count, MPI_DOUBLE, nextProcess, 0, MPI_COMM_WORLD);
                }

                nextProcess++;
            }

            process_col_count = -1;
            for (; nextProcess < size; nextProcess++) {
                col_indices[nextProcess] = process_col_count;
                MPI_Send(&process_col_count, 1, MPI_INT, nextProcess, 0, MPI_COMM_WORLD);
            }

            col_index = base_element_index;
            for (nextProcess = 1; nextProcess < size && col_indices[nextProcess]; nextProcess++) {
                for (int c = 0; c < col_indices[nextProcess]; c++, col_index++) {
                    MPI_Recv(column, equations_count, MPI_DOUBLE, nextProcess, 0, MPI_COMM_WORLD, &status);
                    replace_with_column(col_index);
                }
            }
        }
        else if (rank < size) {
            MPI_Recv(&process_col_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            if (process_col_count > 0) {
                columns = new double* [process_col_count];
                for (int c = 0; c < process_col_count; c++) {
                    columns[c] = new double[equations_count];
                    MPI_Recv(columns[c], equations_count, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
                }

                for (int c = 0; c < process_col_count; c++) {
                    calculate1(columns[c], base_element_index);
                }

                for (int c = 0; c < process_col_count; c++) {
                    MPI_Send(columns[c], equations_count, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
                }

                delete[] columns;
            }
        }
    }
}

void backStitution(int size, int rank) {
    int next_process = 1;
    for (int base_element_index = equations_count - 1; base_element_index > 0; --base_element_index) {
        if (rank == 0) {
            for (int i = base_element_index - 1; i >= 0; --i)
                coefficients_buffer[i] = ab[i * col_count + base_element_index];
        }

        MPI_Bcast(coefficients_buffer, equations_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            // RR
            int col_index, indices[2];
            indices[0] = base_element_index;
            indices[1] = equations_count;

            process_col_count = 2;
            MPI_Send(&process_col_count, 1, MPI_INT, next_process, 0, MPI_COMM_WORLD);
            for (int i = 0; i < 2; i++) {
                col_index = indices[i];
                set_column(indices[i]);
                MPI_Send(column, equations_count, MPI_DOUBLE, next_process, 0, MPI_COMM_WORLD);
            }

            process_col_count = -1;
            for(int i = 1; i < size; i++) {
                if (i != next_process) {
                    MPI_Send(&process_col_count, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                }
            }

            for (int i = 0; i < 2; i++) {
                MPI_Recv(column, equations_count, MPI_DOUBLE, next_process, 0, MPI_COMM_WORLD, &status);
                replace_with_column(indices[i]);
            }

            if (next_process + 1 < size) {
                next_process += 1;
            }
            else {
                next_process = 1;
            }
        }
        else if (rank < size) {

            MPI_Recv(&process_col_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            if (process_col_count > 0) {
                columns = new double* [process_col_count];
                for (int c = 0; c < process_col_count; c++) {
                    columns[c] = new double[equations_count];
                    MPI_Recv(columns[c], equations_count, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
                }

                for (int c = 0; c < process_col_count; c++) {
                    calculate2(columns[c], base_element_index);
                }

                for (int c = 0; c < process_col_count; c++) {
                    MPI_Send(columns[c], equations_count, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
                }

                delete[] columns;
            }
        }
    }

    if (rank == 0) {
        for (int i = 0; i <= equations_count - 1; ++i)
            x[i] = ab[i * col_count + equations_count];
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    std::chrono::steady_clock::time_point start;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        /*std::cout << "Enter count of equations: ";
        std::cin >> equations_count;
        generate(equations_count);*/

        read();
        col_indices = new int[size];
        col_indices[0] = -1;

        std::cout << "MPI solution" << std::endl;
    }

    MPI_Bcast(&equations_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&col_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

    coefficients_buffer = new double[equations_count];
    column = new double[equations_count];

    if (rank == 0) {
        start = std::chrono::high_resolution_clock::now();
        std::cout << "Starting gauss elimination..." << std::endl;
    }
    elimination(size, rank);
    if (rank == 0) {
        std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Time for gauss elimination: " << time.count() << " milliseconds" << std::endl;
        std::cout << "Starting back substitution..." << std::endl;
    }
    backStitution(size, rank);
    if (rank == 0) {
        std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Time for solution: " << time.count() << " milliseconds" << std::endl;
        delete[] col_indices;
    }

    delete[] coefficients_buffer;
    delete[] column;

    if (rank == 0) {
        write();
        write_to_console();
        delete[] x;
        delete[] ab;
    }

    MPI_Finalize();
}