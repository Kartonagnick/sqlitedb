
#pragma once

#ifndef dTEST_LIST_STABLE_USED_ 
#define dTEST_LIST_STABLE_USED_ 1

#ifndef STABLE_RELEASE
    #error #include <test_list_stable.hpp> require #define STABLE_RELEASE
#endif

    #define dFORBID_ZERO_SIZE_ARRAY
    #define INCLUDE_AUTO_GENERATED

    // #define INCLUDE_STRESS_TESTS
    #define INCLUDE_LONG_LONG_TESTS
    #define INCLUDE_LONG_TESTS

//==============================================================================
//===== fsystem ============================================||==================
    #define TEST_MODERN                                     // ready!
    #define TEST_CLASSIC                                    // ready!
//==========================================================||==================
//==============================================================================

#endif // !dTEST_LIST_STABLE_USED_

