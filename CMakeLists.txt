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

FetchContent_MakeAvailable(json-c libcurl)

file(GLOB_RECURSE SOURCES "src/*.c")
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PUBLIC json-c libcurl)