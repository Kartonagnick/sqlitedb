
# 2021y-01m-22d. WorkSpace project.
################################################################################
################################################################################

set(gDEBUG "$ENV{eDEBUG}")
cmake_minimum_required(VERSION 3.1)

include("${CMAKE_CURRENT_LIST_DIR}/setup.cmake")
cmake_scenario("minimalist")

project(${gNAME_PROJECT})

################################################################################
################################################################################

#--- build fast and thread-safe sqlite3
#--- without ICU
make_target(
    PATH_TO_SOURCES "depends/sqlite3"
    PREPROCESSOR 
        "SQLITE_CORE=1"
        "SQLITE_ENABLE_COLUMN_METADATA"
        "SQLITE_ENABLE_STAT2"
        "SQLITE_DEFAULT_RECURSIVE_TRIGGERS=1"
        "SQLITE_ENABLE_FTS3"
        "SQLITE_ENABLE_FTS3_PARENTHESIS"
        "SQLITE_ENABLE_RTREE"
#        "SQLITE_ENABLE_ICU"
        "SQLITE_DEFAULT_PAGE_SIZE=8192"
        "SQLITE_DEFAULT_CACHE_SIZE=8000"
        "SQLITE_DEFAULT_TEMP_CACHE_SIZE=2000"
        "SQLITE_DEFAULT_FILE_FORMAT=4"
        "SQLITE_DEFAULT_JOURNAL_SIZE_LIMIT=1048576"
        "SQLITE_THREADSAFE=1"
)

#--- sqlitedb
make_target(DEPENDENCIES "sqlite3")

#--- automatically link with 'mygetst', 'gmock', or 'gtest' 
#--- depending on which libraries are available
make_target(
    PATH_TO_SOURCES "test"
    INCLUDES "${gDIR_SOURCE}/include"
    DEPENDENCIES "sqlitedb" 
)

################################################################################
################################################################################

