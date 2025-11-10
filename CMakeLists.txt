cmake_minimum_required(VERSION 3.19)

set(PROJECT_NAME beelight-device)
include(FetchContent)

project(${PROJECT_NAME})

option(SIMULATOR "APPLICATION SIMULATOR BUILD" On)
option(COM_SIMULATOR "COM SIMULATOR BUILD" Off)
option(BUILD_TESTING "Build unit-tests" Off)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -g")
set(CMAKE_C_FLAGS "-O0 -g")

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# LVGL library
set(LV_BUILD_CONF_DIR ${SRC_DIR})
set(CONFIG_LV_BUILD_DEMOS Off)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    message("ccache found")
   set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

if(SIMULATOR)
    message("Simulator BUILD")
    set(CUSTOM_DEFS 
        SIMULATOR=1 
        MEMSIZE_KB=64 
        CONFIG_LV_USE_SYSMON=1 
        LV_USE_MEM_MONITOR=1
        LV_USE_PERF_MONITOR=1
    )
    if (WIN32)
    # built ok with https://github.com/niXman/mingw-builds-binaries/releases
        message("Windows BUILD")
        FetchContent_Declare(sdl2
            URL https://github.com/libsdl-org/SDL/releases/download/release-2.32.10/SDL2-devel-2.32.10-mingw.zip
            SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/libs/sdl2"
            DOWNLOAD_EXTRACT_TIMESTAMP true
        )
        message("Make SDL available")
        FetchContent_MakeAvailable(sdl2)
        list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/libs/sdl2/cmake")
        include(sdl2-config)
        message("Make SDL available done")
        set(CUSTOM_DEFS ${CUSTOM_DEFS} 
            LV_USE_SDL=1
            LV_USE_OS=LV_OS_SDL2
            DRAW_STACK_SIZE=32768
        )
    else()
        message("X11 BUILD")
        set(CUSTOM_DEFS ${CUSTOM_DEFS}      
            CONFIG_LV_USE_X11=1
            LV_USE_OS=LV_OS_NONE
        )
    endif()
    # set_property(GLOBAL PROPERTY HAS_PARENT_SCOPE true)
    if(COM_SIMULATOR)
        message("Communication simulator BUILD")
        add_subdirectory(sim)
    endif()
    set(ADDITIONAL_INCLUDES ${SDL2_INCLUDE_DIRS} sim)
    set(CONFIG_LV_BUILD_EXAMPLES On)
else()
    message("Target BUILD")
    set(CONFIG_LV_BUILD_EXAMPLES Off)
    #    https://github.com/esp-arduino-libs/ESP32_Display_Panel.git
    #     https://github.com/esp-arduino-libs/ESP32_IO_Expander.git#v1.1.0
    #     https://github.com/esp-arduino-libs/esp-lib-utils.git#v0.2.0
endif()

FetchContent_Declare(lvgl
    GIT_REPOSITORY "https://github.com/lvgl/lvgl.git"
    GIT_TAG "v9.4.0"
    SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/libs/lvgl"
)
message("Make LVGL available")
FetchContent_MakeAvailable(lvgl)
message("Make LVGL available done")

add_subdirectory(src)

add_executable(${PROJECT_NAME} ${source_files})
target_include_directories(${PROJECT_NAME} PRIVATE ${include_files} ${SRC_DIR} ${ADDITIONAL_INCLUDES} ${CMAKE_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/libs/lvgl/src)
target_compile_definitions(${PROJECT_NAME} PRIVATE ${CUSTOM_DEFS})

#LVGL BUILD
target_compile_definitions(lvgl PRIVATE ${CUSTOM_DEFS})
if(SIMULATOR AND WIN32)
    target_include_directories(lvgl PRIVATE ${SDL2_INCLUDE_DIRS})
endif()
target_link_libraries(${PROJECT_NAME} lvgl)

if(SIMULATOR)
    if (WIN32)
        target_link_libraries(${PROJECT_NAME} SDL2::SDL2-static lvgl_examples ws2_32)
    else()
        target_link_libraries(${PROJECT_NAME} X11 lvgl_examples pthread)
    endif()
endif()

if(BUILD_TESTING)
    message("Unit-tests BUILD")
    enable_testing()
    add_subdirectory(test)
endif()