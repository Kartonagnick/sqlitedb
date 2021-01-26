
// [2021y-01m-24d] Idrisov Denis R.

#pragma once
#ifndef dSQLITEDB_DATABASE_USED_ 
#define dSQLITEDB_DATABASE_USED_ 1

#include <string>

//==============================================================================
//==============================================================================

namespace db
{
    using str_t = ::std::string;

    bool remove(const char*  name) noexcept;
    bool exists(const char*  name) noexcept;

    bool remove(const str_t& name) noexcept;
    bool exists(const str_t& name) noexcept;

} // namespace db

//==============================================================================
//==============================================================================

#endif // !dSQLITEDB_DATABASE_USED_

