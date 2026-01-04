add_executable(
    png2lvimgdsc png2lvimgdsc.c
)

set_target_properties(png2lvimgdsc PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}
)

if(WIN32)
    add_custom_command(TARGET png2lvimgdsc
                    POST_BUILD
                    COMMAND cmd /C ${CMAKE_CURRENT_SOURCE_DIR}/generate.bat)
endif()
