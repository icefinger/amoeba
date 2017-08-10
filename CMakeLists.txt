# (1)
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(Amoeba)


# check for x64 or x86
get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)
if (${LIB64} STREQUAL "TRUE")
    set(LIBSUFFIX 64)
else()
    set(LIBSUFFIX "")
endif()

if (ENABLE_ROOT)
   include(FindROOT.cmake)
   #root for tests

   link_directories(${ROOT_LIBRARY_DIR})
   include_directories(${ROOT_INCLUDE_DIR})
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_ROOT=1 ${ROOT_LIBRARIES}")

endif ()


# (4) organize project libraries
file(GLOB sources ./src/[_a-zA-Z0-9]*.cc)
file(GLOB headers include/*)
message (${headers})

file(GLOB test_sources ./tests/src/[_a-zA-Z0-9]*.cc)
file(GLOB test_headers tests/include)


#list(REMOVE_ITEM sources ${test_sources})

#message (${sources})
#message (${test_sources})

file(GLOB PROGRAMS programs/[_a-zA-Z1-9]*.cxx)
file(GLOB TESTS tests/[_a-zA-Z1-9]*.cxx)
include_directories (${headers})
if(ENABLE_TESTING)
        include_directories (${test_headers})
endif(ENABLE_TESTING)

# (5) set the needed cflags
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -pedantic -Wno-non-virtual-dtor -Wno-long-long -Wwrite-strings -Wpointer-arith -Woverloaded-virtual -Wno-variadic-macros -Wshadow -pipe -std=c++11 -DPROJECT_SOURCE_DIR=\\\"${PROJECT_SOURCE_DIR}\\\"")

#add other
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

if (USE_NPROCESS)
  link_directories($ENV{NPROCESS_LIBRARY_DIR})
  include_directories($ENV{NPROCESS_INCLUDE_DIR}/NSingle $ENV{NPROCESS_INCLUDE_DIR}/NProcess)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_NPROCESS=1 -lNProcess")
endif ()

if (DEBUG)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else ()
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif ()

if (FULL_OPTIMIZATION)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -flto")
endif (FULL_OPTIMIZATION)

if (${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER "4.8.9")
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=auto")
endif(${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER "4.8.9")


SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__CXX_MIN_VERSION=${CMAKE_CXX_COMPILER_VERSION}")


message (${CMAKE_CXX_FLAGS})


# (6) finish with all the binaries
if(ENABLE_TESTING)
  add_library(amoeba SHARED ${sources} ${test_sources})
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_TESTING=1")
  foreach(aprog IN LISTS TESTS)
    get_filename_component(aprogname ${aprog} NAME_WE)
    add_executable(${aprogname} ${aprog})
    target_link_libraries(${aprogname} amoeba)
  endforeach ()
else(ENABLE_TESTING)
  add_library(amoeba SHARED ${sources})
  foreach(aprog IN LISTS PROGRAMS)
    get_filename_component(aprogname ${aprog} NAME_WE)
    add_executable(${aprogname} ${aprog})
    target_link_libraries(${aprogname} amoeba)
  endforeach ()
endif(ENABLE_TESTING)
