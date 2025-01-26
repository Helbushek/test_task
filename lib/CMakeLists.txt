add_library(libcustom SHARED library.cpp library.h)

target_include_directories(libcustom 
    PUBLIC  
    ${CMAKE_CURRENT_SOURCE_DIR}
)