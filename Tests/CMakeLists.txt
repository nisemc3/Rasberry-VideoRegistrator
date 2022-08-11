get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
include(CodeCoverage)

#-------------------------------storagemanager------------------------------------#

project(storagemanager_test)
add_executable(${PROJECT_NAME}
    storagemanager-test/storagemanager-test.hpp
    storagemanager-test/storagemanager-test.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/..)
target_compile_options(${PROJECT_NAME}
    PRIVATE
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${OpenCV_LIBS}
        Threads::Threads
        ${Boost_LIBRARIES}
)

Set(Coverage "${PROJECT_NAME}_coverage")

setup_target_for_coverage_gcovr_html(
    NAME ${Coverage}               # New target name
         EXECUTABLE                # Executable in PROJECT_BINARY_DIR
         ${PROJECT_NAME}
         DEPENDENCIES              # Dependencies to build first
         ${PROJECT_NAME}

         BASE_DIRECTORY "../"                   # Base directory for report
                                                #  (defaults to PROJECT_SOURCE_DIR)
         EXCLUDE  "${CMAKE_CURRENT_SOURCE_DIR}/*"
                  "${CMAKE_CURRENT_SOURCE_DIR}/../Headers/*"
                                                # Patterns to exclude (can be relative
                                                #  to BASE_DIRECTORY, with CMake 3.4+)
)

append_coverage_compiler_flags()


#-------------------------------videocaptureclient------------------------------------#

project(videocaptureclient_test)
add_executable(${PROJECT_NAME}
    videocaptureclient-test/videocaptureclient-test.hpp
    videocaptureclient-test/videocaptureclient-test.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/..)
target_compile_options(${PROJECT_NAME}
    PUBLIC
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${OpenCV_LIBS}
        Threads::Threads
        ${Boost_LIBRARIES}
)

Set(Coverage "${PROJECT_NAME}_coverage")

setup_target_for_coverage_gcovr_html(
    NAME ${Coverage}               # New target name
         EXECUTABLE                # Executable in PROJECT_BINARY_DIR
         ${PROJECT_NAME}
         DEPENDENCIES              # Dependencies to build first
         ${PROJECT_NAME}

         BASE_DIRECTORY "../"                   # Base directory for report
                                                #  (defaults to PROJECT_SOURCE_DIR)
         EXCLUDE  "${CMAKE_CURRENT_SOURCE_DIR}/*"
                  "${CMAKE_CURRENT_SOURCE_DIR}/../Headers/*"
                                                # Patterns to exclude (can be relative
                                                #  to BASE_DIRECTORY, with CMake 3.4+)
)

append_coverage_compiler_flags()


#-------------------------------videocapturemanager------------------------------------#

project(videocapturemanager_test)
add_executable(${PROJECT_NAME}
    videocapturemanager-test/videocapturemanager-test.cpp
    videocapturemanager-test/videocapturemanager-test.hpp
)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/..)
target_compile_options(${PROJECT_NAME}
    PRIVATE
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${OpenCV_LIBS}
        Threads::Threads
        ${Boost_LIBRARIES}
)

Set(Coverage "${PROJECT_NAME}_coverage")

setup_target_for_coverage_gcovr_html(
    NAME ${Coverage}               # New target name
         EXECUTABLE                # Executable in PROJECT_BINARY_DIR
         ${PROJECT_NAME}
         DEPENDENCIES              # Dependencies to build first
         ${PROJECT_NAME}

         BASE_DIRECTORY "../"                   # Base directory for report
                                                #  (defaults to PROJECT_SOURCE_DIR)
         EXCLUDE  "${CMAKE_CURRENT_SOURCE_DIR}/*"
                  "${CMAKE_CURRENT_SOURCE_DIR}/../Headers/*"
                                                # Patterns to exclude (can be relative
                                                #  to BASE_DIRECTORY, with CMake 3.4+)
)

append_coverage_compiler_flags()


#-------------------------------threadpool------------------------------------#

project(threadpool_test)
add_executable(${PROJECT_NAME}
    threadpool-test/threadpool-test.cpp
    threadpool-test/threadpool-test.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/..)
target_compile_options(${PROJECT_NAME}
    PRIVATE
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${OpenCV_LIBS}
        Threads::Threads
        ${Boost_LIBRARIES}
)

Set(Coverage "${PROJECT_NAME}_coverage")

setup_target_for_coverage_gcovr_html(
    NAME ${Coverage}               # New target name
         EXECUTABLE                # Executable in PROJECT_BINARY_DIR
         ${PROJECT_NAME}
         DEPENDENCIES              # Dependencies to build first
         ${PROJECT_NAME}

         BASE_DIRECTORY "../"                   # Base directory for report
                                                #  (defaults to PROJECT_SOURCE_DIR)
         EXCLUDE  "${CMAKE_CURRENT_SOURCE_DIR}/*"
                  "${CMAKE_CURRENT_SOURCE_DIR}/../Headers/*"
                                                # Patterns to exclude (can be relative
                                                #  to BASE_DIRECTORY, with CMake 3.4+)
)

append_coverage_compiler_flags()

#-------------------------------connector------------------------------------#

project(connector_test)
add_executable(${PROJECT_NAME}
    connector-test/connector-test.cpp
    connector-test/connector-test.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/..)
target_compile_options(${PROJECT_NAME}
    PRIVATE
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${OpenCV_LIBS}
        Threads::Threads
        ${Boost_LIBRARIES}
)

Set(Coverage "${PROJECT_NAME}_coverage")

setup_target_for_coverage_gcovr_html(
    NAME ${Coverage}               # New target name
         EXECUTABLE                # Executable in PROJECT_BINARY_DIR
         ${PROJECT_NAME}
         DEPENDENCIES              # Dependencies to build first
         ${PROJECT_NAME}

         BASE_DIRECTORY "../"                   # Base directory for report
                                                #  (defaults to PROJECT_SOURCE_DIR)
         EXCLUDE  "${CMAKE_CURRENT_SOURCE_DIR}/*"
                  "${CMAKE_CURRENT_SOURCE_DIR}/../Headers/*"
                                                # Patterns to exclude (can be relative
                                                #  to BASE_DIRECTORY, with CMake 3.4+)
)

append_coverage_compiler_flags()
