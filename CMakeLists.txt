cmake_minimum_required(VERSION 3.19)

set(PROJECT_NAME beelight-device)
include(FetchContent)

project(${PROJECT_NAME})

option(SIMULATOR "SIMULATOR BUILD" On)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
set(CMAKE_C_FLAGS "-O0 -g")

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
# LVGL library
set(LV_BUILD_CONF_PATH ${SRC_DIR}/lv_conf.h)
set(CONFIG_LV_BUILD_DEMOS Off)
set(CONFIG_LV_BUILD_EXAMPLES On) #attention a ne pas l'utiliser dans la partie emb

if(SIMULATOR)
    message("SIMULATOR BUILD")
    set(CUSTOM_DEFS "-DSIMULATOR ")
    # set_property(GLOBAL PROPERTY HAS_PARENT_SCOPE true)
else()
    message("TARGET BUILD")
    #    https://github.com/esp-arduino-libs/ESP32_Display_Panel.git
    #     https://github.com/esp-arduino-libs/ESP32_IO_Expander.git#v1.1.0
    #     https://github.com/esp-arduino-libs/esp-lib-utils.git#v0.2.0
endif()
    
FetchContent_Declare(lvgl
    GIT_REPOSITORY "https://github.com/lvgl/lvgl.git"
    GIT_TAG "v9.3.0"
    SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/libs/lvgl"
)
message("Make LVGL available")
FetchContent_MakeAvailable(lvgl)
message("Make LVGL available done")
add_subdirectory(src)

add_executable(${PROJECT_NAME} ${source_files} )
target_include_directories(${PROJECT_NAME} PRIVATE ${include_files} ${SRC_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/libs/lvgl/src)
target_compile_definitions(${PROJECT_NAME} PRIVATE ${CUSTOM_DEFS} -DLV_CONF_PATH="${LV_BUILD_CONF_PATH}")
target_link_libraries(${PROJECT_NAME} lvgl)

if(SIMULATOR)
target_link_libraries(${PROJECT_NAME} X11 lvgl_examples pthread)
endif()
