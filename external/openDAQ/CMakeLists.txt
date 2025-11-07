FetchContent_Declare(
	openDAQ
	GIT_REPOSITORY https://github.com/openDAQ/openDAQ.git
	GIT_TAG main
	GIT_PROGRESS ON
)

FetchContent_MakeAvailable(openDAQ)