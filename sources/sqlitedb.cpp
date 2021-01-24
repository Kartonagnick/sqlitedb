
// [2021y-01m-23d] Idrisov Denis R.

#include <sqlitedb/sqlitedb.hpp>
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
            if(this->m_data)
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

    stmt connection::operator << (const str_t& sql) const noexcept
    {
        auto& device = *this->m_data;
        auto* cursor = device.begQuery(sql);
        return stmt(cursor);
    }

    stmt connection::operator << (const char* sql) const noexcept
    {
        auto& device = *this->m_data;
        auto* cursor = device.begQuery(sql);
        return stmt(cursor);
    }

} // namespace db

//==============================================================================
//==============================================================================
namespace db
{
    void connection::dropTable(const str_t& table) const noexcept
    {
        const str_t sql = "DROP TABLE IF EXISTS " + table;
        *this << sql;
    }

    bool connection::existTable(const char* name) const noexcept
    {
        assert(name);
        const char* sql 
            = "SELECT count(*) FROM sqlite_master "
              "WHERE type='table' AND name=?";

        size_t count = 0;
        const auto lambda = [&count](const size_t v) noexcept
            { count = v; };

        *this << sql << name >> count;
        assert(count == 0 || count == 1);
        return count != 0;
    }

    bool connection::existColumn(const char* table, const char* column) const noexcept
    {
        assert(table);
        assert(column);
        const char* sql
            = "SELECT COUNT(*) AS CNTREC FROM pragma_table_info(?) WHERE name=?";

        size_t count = 0;
        const auto lambda = [&count](const size_t v) noexcept
            { count = v; };

        *this << sql << table << column
            >> count;
        assert(count == 0 || count == 1);
        return count != 0;
    }

} // namespace db

//==============================================================================
//==============================================================================

