cmake_minimum_required(VERSION 3.12)
project(multi)

set(CMAKE_CXX_STANDARD 20)

if(${ANDROID_ABI} STREQUAL "armeabi-v7a")
    add_definitions(-DVER_x32=true)
    set(GLOSSHOOK_ABI "ARM")
elseif(${ANDROID_ABI} STREQUAL "arm64-v8a")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,max-page-size=65536")
    add_definitions(-DVER_x32=false)
    set(GLOSSHOOK_ABI "ARM64")
endif()


# Add source files
file(GLOB_RECURSE SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/*.c*
        ${CMAKE_CURRENT_LIST_DIR}/**/*.c*
        ${CMAKE_CURRENT_LIST_DIR}/**/**/*.c*
        ${CMAKE_CURRENT_LIST_DIR}/**/**/**/*.c*
        ${CMAKE_CURRENT_LIST_DIR}/**/**/**/**/*.c*
        ${CMAKE_CURRENT_LIST_DIR}/**/**/**/**/**/*.c*
        ${CMAKE_CURRENT_LIST_DIR}/**/**/**/**/**/**/*.c*
        ${CMAKE_CURRENT_LIST_DIR}/**/**/**/**/**/**/**/*.c*
        ${CMAKE_CURRENT_LIST_DIR}/**/**/**/**/**/**/**/**/*.c*
        ${CMAKE_CURRENT_LIST_DIR}/**/**/**/**/**/**/**/**/**/*.c*
)

# Add include directories
include_directories(
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/vendor/armhook
        ${CMAKE_CURRENT_LIST_DIR}/vendor/
)


# Opus
#add_library(libopus STATIC IMPORTED)
#set_target_properties(libopus PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/vendor/opus/libopus.a)

add_library(multi SHARED ${SOURCES})
target_link_libraries(multi
        log
        android
        EGL
        GLESv3
        shadowhook::shadowhook
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GlossHook/libs/${GLOSSHOOK_ABI}/libGlossHook.a
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GlossHook/libs/${GLOSSHOOK_ABI}/libGlossHook.so
)


# Set compilation flags
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -s -ffunction-sections -fdata-sections -pthread -Wall -O3 -fexceptions -fvisibility=hidden -fvisibility-inlines-hidden")
# Release: hidden symbol visibility for smaller binary and encapsulation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -g -pthread -Wall -O3 -fexceptions -fstack-protector-strong")
target_compile_options(multi PRIVATE
        $<$<CONFIG:Debug>:-fvisibility=default>
        $<$<CONFIG:Release>:-fvisibility=hidden>
)
target_compile_options(multi PRIVATE
        -Wno-register
)
target_link_options(multi PRIVATE "-Wl,-z,max-page-size=16384")

