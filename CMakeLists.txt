cmake_minimum_required(VERSION 3.10)

project(handycpp VERSION 1.0.0 LANGUAGES CXX)

###################### external projects ###########################################

# doctest
option(INCLUDE_DOCTEST "include doc test project" OFF)
include(FetchContent)
if(INCLUDE_DOCTEST)
FetchContent_Populate(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest
        GIT_TAG 2.4.7
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/doctest
)
endif(INCLUDE_DOCTEST)


################################# project ##################################################
include(GNUInstallDirs)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type")

add_library(${PROJECT_NAME} INTERFACE)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
        )

if (INCLUDE_DOCTEST)
    target_include_directories(${PROJECT_NAME} INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/doctest>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
            )
    target_compile_definitions(${PROJECT_NAME} INTERFACE HAS_DOCTEST)
    message("doctest on")
else ()
    message("doctest off")
endif (INCLUDE_DOCTEST)

########################################## test ######################################################

option(TEST_ON "build and run test" OFF)

if (TEST_ON)
    add_executable(handycpp_test test/test.cpp)
    target_compile_features(handycpp_test INTERFACE cxx_std_17)
    target_compile_options(handycpp_test PRIVATE -Wall -Werror -Wextra -pedantic)
    target_compile_definitions(handycpp_test PRIVATE HANDYCPP_TEST)
    target_link_libraries(handycpp_test PRIVATE ${PROJECT_NAME})
    include(${CMAKE_SOURCE_DIR}/thirdparty/doctest/scripts/cmake/doctest.cmake)
    include(CTest)
    doctest_discover_tests(handycpp_test)

    message("test on")
else ()
    message("test off: ${TEST_ON}")

endif (TEST_ON)

############################################# Install ##################################################
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)
if (NOT INCLUDE_INSTALL_DIR)
    set(INCLUDE_INSTALL_DIR include/handycpp)
endif ()
configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
        PATH_VARS INCLUDE_INSTALL_DIR)
install(EXPORT ${PROJECT_NAME}_Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)
install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(FILES ${doctest_SOURCE_DIR}/doctest/doctest.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/doctest/)
install(FILES ${PROJECT_SOURCE_DIR}/LICENSE DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/share/copyright/)

