#ifndef MATH_TOOLS_H
#define MATH_TOOLS_H

#include <vector>
#include <iostream>

#ifdef _OPENMP
# include <omp.h>
#endif

//template <class T>
//std::vector<T> MatrixTimesVector(std::vector<T> A, std::vector<T> b);
//
//template <typename T>
//T DotProduct(std::vector<T> a, std::vector<T> b);

// Vecotr based
template <typename T>
void MatrixTimesVector(const std::vector<T> &A, const std::vector<T> &b, std::vector<T> &c) {
  // Check indexes match
  if (A.size() % b.size() != 0) {
    std::cerr << "ERROR: Indexes do not match. "
              << A.size() << " is not a multiple of "
              << b.size() << std::endl;
  }

  size_t ncols = b.size();
  size_t nrows = A.size() / ncols;
  size_t nthreads = 1;

# ifdef _OPENMP
# pragma omp parallel
  {
    if (omp_get_thread_num() == 0)
      nthreads = omp_get_num_threads();
  }  
# endif

  size_t istart = 0;
  size_t iend = nrows; 

# ifdef _OPENMP
#   pragma omp parallel private(istart,iend)
  {
    int thread_id = omp_get_thread_num();
    size_t size = nrows / nthreads;
    istart = size*thread_id;
    iend = thread_id == nthreads -1 ? nrows : istart + size;
# endif
    for (size_t i = istart; i < iend; i++) {
      size_t incols = i*ncols;
      T a = 0;
      for (size_t j = 0; j < ncols; j++) {
        a += A[incols + j] * b[j];
      }
      c[i] = a;
    }

# ifdef _OPENMP
  }
#endif
}

template <typename T>
T DotProduct(const std::vector<T> &a, const std::vector<T> &b) {
  // Check that sizes are the same
  if (a.size() != b.size()) {
    std::cerr << "ERROR: Indexes do not match. "
              << a.size() << " does not match "
              << b.size() << std::endl;
  }

  T c = 0;
  for (size_t i = 0; i < b.size(); i++) {
    c += a[i]*b[i];
  }

  return c;
}

// Pointer based
//void MatrixTimesVector(const double * A, const double * b, double * c, 
//                       size_t sizeA, size_t sizeb) {
//  // Check indexes match
//  if (sizeA % sizeb != 0) {
//    std::cerr << "ERROR: Indexes do not match. "
//              << sizeA << " is not a multiple of "
//              << sizeb << std::endl;
//  }
//
//  size_t ncols = sizeb;
//  size_t nrows = sizeA / ncols;
//
//  for (size_t i = 0; i < nrows; i++) {
//    size_t incols = i*ncols;
//    c[i] = 0;
//    for (size_t j = 0; j < ncols; j++) {
//      c[i] += A[incols + j] * b[j];
//    }
//  }
//}

template <typename T>
void MatrixTimesVector(const T * A, const T * b, T * c, size_t sizeA, size_t sizeb) {
  // Check indexes match
  if (sizeA % sizeb != 0) {
    std::cerr << "ERROR: Indexes do not match. "
              << sizeA << " is not a multiple of "
              << sizeb << std::endl;
  }

  size_t ncols = sizeb;
  size_t nrows = sizeA / ncols;

  for (size_t i = 0; i < nrows; i++) {
    size_t incols = i*ncols;
    c[i] = 0;
    for (size_t j = 0; j < ncols; j++) {
      c[i] += A[incols + j] * b[j];
    }
  }
}

#endif
