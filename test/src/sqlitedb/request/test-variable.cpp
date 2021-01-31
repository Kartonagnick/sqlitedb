
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_VARIABLE

#define dTEST_COMPONENT db, request
#define dTEST_METHOD variable
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
    //--- запрашиваем одно знвчение, и получаем одно значение.
    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);

        //--- create table
        staff::makeTable(con, "users");
        for(size_t i = 3; i != 10; ++i)
            staff::addToTable(con, i, i*2);

        str_t dst;
        const char* sql = "select count() from users";
        ASSERT_NO_THROW(con << sql >> dst);
        ASSERT_TRUE(dst == "7");
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(001)
{
    //--- хотим получить одно значение
    //--- но при этом запросили целую строку
    
    //--- это не логично, но теоретически имеет место быть
    //--- в этом случае наша переменная получит значение первого столбца ответа.
    //--- остальные столбцы будут проигнорированны

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);

        //--- create table
        staff::makeTable(con, "users");
        for(size_t i = 3; i != 10; ++i)
            staff::addToTable(con, i, i*2);

        const char* sql = "select * from users where login = 3";

        str_t dst;
        ASSERT_NO_THROW(con << sql >> dst);
        ASSERT_TRUE(dst == "3");
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(002)
{
    //--- хотим получить одно значение
    //--- но при этом запросили сразу несколько строк ответа

    //--- это не логично, но теоретически имеет место быть.
    //--- RELEASE
    //---   в этом случае считываем значение первого столбика первой строки
    //---   остальные строки будут проигнорированы

    //--- DEBUG
    //---   должен сработать assert

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);

        //--- create table
        staff::makeTable(con, "users");
        for(size_t i = 3; i != 10; ++i)
            staff::addToTable(con, i, i*2);

        str_t dst;
        const char* sql = "select * from users";
        
        #ifdef NDEBUG
            ASSERT_NO_THROW(con << sql >> dst);
            ASSERT_TRUE(dst == "3"); 
        #else
            ASSERT_DEATH_DEBUG(con << sql >> dst);
        #endif
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(003)
{
    //--- хотим получить одно значение
    //--- но база прислала нам пустой ответ.

    //--- это не логично, но теоретически имеет место быть.
    //--- RELEASE
    //---   std::runtime_error

    //--- DEBUG
    //---   должен сработать assert

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);

        //--- create table
        staff::makeTable(con, "users");
        for(size_t i = 3; i != 10; ++i)
            staff::addToTable(con, i, i*2);

        str_t dst;
        const char* sql = "SELECT * FROM users WHERE login = 1000";
        
        #ifdef NDEBUG
            ASSERT_ANY_THROW(con << sql >> dst);
        #else
            ASSERT_DEATH_DEBUG(con << sql >> dst);
        #endif
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

//==============================================================================
//==============================================================================
#endif // ! TEST_VARIABLE
