
#pragma once
#ifndef dTEST_LIST_DEV_USED_ 
#define dTEST_LIST_DEV_USED_ 1

#ifdef STABLE_RELEASE
    #error STABLE_RELEASE require #include <test_list_stable.hpp>
#endif

    #define CODE_GENERATION_ON
    #define INCLUDE_AUTO_GENERATED

    // #define INCLUDE_STRESS_TESTS
    #define INCLUDE_LONG_LONG_TESTS
    #define INCLUDE_LONG_TESTS

//==============================================================================
//===== modern/classic =====================================||==================
    // #define TEST_MODERN                                  // ready!
    // #define TEST_CLASSIC                                 // ready!
//==============================================================================
//===== sqlitedb ===========================================||==================
    #define TEST_STAFF                                      // ready!
    //#define TEST_EXISTS                                     // ready!

    // #define TEST_CONSTRUCT                                  // ready!


    // #define TEST_TABLE                                      // in progress...
    // #define TEST_CONTENT                                    // in progress...
//==========================================================||==================
//==============================================================================

#endif // !dTEST_LIST_DEV_USED_
