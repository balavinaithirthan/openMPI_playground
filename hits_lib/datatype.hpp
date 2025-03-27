#pragma once
#include "hit.hpp"
#include <mpi.h>
namespace hits_lib {

// Singleton accessor for hit type
inline MPI_Datatype &get_hit_type() {
  static MPI_Datatype particle_type = []() {
    int block_lengths[4] = {1, 1, 1, 1};
    MPI_Datatype types[4] = {MPI_C_BOOL, MPI_DOUBLE, MPI_UNSIGNED_LONG,
                             MPI_UNSIGNED_LONG};
    MPI_Aint displacements[4];

    displacements[0] = offsetof(hits_lib::Hit, on);         // bool on
    displacements[1] = offsetof(hits_lib::Hit, length);     // double length
    displacements[2] = offsetof(hits_lib::Hit, position_x); // size_t position_x
    displacements[3] = offsetof(hits_lib::Hit, position_y); // size_t position_y

    // we may need many of these custom types for metadata
    MPI_Datatype temp_type;
    MPI_Type_create_struct(3, block_lengths, displacements, types, &temp_type);
    MPI_Type_commit(&temp_type);
    return temp_type;
  }();

  return particle_type;
}

inline void free_hit_type() { MPI_Type_free(&get_hit_type()); }

} // namespace hits_lib