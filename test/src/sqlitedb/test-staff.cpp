
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_STAFF

#define dTEST_COMPONENT staff
#define dTEST_METHOD filesystem
#define dTEST_TAG tdd

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
    ASSERT_THROW(staff::openRead(filename), std::runtime_error);
    {
        auto out = staff::openWrite(filename);
        out << "test-000\n";
        out << "test-001\n";
    }
    {
        str_t line1, line2;
        auto in = staff::openRead(filename);
        in >> line1;
        ASSERT_TRUE(line1 == "test-000");

        in >> line2;
        ASSERT_TRUE(line2 == "test-001");
    }
    ASSERT_TRUE(staff::fileExists(filename));
    staff::fileDelete(filename);
    ASSERT_TRUE(!staff::fileExists(filename));
    ASSERT_THROW(staff::openRead(filename), std::runtime_error);
}

// --- death
TEST_COMPONENT(001)
{
    const char* filename = nullptr;
    ASSERT_DEATH_DEBUG(staff::openRead(filename));
    (void)filename;
}
TEST_COMPONENT(002)
{
    const char* filename = "";
    ASSERT_DEATH_DEBUG(staff::openRead(filename));
    (void)filename;
}
TEST_COMPONENT(003)
{
    const char* filename = nullptr;
    ASSERT_DEATH_DEBUG(staff::openWrite(filename));
    (void)filename;
}
TEST_COMPONENT(004)
{
    const char* filename = "";
    ASSERT_DEATH_DEBUG(staff::openWrite(filename));
    (void)filename;
}
TEST_COMPONENT(005)
{
    const char* filename = nullptr;
    ASSERT_DEATH_DEBUG(staff::fileExists(filename));
    (void)filename;
}
TEST_COMPONENT(006)
{
    const char* filename = "";
    ASSERT_DEATH_DEBUG(staff::fileExists(filename));
    (void)filename;
}
TEST_COMPONENT(007)
{
    const char* filename = nullptr;
    ASSERT_DEATH_DEBUG(staff::fileDelete(filename));
    (void)filename;
}
TEST_COMPONENT(008)
{
    const char* filename = "";
    ASSERT_DEATH_DEBUG(staff::fileDelete(filename));
    (void)filename;
}

//==============================================================================
//==============================================================================
#endif // ! TEST_STAFF