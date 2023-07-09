#ifndef SPELLER_LOCALE_HPP
#define SPELLER_LOCALE_HPP

////////////////////////////////////////////////////////////////////////////////
// Standard Headers
#include <map>
#include <string>
////////////////////////////////////////////////////////////////////////////////
// User Defined Headers
#include <speller/alphabet.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace speller {

/// Access alphabet of a locale globally via configured name
/// @note English alphabet is configured to #default_locale_name by default.
class Locale {
public:
    /// English alphabet locale name
    static constexpr char default_locale_name[] = "en";

    /// @warning Throws if locale does not exist.
    explicit Locale(std::string locale_name = default_locale_name);

    /// Add locale globally
    /// @warning Throws if locale exists.
    static void add_locale(std::string locale_name, Alphabet alphabet);

    /// Access alphabet of given locale
    const Alphabet& get_alphabet() const& noexcept;

private:
    static std::map<std::string, Alphabet> locales;

    std::string locale_name;
};

} // namespace speller

#endif // SPELLER_LOCALE_HPP
