#include <speller/letter.hpp>
////////////////////////////////////////////////////////////////////////////////
// Standard Headers
#include <string>
#include <string_view>
////////////////////////////////////////////////////////////////////////////////

namespace speller {

Letter::Letter(const std::string& str) noexcept
    : sw(str)
{
}

Letter::Letter(std::string_view sw_value) noexcept
    : sw(sw_value)
{
}

std::string_view Letter::string_view() const noexcept
{
    return sw;
}

std::string Letter::string() const
{
    return static_cast<std::string>(sw);
}

} // namespace speller
