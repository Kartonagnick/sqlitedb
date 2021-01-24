
// [2021y-01m-23d] Idrisov Denis R.

#include <sqlitedb/stmt.hpp>
#include "numeric_cast.hpp"
#include <sqlite3.h>
#include "device.hpp"
#include <stdexcept>
#include <cassert>
#include <cstring>

//==============================================================================
//===[integer] ====================================================================
namespace db
{
    using stmtT = ::sqlite3_stmt;
    using sizeT = ::std::atomic_size_t;

    inline int integer(const size_t value) noexcept
    {
        return ::tools::assert_numeric_cast<int>(value);
    }

    inline stmtT* cursor(void* ptr) noexcept
    {
        assert(ptr);
        return static_cast<stmtT*>(ptr);
    }

} // namespace db

//==============================================================================
//===[sqlite3] =================================================================
namespace db
{
    // bind
    namespace
    {
        void bindText(stmtT* cursor, const size_t index, 
            const char* value, const size_t len) noexcept
        {
            assert(cursor);
            assert(value);
            assert(std::strlen(value) >= len);
            const int ret = ::sqlite3_bind_text(
                cursor, 
                ::db::integer(index), 
                value, 
                ::db::integer(len), 
                SQLITE_TRANSIENT
            );
            assert(ret == SQLITE_OK);
            (void)ret;
        }

        void bindInt(stmtT* cursor, const size_t index, const int value) noexcept
        {
            assert(cursor);
            const int ret = ::sqlite3_bind_int(
                cursor, 
                ::db::integer(index), 
                value
            );
            assert(ret == SQLITE_OK);
            (void)ret;
        }

        template<class T> 
        void bindIntTemplate(stmtT* cursor, sizeT& index, T value) noexcept
        {
            assert(cursor);
            const int v = ::tools::numeric_cast<int>(value);
            ::db::bindInt(cursor, index, v);
            ++index;
        }

    } //namespace

} // namespace db


//==============================================================================
//===[connection::data] ========================================================
namespace db
{
    stmt::stmt(stmt&& rhs) noexcept
        : m_cursor(rhs.m_cursor)
        , m_index(m_index.load())
    {
        rhs.m_cursor = nullptr;
        rhs.m_index  = 0;
    }

    stmt::stmt(void* cursor) noexcept
        : m_cursor(cursor)
        , m_index(1)
    {}

    stmt::~stmt()
    {
        this->endQuery();
    }

    bool stmt::next() 
    { 
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);        
        const auto result = ::sqlite3_step(cur);
        assert(result == SQLITE_ROW || result == SQLITE_DONE);
        if (result != SQLITE_ROW && result != SQLITE_DONE)
            throw std::runtime_error("[db::next] sqlite3_step: failed");
        return result == SQLITE_ROW;
    }

    void stmt::endQuery() noexcept 
    { 
        if(!this->m_cursor)
        {
            assert(this->m_index  == 0);
            return;
        }
        auto* cur = ::db::cursor(this->m_cursor);        
        
        this->next();
        const auto ret = ::sqlite3_finalize(cur);
        assert(ret == SQLITE_OK);
        this->m_cursor = nullptr;
        this->m_index  = 0;
    }

    void stmt::bind(const void* value, const size_t size) noexcept
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        const int ret = ::sqlite3_bind_blob(
            ::db::cursor(this->m_cursor), 
            ::db::integer(this->m_index), 
            value, 
            ::db::integer(size), 
            SQLITE_TRANSIENT
        );
        assert(ret == SQLITE_OK);
        (void)ret;
    }

    void stmt::bind(const ::std::nullptr_t) noexcept
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        const auto ret = ::sqlite3_bind_null(
            ::db::cursor(this->m_cursor), 
            ::db::integer(this->m_index)
        );
        assert(ret == SQLITE_OK);
        (void)ret;
        ++this->m_index;
    }

    void stmt::bind(const char* value) noexcept
    {
        assert(this->m_index > 0);
        assert(value);
        assert(this->m_cursor);
        ::db::bindText(
            ::db::cursor(this->m_cursor), 
            ::db::integer(this->m_index),
            value, 
            std::strlen(value)
        );
        ++this->m_index;
    }

    void stmt::bind(const str_t& value) noexcept
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        ::db::bindText(
            ::db::cursor(this->m_cursor), 
            ::db::integer(this->m_index),
            value.c_str(), 
            value.length()
        );
        ++this->m_index;
    }
                                                 
    void stmt::bind(const float value) noexcept
    {
        assert(this->m_cursor);
        assert(this->m_index > 0);
        const double v = static_cast<double>(value);
        this->bind(v);
    }

    void stmt::bind(const double value) noexcept
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        const int ret = ::sqlite3_bind_double(
            ::db::cursor(this->m_cursor), 
            ::db::integer(this->m_index),
            value
        );
        assert(ret == SQLITE_OK);
        (void)ret;
        ++this->m_index;
    }
    
    void stmt::bind(const bool value) noexcept
    {
        assert(this->m_cursor);
        assert(this->m_index > 0);
        const ::std::uint8_t v(value);
        return this->bind(v);
    }

    void stmt::bind(const ::std::int8_t value) noexcept
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        ::db::bindIntTemplate(cur, this->m_index, value);
    }

    void stmt::bind(const ::std::int16_t value) noexcept
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        ::db::bindIntTemplate(cur, this->m_index, value);
    }

    void stmt::bind(const ::std::int32_t value) noexcept
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        ::db::bindIntTemplate(cur, this->m_index, value);
    }

    void stmt::bind(const ::std::int64_t value) noexcept
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        ::db::bindIntTemplate(cur, this->m_index, value);
    }

    void stmt::bind(const ::std::uint8_t value) noexcept
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        ::db::bindIntTemplate(cur, this->m_index, value);
    }

    void stmt::bind(const ::std::uint16_t value) noexcept
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        ::db::bindIntTemplate(cur, this->m_index, value);
    }

    void stmt::bind(const ::std::uint32_t value) noexcept
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        ::db::bindIntTemplate(cur, this->m_index, value);
    }

    void stmt::bind(const ::std::uint64_t value) noexcept
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        ::db::bindIntTemplate(cur, this->m_index, value);
    }

} // namespace db

