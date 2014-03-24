PROJECT(cmake-gui C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

FILE(GLOB CMAKE_SRC "src/*.c" "src/*.cpp")

ADD_EXECUTABLE(cmake-gui ${CMAKE_SRC})
TARGET_LINK_LIBRARIES(cmake-gui be tracker root)
ADD_CUSTOM_COMMAND(TARGET cmake-gui COMMAND rc -o cmake-haiku.rsrc ${CMAKE_CURRENT_SOURCE_DIR}/res/cmake-haiku.rdef COMMENT "Compiling resources")
ADD_CUSTOM_COMMAND(TARGET cmake-gui COMMAND xres -o cmake-gui cmake-haiku.rsrc COMMENT "Adding resources to executable")
ADD_CUSTOM_COMMAND(TARGET cmake-gui COMMAND mimeset --all cmake-gui COMMENT "Adjusting MIME types")

INSTALL(TARGETS cmake-gui DESTINATION /system/apps/)
