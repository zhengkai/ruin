find_package(Git QUIET)


set(GIT_BRANCH "")
set(GIT_HASH   "git-not-found")
set(GIT_DIRTY  "")
if(GIT_FOUND)
	if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
		set(GIT_BRANCH "unknown")
		set(GIT_HASH "unknown")
		execute_process(
			COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			OUTPUT_VARIABLE GIT_BRANCH
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)
		execute_process(
			COMMAND ${GIT_EXECUTABLE} log -1 --format=%h
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			OUTPUT_VARIABLE GIT_HASH
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)
		execute_process(
			COMMAND ${GIT_EXECUTABLE} status --porcelain
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			OUTPUT_VARIABLE GIT_STATUS_OUTPUT
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)
		if("${GIT_STATUS_OUTPUT}" STREQUAL "")
			set(GIT_DIRTY "no")
		else()
			set(GIT_DIRTY "yes")
		endif()
	else()
		set(GIT_HASH   "not-a-git-repository")
	endif()
endif()

unset(ENV{SOURCE_DATE_EPOCH})
string(TIMESTAMP BUILD_TIMESTAMP "%Y-%m-%d %H:%M:%S")

configure_file(
	src/build.h.in
	${CMAKE_BINARY_DIR}/generated/build.h
)
