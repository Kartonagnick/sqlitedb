
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_STAFF

#define dTEST_COMPONENT staff
#define dTEST_METHOD filesystem
#define dTEST_TAG tdd

#include <sqlitedb/sqlitedb.hpp>

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
    ASSERT_TRUE(staff::fileDelete(filename));
    ASSERT_TRUE(!staff::fileExists(filename));
    ASSERT_THROW(staff::openRead(filename), std::runtime_error);
}

TEST_COMPONENT(001)
{
    const char* base = "test-000.db";
    const char* shm  = "test-000.db-shm";
    const char* wal  = "test-000.db-wal";

    staff::fileDelete(base);
    staff::fileDelete(shm);
    staff::fileDelete(wal);

    ASSERT_TRUE(!staff::fileExists(base));
    ASSERT_TRUE(!staff::fileExists(shm));
    ASSERT_TRUE(!staff::fileExists(wal));

    staff::openWrite(base) << "test-000\n";
    staff::openWrite(shm)  << "test-000\n";
    staff::openWrite(wal)  << "test-000\n";

    ASSERT_TRUE(staff::fileExists(base));
    ASSERT_TRUE(staff::fileExists(shm));
    ASSERT_TRUE(staff::fileExists(wal));

    ASSERT_TRUE(staff::dbaseDelete(base));

    ASSERT_TRUE(!staff::fileExists(base));
    ASSERT_TRUE(!staff::fileExists(shm));
    ASSERT_TRUE(!staff::fileExists(wal));
}

TEST_COMPONENT(002)
{
    const char* base = "test-000.db";
    const char* shm  = "test-000.db-shm";
    const char* wal  = "test-000.db-wal";

    staff::fileDelete(base);
    staff::fileDelete(shm);
    staff::fileDelete(wal);

    ASSERT_TRUE(!staff::fileExists(base));
    ASSERT_TRUE(!staff::fileExists(shm));
    ASSERT_TRUE(!staff::fileExists(wal));

    staff::openWrite(base) << "test-000\n";
    staff::openWrite(shm)  << "test-000\n";
    staff::openWrite(wal)  << "test-000\n";

    ASSERT_TRUE(staff::fileExists(base));
    ASSERT_TRUE(staff::fileExists(shm));
    ASSERT_TRUE(staff::fileExists(wal));

    ASSERT_TRUE(staff::dbaseDelete("test-000"));

    ASSERT_TRUE(!staff::fileExists(base));
    ASSERT_TRUE(!staff::fileExists(shm));
    ASSERT_TRUE(!staff::fileExists(wal));
}

TEST_COMPONENT(003)
{
    const char* base = "test-000.db";

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    ASSERT_TRUE(!staff::fileExists(base));
    ASSERT_NO_THROW(staff::makeTableAge(base, "age"));
    ASSERT_TRUE(staff::fileExists(base));
    ASSERT_TRUE(staff::dbaseDelete(base));
    ASSERT_TRUE(!staff::fileExists(base));
}

TEST_COMPONENT(004)
{
    const char* base = "test-000";
    const char* shm  = "test-000-shm";
    const char* wal  = "test-000-wal";

    ASSERT_NO_THROW(staff::dbaseDelete(base));

    ASSERT_TRUE(!staff::fileExists(base));
    ASSERT_TRUE(!staff::fileExists(shm));
    ASSERT_TRUE(!staff::fileExists(wal));

    ASSERT_NO_THROW(staff::makeTableAge(base, "age"));
    staff::openWrite(shm) << "test-000\n";
    staff::openWrite(wal) << "test-000\n";

    ASSERT_TRUE(staff::fileExists(base));
    ASSERT_TRUE(staff::fileExists(shm));
    ASSERT_TRUE(staff::fileExists(wal));

    ASSERT_TRUE(staff::dbaseDelete(base));

    ASSERT_TRUE(!staff::fileExists(base));
    ASSERT_TRUE(!staff::fileExists(shm));
    ASSERT_TRUE(!staff::fileExists(wal));
}

#ifndef NDEBUG

TEST_COMPONENT(005)
{
    const char* filename = nullptr;
    ASSERT_DEATH_DEBUG(staff::openRead(filename));
    (void)filename;
}
TEST_COMPONENT(006)
{
    const char* filename = "";
    ASSERT_DEATH_DEBUG(staff::openRead(filename));
    (void)filename;
}
TEST_COMPONENT(007)
{
    const char* filename = nullptr;
    ASSERT_DEATH_DEBUG(staff::openWrite(filename));
    (void)filename;
}
TEST_COMPONENT(008)
{
    const char* filename = "";
    ASSERT_DEATH_DEBUG(staff::openWrite(filename));
}
TEST_COMPONENT(009)
{
    const char* filename = nullptr;
    ASSERT_DEATH_DEBUG(staff::fileExists(filename));
}
TEST_COMPONENT(010)
{
    const char* filename = "";
    ASSERT_DEATH_DEBUG(staff::fileExists(filename));
}
TEST_COMPONENT(011)
{
    const char* filename = nullptr;
    ASSERT_DEATH_DEBUG(staff::fileDelete(filename));
}
TEST_COMPONENT(012)
{
    const char* filename = "";
    ASSERT_DEATH_DEBUG(staff::fileDelete(filename));
}
TEST_COMPONENT(013)
{
    const char* filename = nullptr;
    ASSERT_DEATH_DEBUG(staff::dbaseDelete(filename));
}
TEST_COMPONENT(014)
{
    const char* filename = "";
    ASSERT_DEATH_DEBUG(staff::dbaseDelete(filename));
}

#endif //!!NDEBUG

//==============================================================================
//==============================================================================
#endif // ! TEST_STAFF