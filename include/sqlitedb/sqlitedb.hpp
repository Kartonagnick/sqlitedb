
// [2021y-01m-23d] Idrisov Denis R.

#pragma once
#ifndef dSQLITEDB_USED_ 
#define dSQLITEDB_USED_ 1

#include <string>

namespace db
{
    using str_t = ::std::string;

    //class settings;
    class connection;

    enum eOPENMODE 
    { 
        eREADONLY  = 1<<1, ///< Open with 'read only' mode
        eREADWRITE = 1<<2, ///< Open with 'read and write' mode
        eCREATE    = 1<<3, ///< Create the database if it does not exist.
        eOPEN_URI  = 1<<4  ///< open for creating database
    };

    connection connect(const str_t& path);
    connection connect(const str_t& path);

} // namespace db

//==============================================================================
//==============================================================================

#endif // !dSQLITEDB_USED_

