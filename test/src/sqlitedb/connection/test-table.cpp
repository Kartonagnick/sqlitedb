
#include <mygtest/modern.hpp>
#include <sqlitedb/sqlitedb.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_TABLE

#define dTEST_COMPONENT db, connection
#define dTEST_METHOD table
#define dTEST_TAG tdd

//==============================================================================
//==============================================================================

#include <sqlitedb/connection.hpp>
#include "test-staff.hpp"
using str_t  = ::std::string;
namespace staff = staff_sqlitedb;
namespace me = db;

//==============================================================================
//==============================================================================

namespace
{
    const char* base = "000-sample.db";
}

//==============================================================================
//==============================================================================

// --- existTable
TEST_COMPONENT(000)
{
    ASSERT_NO_THROW(staff::dbaseDelete(base));
    ASSERT_NO_THROW(staff::makeTableAge(base, "users"));
    ASSERT_NO_THROW(staff::makeTableAge(base, "clients"));
    {
        const auto con = db::connect(base);
        ASSERT_TRUE( con.existTable("users"   ));
        ASSERT_TRUE( con.existTable("clients" ));
        ASSERT_TRUE(!con.existTable("no_exist"));
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

#ifndef NDEBUG
TEST_COMPONENT(001)
{
    const char* table = nullptr;
    const auto con = db::connect(base, db::eCREATE);
    ASSERT_DEATH_DEBUG(con.existTable(table));
}
TEST_COMPONENT(002)
{
    const char* table = "";
    const auto con = db::connect(base, db::eCREATE);
    ASSERT_DEATH_DEBUG(con.existTable(table));
}
TEST_COMPONENT(003)
{
    const str_t table = "";
    const auto con = db::connect(base, db::eCREATE);
    ASSERT_DEATH_DEBUG(con.existTable(table));
}
#endif // !!NDEBUG

// --- dropTable
TEST_COMPONENT(004)
{
    ASSERT_NO_THROW(staff::dbaseDelete(base));
    ASSERT_NO_THROW(staff::makeTableAge(base, "users"));
    ASSERT_NO_THROW(staff::makeTableAge(base, "clients"));
    {
        const auto con = db::connect(base, db::eREADWRITE);
        ASSERT_TRUE( con.existTable("users"   ));
        ASSERT_TRUE( con.existTable("clients" ));
        ASSERT_TRUE(!con.existTable("no_exist"));

        ASSERT_TRUE( con.dropTable("users"    ));
        ASSERT_TRUE( con.dropTable("clients"  ));
        ASSERT_TRUE(!con.dropTable("no_exist" ));

        ASSERT_TRUE(!con.existTable("users"   ));
        ASSERT_TRUE(!con.existTable("clients" ));
        ASSERT_TRUE(!con.existTable("no_exist"));
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

#ifndef NDEBUG
TEST_COMPONENT(005)
{
    const char* table = nullptr;
    ASSERT_NO_THROW(staff::dbaseDelete(base));
    ASSERT_NO_THROW(staff::makeTableAge(base, "users"));
    const auto con = db::connect(base, db::eREADWRITE);
    ASSERT_DEATH_DEBUG(con.dropTable(table));
}
TEST_COMPONENT(006)
{
    const char* table = "";
    ASSERT_NO_THROW(staff::dbaseDelete(base));
    ASSERT_NO_THROW(staff::makeTableAge(base, "users"));
    const auto con = db::connect(base, db::eREADWRITE);
    ASSERT_DEATH_DEBUG(con.dropTable(table));
}
TEST_COMPONENT(007)
{
    const str_t table = "";
    ASSERT_NO_THROW(staff::dbaseDelete(base));
    ASSERT_NO_THROW(staff::makeTableAge(base, "users"));
    const auto con = db::connect(base, db::eREADWRITE);
    ASSERT_DEATH_DEBUG(con.dropTable(table));
}
#endif // !!NDEBUG

//==============================================================================
//==============================================================================
#endif // ! TEST_TABLE
