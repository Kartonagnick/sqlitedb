
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_EXISTS_DATABASE

#define dTEST_COMPONENT db
#define dTEST_METHOD existDatabase
#define dTEST_TAG tdd

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
    ASSERT_TRUE(!staff::fileExists(filename));
    ASSERT_TRUE(!db::existDatabase(filename));
    {
        auto out = staff::openWrite(filename);
        out << "test-000\n";
    }
    ASSERT_TRUE(staff::fileExists(filename));
    ASSERT_TRUE(db::existDatabase(filename));
    
    ASSERT_TRUE( staff::fileDelete(filename));
    ASSERT_TRUE(!staff::fileExists(filename));
    ASSERT_TRUE(!db::existDatabase(filename));
}

TEST_COMPONENT(001)
{
    const str_t filename = "test-000.txt";

    staff::fileDelete(filename);
    ASSERT_TRUE(!staff::fileExists(filename));
    ASSERT_TRUE(!db::existDatabase(filename));
    {
        auto out = staff::openWrite(filename);
        out << "test-000\n";
    }
    ASSERT_TRUE(staff::fileExists(filename));
    ASSERT_TRUE(db::existDatabase(filename));
    
    ASSERT_TRUE( staff::fileDelete(filename));
    ASSERT_TRUE(!staff::fileExists(filename));
    ASSERT_TRUE(!db::existDatabase(filename));
}

#ifndef NDEBUG
TEST_COMPONENT(002)
{
    const char* filename = nullptr;
    ASSERT_DEATH_DEBUG(db::existDatabase(filename));
}
TEST_COMPONENT(003)
{
    const char* filename = "";
    ASSERT_DEATH_DEBUG(db::existDatabase(filename));
}
TEST_COMPONENT(004)
{
    const str_t filename = "";
    ASSERT_DEATH_DEBUG(db::existDatabase(filename));
}
#endif // !!NDEBUG

//==============================================================================
//==============================================================================
#endif // ! TEST_EXISTS_DATABASE