
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_TUPLE

#define dTEST_COMPONENT db, request
#define dTEST_METHOD tuple
#define dTEST_TAG tdd

#include "test-staff.hpp"
#include <sqlitedb/sqlitedb.hpp>

namespace staff = staff_sqlitedb;
using str_t = ::std::string;

//==============================================================================
//==============================================================================

static const char* base = "00-tuple.db";

//==============================================================================
//==============================================================================

TEST_COMPONENT(000)
{
    //--- идеальный случай
    //--- запрашиваем одну строку и получаем одну строку
    //--- количество параметров tuple совпдаает с количеством столбцов.

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);

        //--- create table
        staff::makeTable(con, "users");
        for(size_t i = 3; i != 10; ++i)
            staff::addToTable(con, i, i*2);

        std::tuple<int, int> dst;
        const char* sql = "select * from users where login = 3";
        ASSERT_NO_THROW(con << sql >> dst);
        ASSERT_TRUE(3 == std::get<0>(dst));
        ASSERT_TRUE(6 == std::get<1>(dst));
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(001)
{
    //--- запрашиваем одну строку и получаем одну строку
    //--- количество параметров tuple меньше, чем количество столбцов.

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);

        //--- create table
        staff::makeTable(con, "users");
        for(size_t i = 3; i != 10; ++i)
            staff::addToTable(con, i, i*2);

        std::tuple<str_t> dst;
        const char* sql = "select * from users where login = 3";
        ASSERT_NO_THROW(con << sql >> dst);
        ASSERT_TRUE("3" == std::get<0>(dst));
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(002)
{
    //--- запрашиваем одну строку и получаем одну строку
    //--- количество параметров tuple больше, чем количество столбцов.

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);

        //--- create table
        staff::makeTable(con, "users");
        for(size_t i = 3; i != 10; ++i)
            staff::addToTable(con, i, i*2);

        std::tuple<str_t, str_t, int> dst;
        const char* sql = "select * from users where login = 3";

        #ifdef NDEBUG
            ASSERT_ANY_THROW(con << sql >> dst);
        #else
            ASSERT_DEATH_DEBUG(con << sql >> dst);
        #endif
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(003)
{
    //--- prepare
    //--- запрашиваем одну строку, а получаем множество.
    ASSERT_NO_THROW(staff::dbaseDelete(base));

    //--- create database
    db::connection con = db::connect(base, db::eCREATE);

    //--- create table
    staff::makeTable(con, "users");
    for(size_t i = 3; i != 10; ++i)
        staff::addToTable(con, i, i*2);
}

TEST_COMPONENT(004)
{
    //--- реализация
    //--- запрашиваем одну строку, а получаем множество.
    
    db::connection con = db::connect(base, db::eREADWRITE);
    const char* sql = "select * from users";

    std::tuple<str_t, str_t> dst;

    #ifdef NDEBUG
        ASSERT_NO_THROW(con << sql >> dst);
        ASSERT_TRUE("3" == std::get<0>(dst));
        ASSERT_TRUE("6" == std::get<1>(dst));
    #else
        ASSERT_DEATH_DEBUG(con << sql >> dst);
    #endif
}

TEST_COMPONENT(005)
{
    //--- очистка
    //--- запрашиваем одну строку, а получаем множество.
    ASSERT_NO_THROW(staff::dbaseDelete(base));
}


TEST_COMPONENT(006)
{
    //--- запрашиваем одну строку, а получаем пустой ответ.
    
    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);

        //--- create table
        staff::makeTable(con, "users");
        for(size_t i = 3; i != 10; ++i)
            staff::addToTable(con, i, i*2);

        std::tuple<str_t, str_t> dst{ "aa", "bb" };
        const char* sql = "select * from users where login = 1000";

        ASSERT_NO_THROW(con << sql >> dst);
        ASSERT_TRUE("aa" == std::get<0>(dst));
        ASSERT_TRUE("bb" == std::get<1>(dst));
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

#if 0
TEST_COMPONENT(007)
{
    //--- пуcтой std::tuple<>
    //--- запрещен времени компиляции
    
    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);

        //--- create table
        staff::makeTable(con, "users");
        for(size_t i = 3; i != 10; ++i)
            staff::addToTable(con, i, i*2);

        std::tuple<> dst;
        const char* sql = "select * from users";
        con << sql >> std::tie();
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}
#endif

//==============================================================================
//==============================================================================
#endif // ! TEST_TUPLE
