#ifndef SPELLER_UTILITY_HPP
#define SPELLER_UTILITY_HPP

////////////////////////////////////////////////////////////////////////////////
// Standard Headers
#include <filesystem>
#include <iosfwd>
#include <regex>
#include <string>
#include <vector>
////////////////////////////////////////////////////////////////////////////////

namespace util {

void enable_exceptions(std::istream& is);

std::string file_to_string(const std::filesystem::path& filename);

std::vector<std::string> get_all_matches(const std::string& str, const std::regex& rgx);

} // namespace util

////////////////////////////////////////////////////////////////////////////////
// INLINE DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Standard Headers
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <istream>
#include <iterator>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
////////////////////////////////////////////////////////////////////////////////

namespace util {

inline void enable_exceptions(std::istream& is)
{
    is.exceptions(std::ios::badbit | std::ios::failbit);
}

inline std::string file_to_string(const std::filesystem::path& filename)
{
    if (!std::filesystem::exists(filename)) {
        throw std::runtime_error("File does not exist: " + filename.string());
    }
    std::ifstream ifs(filename);
    std::stringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}

inline std::vector<std::string> get_all_matches(const std::string& str, const std::regex& rgx)
{
    std::vector<std::string> results;
    std::copy(std::sregex_token_iterator(str.begin(), str.end(), rgx),
        std::sregex_token_iterator(), std::back_inserter(results));
    return results;
}

} // namespace util

#endif // SPELLER_UTILITY_HPP
