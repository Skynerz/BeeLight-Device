find_package(GTest REQUIRED)

if(WIN32)
    set(GTEST_LIBS
        ${GTEST_ROOT}/lib/libgtest.a
        ${GTEST_ROOT}/lib/libgtest_main.a
    )
endif()

include_directories(
    ${GTEST_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/libs/lvgl/src
    ${include_files}
    )

file(GLOB_RECURSE stubs_src CONFIGURE_DEPENDS stubs/*.cpp)
add_library(common_lib STATIC ${stubs_src} main.cpp ${CMAKE_SOURCE_DIR}/src/sim/beelog_sim.cpp)
target_include_directories(common_lib PUBLIC ${include_files} ${CMAKE_SOURCE_DIR}/libs/lvgl/src)

if(WIN32)
    set(LIBRARIES ${GTEST_LIBS} common_lib ws2_32)
    set(LIBRARIES_W_LVGL ${LIBRARIES} lvgl SDL2::SDL2-static)
else()
    set(LIBRARIES gtest gtest_main common_lib pthread)
    set(LIBRARIES_W_LVGL ${LIBRARIES} lvgl)
    if(NOT CICD_BUILD)
        set(LIBRARIES_W_LVGL ${LIBRARIES_W_LVGL} X11)
    endif()
endif()

function(addTest)
    add_executable(${ARGV} ${stubs_src})
    target_compile_definitions(${ARGV0} PRIVATE ${CUSTOM_DEFS} TESTING=1)
    target_link_libraries(${ARGV0} ${LIBRARIES})
    gtest_discover_tests(${ARGV0})
endfunction()

function(addTestLVGL)
    add_executable(${ARGV})
    target_link_libraries(${ARGV0} ${LIBRARIES_W_LVGL})
    gtest_discover_tests(${ARGV0})
endfunction()

## Add the tests
addTest(test_Event
    test_Event.cpp
    ${SRC_DIR}/Event.cpp)

addTestLVGL(test_EventLVGL
    test_EventLVGL.cpp
    ${SRC_DIR}/Event.cpp)

addTest(test_ScreenNavigation
    ui/test_ScreenNavigation.cpp
    ${SRC_DIR}/ui/ScreenNavigation.cpp)

addTest(test_port_persistency_sim
    test_port_persistency_sim.cpp
    ${SRC_DIR}/sim/port_persistency_sim.c
    ${SRC_DIR}/sim/port_persistency_sim_int.c)

# addTest(test_NavigationModel
#     model/test_NavigationModel.cpp
#     ${SRC_DIR}/model/NavigationModel.cpp)
