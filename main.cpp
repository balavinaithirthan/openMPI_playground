#include <mpi.h>

void test_modulate_filters(int size, int rank, int filter_number);
void test_modulate_problem_size(int size, int rank, int problem_size);
void test_modulate_function_type(int size, int rank);

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size); // number of procs
  test_modulate_function_type(size, rank);
  MPI_Finalize();

  // MPI_Init(&argc, &argv);
  // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  // MPI_Comm_size(MPI_COMM_WORLD, &size); // number of procs
  // test_modulate_filters(size, rank, 20);
  // MPI_Finalize();



  return 0;
}

// mpirun -np 5 ./hello_mpi
// mpirun -np 4 xterm -e gdb -ex run --args ./MyMPIProject
