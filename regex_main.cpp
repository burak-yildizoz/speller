////////////////////////////////////////////////////////////////////////////////
// Standard Headers
#include <cstdlib>
#include <exception>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
////////////////////////////////////////////////////////////////////////////////
// User Defined Headers
#include "utility.hpp"
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
try {
    // Obtain speller resource file path
    const std::string speller_filename = (argc > 1) ? argv[1] : "tr.txt";
    std::cout << "Speller filename: " << speller_filename << "\n";
    std::cout << std::endl;

    // Read speller content
    const std::string speller = util::file_to_string(speller_filename);

    // Configure standard input
    util::enable_exceptions(std::cin);
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
        const std::vector<std::string> results = util::get_all_matches(speller, rgx);

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
