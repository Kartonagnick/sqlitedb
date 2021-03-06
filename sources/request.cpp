
// [2021y-01m-23d] Idrisov Denis R.

#include <sqlitedb/request.hpp>
#include "numeric_cast.hpp"
#include "exception.hpp"
#include <sqlite3.h>
#include "device.hpp"
#include <stdexcept>
#include <cassert>
#include <cstring>

//==============================================================================
//=== [integer] ================================================================
namespace db
{
    using sizeT = ::std::atomic_size_t;
    using stmtT = ::sqlite3_stmt;

    static inline int integer(const size_t value) noexcept
    {
        return ::tools::assert_numeric_cast<int>(value);
    }

    static inline void throwIfError(const int code)
    {
        assert(code == SQLITE_OK);
        if (code != SQLITE_OK)
            throw db::exception(code, "[request::bind] can`t bind argument");
    }

} // namespace db

//==============================================================================
//=== [sqlite3] ================================================================
namespace db
{
    // bind
    namespace
    {
        void bindText(stmtT* cursor, const size_t index, const char* value, const size_t len) 
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
            db::throwIfError(ret);
        }

        void bindInt(stmtT* cursor, const size_t index, const int value)
        {
            assert(cursor);
            const int ret = ::sqlite3_bind_int(
                cursor, 
                ::db::integer(index), 
                value
            );
            db::throwIfError(ret);
        }

        template<class T> 
        void bindIntTemplate(stmtT* cursor, sizeT& index, T value) 
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
    request::request(request&& rhs) noexcept
        : m_cursor(rhs.m_cursor)
        , m_index(rhs.m_index.load())
        , m_start(rhs.m_start)
    {
        rhs.m_cursor = nullptr;
        rhs.m_start  = 0;
        rhs.m_index  = 0;
    }

    request::request(stmtT* cursor) noexcept
        : m_cursor(cursor)
        , m_index(1)
        , m_start()
    {}

    request::~request() noexcept(false)
    {
        this->clear();
    }

    bool request::next() 
    { 
        assert(this->m_cursor);
        const auto code = ::sqlite3_step(this->m_cursor);
        assert(code == SQLITE_ROW || code == SQLITE_DONE);

        if (code != SQLITE_ROW && code != SQLITE_DONE)
            throw db::exception(code, "[db::next] sqlite3_step: failed");

        return code == SQLITE_ROW;
    }

    void request::finalize() noexcept
    {
        const auto ret = ::sqlite3_finalize(this->m_cursor);
        assert(ret == SQLITE_OK);
		(void) ret;
        this->m_cursor = nullptr;
        this->m_index  = 0;
    }

    void request::clear()  
    { 
        if(!this->m_cursor)
        {
            assert(this->m_index  == 0);
            return;
        }
        std::exception_ptr eptr;
        try
        {
            this->next();
        }
        catch (...)
        {
            eptr = std::current_exception();
        }
        this->finalize();
        if (eptr)
            std::rethrow_exception(eptr);
    }

    void request::bind(const void* value, const size_t size) 
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        const int ret = ::sqlite3_bind_blob(
            this->m_cursor, 
            ::db::integer(this->m_index), 
            value, 
            ::db::integer(size), 
            SQLITE_TRANSIENT
        );
        db::throwIfError(ret);
    }

    void request::bind(const ::std::nullptr_t) 
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        const auto ret = ::sqlite3_bind_null(
            this->m_cursor, 
            ::db::integer(this->m_index)
        );
        db::throwIfError(ret);
        ++this->m_index;
    }

    void request::bind(const char* value) 
    {
        assert(this->m_index > 0);
        assert(value);
        assert(this->m_cursor);
        ::db::bindText(
            this->m_cursor, 
            this->m_index,
            value, 
            std::strlen(value)
        );
        ++this->m_index;
    }

    void request::bind(const str_t& value) 
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        ::db::bindText(
            this->m_cursor, 
            this->m_index,
            value.c_str(), 
            value.length()
        );
        ++this->m_index;
    }
                                                 
    void request::bind(const float value) 
    {
        assert(this->m_cursor);
        assert(this->m_index > 0);
        const double v = static_cast<double>(value);
        this->bind(v);
    }

    void request::bind(const double value) 
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        const int ret = ::sqlite3_bind_double(
            this->m_cursor, 
            ::db::integer(this->m_index),
            value
        );
        db::throwIfError(ret);
        ++this->m_index;
    }
    
    void request::bind(const bool value) 
    {
        assert(this->m_cursor);
        assert(this->m_index > 0);
        const ::std::uint8_t v(value);
        return this->bind(v);
    }

    void request::bind(const ::std::int8_t value) 
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        ::db::bindIntTemplate(this->m_cursor, this->m_index, value);
    }

    void request::bind(const ::std::int16_t value) 
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        ::db::bindIntTemplate(this->m_cursor, this->m_index, value);
    }

    void request::bind(const ::std::int32_t value) 
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        ::db::bindIntTemplate(this->m_cursor, this->m_index, value);
    }

    void request::bind(const ::std::int64_t value) 
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        ::db::bindIntTemplate(this->m_cursor, this->m_index, value);
    }

    void request::bind(const ::std::uint8_t value) 
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        ::db::bindIntTemplate(this->m_cursor, this->m_index, value);
    }

    void request::bind(const ::std::uint16_t value) 
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        ::db::bindIntTemplate(this->m_cursor, this->m_index, value);
    }

    void request::bind(const ::std::uint32_t value) 
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        ::db::bindIntTemplate(this->m_cursor, this->m_index, value);
    }

    void request::bind(const ::std::uint64_t value) 
    {
        assert(this->m_index > 0);
        assert(this->m_cursor);
        ::db::bindIntTemplate(this->m_cursor, this->m_index, value);
    }

} // namespace db

