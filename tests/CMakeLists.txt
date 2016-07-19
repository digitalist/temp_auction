cmake_minimum_required(VERSION 3.3)
project (AuctionTest C CXX)
set(TESTS_PROJECT_NAME AuctionTest)
enable_testing()
ADD_DEFINITIONS(-std=c++14 -Wall -DBOOST_LOG_DYN_LINK -DBOOST_TEST_DYN_LINK )
set(SOURCE_FILES ../Auction.h ../Auction.cpp AuctionTest.cpp)
add_executable(${TESTS_PROJECT_NAME} ${SOURCE_FILES})
#FIND_PACKAGE (Boost COMPONENTS unit_test_framework REQUIRED)
FIND_PACKAGE (Boost)
if(Boost_FOUND)
    target_include_directories(${TESTS_PROJECT_NAME} PRIVATE ${BOOST_INCLUDE_DIRS})
endif()







