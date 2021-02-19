find_library(CPPUNIT REQUIRED)

add_executable(tests TestSuite.cpp TestExample.cpp)

target_link_libraries(tests ${LIBRARY} cppunit)