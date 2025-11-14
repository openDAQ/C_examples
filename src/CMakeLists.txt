get_current_folder_name(TARGET_FOLDER_NAME)

project(labVIEW_example_suite VERSION 1.0.0 LANGUAGES C CXX)



set(C_EXAMPLES 
    connect_device_simple.c
    build_instance_simple.c
    discover_devices_simple.c
    signals_from_device_simple.c)


#prepend_include(${TARGET_FOLDER_NAME} C_EXAMPLES)


foreach(src ${C_EXAMPLES})
    get_filename_component(exec_name ${src} NAME_WLE)
    add_executable(${exec_name} ${src})
    target_link_libraries(${exec_name} PRIVATE daq::copendaq)
	add_compile_definitions(MODULE_PATH="${OPENDAQ_MODULES_DIR}")
endforeach()

