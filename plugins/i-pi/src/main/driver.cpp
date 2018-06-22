#include <cmath>
#include <cassert>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <cstdlib>

#include "io_tools/read_nrg.h"
#include "io_tools/write_nrg.h"

#include "bblock/system.h"

#include "../external/sockets.h"

//#define PRINT_GRADS
//#define NUM_GRADS
namespace {

static std::vector<bblock::System> systems;

} // namespace

const int LENMSG = 12;
const int LENINIT = 2048;
const std::string STATUS     = "STATUS      ";
const std::string NEEDINIT   = "NEEDINIT    ";
const std::string HAVEDATA   = "HAVEDATA    ";
const std::string READY      = "READY       ";
const std::string GETFORCE   = "GETFORCE    ";
const std::string POSDATA    = "POSDATA     ";
const std::string FORCEREADY = "FORCEREADY  ";

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{

  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " <input.nrg> <port> <host>"
              << std::endl;
    return 0;
  }

  try {
    std::ifstream ifs(argv[1]);

    if (!ifs){
      throw std::runtime_error("could not open the NRG file");
    }

    tools::ReadNrg(argv[1], systems);
  } catch (const std::exception& e) {
    std::cerr << " ** Error ** : " << e.what() << std::endl;
    return 1;
  }

  // Initialize defaults
  int socket = 0;
  int inet = 0;
  int port = atoi(argv[2]);
  char * host = argv[3];
  

  open_socket(&socket, &inet, &port, host);

  // Variables needed for MD loop
  char init_buffer[LENINIT + 1];
  char header[LENMSG + 1];
  std::vector<double> box = {100, 0, 0, 0, 100, 0, 0, 0, 100};
  std::vector<double> boxi = {100, 0, 0, 0, 100, 0, 0, 0, 100};
  std::vector<double> virial = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  int rid;
  int buffl = LENMSG;
  bool isinit = false;
  bool hasdata = false;

  int nat = int(systems[0].GetNumRealSites());
  double energy = 0.0;
  int bsize = 0;
  std::vector<double> buffer;
  
  try {
  
    while (true) {
       
      while (true) {
  
        buffl = LENMSG;
        if (!hasdata) {
          readbuffer(socket,header,buffl);
        } else {
          throw std::runtime_error("Wrapper did not ask for data yet");
        }
  
        if (strncmp(header,STATUS.data(),STATUS.length()) == 0) {
          writebuffer(socket,READY.data(),READY.length());
        } else {
          break;
        }
      }
  
      if (strncmp(header,POSDATA.data(),POSDATA.length()) == 0) {
        readbuffer(socket, (char*) box.data(), 9*sizeof(double));
        readbuffer(socket, (char*) boxi.data(), 9*sizeof(double));
        readbuffer(socket, (char*) &nat, sizeof(int));
  
        if (bsize == 0) {
          bsize = 3*nat;
          buffer = std::vector<double>(3*nat);
        } else if (bsize != 3*nat) {
          throw std::runtime_error("Number of atoms has changed.");
        }
  
        readbuffer(socket, (char*) buffer.data(), bsize*sizeof(double));
  
      } else {
        throw std::runtime_error("Wrapper did not send the positions");
      }
  
      // Get forces here and store them in buffer
      for (size_t i = 0; i < buffer.size(); i++) {
        buffer[i] /= 1.8897259886;
      }
      systems[0].SetOriginalOrderRealSysXyz(buffer);
      energy = systems[0].Energy(buffer, true) / 627.509;
      buffer = systems[0].GetOriginalOrderRealGrads();
      for (size_t i = 0; i < buffer.size(); i++) {
//        buffer[i] = -buffer[i] / 627.509;
        buffer[i] = -buffer[i] / 1.8897259886 / 627.509;
      }
      // TODO check virial
      virial = std::vector<double>(9,0.0);
  
      hasdata = true;
      header[0] = '\0';
  
      while (true) {
        buffl = LENMSG;
        if (hasdata) {
          readbuffer(socket,header,buffl);
        } else {
          throw std::runtime_error("No data to sent to wrapper");
        }
  
        if (strncmp(header,STATUS.data(),STATUS.length()) == 0) {
          writebuffer(socket,HAVEDATA.data(),HAVEDATA.length());
        } else {
          break;
        }
      }
  
      if (strncmp(header,GETFORCE.data(),GETFORCE.length()) == 0) {
        writebuffer(socket, FORCEREADY.data(), FORCEREADY.length());
        writebuffer(socket,(char*) &energy,sizeof(double));
        writebuffer(socket,(char*) &nat,sizeof(int));
        writebuffer(socket,(char*) buffer.data(), bsize*sizeof(double));
        writebuffer(socket,(char*) virial.data(),9*sizeof(double));
        nat=0;
        writebuffer(socket,(char*) &nat,sizeof(int));
  
      } else {
        throw std::runtime_error("Wrapper did not ask for forces");
      }
      
      hasdata = false;
      
    }

  } catch (const std::exception& e) {
    std::cerr << " ** Error ** : " << e.what() << std::endl;
    return 1;
  }
    
  
