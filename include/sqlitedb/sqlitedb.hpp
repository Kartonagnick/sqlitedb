
// [2021y-01m-23d] Idrisov Denis R.

#pragma once
#ifndef dSQLITEDB_USED_ 
#define dSQLITEDB_USED_ 1

#include <sqlitedb/database.hpp>
#include <sqlitedb/connect.hpp>
#include <sqlitedb/stmt.hpp>
#include <string>
#include <memory>

//==============================================================================
//=== [connection] =============================================================

namespace db
{
    class device;

    class connection
    {
        friend connection connect(
            const str_t&, 
            const eOPENMODE, 
            const size_t
        );

        connection() noexcept;
    public:
       ~connection();

        connection(const connection&)            noexcept;
        connection(connection&&)                 noexcept;
        connection& operator=(const connection&) noexcept;
        connection& operator=(connection&&)      noexcept;

        stmt operator << (const str_t& sql) const noexcept;
        stmt operator << (const char*  sql) const noexcept;

        void dropTable(const str_t&) const noexcept;

        bool existTable(const char*) const noexcept;

        template<class s>
        bool existTable(const s& n) const noexcept
            { return this->existTable(&n[0]); }

        bool existColumn(const char*, const char*)  const noexcept;

        template<class s>
        bool existColumn(const s& n, const s& col) const noexcept
            { return this->existColumn(&n[0], &col[0]); }

    private:
        using shared_t = std::shared_ptr<device>;
        shared_t m_data;
    };

} // namespace db

//==============================================================================
//==============================================================================

#endif // !dSQLITEDB_USED_

