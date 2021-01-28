
#include <mygtest/modern.hpp>
#include <sqlitedb/sqlitedb.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_OPERATOR_SQL

#define dTEST_COMPONENT db, connection
#define dTEST_METHOD operator_sql
#define dTEST_TAG tdd

//==============================================================================
//==============================================================================

#include <sqlitedb/connection.hpp>
#include "test-staff.hpp"
using str_t  = ::std::string;
namespace staff = staff_sqlitedb;

//==============================================================================
//==============================================================================

namespace
{
    const char* base = "000-sample.db";
}

//==============================================================================
//==============================================================================

TEST_COMPONENT(000)
{
    // --- simple sql-request
    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        size_t count = size_t(-1);
        const auto con = db::connect(base, db::eCREATE);
        ASSERT_NO_THROW(
            con << "SELECT COUNT(*) FROM sqlite_master" >> count
        );
        ASSERT_TRUE(count == 0);
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(001)
{
    // --- simple sql-request
    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        const auto con = db::connect(base, db::eCREATE);
        ASSERT_ANY_THROW(con << "123aadsa");
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

#ifndef NDEBUG
TEST_COMPONENT(002)
{
    // -- prepare
    ASSERT_NO_THROW(staff::dbaseDelete(base));
    ASSERT_NO_THROW(staff::makeTableAge(base, "users"));
}
TEST_COMPONENT(003)
{
    const char* sql = nullptr;
    const auto con = db::connect(base);
    ASSERT_DEATH_DEBUG(con << sql);
}
TEST_COMPONENT(004)
{
    const char* sql = "";
    const auto con = db::connect(base);
    ASSERT_DEATH_DEBUG(con << sql);
}
TEST_COMPONENT(005)
{
    const str_t sql = "";
    const auto con = db::connect(base);
    ASSERT_DEATH_DEBUG(con << sql);
}
TEST_COMPONENT(006)
{
    // --- clean
    ASSERT_TRUE(staff::dbaseDelete(base));
}
#endif // !!NDEBUG

//==============================================================================
//==============================================================================
#endif // ! TEST_OPERATOR_SQL
