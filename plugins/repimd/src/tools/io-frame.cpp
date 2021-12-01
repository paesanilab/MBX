#include <cassert>

#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "io-frame.h"

////////////////////////////////////////////////////////////////////////////////

namespace {

//----------------------------------------------------------------------------//

const char endmark[] = "-------------------end-of-frame-------------------";

//----------------------------------------------------------------------------//

}  // namespace

////////////////////////////////////////////////////////////////////////////////

namespace kit {
namespace io {

//----------------------------------------------------------------------------//

sec::sec(const char* name, const double& data) : m_name(name), m_ndata(1), m_data(&data) { assert(name && name[0]); }

//----------------------------------------------------------------------------//

sec::sec(const char* name, size_t ndata, const double* data) : m_name(name), m_ndata(ndata), m_data(data) {
    assert(name && name[0] && ndata > 0);
}

//----------------------------------------------------------------------------//

std::ostream& operator<<(std::ostream& os, const sec& s) {
    using std::ios;
    using std::setw;

    ios::fmtflags saved_flags = os.flags();

    os.setf(ios::scientific);
    os.precision(9);

    os << '{' << s.m_name << "}\n";

    size_t i(0);

    for (; i + 2 < s.m_ndata; i += 3)
        os << setw(16) << s.m_data[i] << ' ' << setw(16) << s.m_data[i + 1] << ' ' << setw(16) << s.m_data[i + 2]
           << '\n';

    for (; i + 1 < s.m_ndata; i += 2) os << setw(16) << s.m_data[i] << ' ' << setw(16) << s.m_data[i + 1] << '\n';

    for (; i < s.m_ndata; ++i) os << setw(16) << s.m_data[i] << '\n';

    os.flags(saved_flags);

    return os;
}

//----------------------------------------------------------------------------//

std::ostream& operator<<(std::ostream& os, const endframe& ef) { return os << endmark << '\n'; }

//----------------------------------------------------------------------------//

void frame::clear() { m_contents.clear(); }

//----------------------------------------------------------------------------//

void frame::require(const char* name) {
    assert(name);

    m_contents.insert(std::make_pair(name, std::vector<double>()));
}

//----------------------------------------------------------------------------//

const std::vector<double>* frame::get(const char* name) const {
    assert(name);

    map_type::const_iterator i = m_contents.find(name);

    return (i == m_contents.end() ? 0 : &(i->second));
}

//----------------------------------------------------------------------------//

std::istream& operator>>(std::istream& is, frame& f) {
    using namespace std;

    frame::map_type sections;

    frame::map_type::iterator curr = sections.end();

    string line;
    while (getline(is, line)) {
        size_t pos = line.find_first_not_of(" \t");

        if (pos == string::npos)  // empty line
            continue;

        if (line[pos] == '#')  // a comment
            continue;

        if (line.find("end-of-frame") != string::npos)  // end of frame
            break;

        // the line must be either data (for current section) or a title

        if (line[pos] == '{') {  // possibly a title

            size_t endpos = line.find('}', pos + 1);
            if (endpos == string::npos) throw runtime_error("unterminated {title}");

            if (pos + 1 == endpos) throw runtime_error("empty {title}");

            string title = line.substr(pos + 1, endpos - pos - 1);

            pos = title.find_first_not_of(" \t");
            if (pos == string::npos) throw runtime_error("empty {title}");

            endpos = title.find_last_not_of(" \t");
            assert(endpos + 1 > pos);

            title = title.substr(pos, endpos + 1 - pos);

            if (curr != sections.end() && curr->second.empty()) {
                ostringstream oss;
                oss << "section '{" << curr->first << "}' is empty";
                throw runtime_error(oss.str());
            }

            pair<frame::map_type::iterator, bool> ret = sections.insert(make_pair(title, vector<double>()));

            if (!ret.second) {
                ostringstream oss;
                oss << "section '{" << title << "}' is not unique";
                throw runtime_error(oss.str());
            }

            curr = ret.first;

        } else {  // not a title

            if (curr == sections.end()) throw runtime_error("unexpected text instead of a {title}");

            while (pos != string::npos) {
                size_t endpos = line.find_first_of(" \t", pos);
                string token = line.substr(pos, endpos - pos);
                istringstream iss(token);
                double d;
                iss >> d;
                if (!iss || !iss.eof()) {
                    ostringstream oss;
                    oss << "could not convert '" << token << "' to real number";
                    throw runtime_error(oss.str());
                }
                curr->second.push_back(d);
                pos = line.find_first_not_of(" \t", endpos);
            }
        }
    }

    if (curr != sections.end() && curr->second.empty()) {
        ostringstream oss;
        oss << "section '{" << curr->first << "}' is empty";
        throw runtime_error(oss.str());
    }

    // check expectations

    if (!f.m_contents.empty()) {
        frame::map_type::const_iterator i1 = f.m_contents.begin();
        for (; i1 != f.m_contents.end(); ++i1) {
            frame::map_type::const_iterator i2 = sections.find(i1->first);
            if (i2 == sections.end() || i2->second.empty()) {
                ostringstream oss;
                oss << "could not load the '" << i1->first << "' section";
                throw runtime_error(oss.str());
            }
        }
    }

    f.m_contents.swap(sections);

    return is;
}

//----------------------------------------------------------------------------//

void load_time_temp_mass_pos_vel(const char* filename, double& time, double& temperature, size_t& natom, double*& mass,
                                 double*& pos, double*& vel) {
    assert(filename);

    std::ifstream ifs(filename);
    if (!ifs) {
        std::ostringstream oss;
        oss << "could not open '" << filename << "' for reading";
        throw std::runtime_error(oss.str());
    }

    frame f;

    f.require("time");
    f.require("temperature");
    f.require("mass");
    f.require("positions");
    f.require("velocities");

    try {
        ifs >> f;
    } catch (const std::exception& e) {
        std::ostringstream oss;
        oss << "'" << filename << "': " << e.what();
        throw std::runtime_error(oss.str());
    }

    const std::vector<double>* ptr = f.get("time");
    assert(ptr);  // required
    if (ptr->size() > 1) {
        std::ostringstream oss;
        oss << "'" << filename << "': too many elements in {time} section";
        throw std::runtime_error(oss.str());
    }
    time = ptr->operator[](0);

    ptr = f.get("temperature");
    assert(ptr);  // required
    if (ptr->size() > 1) {
        std::ostringstream oss;
        oss << "'" << filename << "': too many elements in {temperature} section";
        throw std::runtime_error(oss.str());
    }
    temperature = ptr->operator[](0);

    const std::vector<double>* mptr = f.get("mass");
    assert(mptr);  // required
    natom = mptr->size();

    const std::vector<double>* posptr = f.get("positions");
    assert(posptr);  // required

    const std::vector<double>* velptr = f.get("velocities");
    assert(velptr);  // required

    if (3 * natom != posptr->size() || 3 * natom != velptr->size()) {
        std::ostringstream oss;
        oss << "'" << filename
            << "': incommensurate number of elements"
               " in mass/positions/velocities";
        throw std::runtime_error(oss.str());
    }

    mass = new double[natom];

    pos = new double[3 * natom];
    vel = new double[3 * natom];

    for (size_t n = 0; n < natom; ++n) {
        mass[n] = mptr->operator[](n);
        for (size_t k = 0; k < 3; ++k) {
            size_t j = 3 * n + k;

            pos[j] = posptr->operator[](j);
            vel[j] = velptr->operator[](j);
        }
    }
}

//----------------------------------------------------------------------------//

}  // namespace io
}  // namespace kit

////////////////////////////////////////////////////////////////////////////////
