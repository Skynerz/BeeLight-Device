cmake_minimum_required(VERSION 3.19)

set(PROJECT_NAME beelight-device)
include(FetchContent)

project(${PROJECT_NAME})

option(SIMULATOR "SIMULATOR BUILD" On)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -g")
set(CMAKE_C_FLAGS "-O0 -g")

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# LVGL library
set(LV_BUILD_CONF_DIR ${SRC_DIR})
set(CONFIG_LV_BUILD_DEMOS Off)
set(CONFIG_LV_BUILD_EXAMPLES On) #attention a ne pas l'utiliser dans la partie emb
set(CONFIG_LV_USE_THORVG_INTERNAL Off)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    message("ccache found")
   set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

if(SIMULATOR)
    message("SIMULATOR BUILD")
    set(CUSTOM_DEFS 
        SIMULATOR=1 
        MEMSIZE_KB=64 
        CONFIG_LV_USE_X11=1
        CONFIG_LV_USE_SYSMON=1 
        LV_USE_MEM_MONITOR=1
        LV_USE_PERF_MONITOR=1
    )
    # set_property(GLOBAL PROPERTY HAS_PARENT_SCOPE true)
    add_subdirectory(sim)
    set(ADDITIONAL_INCLUDES sim)
    set(CONFIG_LV_BUILD_EXAMPLES On)
else()
    message("TARGET BUILD")
    set(CONFIG_LV_BUILD_EXAMPLES Off)
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

add_executable(${PROJECT_NAME} ${source_files})
target_include_directories(${PROJECT_NAME} PRIVATE ${include_files} ${SRC_DIR} ${ADDITIONAL_INCLUDES} ${CMAKE_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/libs/lvgl/src)
target_compile_definitions(${PROJECT_NAME} PRIVATE ${CUSTOM_DEFS} LV_CONF_PATH="${LV_BUILD_CONF_PATH}")
target_link_libraries(${PROJECT_NAME} lvgl)

if(SIMULATOR)
    target_compile_definitions(lvgl PUBLIC ${CUSTOM_DEFS} LV_CONF_PATH="${LV_BUILD_CONF_PATH}")
    target_link_libraries(${PROJECT_NAME} X11 lvgl_examples pthread)
endif()

enable_testing()
add_subdirectory(test)
