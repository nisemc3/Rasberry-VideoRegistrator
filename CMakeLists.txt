cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(Rasberry-VideoRegistrator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(BUILD_PATH "${PROJECT_SOURCE_DIR}/../build-test-videoserv")
set(RUNTIME_OUTPUT_DIRECTORY "${BUILD_PATH}")
set(LIBRARY_OUTPUT_DIRECTORY "${BUILD_PATH}")
set(ARCHIVE_OUTPUT_DIRECTORY "${BUILD_PATH}")


add_compile_options(
    -Werror

    -Wall
    -Wextra
    -Wpedantic

    -Wcast-align
    -Wcast-qual
    -Wconversion
    -Wctor-dtor-privacy
    -Wenum-compare
    -Wfloat-equal
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wredundant-decls
    -Wsign-conversion
    -Wsign-promo
)

set(ENABLE_UNIT_TESTS ON)
set(COMPILE_ARM OFF)

set(LIBS_PATH "${PROJECT_SOURCE_DIR}/../kpp_libs/linux")
set(OpenCV_DIR "${LIBS_PATH}/OpenCV_4.5.0/")
find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED PATHS ${OpenCV_DIR} NO_DEFAULT_PATH)

set(HEADERS
    Headers/threadpool.hpp
    Headers/videocaptureclient.hpp
    Headers/commandlinereader.hpp
    Headers/storagemanager.hpp
    Headers/io.hpp
    Headers/connector.hpp
    Headers/functions.hpp
    Headers/videocapturemanager.hpp
)

set(SOURSES
    Sources/main.cpp
    Sources/threadpool.cpp
    Sources/videocaptureclient.cpp
    Sources/commandlinereader.cpp
    Sources/storagemanager.cpp
    Sources/connector.cpp
    Sources/videocapturemanager.cpp
)

add_executable(
    ${PROJECT_NAME}
    ${SOURSES}
    ${HEADERS}
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME}
    ${OpenCV_LIBS}
    Threads::Threads
)

if(ENABLE_UNIT_TESTS)
    if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
        add_subdirectory(Tests)
    endif()
endif()

cmake_host_system_information(RESULT HARDWARE_TYPE QUERY OS_PLATFORM)
if (${HARDWARE_TYPE} MATCHES "arm")
    set(LIBS_PATH "${PROJECT_SOURCE_DIR}")
    set(OpenCV_DIR "${LIBS_PATH}/OpenCV_4.5.0/")
    set(COMPILE_ARM ON)
endif()




