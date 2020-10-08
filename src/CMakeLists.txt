
add_library(staticqueue STATIC 
    queue.h
    queue.c
)

target_include_directories(
    staticqueue PUBLIC 
        $<INSTALL_INTERFACE:include/DekaEventLibrary>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>)
