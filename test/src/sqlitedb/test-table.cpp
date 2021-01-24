
#include <mygtest/modern.hpp>
#include <sqlitedb/sqlitedb.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_TABLE

#define dTEST_COMPONENT db, connection
#define dTEST_METHOD table
#define dTEST_TAG tdd

namespace me = db;

//==============================================================================
//==============================================================================

TEST_COMPONENT(000)
{
    db::remove("samples.db");
    ASSERT_TRUE(!db::exists("samples.db"));
    {
        db::connection con
            = db::connect("samples.db", db::eCREATE);

        ASSERT_TRUE(!con.existTable("users"));
        ASSERT_TRUE(!con.existTable("clients"));

        const char* sql_users =
            R"RAW(
            CREATE TABLE users (
                login        INTEGER   PRIMARY KEY,
                commission   INTEGER   NOT NULL
            )
        )RAW";

        const char* sql_clients =
            R"RAW(
            CREATE TABLE clients (
                login        INTEGER   PRIMARY KEY,
                commission   INTEGER   NOT NULL
            )
        )RAW";

        con << sql_users;
        ASSERT_TRUE(con.existTable("users"));

        con << sql_clients;
        ASSERT_TRUE(con.existTable("clients"));

        con.dropTable("users");
        ASSERT_TRUE(!con.existTable("users"));

        con.dropTable("clients");
        ASSERT_TRUE(!con.existTable("clients"));
    }
    ASSERT_TRUE(db::remove("samples.db"));
    ASSERT_TRUE(!db::exists("samples.db"));
}

//==============================================================================
//==============================================================================
#endif // ! TEST_TABLE
