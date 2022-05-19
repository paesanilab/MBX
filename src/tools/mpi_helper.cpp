#if defined HAVE_MPI && HAVE_MPI > 0
#include <mpi.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <stdlib.h> 
#include <cstring>

#include "mpi_helper.h"    


namespace MPIUtility{

// helper function: number of combination from n choose k
template<typename Real>
size_t MPIController<Real>::_n_choose_k_(size_t n, size_t k){
    size_t result = 1;
    for (size_t ii=0; ii < k; ii++){
        result *= (n-ii) / (ii+1);
    }
    return result;   
};

};  // end of MPIUtility







double calculate(std::vector<double> inputvector){

    


    double result = 0.0;  
    size_t itotal = inputvector.size();
    int bodies = 2;

    size_t istart=0, icount=itotal;



    // update istart and icount for this world
    MPIUtility::MPIController<double> mpi_helper;

    mpi_helper.get_start_and_count(itotal, bodies, istart, icount);

    // Some functions that make calculations
    std::vector<double> temp_result(icount, 0.0);
    for (size_t ii = 0; ii < icount; ii++){
        temp_result[ii] = inputvector[istart + ii] * 0.1 + 0.001 * ii;
        result += temp_result[ii];
    }


    // Gather for a single value
    result = mpi_helper.sum_all_worlds(result);


    // Gather for vectors
    std::vector<double> result_vector = mpi_helper.concat_vector(&temp_result[0], itotal);


    for (auto it=result_vector.begin(); it != result_vector.end(); it++){
        std::cout << mpi_helper.world_rank_ << " \t " << *it << std::endl;
    }

    return result;
}



int main(){

    std::vector<double> inbuff;

    size_t itotal = 20;

    for(size_t i=0; i<itotal; i++){
        double v =  2.1 * i ;
        inbuff.push_back( v );
    }

    double result = calculate(inbuff);

    std::cout << result << std::endl;


    return 0;
}
#endif // HAVE_MPI==1
