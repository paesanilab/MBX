#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif // HAVE_CONFIG_H

#include <cmath>
#include <cassert>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>

#include "read-nrg.h"
#include "write-nrg.h"

#include "system.h"


#define PRINT_GRADS
//#define CHECK_GRADS
namespace {

static std::vector<bblock::system> systems;

} // namespace

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{

  if (argc != 2) {
    std::cerr << "usage: energy h2o_ion.nrg"
              << std::endl;
    return 0;
  }

  try {
    std::ifstream ifs(argv[1]);

    if (!ifs){
      throw std::runtime_error("could not open the NRG file");
    }

    tools::read_nrg(argv[1], systems);
  } catch (const std::exception& e) {
    std::cerr << " ** Error ** : " << e.what() << std::endl;
    return 1;
  }

  std::string outfile = "output.nrg";
  const char* fname = outfile.c_str();
  tools::write_nrg(fname,systems);

  

//    size_t nw = (molec[0].natm - 1)/3;
//
//    sys::system my_system;
//
//    for(size_t n = 0; n < nw; ++n){
//	std::shared_ptr<molecule> tmp 
//	                      = std::make_shared<x2o::ttm4>(molec[0].xyz + 9*n);
//
//	my_system.add_molecule(tmp);
//    }
//
//    std::shared_ptr<molecule> tmp 
//	= std::make_shared<x::ion>(molec[0].xyz + 9*nw);
//
//    my_system.add_molecule(tmp);
//    
//    h2o_ion::mbnrg_h2o_ion pot;
//
//    double grd[3*my_system.nsites];
//    //double E = pot(nw, molec[0].xyz, grd);
//    double E = pot(my_system, grd);
//    double Enogrd = pot(my_system);
//
//    std::cout.precision(6);
//    std::cout.setf(std::ios::fixed, std::ios::floatfield);
//    std::cout << "       E = " << E 
//              << "    E[nogrd] = " << Enogrd
//              << std::endl;
//#ifdef PRINT_GRADS
//    std::cout << "Gradients on the atoms: " << std::endl;
//    for(size_t n = 0; n < nw; ++n) {   //grd[] contains also the M-site. This part only prints out atomic gradients from grd[]
//	for(size_t m = 0; m < 3; ++m) {
//	    std::cout << n*3 + (m+1) ;
//	    for(size_t p = 0; p < 3; ++p) {
//		std::cout << "    " << grd[3*(4*n+m)+p] << "    ";
//	    }
//	    std::cout << "\n";
//	}
//    }   
//    std::cout << nw*3 + 1 ;  
//    for (size_t p = 0; p < 3; ++p) {
//	std::cout << "    " << grd[3*(4*nw) + p] << "    ";
//    }
//    std::cout << "\n";
//
//#endif //PRINT_GRADS
//
//#ifdef CHECK_GRADS 
//    std::cout << " error in gradients (analytical - finite differences)\n"
//	    << "   Note: the system class has the m-site position. therefore,\n"
//            << "         number of atoms = nw*4+1"
//	    << std::endl;
//    std::cout << "Printing: \n "
//	    << "# grd[]   gfd[]   error \n";
//
//    const double eps = 1.0e-4;
//    for(size_t n = 0; n < 3*my_system.nsites; ++n){
//	const double x_orig = my_system.coords[n];
//
//	my_system.coords[n] = x_orig + eps;
//	my_system.update_molecules();
//	const double Ep = pot(my_system);
//
//	my_system.coords[n] = x_orig + 2*eps;
//	my_system.update_molecules();
//	const double E2p = pot(my_system);
//
//	my_system.coords[n] = x_orig - 2*eps;
//	my_system.update_molecules();
//	const double E2m = pot(my_system);
//
//	my_system.coords[n] = x_orig - eps;
//	my_system.update_molecules();
//	const double Em = pot(my_system);
//
//	const double gfd = (8*(Ep - Em) - (E2p - E2m))/(12*eps);
//	my_system.coords[n] = x_orig;
//	my_system.update_molecules();
//
//	std::cout << n << ' ' << grd[n] 
//	               << ' ' << gfd
//	               << ' ' << std::fabs(grd[n] - gfd) << '\n';
//    }
//#endif // CHECK_GRADS

    return 0;
}
