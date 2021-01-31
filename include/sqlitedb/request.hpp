
// [2021y-01m-23d] Idrisov Denis R.

#pragma once
#ifndef dSQLITEDB_REQUEST_USED_ 
#define dSQLITEDB_REQUEST_USED_ 1

#include <type_traits>
#include <exception>
#include <cstdint>
#include <cassert>
#include <string>
#include <atomic>

struct sqlite3_stmt;

//==============================================================================
//==============================================================================
namespace db
{
    template<class t> 
    using degradate = ::std::remove_cv_t<
        ::std::remove_reference_t<t> 
    >;

    template<class T> class getval;
    using str_t = ::std::string;
    class connection;
    class irerator;
    class cursor;

    class request
    {
        template<class T> 
        friend class getval;
        friend class connection;
        friend class irerator;
        friend class cursor;

        using sizeT = ::std::atomic_size_t;
        using stmtT = ::sqlite3_stmt;
    public:
        request(const request&)            = delete;
        request& operator=(const request&) = delete;
        request& operator=(request&&)      = delete;

       ~request() noexcept(false);
    public:
        template<class T>
        request&& operator << (const T& value) &&;

        template<class T> void operator >> (T&& dst) &&;
    private:
        void check_bind_count() const;
        size_t bind_count() const noexcept;

        size_t columns() const noexcept;

        template<class T> auto get_value(const size_t index);

        template<class T> void get_value(const size_t index, T&& dst);
    private:
        void bind(const void* blob, const size_t size);

        void bind(const ::std::nullptr_t);

        void bind(const bool            value);
        void bind(const float           value);
        void bind(const double          value);

        void bind(const char*           value);
        void bind(const str_t&          value);

        void bind(const ::std::int8_t   value);
        void bind(const ::std::int16_t  value);
        void bind(const ::std::int32_t  value);
        void bind(const ::std::int64_t  value);

        void bind(const ::std::uint8_t  value);
        void bind(const ::std::uint16_t value);
        void bind(const ::std::uint32_t value);
        void bind(const ::std::uint64_t value);
    private:
        int             get_type  (const size_t index) noexcept;
        const void*     get_blob  (const size_t index) noexcept;
        size_t          get_bytes (const size_t index) noexcept;
        str_t           get_text  (const size_t index) noexcept;
        double          get_double(const size_t index) noexcept;
        float           get_float (const size_t index) noexcept;
        bool            get_bool  (const size_t index) noexcept;
        ::std::int8_t   get_int8  (const size_t index) noexcept;
        ::std::int16_t  get_int16 (const size_t index) noexcept;
        ::std::int32_t  get_int32 (const size_t index) noexcept;
        ::std::int64_t  get_int64 (const size_t index) noexcept;
        ::std::uint8_t  get_uint8 (const size_t index) noexcept;
        ::std::uint16_t get_uint16(const size_t index) noexcept;
        ::std::uint32_t get_uint32(const size_t index) noexcept;
        ::std::uint64_t get_uint64(const size_t index) noexcept;
    private:
        request(request&&)     noexcept;
        request(stmtT* cursor) noexcept;
        
        void finalize() noexcept;

        void clear();
        bool next();
    private:
        stmtT* m_cursor;
        sizeT  m_index;
        int    m_start;
    };


    #define dGETVALUE_DB(type, method)                   \
    template<> class getval<type>                        \
    {                                                    \
        friend class request;                            \
        static type                                      \
        get(request& owner, const size_t index) noexcept \
            { return owner.get_##method(index); }        \
    }

    template<class T> class getval
    {
        using p = ::std::add_pointer_t<T>;
        using c = ::std::add_const_t<p>;
        static_assert(::std::is_same<T, c > ::value,
            "not support: user type 'T' has no specialization"
        );
    };


    dGETVALUE_DB(db::str_t    , text  );
    dGETVALUE_DB(double       , double);
    dGETVALUE_DB(float        , float );
    dGETVALUE_DB(bool         , bool  );
    dGETVALUE_DB(std::int8_t  , int8  );
    dGETVALUE_DB(std::int16_t , int16 );
    dGETVALUE_DB(std::int32_t , int32 );
    dGETVALUE_DB(std::int64_t , int64 );
    dGETVALUE_DB(std::uint8_t , uint8 );
    dGETVALUE_DB(std::uint16_t, uint16);
    dGETVALUE_DB(std::uint32_t, uint32);
    dGETVALUE_DB(std::uint64_t, uint64);
    #undef dGETVALUE_DB

} // namespace db

namespace db
{
    template<class T> 
    void request::get_value(const size_t index, T&& dst)
        { dst = this->get_value<T>(index); }

    template<class T>
    auto request::get_value(const size_t index)
    {
        using x = ::db::degradate<T>;
        using from = ::db::getval<x>;

        ::std::exception_ptr eptr;
        try
        {
            return from::get(*this, index);
        }
        catch (...)
        {
            eptr = ::std::current_exception();
        }
        assert(eptr);
        this->finalize();
        ::std::rethrow_exception(eptr);
    }

    template<class T>
    request&& request::operator << (const T& value) &&
    {
        ::std::exception_ptr eptr;
        try
        {
            this->bind(value);
        }
        catch (...)
        {
            eptr = ::std::current_exception();
        }

        if (eptr)
        {
            this->finalize();
            ::std::rethrow_exception(eptr);
        }
        return ::std::move(*this);
    }

} // namespace db

#include <sqlitedb/cursor.hpp>

namespace db
{
    template<class T>
    void request::operator >> (T&& dst) &&
    {
        if (this->m_start != 0)
        {
            ::db::irerator(*this).get(dst);
            return;
        }
        this->m_start = 1;
        this->check_bind_count();
        ::db::cursor(*this).get(dst);
        this->m_start = 0;
    }

} // namespace db

//==============================================================================
//==============================================================================
#endif // !dSQLITEDB_REQUEST_USED_
