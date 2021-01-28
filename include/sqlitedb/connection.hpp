
// [2021y-01m-23d] Idrisov Denis R.

#pragma once
#ifndef dSQLITEDB_CONNECTION_USED_ 
#define dSQLITEDB_CONNECTION_USED_ 1

#include <sqlitedb/request.hpp>
#include <string>
#include <memory>

//==============================================================================
//=== [eOPENMODE] ==============================================================

namespace db
{
    using str_t = ::std::string;

    enum eOPENMODE 
    { 
        // The database is opened in read-only mode. 
        // If the database does not already exist, an error is returned.
        eREADONLY  = 1<<1, 

        // The database is opened for reading and writing if possible, 
        //   or reading only if the file is write protected by the operating system. 
        // In either case the database must already exist, otherwise an error is returned.
        eREADWRITE = 1<<2, 

        // The filename can be interpreted as a URI if this flag is set.
        eOPEN_URI  = 1<<3, 

        // The database is opened for reading and writing, 
        //   and is created if it does not already exist. 
        eCREATE    = 1<<4, 

    };

} // namespace db

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

        request operator << (const str_t& sql) const;
        request operator << (const char*  sql) const;
    public:
        bool existTable(const char* name) const;

        // table must be exist
        bool existColumn(const char* table, const char* column) const;

        bool dropTable(const str_t& name) const;

        // table must be exist
        bool dropColumn(const str_t& table, const str_t& column) const;
    public:
        template<class s>
        bool existTable(const s& name) const
            { return this->existTable(&name[0]); }

        template<class s1, class s2>
        bool existColumn(const s1& table, const s2& column) const
            { return this->existColumn(&table[0], &column[0]); }
    private:
        using shared_t = std::shared_ptr<device>;
        shared_t m_data;
    };

} // namespace db

//==============================================================================
//=== [connect] ================================================================

namespace db
{
    connection connect(

        // path to database file or URI
        const str_t& path, 

        // open mode
        const eOPENMODE mode = eREADONLY,

        // This routine sets a busy handler that sleeps 
        //   for a specified amount of time when a table is locked. 
        // The handler will sleep multiple times until at least "ms" milliseconds
        //   of sleeping have accumulated. 
        // After at least "ms" milliseconds of sleeping, 
        //   the handler returns 0 which causes sqlite3_step() 
        //   to return SQLITE_BUSY.

        const size_t timeout = 150 // milliseconds
    );

} // namespace db

//==============================================================================
//==============================================================================


#endif // !dSQLITEDB_CONNECTION_USED_

