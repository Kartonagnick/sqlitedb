
// [2021y-01m-23d] Idrisov Denis R.

#include <sqlitedb/connection.hpp>
#include "exception.hpp"
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
    bool connection::dropTable(const str_t& table) const
    {
        //const str_t sql = "DROP TABLE IF EXISTS " + table;
        const str_t sql = "DROP TABLE " + table;
        try
        {
            *this << sql;
            return true;
        }
        catch (const db::exception& e)
        {
            const auto code = e.code();
            if (code == db::exception::eTABLE_DOES_NOT_EXIST)
                return false;
            throw;
        }
    }

    bool connection::dropTable(const char* name) const
    {
        assert(name);
        assert(*name != 0);
        return this->dropTable(str_t(name));
    }


    bool connection::dropColumn(const str_t& table, const str_t& column) const
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
        assert(*name != 0);

        const char* sql 
            = "SELECT count(*) FROM sqlite_master WHERE type='table' AND name=?";

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

        #if 0
            // if exist column  -> true
            // if !exist column -> false
            // if !exist table  -> false
            const char* sql = "SELECT COUNT(*) FROM pragma_table_info(?) WHERE name=?";
        #endif

        #if 0
            | info_table | info_column |   description    |
            |      1     |     1       | column exist     |
            |      1     |     0       | column not exist |
            |      0     |     0       | table not exist  |
        #endif

        const char* sql= R"raw(
            SELECT
                info_table.count as info_table, info_column.count as info_column
            FROM
                (SELECT count() as count FROM sqlite_master WHERE type='table' AND name=:A) 
                    info_table,
                (SELECT count() as count from pragma_table_info(:A) where name =:B) 
                    info_column
        )raw";
        
        bool exist_table  = false;
        bool exist_column = false;

        #ifndef NDEBUG
            size_t count = 0;
            const auto lambda = [&count, &exist_table, &exist_column]
            (const bool table, const bool column) noexcept
            {
                ++count;
                exist_table  = table; 
                exist_column = column;
                return true;
            };
        #else
            const auto lambda = [&exist_table, &exist_column]
            (const bool table, const bool column) noexcept
            {
                exist_table  = table; 
                exist_column = column;
                return true;
            };
        #endif

        *this << sql << table << column
            >> lambda;

        assert(count == 0 || count == 1);

        if (!exist_table)
            throw std::runtime_error(
                "[connection::existColumn] table: '" + str_t(table) + "' not exist"
            );
        return exist_column;
    }

} // namespace db

//==============================================================================
//==============================================================================
namespace db
{
    connection connect(const str_t& path, const eOPENMODE mode, const size_t timeout)
    {
        assert(!path.empty());
        auto shared = ::std::make_shared<db::device>(path, mode, timeout);
        assert(shared);
        connection result;
        result.m_data = std::move(shared);
        return result;
    }

    connection connect(const char* path, const eOPENMODE mode, const size_t timeout)
    {
        assert(path);
        assert(*path != 0);
        return db::connect(str_t(path), mode, timeout);
    }

} // namespace db

//==============================================================================
//==============================================================================
