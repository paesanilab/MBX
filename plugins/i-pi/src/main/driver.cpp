/******************************************************************************
Copyright 2019 The Regents of the University of California.
All Rights Reserved.

Permission to copy, modify and distribute any part of this Software for
educational, research and non-profit purposes, without fee, and without
a written agreement is hereby granted, provided that the above copyright
notice, this paragraph and the following three paragraphs appear in all
copies.

Those desiring to incorporate this Software into commercial products or
use for commercial purposes should contact the:
Office of Innovation & Commercialization
University of California, San Diego
9500 Gilman Drive, Mail Code 0910
La Jolla, CA 92093-0910
Ph: (858) 534-5815
FAX: (858) 534-7345
E-MAIL: invent@ucsd.edu

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE, EVEN IF THE UNIVERSITY
OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE SOFTWARE PROVIDED HEREIN IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS. THE UNIVERSITY OF CALIFORNIA MAKES NO
REPRESENTATIONS AND EXTENDS NO WARRANTIES OF ANY KIND, EITHER IMPLIED OR
EXPRESS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT THE USE OF THE
SOFTWARE WILL NOT INFRINGE ANY PATENT, TRADEMARK OR OTHER RIGHTS.
******************************************************************************/

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
#include "json/json.h"

//#define PRINT_GRADS
//#define NUM_GRADS
namespace {

static std::vector<bblock::System> systems;

}  // namespace

const int LENMSG = 12;
const int LENINIT = 2048;
const std::string STATUS = "STATUS      ";
const std::string NEEDINIT = "NEEDINIT    ";
const std::string HAVEDATA = "HAVEDATA    ";
const std::string READY = "READY       ";
const std::string GETFORCE = "GETFORCE    ";
const std::string POSDATA = "POSDATA     ";
const std::string FORCEREADY = "FORCEREADY  ";
const std::string INIT = "INIT        ";

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input.nrg> <mbx.json> " << std::endl;
        return 0;
    }
    
    std::ifstream ifjson;
    nlohmann::json j;

    try {
        std::ifstream ifs(argv[1]);

        if (!ifs) {
            throw std::runtime_error("could not open the NRG file");
        }

        tools::ReadNrg(argv[1], systems);

        systems[0].SetUpFromJson(argv[2]);
        ifjson.open(argv[2]);
        j = nlohmann::json::parse(ifjson);

    } catch (const std::exception& e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        return 1;
    }

    std::vector<double> json_box;
    try {
        std::vector<double> box2 = j["MBX"]["box"];
        json_box = box2;
    } catch(...) {
        json_box.clear();
    }

    // Initialize defaults
    int socket = 0;
    int inet = 0;

    int port;
    try {
        port = j["i-pi"]["port"];
    } catch(...) {
        port = 34543;
    }

    std::string host_s;
    try {
        host_s = j["i-pi"]["localhost"];
    } catch(...) {
        host_s = "localhost";
    }

    ifjson.close();

    char host[host_s.size() + 1];
    strcpy(host,host_s.c_str());

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
    size_t dipole_count = 0;

    int nat = int(systems[0].GetNumRealSites());
    double energy = 0.0;
    int bsize = 0;
    std::vector<double> buffer;

    try {
        while (true) {
            while (true) {
                buffl = LENMSG;
                if (!hasdata) {
                    readbuffer(socket, header, buffl);
                } else {
                    throw std::runtime_error("Wrapper did not ask for data yet");
                }

                if (strncmp(header, STATUS.data(), STATUS.length()) == 0) {
                    if (!isinit) {
                        writebuffer(socket, NEEDINIT.data(), NEEDINIT.length());
                    } else {
                        writebuffer(socket, READY.data(), READY.length());
                    }
                } else if (strncmp(header, INIT.data(), INIT.length()) == 0) {
                    readbuffer(socket, (char*)&rid, sizeof(int));
                    readbuffer(socket, (char*)&cbuf, sizeof(int));
                    readbuffer(socket, init_buffer, cbuf);
                    isinit = true;
                } else {
                    break;
                }
            }

            if (strncmp(header, POSDATA.data(), POSDATA.length()) == 0) {
                readbuffer(socket, (char*)box.data(), 9 * sizeof(double));
                readbuffer(socket, (char*)boxi.data(), 9 * sizeof(double));
                readbuffer(socket, (char*)&nat, sizeof(int));

                if (bsize == 0) {
                    bsize = 3 * nat;
                    buffer = std::vector<double>(3 * nat);
                } else if (bsize != 3 * nat) {
                    throw std::runtime_error("Number of atoms has changed.");
                }

                readbuffer(socket, (char*)buffer.data(), bsize * sizeof(double));

            } else {
                throw std::runtime_error("Wrapper did not send the positions");
            }

            for (size_t i = 0; i < box.size(); i++) {
                box[i] /= 1.8897259886;
            }

            // Get forces here and store them in buffer
            for (size_t i = 0; i < buffer.size(); i++) {
                buffer[i] /= 1.8897259886;
            }

            dipole_count++;
            if (rid != rid_old || dipole_count > 5000) {
                systems[0].ResetDipoleHistory();
                dipole_count = 0;
            }

            rid_old = rid;
            systems[0].SetRealXyz(buffer);

            if (json_box.size()) {
                systems[0].SetPBC(box);
            } else {
                systems[0].SetPBC();
            }

            energy = systems[0].Energy(true) / 627.509;
            buffer = systems[0].GetRealGrads();
            for (size_t i = 0; i < buffer.size(); i++) {
                //        buffer[i] = -buffer[i] / 627.509;
                buffer[i] = -buffer[i] / 1.8897259886 / 627.509;
            }
            // TODO check virial
            virial = systems[0].GetVirial();
            for (size_t i=0; i<9; i++) {
                virial[i] /= 627.509;
            }

            hasdata = true;
            header[0] = '\0';

            while (true) {
                buffl = LENMSG;
                if (hasdata) {
                    readbuffer(socket, header, buffl);
                } else {
                    throw std::runtime_error("No data to sent to wrapper");
                }

                if (strncmp(header, STATUS.data(), STATUS.length()) == 0) {
                    writebuffer(socket, HAVEDATA.data(), HAVEDATA.length());
                } else {
                    break;
                }
            }

            if (strncmp(header, GETFORCE.data(), GETFORCE.length()) == 0) {
                writebuffer(socket, FORCEREADY.data(), FORCEREADY.length());
                writebuffer(socket, (char*)&energy, sizeof(double));
                writebuffer(socket, (char*)&nat, sizeof(int));
                writebuffer(socket, (char*)buffer.data(), bsize * sizeof(double));
                writebuffer(socket, (char*)virial.data(), 9 * sizeof(double));
                nat = 0;
                writebuffer(socket, (char*)&nat, sizeof(int));

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
