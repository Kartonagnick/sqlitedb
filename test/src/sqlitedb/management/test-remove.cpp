
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_REMOVE

#define dTEST_COMPONENT db
#define dTEST_METHOD remove
#define dTEST_TAG tdd

#include "test-staff.hpp"
namespace staff = staff_sqlitedb;

using str_t = ::std::string;

namespace db
{
    bool remove(const char*  name) noexcept;
    bool remove(const str_t& name) noexcept;

} // namespace db
namespace me = db;
//==============================================================================
//==============================================================================

TEST_COMPONENT(000)
{
    const char* filename = "test-000.txt";
    staff::fileDelete(filename);
    {
        auto out = staff::openWrite(filename);
        out << "test-000\n";
    }
    ASSERT_TRUE(staff::fileExists(filename));
    ASSERT_TRUE(db::remove(filename));
    ASSERT_TRUE(!staff::fileExists(filename));
}

TEST_COMPONENT(001)
{
    const str_t filename = "test-000.txt";
    staff::fileDelete(filename);
    {
        auto out = staff::openWrite(filename);
        out << "test-000\n";
    }
    ASSERT_TRUE(staff::fileExists(filename));
    ASSERT_TRUE(db::remove(filename));
    ASSERT_TRUE(!staff::fileExists(filename));
}

#ifndef NDEBUG
TEST_COMPONENT(002)
{
    const char* filename = nullptr;
    ASSERT_DEATH_DEBUG(me::remove(filename));
}
TEST_COMPONENT(003)
{
    const char* filename = "";
    ASSERT_DEATH_DEBUG(me::remove(filename));
}
TEST_COMPONENT(004)
{
    const str_t filename = "";
    ASSERT_DEATH_DEBUG(me::remove(filename));
}
#endif // !!NDEBUG

//==============================================================================
//==============================================================================
#endif // ! TEST_REMOVE