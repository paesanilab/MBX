#ifndef INCLUDE_TOOLS_CU_EXCEPTION_H
#define INCLUDE_TOOLS_CU_EXCEPTION_H

#include <iostream>
#include <exception>
#include <string>

class CUException : public std::exception {
 public:
   CUException(const char * func, 
                   const char * file,
                   const long int line,
                   const std::string errormsg) {

     std::string funcname(func);
     std::string filename(file);
     std::string linenum = std::to_string(line);
     
     message_ = "** ERROR ** C++ Exception in function " + funcname
              + ", in line " + linenum + " of file " + filename + ". "
              + errormsg;
   }
                           
   const char * what () const throw () {
      return message_.c_str();
   }

 protected:
  std::string message_;
};

#endif
