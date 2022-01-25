cmake_minimum_required(VERSION 3.10)

project(MD5-Tester)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# only copies config if source md5_tester.config was changed.
configure_file(config/md5_tester.config config/md5_tester.config COPYONLY)

add_executable(md5_tester md5_tester.cpp md5.cpp)