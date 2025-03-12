//--seed=match14 --recoverseeds --format=none --gfextend --exact=20 --nochain
//--hspthresh=top.1% --nogapped

#include "Load_Balance.hpp"
#include "debug.hpp"
#include "filters.hpp"
#include "hit.hpp"
#include <functional>
#include <map>
#include <mpi.h>
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

int PROBLEM_SIZE = 20;
int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size); // number of procs
  /////////////////
  /////////
  ////////

  // TODO: only the parent should do this in the future!
  const std::string seq1 = std::string(PROBLEM_SIZE, 'A');
  const std::string seq2 = std::string(PROBLEM_SIZE, 'A');
  const int kmer_size = 10;
  auto filter_order = {std::make_tuple(0, 3)};
  std::vector<std::function<void(std::vector<filters::Hit> &, int)>>
      filter_list = {[&](std::vector<filters::Hit> &vec, int rank) {
                       filters::gapFreeCompute(vec, seq1, seq2);
                     },
                     [&](std::vector<filters::Hit> &vec, int rank) {
                       filters::anchorCompute(vec, seq1, seq2);
                     },
                     [&](std::vector<filters::Hit> &vec, int rank) {
                       filters::hspthreshCompute(vec, seq1, seq2);
                     }};

  std::map<std::string, std::vector<size_t>> seedsOneMap =
      filters::findSeeds(seq1, kmer_size);
  std::map<std::string, std::vector<size_t>> seedsTwoMap =
      filters::findSeeds(seq2, kmer_size);
  auto hits = filters::findHits(seedsTwoMap, seedsTwoMap, kmer_size);
  if (rank == 0) {
    load_balance::print_vector(hits);
  }

  ////////////////
  ////////////////
  ////////////////

  load_balance::MPI_kernel(size, rank, PROBLEM_SIZE, filter_list, filter_order,
                           hits);
  MPI_Finalize();
  return 0;
}