
#pragma once

#ifdef _MSC_VER
    #ifdef STABLE_RELEASE
        #pragma message("Build stable release version")
    #else
        #pragma message("Build development version")
    #endif
#endif

#include "numeric_cast.hpp"
#include <type_traits>
#include <sqlite3.h>
#include <stdexcept>
#include <cassert>
#include <cstring>
#include <cstdint>
#include <memory>
#include <string>
#include <atomic>
#include <tuple>



