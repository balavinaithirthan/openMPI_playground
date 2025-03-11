// goal pipeline = ./src/lastz_D small1.fa small2.fa --seed=match14
// --recoverseeds
// --format=none --gfextend --ydrop=10
#include "Load_Balance.hpp"
#include "filters.hpp"
#include <functional>
#include <map>
#include <mpi.h>
#include <tuple>
#include <vector>

auto FILTER_ORDER = {std::make_tuple(0, 1)};
std::vector<std::function<void(std::vector<int> &, int)>> FILTER_LIST = {
    [](std::vector<int> &vec, int rank) { printf("filter 1\n"); }};

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

int PROBLEM_SIZE = 30;
int main(int argc, char **argv) {
  std::string seq1 = std::string(PROBLEM_SIZE, 'A');
  std::string seq2 = std::string(PROBLEM_SIZE, 'A');
  int kmer_size = 10;
  std::map<std::string, std::vector<size_t>> seedsOneMap =
      filters::findSeeds(seq1, kmer_size);
  std::map<std::string, std::vector<size_t>> seedsTwoMap =
      filters::findSeeds(seq2, kmer_size);

  auto hits = filters::findHits(seedsTwoMap, seedsTwoMap, kmer_size);
  // filters::debug_hits(hits);
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size); // number of procs
  load_balance::MPI_kernel(size, rank, PROBLEM_SIZE, FILTER_LIST, FILTER_ORDER);
  MPI_Finalize();
  return 0;
}