////////////////////////////////////////////////////////////////////////////////
// Standard Headers
#include <algorithm>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <functional>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <string_view>
#include <vector>
////////////////////////////////////////////////////////////////////////////////
// User Defined Headers
#include <speller/alphabet.hpp>
#include <speller/locale.hpp>
#include <speller/utility.hpp>
#include <speller/word.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace {

class WordInfo {
public:
    explicit WordInfo(const std::string& str, const std::string& locale_name)
        : word_lowercase(speller::Word(str, locale_name).tolower())
    {
        for (size_t i = 0; i < word_lowercase.length(); i++) {
            const size_t hash = std::hash<speller::Letter>()(word_lowercase.at(i));
            hashes.emplace(hash);
        }
    }

    size_t length() const noexcept
    {
        return word_lowercase.length();
    }

    const std::string& get_lowercase_str() const& noexcept
    {
        return word_lowercase;
    }

    std::multiset<size_t> get_hashes() const
    {
        return hashes;
    }

private:
    speller::Word word_lowercase;
    std::multiset<size_t> hashes;
};

struct ResultInfo {
    std::string str;
    std::vector<speller::Letter> joker_letters;
};

} // namespace

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
    const std::vector<std::string> speller_orig = util::file_to_vector(speller_path);
    // Convert to lowercase and calculate hashes
    std::vector<WordInfo> speller_infos;
    speller_infos.reserve(speller_orig.size());
    std::transform(speller_orig.begin(), speller_orig.end(),
        std::back_inserter(speller_infos),
        [locale_name](const std::string& str) { return WordInfo(str, locale_name); });

    // Configure standard input
    util::enable_exceptions(std::cin);
    std::cin.tie(&std::cout);

    // Run in an infinite loop
    while (true) {
        // Obtain search string
        std::cout << "Search: ";
        std::string search_str;
        std::cin >> search_str;

        // Analyze search string
        const WordInfo search_info = WordInfo(search_str, locale_name);
        // Obtain letters to match
        const std::multiset<size_t> search_hashes = search_info.get_hashes();
        // Count wild cards
        constexpr char joker_letter = '?';
        const size_t num_jokers = std::count(search_str.begin(), search_str.end(), joker_letter);

        // Create locale object
        const speller::Locale locale(locale_name);
        // Create alphabet object
        const speller::Alphabet& alphabet = locale.get_alphabet();

        // Search database
        std::vector<ResultInfo> results;
        for (const WordInfo& info : speller_infos) {
            // Lengths must match
            if (info.length() != search_info.length()) {
                continue;
            }

            // Calculate number of common letters
            const std::multiset<size_t> info_hashes = info.get_hashes();
            std::multiset<size_t> common_hashes;
            std::set_intersection(search_hashes.begin(), search_hashes.end(),
                info_hashes.begin(), info_hashes.end(),
                std::inserter(common_hashes, common_hashes.begin()));
            const size_t common_letters = common_hashes.size();

            // Check whether all letters match
            if (common_letters + num_jokers != search_info.length()) {
                continue;
            }

            // Find joker letters
            std::multiset<size_t> joker_hashes;
            std::set_difference(info_hashes.begin(), info_hashes.end(),
                common_hashes.begin(), common_hashes.end(),
                std::inserter(joker_hashes, joker_hashes.begin()));

            // Add to results
            ResultInfo result;
            result.str = info.get_lowercase_str();
            for (size_t hash : joker_hashes) {
                speller::Letter letter = alphabet.get_letter_from_hash(hash);
                result.joker_letters.push_back(letter);
            }
            results.emplace_back(std::move(result));
        }

        // Print matches
        std::cout << "A total of " << results.size() << " matches found.\n";
        for (const ResultInfo& res : results) {
            std::cout << res.str;
            if (num_jokers != 0) {
                std::cout << "\t";
                std::transform(res.joker_letters.begin(), res.joker_letters.end(),
                    std::ostream_iterator<std::string_view>(std::cout),
                    std::mem_fn(&speller::Letter::string_view));
            }
            std::cout << "\n";
        }
        std::cout << std::endl;
    }
} catch (const std::exception& e) {
    // Print error and exit
    std::cout << "Exception: " << e.what() << std::endl;
    return EXIT_FAILURE;
}
