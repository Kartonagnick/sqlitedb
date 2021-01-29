
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_DROP_DATABASE

#define dTEST_COMPONENT db
#define dTEST_METHOD dropDatabase
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
    ASSERT_TRUE(!db::dropDatabase(filename));
}

TEST_COMPONENT(001)
{
    const str_t base = "test-000.db";
    const str_t shm = "test-000.db-shm";
    const str_t wal = "test-000.db-wal";

    staff::fileDelete(base);
    staff::fileDelete(shm);
    staff::fileDelete(wal);

    ASSERT_TRUE(!staff::fileExists(base));
    ASSERT_TRUE(!staff::fileExists(shm ));
    ASSERT_TRUE(!staff::fileExists(wal ));

    staff::openWrite(base) << "test-000\n";
    staff::openWrite(shm ) << "test-000\n";
    staff::openWrite(wal ) << "test-000\n";

    ASSERT_TRUE(staff::fileExists(base));
    ASSERT_TRUE(staff::fileExists(shm ));
    ASSERT_TRUE(staff::fileExists(wal ));

    ASSERT_TRUE(db::dropDatabase(base));

    ASSERT_TRUE(!staff::fileExists(base));
    ASSERT_TRUE(!staff::fileExists(shm ));
    ASSERT_TRUE(!staff::fileExists(wal ));
}

TEST_COMPONENT(002)
{
    const str_t base = "test-000.db";
    const str_t shm = "test-000.db-shm";
    const str_t wal = "test-000.db-wal";

    staff::fileDelete(base);
    staff::fileDelete(shm);
    staff::fileDelete(wal);

    ASSERT_TRUE(!staff::fileExists(base));
    ASSERT_TRUE(!staff::fileExists(shm ));
    ASSERT_TRUE(!staff::fileExists(wal ));

    ASSERT_NO_THROW(staff::makeTable(base, "users"));
    ASSERT_TRUE(db::dropDatabase(base));

    ASSERT_TRUE(!staff::fileExists(base));
    ASSERT_TRUE(!staff::fileExists(shm ));
    ASSERT_TRUE(!staff::fileExists(wal ));
}


#ifndef NDEBUG
TEST_COMPONENT(003)
{
    const char* filename = nullptr;
    ASSERT_DEATH_DEBUG(db::existDatabase(filename));
}
TEST_COMPONENT(004)
{
    const char* filename = "";
    ASSERT_DEATH_DEBUG(db::existDatabase(filename));
}
TEST_COMPONENT(005)
{
    const str_t filename = "";
    ASSERT_DEATH_DEBUG(db::existDatabase(filename));
}
#endif // !!NDEBUG

//==============================================================================
//==============================================================================
#endif // ! TEST_DROP_DATABASE
