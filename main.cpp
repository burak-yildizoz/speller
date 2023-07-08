#include <algorithm>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

void enable_exceptions(std::istream& is)
{
    is.exceptions(std::ios::badbit | std::ios::failbit);
}

std::string file_to_string(const std::filesystem::path& filename)
{
    if (!std::filesystem::exists(filename)) {
        throw std::runtime_error("File does not exist: " + filename.string());
    }
    std::ifstream ifs(filename);
    std::stringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}

std::vector<std::string> get_all_matches(const std::string& str, const std::regex& rgx)
{
    std::vector<std::string> results;
    std::copy(std::sregex_token_iterator(str.begin(), str.end(), rgx),
        std::sregex_token_iterator(), std::back_inserter(results));
    return results;
}

int main(int argc, char** argv)
try {
    // Obtain speller resource file path
    const std::string speller_filename = (argc > 1) ? argv[1] : "tr.txt";
    std::cout << "Speller filename: " << speller_filename << "\n";
    std::cout << std::endl;

    // Read speller content
    const std::string speller = file_to_string(speller_filename);

    // Configure standard input
    enable_exceptions(std::cin);
    std::cin.tie(&std::cout);

    // Run in an infinite loop
    while (true) {
        // Obtain regex to search in speller
        std::cout << "Search: ";
        std::string search_str;
        std::cin >> search_str;

        // Obtain matches
        static const std::string non_whitespace_regex_str = R"(\S*)";
        const std::regex rgx(non_whitespace_regex_str + search_str + non_whitespace_regex_str);
        const std::vector<std::string> results = get_all_matches(speller, rgx);

        // Print matches
        std::cout << "A total of " << results.size() << " matches found.\n";
        for (const std::string& res : results) {
            std::cout << res << "\n";
        }
        std::cout << std::endl;
    }
} catch (const std::exception& e) {
    // Print error and exit
    std::cout << "Exception: " << e.what() << std::endl;
    return EXIT_FAILURE;
}
