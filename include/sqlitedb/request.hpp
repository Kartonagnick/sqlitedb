
// [2021y-01m-23d] Idrisov Denis R.

#pragma once
#ifndef dSQLITEDB_REQUEST_USED_ 
#define dSQLITEDB_REQUEST_USED_ 1

#include <type_traits>
#include <cstdint>
#include <string>
#include <atomic>

struct sqlite3_stmt;

//==============================================================================
//==============================================================================
namespace db
{
    template<class T> class getval;
    using str_t = ::std::string;
    class connection;
    class cursor;

    class request
    {
        template<class T> 
        friend class getval;
        friend class connection;
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
        request&& operator << (T&& value) && noexcept
        { 
            this->bind(::std::forward<T>(value));
            return std::move(*this);
        }

        template<class T> void operator >> (T&& dst) &&;

    private:
        size_t columns() const noexcept;

        template<class T> auto get_value(const size_t index)
        { 
            using y = ::std::remove_reference_t<T>;
            using x = ::std::remove_cv_t<y>;
            using from = db::getval<x>;
            return from::get(*this, index);
        }
    private:
        void bind(const void* blob, const size_t size) noexcept;

        void bind(const ::std::nullptr_t) noexcept;

        void bind(const bool            value) noexcept;
        void bind(const float           value) noexcept;
        void bind(const double          value) noexcept;

        void bind(const char*           value) noexcept;
        void bind(const str_t&          value) noexcept;

        void bind(const ::std::int8_t   value) noexcept;
        void bind(const ::std::int16_t  value) noexcept;
        void bind(const ::std::int32_t  value) noexcept;
        void bind(const ::std::int64_t  value) noexcept;

        void bind(const ::std::uint8_t  value) noexcept;
        void bind(const ::std::uint16_t value) noexcept;
        void bind(const ::std::uint32_t value) noexcept;
        void bind(const ::std::uint64_t value) noexcept;
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
        void finalize();
        bool next();
    private:
        stmtT* m_cursor;
        sizeT  m_index;
    };


    #define dGETVALUE_DB(type, method)                       \
    template<> class getval<type>                            \
    {                                                        \
        public:                                              \
        static type                                          \
            get(request& owner, const size_t index) noexcept \
                { return owner.get_##method(index); }        \
    }

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

#include <sqlitedb/cursor.hpp>

namespace db
{
    template<class T>
    void request::operator >> (T&& dst) &&
    { 
        cursor::template get(*this, dst);
    }

} // namespace db

//==============================================================================
//==============================================================================
#endif // !dSQLITEDB_REQUEST_USED_
