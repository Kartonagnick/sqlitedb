
// [2021y-01m-23d] Idrisov Denis R.

#include <sqlitedb/sqlitedb.hpp>
#include <sqlitedb/connect.hpp>
#include "device.hpp"
#include <cassert>

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


