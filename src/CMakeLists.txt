if(SIMULATOR)
    set(TARGET sim)
else()
    set(TARGET emb)
endif()
    
file(GLOB common_src CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp" "${SRC_DIR}/common/*.c*")
file(GLOB_RECURSE target_src CONFIGURE_DEPENDS "${SRC_DIR}/${TARGET}/*.cpp")
set(target_inc "${SRC_DIR}/${TARGET}/")
file(GLOB_RECURSE ui_src CONFIGURE_DEPENDS "${SRC_DIR}/ui/*.cpp")
file(GLOB_RECURSE ui_res CONFIGURE_DEPENDS "${SRC_DIR}/ui/res/*.c")
set(ui_inc "${SRC_DIR}/ui/")
file(GLOB_RECURSE model_src CONFIGURE_DEPENDS "${SRC_DIR}/model/*.cpp")
set(model_inc "${SRC_DIR}/model/")
file(GLOB_RECURSE com_src CONFIGURE_DEPENDS "${SRC_DIR}/com/*.cpp")
set(com_inc "${SRC_DIR}/com/")

set(source_files 
    ${common_src}
    ${ui_src} 
    ${ui_res} 
    ${target_src} 
    ${model_src}
    ${com_src}
    PARENT_SCOPE)

set(include_files 
    ${SRC_DIR}
    ${target_inc} 
    ${ui_inc} 
    ${model_inc}
    ${com_inc}
    PARENT_SCOPE)
