cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 14)


set(MAIN_FILE main.cpp)

add_executable(SimpleBLEScanner ${MAIN_FILE})
target_link_libraries(SimpleBLEScanner  libBLE)
