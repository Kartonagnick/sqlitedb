
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_EXISTS_TABLE

#define dTEST_COMPONENT db
#define dTEST_METHOD existTable
#define dTEST_TAG tdd

#include <sqlitedb/management.hpp>
#include <sqlitedb/connection.hpp>

#include "test-staff.hpp"
namespace staff = staff_sqlitedb;

using str_t = ::std::string;
namespace me = db;
//==============================================================================
//==============================================================================

TEST_COMPONENT(000)
{
    const char* table    = "users";
    const char* filename = "test-000.txt";
    staff::fileDelete(filename);
    ASSERT_TRUE(!staff::fileExists(filename));
    ASSERT_ANY_THROW(db::existTable(filename, table));
}

TEST_COMPONENT(001)
{
    const char* table    = "users";
    const char* filename = "test-000.txt";
    staff::fileDelete(filename);
    ASSERT_TRUE(!staff::fileExists(filename));
    {
        auto out = staff::openWrite(filename);
        out << "test-000\n";
    }
    ASSERT_TRUE(staff::fileExists(filename));
    ASSERT_ANY_THROW(db::existTable(filename, table));
    ASSERT_TRUE(staff::fileDelete(filename));
}

TEST_COMPONENT(002)
{
    const char* table = "users";
    const char* base = "test-000.db";

    staff::dbaseDelete(base);
    ASSERT_NO_THROW(db::connect(base, db::eCREATE));
    ASSERT_TRUE(staff::fileExists(base));
    ASSERT_TRUE(!db::existTable(base, table));
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(003)
{
    const char* table = "users";
    const char* base = "test-000.db";

    staff::dbaseDelete(base);

    ASSERT_NO_THROW(staff::makeTableAge(base, table));
    ASSERT_TRUE(staff::fileExists(base));
    ASSERT_TRUE(db::existTable(base, table));
    ASSERT_TRUE(!db::existTable(base, "noexist"));
    ASSERT_TRUE(staff::dbaseDelete(base));
}

#ifndef NDEBUG
TEST_COMPONENT(004)
{
    const char* filename = nullptr;
    const char* table    = "11";
    ASSERT_DEATH_DEBUG(me::existTable(filename, table));
}
TEST_COMPONENT(005)
{
    const char* filename = "";
    const char* table    = "11";
    ASSERT_DEATH_DEBUG(me::existTable(filename, table));
}
TEST_COMPONENT(006)
{
    const str_t filename = "";
    const char* table    = "11";
    ASSERT_DEATH_DEBUG(me::existTable(filename, table));
}
TEST_COMPONENT(007)
{
    const char* filename = "11";
    const char* table    = nullptr;
    ASSERT_DEATH_DEBUG(me::existTable(filename, table));
}
TEST_COMPONENT(008)
{
    const char* filename = "11";
    const char* table    = "";
    ASSERT_DEATH_DEBUG(me::existTable(filename, table));
}
TEST_COMPONENT(009)
{
    const str_t filename = "11";
    const str_t table    = "";
    ASSERT_DEATH_DEBUG(me::existTable(filename, table));
}
#endif // !!NDEBUG

//==============================================================================
//==============================================================================
#endif // ! TEST_EXISTS_TABLE