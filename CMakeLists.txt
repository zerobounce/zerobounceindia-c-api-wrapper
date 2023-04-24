cmake_minimum_required(VERSION 3.20)
project(ZeroBounce VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

# Download and build json-c
FetchContent_Declare(
  json-c
  GIT_REPOSITORY https://github.com/json-c/json-c.git
  GIT_TAG json-c-0.16-20220414
)
# Download and build libcurl
FetchContent_Declare(
  libcurl
  GIT_REPOSITORY https://github.com/curl/curl.git
  GIT_TAG curl-8_0_1
)
# Download and build unity
FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG        v2.5.2
)
# Download and build fff
FetchContent_Declare(
  fff
  GIT_REPOSITORY https://github.com/meekrosoft/fff.git
  GIT_TAG        v1.1
)

FetchContent_MakeAvailable(json-c libcurl unity fff)

file(GLOB_RECURSE SOURCES "src/*.c")
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PUBLIC json-c libcurl)

add_executable(ZeroBounceTests test/ZeroBounceTest.c)
target_link_libraries(ZeroBounceTests PRIVATE ${PROJECT_NAME} unity)
target_include_directories(ZeroBounceTests PRIVATE ${fff_SOURCE_DIR})
enable_testing()

add_executable(zb_test main.c)
target_link_libraries(zb_test PRIVATE ${PROJECT_NAME})