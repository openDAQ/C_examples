get_current_folder_name(TARGET_FOLDER_NAME)

project(labVIEW_example_suite VERSION 1.0.0 LANGUAGES C)

set(C_EXAMPLE_HEADERS headers/build_instance.h
                      headers/connect_device.h
                      headers/discover_devices.h
                      headers/signals_from_device.h
                      headers/convert_timestamps.h
                      headers/read_in_loop.h
		      headers/example_application.h)

set(C_EXAMPLES 
    connect_device_simple.c
    build_instance_simple.c
    discover_devices_simple.c
    signals_from_device_simple.c
    read_in_loop_simple.c
    convert_timestamps_simple.c
    connect_signal_simple.c
    example_application_v2.c)


foreach(src ${C_EXAMPLES})
    get_filename_component(exec_name ${src} NAME_WLE)
    add_executable(${exec_name} ${src})
	target_include_directories(${exec_name} PRIVATE headers)
    target_link_libraries(${exec_name} PRIVATE daq::copendaq)
    add_compile_definitions(MODULE_PATH="${OPENDAQ_MODULES_DIR}")
endforeach()

