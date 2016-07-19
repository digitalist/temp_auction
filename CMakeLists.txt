cmake_minimum_required(VERSION 3.3)
project(Auction C CXX)
enable_testing()
add_subdirectory(tests)
ADD_DEFINITIONS(-std=c++14 -Wall -DBOOST_LOG_DYN_LINK -DBOOST_TEST_DYN_LINK)

set(SOURCE_FILES Auction.h Auction.cpp main.cpp)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
FIND_PACKAGE (Threads REQUIRED )
FIND_PACKAGE (Boost COMPONENTS log REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(${CMAKE_PROJECT_NAME} ${Boost_LIBRARIES}  pthread)

    add_test(NAME AuctionTest COMMAND AuctionTest)
endif()






