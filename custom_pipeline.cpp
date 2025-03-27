//--seed=match14 --recoverseeds --format=none --gfextend --exact=20 --nochain
//--hspthresh=top.1% --nogapped

#include "Load_Balance.hpp"
#include "datatype.hpp"
#include "debug.hpp"
#include "filters.hpp"
#include "hit.hpp"
#include "parsing.hpp"
#include <cstring>
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
  // print args
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size); // number of procs

  // Parse command-line arguments
  const char *hit_file = nullptr;
  const char *query_file = nullptr;
  const char *reference_file = nullptr;

  for (int i = 1; i < argc; ++i) {
    if (std::strcmp(argv[i], "--hit_file") == 0 && i + 1 < argc) {
      hit_file = argv[i + 1];
    } else if (std::strcmp(argv[i], "--query") == 0 && i + 1 < argc) {
      query_file = argv[i + 1];
    } else if (std::strcmp(argv[i], "--reference") == 0 && i + 1 < argc) {
      reference_file = argv[i + 1];
    }
  }
  if (!hit_file || !query_file || !reference_file) {
    if (rank == 0) {
      std::cerr << "Error: --hit_file, --query, and --reference arguments are "
                   "required.\n";
    }
    MPI_Finalize();
    return 1;
  }

  // get the file with the seed hits
  auto hits = parseHits(hit_file);

  // // extract sequences from query and reference fasta files
  const std::string seq1 = extract_fasta_file(query_file);
  const std::string seq2 = extract_fasta_file(reference_file);
  if (seq1.empty() || seq2.empty()) {
    if (rank == 0) {
      std::cerr << "Error: Failed to extract sequences from query or reference "
                   "files.\n";
    }
    MPI_Finalize();
    return 1;
  }

  hits_lib::get_hit_type();
  auto filter_order = {std::make_tuple(0, 3)};
  std::vector<std::function<void(std::vector<hits_lib::Hit> &, int)>>
      filter_list = {[&](std::vector<hits_lib::Hit> &vec, int rank) {
                       filters::gapFreeCompute(vec, seq1, seq2);
                     },
                     [&](std::vector<hits_lib::Hit> &vec, int rank) {
                       filters::anchorCompute(vec, seq1, seq2);
                     },
                     [&](std::vector<hits_lib::Hit> &vec, int rank) {
                       filters::hspthreshCompute(vec, seq1, seq2);
                     }};

  assert(hits.size() > 0);
  assert(filter_list.size() == 3);
  assert(filter_order.size() == 1);

  load_balance::MPI_kernel(size, rank, PROBLEM_SIZE, filter_list, filter_order,
                           hits);
  hits_lib::free_hit_type();
  MPI_Finalize();
  return 0;
}

/*
To run
mpirun -np 5 ./MyMPIProject --hit_file ../data/seg.txt --query ../data/small2.fa
--reference ../data/small1.fa
*/