//==============================================================================
//==============================================================================

namespace db
{
    int stmt::get_type(const size_t index) noexcept
    {
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        return ::sqlite3_column_type(
            cur, ::db::integer(index)
        );
    }

    const void* stmt::get_blob(const size_t index) noexcept
    {
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        return ::sqlite3_column_blob(
            cur, ::db::integer(index)
        );
    }

    size_t stmt::get_bytes(const size_t index) noexcept
    {
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        return ::sqlite3_column_bytes(
            cur, ::db::integer(index)
        );
    }

    str_t stmt::get_text(const size_t index) noexcept
    {
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        const auto* result = ::sqlite3_column_text(
            cur, ::db::integer(index)
        );
        assert(result);
        return reinterpret_cast<const char*>(result);
    }

    double stmt::get_double(const size_t index) noexcept
    {
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        return ::sqlite3_column_double(
            cur, ::db::integer(index)
        );
    }

    float stmt::get_float(const size_t index) noexcept
    {
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        const auto result = ::sqlite3_column_double(
            cur, ::db::integer(index)
        );
        return ::tools::numeric_cast<float>(result);
    }

    bool stmt::get_bool(const size_t index) noexcept
    {
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        const auto result = ::sqlite3_column_int(
            cur, ::db::integer(index)
        );
        return result != 0;
    }

    ::std::int8_t stmt::get_int8(const size_t index) noexcept
    {
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        const auto result = ::sqlite3_column_int(
            cur, ::db::integer(index)
        );
        return ::tools::numeric_cast<::std::int8_t>(result);
    }

    ::std::int16_t stmt::get_int16(const size_t index) noexcept
    {
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        const auto result = ::sqlite3_column_int(
            cur, ::db::integer(index)
        );
        return ::tools::numeric_cast<::std::int16_t>(result);
    }

    ::std::int32_t stmt::get_int32(const size_t index) noexcept
    {
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        const auto result = ::sqlite3_column_int(
            cur, ::db::integer(index)
        );
        return ::tools::numeric_cast<::std::int32_t>(result);
    }

    ::std::int64_t stmt::get_int64(const size_t index) noexcept
    {
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        const auto result = ::sqlite3_column_int64(
            cur, ::db::integer(index)
        );
        return ::tools::numeric_cast<::std::int64_t>(result);
    }

    ::std::uint8_t stmt::get_uint8(const size_t index) noexcept
    {
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        const auto result = ::sqlite3_column_int(
            cur, ::db::integer(index)
        );
        assert(result >= 0);
        return ::tools::numeric_cast<::std::uint8_t>(result);
    }

    ::std::uint16_t stmt::get_uint16(const size_t index) noexcept
    {
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        const auto result = ::sqlite3_column_int(
            cur, ::db::integer(index)
        );
        assert(result >= 0);
        return ::tools::numeric_cast<::std::uint16_t>(result);
    }

    ::std::uint32_t stmt::get_uint32(const size_t index) noexcept
    {
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        const auto result = ::sqlite3_column_int(
            cur, ::db::integer(index)
        );
        assert(result >= 0);
        return ::tools::numeric_cast<::std::uint32_t>(result);
    }

    ::std::uint64_t stmt::get_uint64(const size_t index) noexcept
    {
        assert(this->m_cursor);
        auto* cur = ::db::cursor(this->m_cursor);
        const auto result = ::sqlite3_column_int64(
            cur, ::db::integer(index)
        );
        assert(result >= 0);
        return ::tools::numeric_cast<::std::uint64_t>(result);
    }

} // namespace db