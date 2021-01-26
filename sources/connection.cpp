
// [2021y-01m-23d] Idrisov Denis R.

#include <sqlitedb/connection.hpp>
#include "device.hpp"
#include <cassert>

//==============================================================================
//=== [constructors] ===========================================================
namespace db
{
    connection::connection() noexcept
        : m_data()
    {}

    connection::~connection()
    {
        try
        {
            if (this->m_data)
                this->m_data->disconnect();
        }
        catch (const std::exception& e)
        {
            const char* reason = e.what();
            (void) reason;
            assert(false && "(std::exception): from destructor");
        }
    }

    connection::connection(const connection& rhs) noexcept
        : m_data(rhs.m_data)
    {}

    connection::connection(connection&& rhs) noexcept
        : m_data(std::move(rhs.m_data))
    {}

    connection& connection::operator=(const connection& rhs) noexcept
    {
        if (this != &rhs)
            this->m_data = rhs.m_data;
        return *this;
    }

    connection& connection::operator=(connection&& rhs) noexcept
    {
        if (this != &rhs)
            this->m_data = std::move(rhs.m_data);
        return *this;
    }

    request connection::operator << (const str_t& sql) const
    {
        auto& device = *this->m_data;
        auto* cursor = device.begQuery(sql);
        return request(cursor);
    }

    request connection::operator << (const char* sql) const
    {
        auto& device = *this->m_data;
        auto* cursor = device.begQuery(sql);
        return request(cursor);
    }

} // namespace db

//==============================================================================
//==============================================================================
namespace db
{
    void connection::dropTable(const str_t& table) const
    {
        const str_t sql = "DROP TABLE IF EXISTS " + table;
        *this << sql;
    }


    void connection::dropColumn(const str_t& table, const str_t& column) const
    {
        (void) table;
        (void) column;
        throw std::runtime_error("in developmant");
        #if 0
        - create new table as the one you are trying to change,
        - copy all data,
        - drop old table,
        - rename the new one.
        #endif
    }


    bool connection::existTable(const char* name) const
    {
        assert(name);
        const char* sql 
            = "SELECT count(*) FROM sqlite_master "
              "WHERE type='table' AND name=?";

        size_t count = 0;
        const auto lambda = [&count](const size_t v) noexcept
        { 
            count = v; 
            return true;
        };

        *this << sql << name >> lambda;
        assert(count == 0 || count == 1);
        return count != 0;
    }

    bool connection::existColumn(const char* table, const char* column) const
    {
        assert(table);
        assert(column);
        const char* sql
            = "SELECT COUNT(*) AS CNTREC FROM pragma_table_info(?) WHERE name=?";

        size_t count = 0;
        const auto lambda = [&count](const size_t v) noexcept
        {
            count = v; 
            return true;
        };

        *this << sql << table << column
            >> lambda;
        assert(count == 0 || count == 1);
        return count != 0;
    }

} // namespace db

//==============================================================================
//==============================================================================
namespace db
{
    connection connect(const str_t& path, const eOPENMODE mode, 
        const size_t timeout)
    {
        auto shared = ::std::make_shared<db::device>(path, mode, timeout);
        assert(shared);
        connection result;
        result.m_data = std::move(shared);
        return result;
    }

} // namespace db

//==============================================================================
//==============================================================================
