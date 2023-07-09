#ifndef SPELLER_LETTER_HPP
#define SPELLER_LETTER_HPP

////////////////////////////////////////////////////////////////////////////////
// Standard Headers
#include <functional>
#include <string>
#include <string_view>
////////////////////////////////////////////////////////////////////////////////

namespace speller {

/// std::string_view wrapper that holds reference to a Unicode character
/// @see Alphabet
class Letter {
public:
    /// Implicit constructor from std::string
    Letter(const std::string& str) noexcept;

    /// Implicit constructor from std::string_view
    Letter(std::string_view sw) noexcept;

    /// Conversion to std::string_view
    std::string_view string_view() const noexcept;

    /// Conversion to std::string
    std::string string() const;

private:
    std::string_view sw;
};

} // namespace speller

////////////////////////////////////////////////////////////////////////////////

template <>
struct std::less<speller::Letter> {
    constexpr bool operator()(const speller::Letter& lhs, const speller::Letter& rhs) const noexcept
    {
        return lhs.string_view() < rhs.string_view();
    }
};

template <>
struct std::hash<speller::Letter> {
    [[nodiscard]] size_t operator()(speller::Letter letter) const noexcept
    {
        return std::hash<std::string_view>()(letter.string_view());
    }
};

#endif // SPELLER_LETTER_HPP
