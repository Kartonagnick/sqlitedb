
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_AVAILABLE_DATABASE

#define dTEST_COMPONENT db
#define dTEST_METHOD availableDatabase
#define dTEST_TAG tdd

#include <sqlitedb/connection.hpp>
#include <sqlitedb/management.hpp>

#include "test-staff.hpp"
namespace staff = staff_sqlitedb;
using str_t = ::std::string;

//==============================================================================
//==============================================================================

TEST_COMPONENT(000)
{
    const char* filename = "test-000.txt";
    staff::fileDelete(filename);
    ASSERT_TRUE(!db::availableDatabase(filename));
    {
        auto out = staff::openWrite(filename);
        out << "test-000\n";
    }
    ASSERT_TRUE(!db::availableDatabase(filename));
    ASSERT_TRUE(staff::fileDelete(filename));
}
TEST_COMPONENT(001)
{
    const char* base = "test-000.db";
    staff::dbaseDelete(base);
    ASSERT_TRUE(!db::availableDatabase(base));
    ASSERT_NO_THROW(db::connect(base, db::eCREATE));
    ASSERT_TRUE(db::availableDatabase(base));
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(002)
{
    const str_t filename = "test-000.txt";
    staff::fileDelete(filename);
    ASSERT_TRUE(!db::availableDatabase(filename));
    {
        auto out = staff::openWrite(filename);
        out << "test-000\n";
    }
    ASSERT_TRUE(!db::availableDatabase(filename));
    ASSERT_TRUE(staff::fileDelete(filename));
}
TEST_COMPONENT(003)
{
    const str_t base = "test-000.db";
    staff::dbaseDelete(base);
    ASSERT_TRUE(!db::availableDatabase(base));
    ASSERT_NO_THROW(db::connect(base, db::eCREATE));
    ASSERT_TRUE(db::availableDatabase(base));
    ASSERT_TRUE(staff::dbaseDelete(base));
}

#ifndef NDEBUG
TEST_COMPONENT(004)
{
    const char* filename = nullptr;
    ASSERT_DEATH_DEBUG(db::availableDatabase(filename));
}
TEST_COMPONENT(005)
{
    const char* filename = "";
    ASSERT_DEATH_DEBUG(db::availableDatabase(filename));
}
TEST_COMPONENT(006)
{
    const str_t filename = "";
    ASSERT_DEATH_DEBUG(db::availableDatabase(filename));
}
#endif // !!NDEBUG

//==============================================================================
//==============================================================================
#endif // ! TEST_AVAILABLE_DATABASE