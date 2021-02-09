
// [2021y-01m-26d] Idrisov Denis R.

#pragma once
#ifndef dSQLITEDB_EXCEPTION_USED_ 
#define dSQLITEDB_EXCEPTION_USED_ 100

#include <string>
#include <cassert>
#include <stdexcept>
//==============================================================================
//==============================================================================
namespace db
{
    using str_t = ::std::string;

    class exception : public ::std::runtime_error
    {
        typedef ::std::runtime_error
            base;

        #ifndef NDEBUG
        const char* check(const char* msg) const noexcept
        {
            assert(msg);
            return msg;
        }
        #endif

    public:
        enum eEXTENDED_ERROR
        {
            eDATABASE_NOT_AVAILABLE = 26,
            eTABLE_DOES_NOT_EXIST  = 200,  
        };

        static const char* code2desc(const int code) noexcept;
        static const char* code2name(const int code) noexcept;

    public:
        explicit exception(const int code, str_t msg) noexcept
            : base(::std::move(msg))
            , m_code(code)
        {}

        explicit exception(const int code, const char* msg) 
            #ifndef NDEBUG
            : base(check(msg))
            #else
            : base(msg)
            #endif
            , m_code(code)
        {}

        const char* code2desc() const noexcept;
        const char* code2name() const noexcept;

        int code() const noexcept
        {
            return this->m_code;
        }
    private:
        int m_code;
    };

} // namespace db

//==============================================================================
//==============================================================================
#endif // !dSQLITEDB_EXCEPTION_USED_