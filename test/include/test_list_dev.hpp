
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
//===== sqlitedb/staff =====================================||==================
    // #define TEST_STAFF                                   // ready!
//==============================================================================
//===== sqlitedb/consept ===================================||==================
    // #define TEST_CONSEPT                                 // ready!
//==============================================================================
//===== sqlitedb/management ================================||==================
    // #define TEST_EXISTS                                  // ready!
    // #define TEST_REMOVE                                  // ready!
    // #define TEST_EXISTS_DATABASE                         // ready!
    // #define TEST_AVAILABLE_DATABASE                      // ready!
    // #define TEST_EXISTS_TABLE                            // ready!
    // #define TEST_EXISTS_COLUMN                           // ready!
    // #define TEST_DROP_DATABASE                           // ready!
    // #define TEST_DROP_TABLE                              // ready!
//==============================================================================
//===== sqlitedb/connect ===================================||==================
    // #define TEST_CONNECT                                 // ready!
//==============================================================================
//===== sqlitedb/connection ================================||==================
    // #define TEST_TABLE                                   // ready!
    // #define TEST_COLUMN                                  // ready!
    // #define TEST_OPERATOR_SQL                            // ready!
//==============================================================================
//===== sqlitedb/request ===================================||==================
    #define TEST_TYPE_SAFE                                  // in progress...
    // #define TEST_CONTENT                                 // in progress...
    
//==========================================================||==================
//==============================================================================

#endif // !dTEST_LIST_DEV_USED_
