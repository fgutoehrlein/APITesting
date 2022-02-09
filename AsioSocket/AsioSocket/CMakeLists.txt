cmake_minimum_required (VERSION 3.8)


set(BOOST_ROOT "C:/Program Files/boost/boost_1_78_0")

add_executable (AsioSocketTest "AsioSocketTest.cpp" "AsioSocketTest.h" "HttpConnection.h" "HttpConnectio.cpp")
target_include_directories(AsioSocketTest PRIVATE ${BOOST_ROOT})