
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_LAMBDA

#define dTEST_COMPONENT db, request
#define dTEST_METHOD lambda
#define dTEST_TAG tdd

#include "test-staff.hpp"
#include <sqlitedb/sqlitedb.hpp>

namespace staff = staff_sqlitedb;
using str_t = ::std::string;

//==============================================================================
//==============================================================================

static const char* base = "00-lambda.db";

//==============================================================================
//==============================================================================

TEST_COMPONENT(000)
{
    //--- идеальный случай
    //--- запрашиваем одну строку и получаем одну строку
    //--- количество аргументов лямбды совпдаает с количеством столбцов.

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);

        //--- create table
        staff::makeTable(con, "users");
        for(size_t i = 3; i != 10; ++i)
            staff::addToTable(con, i, i*2);

        size_t index = 0;
        int login = -1;
        int age   = -1;
        const auto dst = [&index, &login, &age](int login_, int age_)
        {
            ++index;
            login = login_;
            age = age_;
            return true;
        };
        const char* sql = "select * from users where login = 3";
        ASSERT_NO_THROW(con << sql >> dst);

        ASSERT_TRUE(index == 1);
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
        size_t index = 0;
        const auto dst = [&index, &login](str_t login_)
        {
            ++index;
            login = std::move(login_);
            return true;
        };
        const char* sql = "select * from users where login = 3";
        ASSERT_NO_THROW(con << sql >> dst);
        ASSERT_TRUE(login == "3");
        ASSERT_TRUE(index == 1);
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

        size_t index = 0;
        int login = -1;
        int age   = -1;
        int dummy = -1;
        const auto dst = [&index, &login, &age, &dummy](int login_, int age_, int dummy_)
        {
            ++index;
            login = login_;
            age   = age_;
            dummy = dummy_;
            return true;
        };
        const char* sql = "select * from users where login = 3";

        #ifdef NDEBUG
            ASSERT_ANY_THROW(con << sql >> dst);
            ASSERT_TRUE(index ==  0);
            ASSERT_TRUE(login == -1);
            ASSERT_TRUE(age   == -1);
            ASSERT_TRUE(dummy == -1);
        #else
            ASSERT_DEATH_DEBUG(con << sql >> dst);
        #endif
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(003)
{
    //--- запрашиваем одну строку, а получаем множество.
    ASSERT_NO_THROW(staff::dbaseDelete(base));

    //--- create database
    db::connection con = db::connect(base, db::eCREATE);

    //--- create table
    staff::makeTable(con, "users");
    for(size_t i = 3; i != 10; ++i)
        staff::addToTable(con, i, i * 2);

    const char* sql = "select * from users";
    int index = 0;
    const auto dst = [&index](int login, int age)
    {
        const int etalon_login = index + 3;
        const int etalon_age = etalon_login * 2;
        ++index;
        EXPECT_TRUE(login == etalon_login);
        EXPECT_TRUE(age   == etalon_age  );
        return index != 6;
    };

    ASSERT_NO_THROW(con << sql >> dst);
    ASSERT_TRUE(index == 6);

    //--- очистка
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

        size_t index = 0;
        str_t login = "aa";
        str_t age = "bb";
        const auto dst = [&index, &login, &age](str_t login_, str_t age_)
        {
            ++index;
            login = std::move(login_);
            age   = std::move(age_);
            return true;
        };
        const char* sql = "select * from users where login = 1000";

        ASSERT_NO_THROW(con << sql >> dst);
        ASSERT_TRUE(index == 0);
        ASSERT_TRUE(login == "aa");
        ASSERT_TRUE(age   == "bb");
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

#if 0
TEST_COMPONENT(007)
{
    //--- лямбда без аргументов
    //--- запрещен времени компиляции
    
    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);

        //--- create table
        staff::makeTable(con, "users");
        for(size_t i = 3; i != 10; ++i)
            staff::addToTable(con, i, i*2);

        const auto dst = []() { return true; };
        const char* sql = "select * from users";
        con << sql >> dst;
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}
#endif

//==============================================================================
//==============================================================================
#endif // ! TEST_LAMBDA
