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
#include "perturb.h"

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
const std::string INIT       = "INIT        ";

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
  bool  perturb=true;

  open_socket(&socket, &inet, &port, host);

  // Variables needed for MD loop
  char init_buffer[LENINIT + 1];
  char header[LENMSG + 1];
  std::vector<double> box = {100, 0, 0, 0, 100, 0, 0, 0, 100};
  std::vector<double> boxi = {100, 0, 0, 0, 100, 0, 0, 0, 100};
  std::vector<double> virial = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  int rid;
  int rid_old = 0;
  int cbuf;
  int buffl = LENMSG;
  bool isinit = false;
  bool hasdata = false;

  // Set method to aspc (by default) for now
  systems[0].SetDipoleMethod("aspc");
//  systems[0].SetDipoleMethod("cg");

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
          if (!isinit) {
            writebuffer(socket,NEEDINIT.data(),NEEDINIT.length());
          } else {
            writebuffer(socket,READY.data(),READY.length());
          }
        } else if (strncmp(header,INIT.data(),INIT.length()) == 0) {
          readbuffer(socket, (char*) &rid, sizeof(int));
          readbuffer(socket, (char*) &cbuf, sizeof(int));
          readbuffer(socket,init_buffer,cbuf);
          isinit = true;
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

      if (rid != rid_old) {
        systems[0].ResetDipoleHistory();
      }
      
      rid_old = rid;
      systems[0].SetRealXyz(buffer);
      energy = systems[0].Energy(true) / 627.509;
      std::vector<double> xyz = systems[0].GetXyz();
      buffer = systems[0].GetRealGrads();
      if (perturb) {
         // Declare perturbation
         noneq::Perturbation my_perturb;
         // Obtain coordinates, charges, pols, and polfacs
         std::vector<double> my_xyz = systems[0].GetRealXyz();
         std::vector<double> my_chg = systems[0].GetRealCharges();
         std::vector<double> my_pol = systems[0].GetRealPolarizabilities();
         std::vector<double> my_polfac = systems[0].GetRealPolarizabilityFactors();
         // initiallize perturbation
         my_perturb.Initialize(my_xyz, my_chg, my_pol, my_polfac);
         // Add perturbation forces (in kcal/mol / A) to buffer.
         // Since buffer is gradient, we substract
         std::vector<double> forces = my_perturb.GetPerturbationPolarizability(5.0,5.0,0,0,+1);
         for (size_t k = 0; k < forces.size(); k++) 
           buffer[k] -= forces[k];
         perturb = false;
      }
      // Convert grads to force in au
      for (size_t i = 0; i < buffer.size(); i++) {
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
      isinit = false;
      
    }

  } catch (const std::exception& e) {
    std::cerr << " ** Error ** : " << e.what() << std::endl;
    return 1;
  }
  
  return 0;
}
