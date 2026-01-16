get_current_folder_name(TARGET_FOLDER_NAME)

project(labVIEW_example_suite_V2 VERSION 1.0.0 LANGUAGES C)



set(C_MODIFIED_EXAMPLES 
    1_discover_devices.c
    2_filter_and_connect_device.c
    3_get_signals.c
    4_filter_and_read_last_value_of_signal.c
    5_stream_reader_read_in_loop.c
    6_read_with_formatted_timestamps.c
    7_print_data_descriptor_and_calculater_sample_rate.c
    8_create_and_read_sample_rate_buffers.c
    2_0_tree_traversal.c
	3_0_property_name_and_value_visualization.c)


foreach(src ${C_MODIFIED_EXAMPLES})
    get_filename_component(exec_name ${src} NAME_WLE)
    add_executable(${exec_name} ${src})
	target_include_directories(${exec_name} PRIVATE util_headers)
    target_link_libraries(${exec_name} PRIVATE daq::copendaq)
    add_compile_definitions(MODULE_PATH="${OPENDAQ_MODULES_DIR}")
endforeach()
