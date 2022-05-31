#if defined HAVE_MPI && HAVE_MPI > 0
#include <mpi.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <stdlib.h> 
#include <stdexcept>



namespace MPIUtility{

// Utility for MPI



// Data type definition for MPI communication.
// Credit to the author of potentional/elecstatics/helpme.h - Andrew C. Simmonett
/*!
 * \brief The MPITypes struct abstracts away the MPI_Datatype types for different floating point modes
 *        using templates to hide the details from the caller.
 */
template <typename Real>
struct MPITypes {
    MPI_Datatype realType_;
    MPI_Datatype complexType_;
    MPITypes() {
        throw std::runtime_error("MPI wrapper has not been implemented for the requested floating point type.");
    }
};

template <>
inline MPITypes<float>::MPITypes() : realType_(MPI_FLOAT), complexType_(MPI_C_COMPLEX) {}

template <>
inline MPITypes<double>::MPITypes() : realType_(MPI_DOUBLE), complexType_(MPI_C_DOUBLE_COMPLEX) {}

template <>
inline MPITypes<long double>::MPITypes() : realType_(MPI_LONG_DOUBLE), complexType_(MPI_C_LONG_DOUBLE_COMPLEX) {}





template<typename Real>
class MPIController{
// This class makes the task splitting and the result gathering for mb-nrg functions

private:  
    // helper function: number of combination from n choose k
    size_t _n_choose_k_(size_t n, size_t k);

    MPITypes<Real> types_;
    /// The MPI communicator instance to use for all reciprocal space work.
    MPI_Comm mpiCommunicator_;

    void _barrier_() {
        if (MPI_Barrier(mpiCommunicator_) != MPI_SUCCESS) throw std::runtime_error("Problem in MPI Barrier call!");
    }

public:
    /// The total number of MPI nodes involved in work space.
    int world_size_;
    /// The MPI rank of this node.
    int world_rank_;
    /// The name of the processors
    char processor_name_[MPI_MAX_PROCESSOR_NAME];
    int name_len_;

    // saving the starting index and the total count of the monomer for each world
    std::vector<int> istart_worlds_, icount_worlds_;

    // flag on if the split task has been done
    bool is_split_;

    // constructor and destructor
    // Credit to the author of potentional/elecstatics/helpme.h - Andrew C. Simmonett
    MPIController():is_split_(false){
        MPI_Init(NULL, NULL);

        if (MPI_Comm_dup(MPI_COMM_WORLD, &mpiCommunicator_) != MPI_SUCCESS)
            throw std::runtime_error("Problem calling MPI_Comm_dup in MPIController constructor.");
        if (MPI_Comm_size(mpiCommunicator_, &world_size_) != MPI_SUCCESS)
            throw std::runtime_error("Problem calling MPI_Comm_size in MPIController constructor.");
        if (MPI_Comm_rank(mpiCommunicator_, &world_rank_) != MPI_SUCCESS)
            throw std::runtime_error("Problem calling MPI_Comm_rank in MPIController constructor.");
        if (MPI_Get_processor_name(processor_name_, &name_len_) != MPI_SUCCESS)
            throw std::runtime_error("Problem calling MPI_Get_processor_name in MPIController constructor.");
    }

    ~MPIController() {
        if (mpiCommunicator_) MPI_Comm_free(&mpiCommunicator_);
    }





    std::vector<size_t> split_tasks(size_t itotal, int bodies) {
    // Function to approximately divide 1b/2b/3b tasks for MPI
    //
    // Input:
    // itotal : number of monomers 
    // bodies : if it is to calculate 1b/2b/3b
    //
    // Updates:
    // istart_worlds_/icount_worlds_ : the begining monomer (inclusive) and the number of the monomer for each world rank after splitting (have to use int instead of size_t due to MPI limits)
    // 
    // Return: number of monomer/dimer/trimer in each world

        std::vector<size_t> n_sample_worlds;
        int icurrent(0), icounts_rank(0); 

        istart_worlds_.clear();
        icount_worlds_.clear();

        for (size_t ii = 0; ii < world_size_; ii++){

            // determine the iend for this rank by approximating percentage 
            // num of dimers is square to the number of monomer, and num of trimer is cubic to that 
            double iend_percentage = 1 - pow( double( double(world_size_ - ii -1) / world_size_ ), 1.0/double(bodies)) ;

            int iend_temp = int (itotal * iend_percentage);
            iend_temp = std::max( icurrent + 1, iend_temp);

            size_t n_sample_temp = _n_choose_k_(itotal - icurrent, bodies) - _n_choose_k_(itotal - iend_temp, bodies);

            istart_worlds_.push_back(icurrent);
            icount_worlds_.push_back(int(iend_temp - icurrent));
            n_sample_worlds.push_back(n_sample_temp);

            icurrent = iend_temp;
        }

        is_split_ = true;
        return n_sample_worlds;
    }


    Real sum_all_worlds(Real &variable){
    // Sum the variable calculated in all MPI processors
    // input variable: the value calculated at each processor
    // output: the sum of input variables from all processors
        Real sums(0.0); 
        MPI_Allreduce(&variable, &sums, 1, types_.realType_, MPI_SUM, mpiCommunicator_);
        return sums;
    }


    std::vector<Real> concat_vector(Real* variable, size_t num_total){
        std::vector<Real> result_concat(num_total, 0);
        if (is_split_){
            Real *buff_recv = (Real *)malloc(sizeof(Real)*num_total);
            MPI_Allgatherv( variable,  icount_worlds_[world_rank_], types_.realType_,
                        buff_recv, &icount_worlds_[0], &istart_worlds_[0], types_.realType_,
                        mpiCommunicator_);
            std::memcpy(&result_concat[0], buff_recv, num_total*sizeof(Real));           
            free(buff_recv);
        } else {
            std::cerr << " Tasks have not been distributed to processors - return 0 as the result !!! " << std::endl;
        }

        _barrier_();
        return result_concat;
    }


    void get_start_and_count(   size_t itotal, int bodies,
                                size_t &istart_world, size_t &icount_world){
        std::vector<size_t> _all_worlds = split_tasks(itotal, bodies);
        istart_world = istart_worlds_[world_rank_];
        icount_world = icount_worlds_[world_rank_];
    };


}; // end of class MPIController


};  // end of namespace MPIUtility
#endif //HAVE_MPI==1
