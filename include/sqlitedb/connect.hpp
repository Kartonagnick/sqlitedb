
// [2021y-01m-23d] Idrisov Denis R.

#pragma once
#ifndef ddSQLITEDB_CONNECT_USED_ 
#define ddSQLITEDB_CONNECT_USED_ 1

#include <string>

//==============================================================================
//=== [connection] =============================================================

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

        // The database is opened for reading and writing, 
        //   and is created if it does not already exist. 
        eCREATE    = 1<<3, 

        // The filename can be interpreted as a URI if this flag is set.
        eOPEN_URI  = 1<<4 
    };

} // namespace db

//==============================================================================
//=== [connect] ================================================================

namespace db
{
    class connection;

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

#endif // !ddSQLITEDB_CONNECT_USED_

