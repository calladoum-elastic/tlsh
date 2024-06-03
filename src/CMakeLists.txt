set(TLSH_SRC_DIR ${TLSH_ROOT_DIR}/src)
set(TLSH_INC_DIR ${TLSH_SRC_DIR}/inc)

configure_file(${TLSH_INC_DIR}/tlsh_version.h.in ${TLSH_INC_DIR}/tlsh_version.h)

set(SOURCE_FILES
    ${TLSH_SRC_DIR}/tlsh_impl.cpp
    ${TLSH_SRC_DIR}/tlsh_util.cpp
    ${TLSH_SRC_DIR}/tlsh.cpp
)

add_library(tlsh STATIC)
target_sources(tlsh PRIVATE ${SOURCE_FILES})
target_include_directories(tlsh PUBLIC ${TLSH_INC_DIR})
target_compile_definitions(tlsh
    PUBLIC
        BUCKETS_${TLSH_BUCKETS}
        CHECKSUM_${TLSH_CHECKSUM}B
)

if(WIN32)
    target_compile_definitions(tlsh PUBLIC _WINDOWS)
elseif(APPLE)
    target_compile_definitions(tlsh PUBLIC _MACOS)
elseif(UNIX)
    target_compile_definitions(tlsh PUBLIC _LINUX)
    target_compile_options(tlsh PUBLIC -fPIC)
else()
    message(FATAL_ERROR "Invalid OS")
 endif()

#
# Build with ASAN if available AND debug build
#
if(TLSH_BUILD_WITH_ASAN)
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-fsanitize=address" HAS_ASAN_AVAILABLE)
    if(HAS_ASAN_AVAILABLE)
        if(WIN32)
            target_compile_definitions(tlsh
            PUBLIC
                $<$<CONFIG:Debug>:_DISABLE_VECTOR_ANNOTATION _DISABLE_STRING_ANNOTATION >
            )
            target_compile_options(tlsh
            PUBLIC
                $<$<STREQUAL:${CMAKE_GENERATOR_PLATFORM},x64>:$<$<CONFIG:Debug>:/fsanitize=address>>
                $<$<STREQUAL:${CMAKE_GENERATOR_PLATFORM},win32>:$<$<CONFIG:Debug>:/fsanitize=address>>
            )
            target_link_options(tlsh
            PUBLIC
                $<$<STREQUAL:${CMAKE_GENERATOR_PLATFORM},win32>:$<$<CONFIG:Debug>:/InferAsanLibs>>
                $<$<STREQUAL:${CMAKE_GENERATOR_PLATFORM},x64>:$<$<CONFIG:Debug>:/InferAsanLibs>>
            )
        else()
            target_compile_options(tlsh
            PUBLIC
                $<$<CONFIG:Debug>:-fsanitize=address>
            )
            target_link_libraries(tlsh
            PUBLIC
                $<$<CONFIG:Debug>:-fsanitize=address>
            )
        endif(WIN32)
    endif(HAS_ASAN_AVAILABLE)
endif(TLSH_BUILD_WITH_ASAN)

install(TARGETS tlsh DESTINATION lib)

install(
    FILES
        ${TLSH_INC_DIR}/tlsh_impl.h
        ${TLSH_INC_DIR}/tlsh_version.h
        ${TLSH_INC_DIR}/tlsh_util.h
        ${TLSH_INC_DIR}/tlsh.h
    DESTINATION
        inc
)

if(TLSH_BUILD_TESTS)
    add_subdirectory(tests)
endif(TLSH_BUILD_TESTS)
