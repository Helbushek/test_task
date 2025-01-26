add_executable(program2 main.cpp)
target_link_libraries(program2 PRIVATE libcustom)

if (WIN32)
    add_custom_command(TARGET program2 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:libcustom>
        $<TARGET_FILE_DIR:program2>
    )
endif()