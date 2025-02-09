#include <mpi.h>

void test_modulate_filters(int size, int rank);


int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size); // number of procs
  test_modulate_filters(size, rank);
  MPI_Finalize();
  return 0;
}

// mpirun -np 5 ./hello_mpi
// mpirun -np 4 xterm -e gdb -ex run --args ./MyMPIProject
