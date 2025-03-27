//--seed=match14 --recoverseeds --format=none --gfextend --exact=20 --nochain
//--hspthresh=top.1% --nogapped

// #include "Load_Balance.hpp"
// #include "filters.hpp"
// #include "hit.hpp"
// #include "parsing.hpp"
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <mpi.h>
#include <sstream>
#include <tuple>
#include <vector>
/*
Steps:
1. Seed exact == 10

2. Generate vector of seeds
- have a way of printing how dense the seeds are in the vector

3. For each seed do a ungapped extension
- extend upper and lower in both directions until a mismatch

4. Filter is length > default length

5. Chain

6. HOW DOES THIS INTERACT WITH HSP??

7.

*/

int main(int argc, char **argv) {
  // MPI_Init(&argc, &argv);
  // int rank, size;
  // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  // MPI_Comm_size(MPI_COMM_WORLD, &size); // number of procs
  // auto hits = parseHits(
  //     "/Users/balavinaithirthan/Main/CS/SummerResearch/openMPI_playground/"
  //     "data/seg.txt");
  // if (rank == 0) {
  //   load_balance::debug_hits(hits);
  // }
  // load_balance::MPI_kernel(size, rank, hits.size(), filter_list,
  // filter_order,
  //                          hits);
  // MPI_Finalize();
  return 0;
}