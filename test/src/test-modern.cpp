
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_MODERN

#define dTEST_COMPONENT example, additional
#define dTEST_METHOD method
#define dTEST_TAG tdd

#define TEST_CASE_NAME old_example
#define TEST_NUMBER(n) old_method_##n

#include <cassert>
#include <stdexcept>

//==============================================================================
//==============================================================================
namespace
{
    template <class callable>
    void death_test(callable& call)
    {
        #ifdef NDEBUG
            // release
            (void) call;
        #else
            // debug
            ASSERT_DEATH(call(), ".*");
        #endif
    }

    void foo()
    {
        assert(false);

        std::cout << "INVALID\n";
        throw ::std::runtime_error("test");
    }
  
}//namespace
//==============================================================================
//==============================================================================

TEST(TEST_CASE_NAME, TEST_NUMBER(000))
{
    ASSERT_DEATH_DEBUG(foo());
}
TEST(TEST_CASE_NAME, TEST_NUMBER(001))
{
    death_test(foo);
}

dREGISTER_UNIT_TEST(SampleTest_Method, tools\\ololo\\SampleTest, Method)
{
    dprint(std::cout << "ok\n");
}

TEST_COMPONENT(002)
{
    ASSERT_DEATH_DEBUG(foo());
}
TEST_COMPONENT(003)
{
    death_test(foo);
}

_TEST_COMPONENT(004)
{
    // --- check compile
}

TEST_COMPONENT(005)
{
    #ifdef _VARIADIC_MAX
        dprint(std::cout << "macro: " << _VARIADIC_MAX << '\n');
    #endif
}

//==============================================================================
//==============================================================================
#endif // ! TEST_MODERN
