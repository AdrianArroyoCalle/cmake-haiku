PROJECT(cmake-gui C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

FILE(GLOB CMAKE_SRC "src/*.c" "src/*.cpp")

ADD_EXECUTABLE(cmake-gui ${CMAKE_SRC})
TARGET_LINK_LIBRARIES(cmake-gui be tracker root)

INSTALL(TARGETS cmake-gui DESTINATION /system/apps/)
