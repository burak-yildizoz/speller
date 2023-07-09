////////////////////////////////////////////////////////////////////////////////
// Standard Headers
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
////////////////////////////////////////////////////////////////////////////////
// User Defined Headers
#include <speller/alphabet.hpp>
#include <speller/locale.hpp>
#include <speller/utility.hpp>
#include <speller/word.hpp>
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
try {
    // Obtain speller resource file path
    const std::filesystem::path speller_path = (argc > 1) ? argv[1] : "tr.txt";
    std::cout << "Speller filename: " << speller_path.string() << "\n";

    // Obtain alphabet resource file, if specified
    std::string locale_name = speller::Locale::default_locale_name;
    const bool has_alphabet = (argc > 2);
    if (has_alphabet) {
        // Print configuration
        const std::string alphabet_filename = argv[2];
        locale_name = (argc > 3) ? argv[3] : speller_path.stem().string();
        std::cout << "Alphabet filename: " << alphabet_filename << "\n";
        std::cout << "Locale: " << locale_name << std::endl;

        // Add locale
        speller::Alphabet alphabet = speller::alphabet_from_file(alphabet_filename);
        speller::Locale::add_locale(locale_name, std::move(alphabet));
    }
    std::cout << std::endl;

    // Read speller content
    const std::string speller_orig = util::file_to_string(speller_path);
    // Convert to lowercase
    const speller::Word speller_lower = speller::Word(speller_orig, locale_name).tolower();

    // Configure standard input
    util::enable_exceptions(std::cin);
    std::cin.tie(&std::cout);

    // Run in an infinite loop
    while (true) {
        // Obtain search string
        std::cout << "Search: ";
        std::string search_str;
        std::cin >> search_str;

        // Convert to lowercase
        search_str = speller::Word(search_str, locale_name).tolower();

        // Create regex from search string
        static const std::regex asterisk_rgx(R"(\*)");
        static const std::regex question_rgx(R"(\?)");
        const std::string lowercase_regex_str = speller::Locale(locale_name).get_alphabet().get_lowercase_letter_regex_str();
        const std::string lowercase_or_space_regex_str = "(" + lowercase_regex_str + "| )";
        std::string regex_str = std::regex_replace(search_str, question_rgx, lowercase_or_space_regex_str);
        regex_str = std::regex_replace(regex_str, asterisk_rgx, lowercase_or_space_regex_str + "+");
        regex_str = "^" + regex_str + "$";

        // Obtain matches
        const std::regex rgx(regex_str);
        const std::vector<std::string> results = util::get_all_matches(speller_lower, rgx);

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
