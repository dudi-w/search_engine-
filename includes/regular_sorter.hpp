// #ifndef TOOLS_HPP
// #define TOOLS_HPP

// #include <string>
// #include <vector>
// #include <utility>


// namespace se{
    
// void sort_links(std::vector<std::pair<std::string, size_t>>& linksVector, size_t num_res);

// } // namespace se

// #endif

#ifndef REGULAR_SORT_HPP 
#define REGULAR_SORT_HPP 

#include <string>
#include <vector>
#include <utility>

#include "sorter.hpp"
 

namespace se
{

class RegularSorter : public Isorter
{
public:
    RegularSorter() = default;
    ~RegularSorter() = default;
    RegularSorter(const RegularSorter&) = default;
    RegularSorter& operator= (const RegularSorter&) = default;

    void sort_links(std::vector<std::pair<std::string, size_t>>& linksVector, size_t num_res)const override;
};

} //se
#endif
