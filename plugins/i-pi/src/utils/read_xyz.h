#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

void ReadXyz(std::ifstream &ifs, std::vector<double> &xyz, std::vector<std::string> &atoms);
void ReadXyzTrajectory(char* filename, std::vector<std::vector<double> > &xyz, std::vector<std::vector<std::string> > &atoms);
