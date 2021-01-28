
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_EXISTS

#define dTEST_COMPONENT db
#define dTEST_METHOD exists
#define dTEST_TAG tdd

#include "test-staff.hpp"
namespace staff = staff_sqlitedb;

using str_t = ::std::string;

namespace db
{
    bool exists(const char*  name) noexcept;
    bool exists(const str_t& name) noexcept;

} // namespace db

//==============================================================================
//==============================================================================

TEST_COMPONENT(000)
{
    const char* filename = "test-000.txt";

    staff::fileDelete(filename);
    ASSERT_TRUE(!staff::fileExists(filename));
    ASSERT_TRUE(!db::exists(filename));
    {
        auto out = staff::openWrite(filename);
        out << "test-000\n";
    }
    ASSERT_TRUE(staff::fileExists(filename));
    ASSERT_TRUE(db::exists(filename));
    
    ASSERT_TRUE( staff::fileDelete(filename));
    ASSERT_TRUE(!staff::fileExists(filename));
    ASSERT_TRUE(!db::exists(filename));
}

TEST_COMPONENT(001)
{
    const str_t filename = "test-000.txt";

    staff::fileDelete(filename);
    ASSERT_TRUE(!staff::fileExists(filename));
    ASSERT_TRUE(!db::exists(filename));
    {
        auto out = staff::openWrite(filename);
        out << "test-000\n";
    }
    ASSERT_TRUE(staff::fileExists(filename));
    ASSERT_TRUE(db::exists(filename));
    
    ASSERT_TRUE( staff::fileDelete(filename));
    ASSERT_TRUE(!staff::fileExists(filename));
    ASSERT_TRUE(!db::exists(filename));
}

#ifndef NDEBUG
TEST_COMPONENT(002)
{
    const char* filename = nullptr;
    ASSERT_DEATH_DEBUG(db::exists(filename));
}
TEST_COMPONENT(003)
{
    const char* filename = "";
    ASSERT_DEATH_DEBUG(db::exists(filename));
}
TEST_COMPONENT(004)
{
    const str_t filename = "";
    ASSERT_DEATH_DEBUG(db::exists(filename));
}
#endif // !!NDEBUG

//==============================================================================
//==============================================================================
#endif // ! TEST_EXISTS