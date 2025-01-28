add_executable(program1 main.cpp)
target_link_libraries(program1 libcustom)

if (WIN32)
    add_custom_command(TARGET program1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:libcustom>
        $<TARGET_FILE_DIR:program1>
    )
    target_link_libraries(program1 ws2_32)
endif()