//  std::vector<double> g;
//  std::cout << "Energies without gradients:" << std::endl;
//  for (size_t i = 0; i < systems.size(); i++) {
//    double energy = systems[i].Energy(g, false);
//    std::cout << std::setprecision(5) << std::scientific
//              << "system["  << std::setfill('.')
//              << std::setw(5) << i << "]= " << std::setfill(' ')
//              << std::setw(20) << std::right << energy 
//              << std::setw(12) << std::right << " kcal/mol" << std::endl;          
//  }
//
//  std::cout << "Energies with gradients:" << std::endl;
//  for (size_t i = 0; i < systems.size(); i++) {
//    std::vector<double> grd;
//    
//    double energy = systems[i].Energy(grd, true);
//    std::cout << std::setprecision(5) << std::scientific
//              << "system["  << std::setfill('.')
//              << std::setw(5) << i << "]= " << std::setfill(' ')
//              << std::setw(20) << std::right << energy
//              << std::setw(12) << std::right << " kcal/mol" 
//              << std::endl << std::endl;
//# ifdef PRINT_GRADS
//
//    std::vector<std::string> atn = systems[i].GetOriginalOrderSysAtNames();
//
//    size_t n_sites = systems[i].GetNumSites();
//
//    std::cout << std::setw(6)  << std::left << "Atom"
//              << std::setw(20) << std::right << "GradientX"
//              << std::setw(20) << std::right << "GradientY"
//              << std::setw(20) << std::right << "GradientZ"
//              << std::endl;
//    for (size_t j = 0; j < n_sites; j++) {
//      if (atn[j] == "virt") continue;
//      std::cout << std::setprecision(3) << std::scientific
//                << std::setw(6) << std::left << atn[j]
//                << std::setw(20) << std::right << grd[3*j]
//                << std::setw(20) << std::right << grd[3*j + 1]
//                << std::setw(20) << std::right << grd[3*j + 2]
//                << std::endl;
//    }
//#ifdef NUM_GRADS
//    std::cout << std::endl << std::setw(6)  << std::left << "Atom"
//              << std::setw(20) << std::right << "Analytical"
//              << std::setw(20) << std::right << "Numerical"
//              << std::setw(20) << std::right << "Difference"
//              << std::endl;
//    // Comparing analytical and numerical
//    std::vector<double> xyz;
//    xyz = systems[i].GetOriginalOrderSysXyz();
//    const double eps = 1.0e-6;
//    for (size_t j = 0; j < n_sites * 3; j++) {
//      if (atn[j/3] == "virt") continue;
//      const double x_orig = xyz[j];
//      xyz[j] = x_orig + eps;
//      systems[i].SetOriginalOrderSysXyz(xyz);
//      const double Ep = systems[i].Energy(g,false);
//      xyz[j] = x_orig + 2 * eps;
//      systems[i].SetOriginalOrderSysXyz(xyz);
//      const double Epp = systems[i].Energy(g,false);
//      xyz[j] = x_orig - eps;
//      systems[i].SetOriginalOrderSysXyz(xyz);
//      const double Em = systems[i].Energy(g,false);
//      xyz[j] = x_orig - 2 * eps;
//      systems[i].SetOriginalOrderSysXyz(xyz);
//      const double Emm = systems[i].Energy(g,false);
//      const double gfd = (8*(Ep - Em) - (Epp - Emm))/(12*eps);
//      xyz[j] = x_orig;
//      systems[i].SetOriginalOrderSysXyz(xyz);
//      std::cout << std::setprecision(5) << std::scientific
//                << std::setw(6) << std::left << atn[j/3]
//                << std::setw(20) << std::right << grd[j]
//                << std::setw(20) << std::right << gfd
//                << std::setw(20) << std::right << std::fabs(grd[j] - gfd)
//                << std::endl;
//    }  
//# endif // NUM_GRADS
//# endif // PRINT_GRADS
//  }
  return 0;
}
