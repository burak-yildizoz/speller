#include <speller/word.hpp>
////////////////////////////////////////////////////////////////////////////////
// Standard Headers
#include <string_view>
////////////////////////////////////////////////////////////////////////////////
// User Defined Headers
#include <speller/alphabet.hpp>
#include <speller/letter.hpp>
#include <speller/locale.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace speller {

Word::Word(std::string str_value, std::string locale_name_value)
    : str(std::move(str_value))
    , locale_name(std::move(locale_name_value))
{
    // create locale object
    const Locale locale(locale_name);
    // obtain letters in current locale
    const std::set<Letter>& all_letters = locale.get_alphabet().get_all_letters();
    // calculate maximum possible character size of a letter
    const size_t max_letter_size = get_max_letter_size(all_letters);
    // string start point
    const char* const begin_ptr = str.c_str();
    // covered length in string
    size_t current_total_len = 0;
    // uncovered start point in string
    auto get_ch_ptr = [&begin_ptr, &current_total_len]() {
        return begin_ptr + current_total_len;
    };
    // add letter and cover the length
    auto emplace = [this, &current_total_len](std::string_view sw) {
        letters.emplace_back(sw);
        current_total_len += sw.length();
    };
    // run until all string is covered
    while (current_total_len != str.length()) {
        // check whether a letter exists at current position
        bool letter_found = false;
        // no need to exceed maximum letter length for searching
        for (size_t len = 1; len <= max_letter_size; len++) {
            // letter candidate
            const std::string_view sw(get_ch_ptr(), len);
            // check whether the candidate is a letter
            if (all_letters.count(sw)) {
                letter_found = true;
                emplace(sw);
                break;
            }
            // bounds check
            bool end_of_string = (current_total_len + len == str.length());
            if (end_of_string) [[unlikely]] {
                break;
            }
        }
        // no letter exists at current position
        // add unknown character to letters
        if (!letter_found) {
            const std::string_view sw(get_ch_ptr(), 1);
            emplace(sw);
        }
    }
}

Word::operator const std::string&() const& noexcept
{
    return str;
}

size_t Word::length() const noexcept
{
    return letters.size();
}

Letter Word::operator[](size_t index) const&
{
    return letters[index];
}

Letter Word::at(size_t index) const&
{
    return letters.at(index);
}

Word Word::tolower() const
{
    // create locale object
    const Locale locale(locale_name);
    // create alphabet object
    const Alphabet& alphabet = locale.get_alphabet();
    // calculate maximum possible character size of a lowercase letter
    const size_t max_lowercase_letter_size = get_max_letter_size(alphabet.get_lowercase_letters());
    // allocate maximum possible space required beforehand
    std::string lower_str;
    lower_str.reserve(letters.size() * max_lowercase_letter_size);
    // convert letters to lowercase
    for (Letter letter : letters) {
        Letter lower_letter = alphabet.tolower(letter);
        lower_str += lower_letter.string();
    }
    return Word(lower_str);
}

Word Word::toupper() const
{
    // create locale object
    const Locale locale(locale_name);
    // create alphabet object
    const Alphabet& alphabet = locale.get_alphabet();
    // calculate maximum possible character size of an uppercase letter
    const size_t max_uppercase_letter_size = get_max_letter_size(alphabet.get_uppercase_letters());
    // allocate maximum possible space required beforehand
    std::string upper_str;
    upper_str.reserve(letters.size() * max_uppercase_letter_size);
    // convert letters to uppercase
    for (Letter letter : letters) {
        Letter upper_letter = alphabet.toupper(letter);
        upper_str += upper_letter.string();
    }
    return Word(upper_str);
}

} // namespace speller
