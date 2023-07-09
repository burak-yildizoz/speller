#include <speller/alphabet.hpp>
////////////////////////////////////////////////////////////////////////////////
// Standard Headers
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iterator>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <vector>
////////////////////////////////////////////////////////////////////////////////

namespace speller {

static const std::set<char> regex_special_characters = {
    '\\',
    '^',
    '$',
    '.',
    '|',
    '?',
    '*',
    '+',
    '(',
    ')',
    '[',
    ']',
    '{',
    '}',
};

template <typename Container>
static std::string get_regex_group_str(const Container& alternative_regexes)
{
    if (alternative_regexes.empty()) {
        return "";
    }
    static const std::string prefix = "(";
    static const std::string suffix = ")";
    auto it = std::cbegin(alternative_regexes);
    auto get_current_regex_string = [&it]() {
        std::string_view sw;
        if constexpr (std::is_same_v<Container::value_type, Letter>) {
            sw = it->string_view();
        } else if constexpr (std::is_same_v<Container::value_type, std::pair<const size_t, std::string>>) {
            sw = it->second;
        } else {
            sw = *it;
        }
        return static_cast<std::string>(sw);
    };
    std::string result = prefix + get_current_regex_string();
    for (++it; it != std::cend(alternative_regexes); ++it) {
        result += "|" + get_current_regex_string();
    }
    result += suffix;
    return result;
}

Alphabet::Alphabet(const std::vector<std::string>& lowercase_letters, const std::vector<std::string>& uppercase_letters)
{
    // pre-checks
    // sizes must be equal
    if (lowercase_letters.size() != uppercase_letters.size()) {
        std::ostringstream oss;
        oss << "Lowercase letter size must be equal to uppercase letter size!";
        oss << " [" << lowercase_letters.size() << " vs " << uppercase_letters.size() << "]";
        throw std::invalid_argument(oss.str());
    }
    const size_t letters_size = lowercase_letters.size();
    // no regex special character
    for (const auto& ref : { std::cref(lowercase_letters), std::cref(uppercase_letters) }) {
        for (const std::string& str : ref.get()) {
            if (std::any_of(str.begin(), str.end(), [](char ch) { return regex_special_characters.count(ch); })) {
                throw std::invalid_argument("Letter contains regex special character: " + str);
            }
        }
    }

    for (size_t i = 0; i < letters_size; i++) {
        const std::string& lower_str = lowercase_letters[i];
        const std::string& upper_str = uppercase_letters[i];

        const size_t lower_hash = std::hash<std::string>()(lower_str);
        const size_t upper_hash = std::hash<std::string>()(upper_str);

        // lowercase_hash_map
        bool inserted_lower;
        std::tie(std::ignore, inserted_lower) = lowercase_hash_map.insert({ lower_hash, lower_str });
        if (!inserted_lower) {
            throw std::invalid_argument("Lowercase letter is duplicate: " + lower_str);
        }

        // uppercase_hash_map
        bool inserted_upper;
        std::tie(std::ignore, inserted_upper) = uppercase_hash_map.insert({ upper_hash, upper_str });
        if (!inserted_upper) {
            throw std::invalid_argument("Uppercase letter is duplicate: " + upper_str);
        }

        // tolower_map
        tolower_map.insert({ upper_hash, lower_hash });

        // toupper_map
        toupper_map.insert({ lower_hash, upper_hash });
    }
}

Letter Alphabet::tolower(Letter uppercase) const& noexcept
{
    const size_t upper_hash = std::hash<Letter>()(uppercase);
    const auto it = tolower_map.find(upper_hash);
    if (it == tolower_map.end()) {
        return uppercase;
    }
    const size_t lower_hash = it->second;
    return lowercase_hash_map.at(lower_hash);
}

Letter Alphabet::toupper(Letter lowercase) const& noexcept
{
    const size_t lower_hash = std::hash<Letter>()(lowercase);
    const auto it = toupper_map.find(lower_hash);
    if (it == toupper_map.end()) {
        return lowercase;
    }
    const size_t upper_hash = it->second;
    return uppercase_hash_map.at(upper_hash);
}

std::set<Letter> Alphabet::get_lowercase_letters() const&
{
    std::set<Letter> lowercase_letters;
    std::transform(lowercase_hash_map.rbegin(), lowercase_hash_map.rend(),
        std::inserter(lowercase_letters, lowercase_letters.begin()),
        [](const auto& it) -> Letter { return it.second; });
    return lowercase_letters;
}

std::set<Letter> Alphabet::get_uppercase_letters() const&
{
    std::set<Letter> uppercase_letters;
    std::transform(uppercase_hash_map.rbegin(), uppercase_hash_map.rend(),
        std::inserter(uppercase_letters, uppercase_letters.begin()),
        [](const auto& it) -> Letter { return it.second; });
    return uppercase_letters;
}

std::set<Letter> Alphabet::get_all_letters() const&
{
    std::set<Letter> lowercase_letters = get_lowercase_letters();
    std::set<Letter> uppercase_letters = get_uppercase_letters();
    lowercase_letters.merge(std::move(uppercase_letters));
    return lowercase_letters;
}

std::string Alphabet::get_lowercase_letter_regex_str() const
{
    return get_regex_group_str(lowercase_hash_map);
}

std::string Alphabet::get_uppercase_letter_regex_str() const
{
    return get_regex_group_str(lowercase_hash_map);
}

std::string Alphabet::get_letter_regex_str() const
{
    return get_regex_group_str(get_all_letters());
}

Alphabet alphabet_from_file(const std::string& filename)
{
    if (!std::filesystem::exists(filename)) {
        throw std::runtime_error("Alphabet file does not exist: " + filename);
    }
    std::ifstream ifs(filename);
    std::vector<std::string> lowercase_letters;
    std::vector<std::string> uppercase_letters;
    for (std::string line; std::getline(ifs, line);) {
        constexpr char separator = '\t';
        const std::string::size_type pos = line.find(separator);
        if (pos == std::string::npos) {
            throw std::runtime_error(R"(Alphabet file contains a line without case separator tab character (\t): )" + line);
        }
        if (pos != line.rfind(separator)) {
            throw std::runtime_error(R"(Alphabet file contains multiple separator tab characters (\t): )" + line);
        }
        std::string lowercase_str = line.substr(0, pos);
        std::string uppercase_str = line.substr(pos + sizeof(separator));
        lowercase_letters.emplace_back(std::move(lowercase_str));
        uppercase_letters.emplace_back(std::move(uppercase_str));
    }
    return Alphabet(std::move(lowercase_letters), std::move(uppercase_letters));
}

size_t get_max_letter_size(const std::set<Letter>& letters)
{
    std::vector<size_t> letter_sizes(letters.size());
    std::transform(letters.begin(), letters.end(), letter_sizes.begin(), [](Letter letter) { return letter.string_view().size(); });
    return *std::max_element(letter_sizes.begin(), letter_sizes.end());
}

} // namespace speller
