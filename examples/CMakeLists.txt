get_current_folder_name(TARGET_FOLDER_NAME)

project(C_example_suite VERSION 1.0.0 LANGUAGES C)

set(EXAMPLE_SOURCES 
    discover_devices.c
    filter_and_connect_device.c
    get_signals.c
    filter_and_read_last_value_of_signal.c
    stream_reader_read_in_loop.c
    read_with_formatted_timestamps.c
    print_data_descriptor_and_calculater_sample_rate.c
    create_and_read_sample_rate_buffers.c
    simple_properties_display_handling.c
    basic_conversions_example.c
    advanced_conversions_example.c
    multi_reader_read_same_rates.c
    tree_traversal.c)

foreach(src ${EXAMPLE_SOURCES})
    get_filename_component(exec_name ${src} NAME_WLE)
    add_executable(${exec_name} ${src})
	target_include_directories(${exec_name} PRIVATE util_headers)
    target_link_libraries(${exec_name} PRIVATE daq::copendaq)
    add_compile_definitions(MODULE_PATH="${OPENDAQ_MODULES_DIR}")
endforeach()
