find_package(GTest REQUIRED)

include_directories(
    ${GTEST_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/libs/lvgl/src
    ${include_files}
    )

file(GLOB_RECURSE stubs_src CONFIGURE_DEPENDS stubs/*.cpp)
add_library(common_lib STATIC ${stubs_src} main.cpp)
target_include_directories(common_lib PUBLIC ${include_files} ${CMAKE_SOURCE_DIR}/libs/lvgl/src)

function(addTest)
    add_executable(${ARGV})
    target_link_libraries(${ARGV0} gtest gtest_main common_lib lvgl pthread)
    gtest_discover_tests(${ARGV0})
endfunction()

## Add the tests
addTest(test_Event
    test_Event.cpp
    ${SRC_DIR}/Event.cpp)

addTest(test_EventLVGL
    test_EventLVGL.cpp
    ${SRC_DIR}/Event.cpp)

addTest(test_NavigationModel
    model/test_NavigationModel.cpp
    ${SRC_DIR}/model/NavigationModel.cpp)
