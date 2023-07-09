#ifndef SPELLER_ALPHABET_HPP
#define SPELLER_ALPHABET_HPP

////////////////////////////////////////////////////////////////////////////////
// Standard Headers
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>
////////////////////////////////////////////////////////////////////////////////
// User Defined Headers
#include <speller/letter.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace speller {

class Alphabet {
public:
    // Define an alphabet by providing lowercase and uppercase letters
    // @note The letters in the same index must correspond to the same case.
    // @warning Throws if any letter contains a regex special character.
    Alphabet(const std::vector<std::string>& lowercase_letters, const std::vector<std::string>& uppercase_letters);

    /// Similar to std::tolower
    Letter tolower(Letter uppercase) const& noexcept;

    /// Similar to std::toupper
    Letter toupper(Letter lowercase) const& noexcept;

    /// Obtain lowercase letters in the alphabet
    std::set<Letter> get_lowercase_letters() const&;

    /// Obtain uppercase letters in the alphabet
    std::set<Letter> get_uppercase_letters() const&;

    /// Obtain all letters in the alphabet
    std::set<Letter> get_all_letters() const&;

    /// Similar to "([a-z])"
    std::string get_lowercase_letter_regex_str() const;

    /// Similar to "([A-Z])"
    std::string get_uppercase_letter_regex_str() const;

    /// Similar to "([a-zA-Z])"
    std::string get_letter_regex_str() const;

    /// @warning Throws if no such letter exists in the alphabet
    Letter get_letter_from_hash(size_t hash) const&;

private:
    std::map<size_t, std::string> lowercase_hash_map;
    std::map<size_t, std::string> uppercase_hash_map;
    std::map<size_t, size_t> tolower_map;
    std::map<size_t, size_t> toupper_map;
};

/**
Read #Alphabet from resource file

Each line represents a lowercase - uppercase pair.
The letters are separated by tab (`\t`) character.

@code
a	A
b	B
...
z	Z
@endcode

@warning Throws if file does not exist or formatting is incorrect.
@see #Alphabet::Alphabet
*/
Alphabet alphabet_from_file(const std::string& filename);

/// Maximum number of characters in a letter
size_t get_max_letter_size(const std::set<Letter>& letters);

} // namespace speller

#endif // SPELLER_ALPHABET_HPP
