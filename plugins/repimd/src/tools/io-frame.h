#ifndef IO_FRAME_H
#define IO_FRAME_H

#include <map>
#include <vector>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

namespace kit {
namespace io {

//----------------------------------------------------------------------------//

struct sec {
    sec(const char* name, const double& data);
    sec(const char* name, size_t ndata, const double* data);

    friend std::ostream& operator<<(std::ostream&, const sec&);

   private:
    const char* m_name;
    const size_t m_ndata;
    const double* m_data;
};

//----------------------------------------------------------------------------//

std::ostream& operator<<(std::ostream&, const sec&);

////////////////////////////////////////////////////////////////////////////////

struct endframe {};

//----------------------------------------------------------------------------//

std::ostream& operator<<(std::ostream&, const endframe&);

////////////////////////////////////////////////////////////////////////////////

struct frame {
    void clear();

    void require(const char*);
    const std::vector<double>* get(const char*) const;

    friend std::istream& operator>>(std::istream&, frame&);

   private:
    typedef std::map<std::string, std::vector<double> > map_type;

   private:
    map_type m_contents;
};

//----------------------------------------------------------------------------//

std::istream& operator>>(std::istream&, frame&);

//----------------------------------------------------------------------------//

// a shortcut for typical use case

void load_time_temp_mass_pos_vel(const char* filename, double& time, double& temperature, size_t& natom, double*& mass,
                                 double*& pos, double*& vel);

//----------------------------------------------------------------------------//

}  // namespace io
}  // namespace kit

////////////////////////////////////////////////////////////////////////////////

#endif  // IO_FRAME_H
