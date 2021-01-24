
// [2021y-01m-23d] Idrisov Denis R.

#pragma once
#ifndef ddSQLITEDB_STMP_USED_ 
#define ddSQLITEDB_STMP_USED_ 1

#include <cstdint>
#include <string>
#include <atomic>

//==============================================================================
//===[connection::data] ========================================================
namespace db
{
    template<class T> class getval;
    using str_t = ::std::string;
    class connection;
    class cursor;

    class stmt
    {
        friend class connection;
        friend class cursor;

        template<class T> 
        friend class getval;

        using sizeT = ::std::atomic_size_t;
    public:
        stmt(stmt&&) noexcept;

        stmt(const stmt&)            = delete;
        stmt& operator=(const stmt&) = delete;
        stmt& operator=(stmt&&)      = delete;

        ~stmt();

    public:
        template<class T>
        stmt& operator << (T&& value) noexcept
        { 
            this->bind(::std::forward<T>(value));
            return *this;
        }

        template<class T>
        decltype(auto) operator >> (T&& dst);

        template<class T> T getValue(const size_t index)
        { 
            using from = db::getval<T>;
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
        stmt(void* cursor) noexcept;
        void endQuery() noexcept;
        bool next();
    private:
        void*  m_cursor;
        sizeT  m_index;
    };


    #define dGETVALUE_DB(type, method)                            \
    template<> class getval<type>                                 \
    {                                                             \
        public:                                                   \
        static type get(stmt& owner, const size_t index) noexcept \
            { return owner.get_##method(index); }                 \
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
    decltype(auto) stmt::operator >> (T&& dst)  
    { 
        //const bool success = owner.endQuery();
        //assert(success);
        //(void)success;

        return cursor::template get<T>(
            *this, std::forward<T>(dst)
        );
    }

} // namespace db

//==============================================================================
//==============================================================================
#endif // !ddSQLITEDB_STMP_USED_
