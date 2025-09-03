if(SIMULATOR)
    set(TARGET sim)
else()
    set(TARGET emb)
endif()
    
file(GLOB_RECURSE target_src CONFIGURE_DEPENDS "${SRC_DIR}/${TARGET}/*.cpp")
set(target_inc "${SRC_DIR}/${TARGET}/")
file(GLOB_RECURSE ui_src CONFIGURE_DEPENDS "${SRC_DIR}/ui/*.cpp")
file(GLOB_RECURSE ui_res CONFIGURE_DEPENDS "${SRC_DIR}/ui/res/*.c")
set(ui_inc "${SRC_DIR}/ui/")

set(source_files ${ui_src} ${ui_res} ${target_src} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp PARENT_SCOPE)
set(include_files ${target_inc} ${ui_inc} PARENT_SCOPE)