//==============================================================================
//==============================================================================

namespace db
{
    void request::check_bind_count() const 
    {
        const size_t count = this->bind_count();
        assert(this->m_index == count + 1);
        if (this->m_index == count + 1)
            return;

        const str_t sc = std::to_string(count);
        const str_t sp = std::to_string(this->m_index - 1);
        throw ::std::logic_error(
            "[request::check_bind_count] "
                "expected " + sc + " arguments, but passed " + sp
        );
    }

    size_t request::bind_count() const noexcept
    {
        assert(this->m_cursor);
		return ::tools::assert_numeric_cast<size_t>(
            ::sqlite3_bind_parameter_count(this->m_cursor)
        );
    }

    size_t request::columns() const noexcept
    {
        assert(this->m_cursor);
		return ::tools::assert_numeric_cast<size_t>(
            ::sqlite3_column_count(this->m_cursor)
        );
    }

    int request::get_type(const size_t index) noexcept
    {
        assert(this->m_cursor);
        assert(index < this->columns());
        return ::sqlite3_column_type(
            this->m_cursor, ::db::integer(index)
        );
    }

    const void* request::get_blob(const size_t index) noexcept
    {
        assert(this->m_cursor);
        assert(index < this->columns());
        return ::sqlite3_column_blob(
            this->m_cursor, ::db::integer(index)
        );
    }

    size_t request::get_bytes(const size_t index) noexcept
    {
        assert(this->m_cursor);
        assert(index < this->columns());
        const auto bytes = ::sqlite3_column_bytes(
            this->m_cursor, ::db::integer(index)
        );
        assert(bytes >= 0);
        return ::tools::assert_numeric_cast<size_t>(bytes);
    }

    str_t request::get_text(const size_t index) noexcept
    {
        assert(this->m_cursor);
        assert(index < this->columns());
        const auto* result = ::sqlite3_column_text(
            this->m_cursor, ::db::integer(index)
        );
        assert(result);
        return reinterpret_cast<const char*>(result);
    }

    double request::get_double(const size_t index) noexcept
    {
        assert(this->m_cursor);
        assert(index < this->columns());
        return ::sqlite3_column_double(
            this->m_cursor, ::db::integer(index)
        );
    }

    float request::get_float(const size_t index) noexcept
    {
        assert(this->m_cursor);
        assert(index < this->columns());
        const auto result = ::sqlite3_column_double(
            this->m_cursor, ::db::integer(index)
        );
        return ::tools::numeric_cast<float>(result);
    }

    bool request::get_bool(const size_t index) noexcept
    {
        assert(this->m_cursor);
        assert(index < this->columns());
        const auto result = ::sqlite3_column_int(
            this->m_cursor, ::db::integer(index)
        );
        return result != 0;
    }

    ::std::int8_t request::get_int8(const size_t index) noexcept
    {
        assert(this->m_cursor);
        assert(index < this->columns());
        const auto result = ::sqlite3_column_int(
            this->m_cursor, ::db::integer(index)
        );
        return ::tools::numeric_cast<::std::int8_t>(result);
    }

    ::std::int16_t request::get_int16(const size_t index) noexcept
    {
        assert(this->m_cursor);
        assert(index < this->columns());
        const auto result = ::sqlite3_column_int(
            this->m_cursor, ::db::integer(index)
        );
        return ::tools::numeric_cast<::std::int16_t>(result);
    }

    ::std::int32_t request::get_int32(const size_t index) noexcept
    {
        assert(this->m_cursor);
        assert(index < this->columns());
        const auto result = ::sqlite3_column_int(
            this->m_cursor, ::db::integer(index)
        );
        return ::tools::numeric_cast<::std::int32_t>(result);
    }

    ::std::int64_t request::get_int64(const size_t index) noexcept
    {
        assert(this->m_cursor);
        assert(index < this->columns());
        const auto result = ::sqlite3_column_int64(
            this->m_cursor, ::db::integer(index)
        );
        return ::tools::numeric_cast<::std::int64_t>(result);
    }

    ::std::uint8_t request::get_uint8(const size_t index) noexcept
    {
        assert(this->m_cursor);
        assert(index < this->columns());
        const auto result = ::sqlite3_column_int(
            this->m_cursor, ::db::integer(index)
        );
        assert(result >= 0);
        return ::tools::numeric_cast<::std::uint8_t>(result);
    }

    ::std::uint16_t request::get_uint16(const size_t index) noexcept
    {
        assert(this->m_cursor);
        assert(index < this->columns());
        const auto result = ::sqlite3_column_int(
            this->m_cursor, ::db::integer(index)
        );
        assert(result >= 0);
        return ::tools::numeric_cast<::std::uint16_t>(result);
    }

    ::std::uint32_t request::get_uint32(const size_t index) noexcept
    {
        assert(this->m_cursor);
        assert(index < this->columns());
        const auto result = ::sqlite3_column_int(
            this->m_cursor, ::db::integer(index)
        );
        assert(result >= 0);
        return ::tools::numeric_cast<::std::uint32_t>(result);
    }

    ::std::uint64_t request::get_uint64(const size_t index) noexcept
    {
        assert(this->m_cursor);
        assert(index < this->columns());
        const auto result = ::sqlite3_column_int64(
            this->m_cursor, ::db::integer(index)
        );
        assert(result >= 0);
        return ::tools::numeric_cast<::std::uint64_t>(result);
    }

} // namespace db