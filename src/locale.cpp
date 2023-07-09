#include <speller/locale.hpp>
////////////////////////////////////////////////////////////////////////////////
// Standard Headers
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>
////////////////////////////////////////////////////////////////////////////////
// User defined Headers
#include <speller/alphabet.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace speller {

static Alphabet get_default_alphabet()
{
    std::vector<std::string> lowercase_letters;
    for (char ch = 'a'; ch <= 'z'; ch++) {
        lowercase_letters.emplace_back(std::string { ch });
    }
    std::vector<std::string> uppercase_letters;
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        uppercase_letters.emplace_back(std::string { ch });
    }
    return Alphabet(std::move(lowercase_letters), std::move(uppercase_letters));
}

std::map<std::string, Alphabet> Locale::locales = { { default_locale_name, get_default_alphabet() } };

Locale::Locale(std::string locale_name_value)
    : locale_name(std::move(locale_name_value))
{
    // given locale must exist
    if (locales.find(locale_name) == locales.end()) {
        throw std::invalid_argument("Locale does not exist: " + locale_name);
    }
}

void Locale::add_locale(std::string locale_name, Alphabet alphabet_value)
{
    bool inserted;
    std::tie(std::ignore, inserted) = locales.insert({ locale_name, std::move(alphabet_value) });
    if (!inserted) {
        throw std::invalid_argument("Locale already exists: " + locale_name);
    }
}

const Alphabet& Locale::get_alphabet() const& noexcept
{
    return locales.at(locale_name);
}

} // namespace speller
