macro(SUBDIRLIST result curdir)

ENDMACRO()

file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
set(boards "")
foreach(child ${children})
	if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/${child})
		if(NOT ${child} STREQUAL comon)
			list(APPEND boards ${child})
		endif()
	endif()
endforeach()

set(launch_json_entries "")
foreach(board ${boards})
	list(APPEND launch_json_entries "        {\n\
            \"type\": \"cortex-debug\",\n\
            \"request\": \"launch\",\n\
            \"servertype\": \"openocd\",\n\
            \"cwd\": \"\${workspaceRoot}\",\n\
            \"executable\": \"\${workspaceRoot}/build/${board}-debug/${board}.elf\",\n\
            \"name\": \"Debug ${board}\",\n\
            \"device\": \"STM32F446RE\",\n\
            \"configFiles\": [\"\${workspaceRoot}/scripts/openocd.cfg\"],\n\
			\"preLaunchTask\": \"make upload-${board}-debug\"\n\
        }")
endforeach()

set(tasks_json_entries "")
foreach(board ${boards})
	list(APPEND tasks_json_entries "		{
		\"label\": \"make upload-${board}-debug\",\n\
		\"type\": \"shell\",\n\
		\"command\": \"make upload-${board}-debug\",\n\
		\"group\": \"test\"\n\
	}")
endforeach()


set(launch_json "{\n    \"version\": \"0.2.0\",\n    \"configurations\": [\n")
list(JOIN launch_json_entries ",\n" launch_json_entries_joined)
string(APPEND launch_json ${launch_json_entries_joined} "\n    ]\n}\n")

set(tasks_json "{\n    \"version\": \"2.0.0\",\n    \"tasks\": [\n")
list(JOIN tasks_json_entries ",\n" tasks_json_entries_joined)
string(APPEND tasks_json ${tasks_json_entries_joined} "\n    ]\n}\n")

file(WRITE .vscode/launch.json ${launch_json})
file(WRITE .vscode/tasks.json ${tasks_json})