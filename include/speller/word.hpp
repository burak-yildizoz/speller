#ifndef SPELLER_WORD_HPP
#define SPELLER_WORD_HPP

////////////////////////////////////////////////////////////////////////////////
// Standard Headers
#include <string>
#include <string_view>
#include <vector>
////////////////////////////////////////////////////////////////////////////////
// User Defined Headers
#include <speller/letter.hpp>
#include <speller/locale.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace speller {

/// Access letters of a string in given locale
/// @see Locale
class Word {
public:
    /// @warning Throws if locale does not exist.
    explicit Word(std::string str, std::string locale_name = Locale::default_locale_name);

    /// Implicit conversion to std::string
    operator const std::string&() const& noexcept;

    /// Number of letters in given locale
    size_t length() const noexcept;

    /// Access letter at given index
    Letter operator[](size_t index) const&;

    /// Access letter at given index with bounds checking
    /// @warning Throws if @a index is greater or equal to #length.
    Letter at(size_t index) const&;

    /// @see Alphabet::tolower
    Word tolower() const;

    /// @see Alphabet::toupper
    Word toupper() const;

private:
    std::string str;
    std::string locale_name;
    std::vector<Letter> letters;
};

} // namespace speller

#endif // SPELLER_WORD_HPP
