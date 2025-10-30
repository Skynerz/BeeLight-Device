find_package(GTest REQUIRED)

include_directories(
    ${GTEST_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/libs/lvgl/src
    ${include_files} 
    )
    
set(LIBS_DEPS gtest gtest_main lvgl pthread)

file(GLOB_RECURSE stubs_src CONFIGURE_DEPENDS stubs/*.cpp)
add_library(stubs_lib STATIC ${stubs_src})
target_include_directories(stubs_lib PUBLIC ${include_files} ${CMAKE_SOURCE_DIR}/libs/lvgl/src)

## Add the tests
add_executable(test_Event
    test_Event.cpp 
    ${SRC_DIR}/Event.cpp)
target_link_libraries(test_Event stubs_lib gtest gtest_main)
gtest_add_tests(TARGET test_Event)

add_executable(test_EventLVGL
    test_EventLVGL.cpp 
    ${SRC_DIR}/Event.cpp)
target_link_libraries(test_EventLVGL stubs_lib ${LIBS_DEPS})
gtest_add_tests(TARGET test_EventLVGL)

add_executable(test_NavigationModel 
    model/test_NavigationModel.cpp 
    ${SRC_DIR}/model/NavigationModel.cpp)
target_link_libraries(test_NavigationModel stubs_lib ${LIBS_DEPS})
gtest_add_tests(TARGET test_NavigationModel)
