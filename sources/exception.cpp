
// [2021y-01m-26d] Idrisov Denis R.

#include "exception.hpp"
//==============================================================================
//==============================================================================
namespace db
{
    const char* code2desc(const int code) noexcept;
    const char* code2name(const int code) noexcept;

    const char* exception::code2desc(const int code) noexcept
    {
        return ::db::code2desc(code);
    }

    const char* exception::code2name(const int code) noexcept
    {
        return ::db::code2desc(code);
    }

    const char* exception::code2desc() const noexcept
    {
        return ::db::code2desc(this->m_code);
    }

    const char* exception::code2name() const noexcept
    {
        return ::db::code2name(this->m_code);
    }

} // namespace db

//==============================================================================
//==============================================================================
