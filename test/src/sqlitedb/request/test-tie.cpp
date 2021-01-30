
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_TUPLE_TIE

#define dTEST_COMPONENT db, request
#define dTEST_METHOD tie
#define dTEST_TAG tdd

#include "test-staff.hpp"
#include <sqlitedb/sqlitedb.hpp>

namespace staff = staff_sqlitedb;
using str_t = ::std::string;

//==============================================================================
//==============================================================================

static const char* base = "00-variable.db";

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

        int login = -1;
        int age = -1;
        const char* sql = "select * from users where login = 3";
        ASSERT_NO_THROW(con << sql >> std::tie(login, age));
        ASSERT_TRUE(login == 3);
        ASSERT_TRUE(age   == 6);
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

        str_t login;
        const char* sql = "select * from users where login = 3";
        ASSERT_NO_THROW(con << sql >> std::tie(login));
        ASSERT_TRUE(login == "3");
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

        str_t login;
        str_t age;
        int dummy;
        const char* sql = "select * from users where login = 3";

        #ifdef NDEBUG
            ASSERT_ANY_THROW(con << sql >> std::tie(login, age, dummy));
        #else
            ASSERT_DEATH_DEBUG(con << sql >> std::tie(login, age, dummy));
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
    str_t login;
    str_t age;

    #ifdef NDEBUG
        ASSERT_NO_THROW(con << sql >> std::tie(login, age));
        ASSERT_TRUE(login == "3");
        ASSERT_TRUE(age   == "6");
    #else
        ASSERT_DEATH_DEBUG(con << sql >> std::tie(login, age));
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

        str_t login = "aa";
        str_t age = "bb";
        const char* sql = "select * from users where login = 1000";

        ASSERT_NO_THROW(con << sql >> std::tie(login, age));
        ASSERT_TRUE(login == "aa");
        ASSERT_TRUE(age   == "bb");
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

        const char* sql = "select * from users";
        con << sql >> std::tie();
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}
#endif

//==============================================================================
//==============================================================================
#endif // ! TEST_TUPLE_TIE
