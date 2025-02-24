cmake_minimum_required(VERSION 3.31.5)

project(gr-render VERSION 0.1.0 LANGUAGES C CXX)

option(GR_COMPILE_LINUX "Compile for Linux" ON)
option(GR_COMPILE_STATIC_LIBRARY "Compile Static Library" OFF)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

set(render_src
    src/gRenderbuffer.cpp
    src/gFramebuffer.cpp
    src/gVertexArray.cpp
    src/gTexture.cpp
    src/gRender.cpp
    src/gShader.cpp
    src/gCommon.cpp
    src/gError.cpp
    src/shader.cpp
    src/gl.cpp
)

set(render_hdr
    include/${PROJECT_NAME}/gRenderbuffer.h
    include/${PROJECT_NAME}/gFramebuffer.h
    include/${PROJECT_NAME}/gVertexArray.h
    include/${PROJECT_NAME}/gTexture.h
    include/${PROJECT_NAME}/gRender.h
    include/${PROJECT_NAME}/gShader.h
    include/${PROJECT_NAME}/gCommon.h
    include/${PROJECT_NAME}/gError.h
)

if(GR_COMPILE_STATIC_LIBRARY)
    add_library(${PROJECT_NAME} STATIC ${render_src} ${render_hdr})
else()
    add_library(${PROJECT_NAME} SHARED ${render_src} ${render_hdr})
endif()
unset(GR_COMPILE_STATIC_LIBRARY CACHE)

find_package(gr-math REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE ${GRMATH_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

if(GR_COMPILE_LINUX)
    option(GR_USE_GLEW "Use glw" ON)

    if(GR_USE_GLEW)
        find_package(GLEW QUIET)
        if(NOT GLEW_FOUND)
        	message(FATAL_ERROR "Please install GLEW.")
        	return()
        endif()

        target_link_libraries(${PROJECT_NAME} PRIVATE ${GLEW_LIBRARIES})
    endif()
    unset(GR_USE_GLEW CACHE)

    set(OpenGL_GL_PREFERENCE LEGACY)

    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})

    ## Install
    install(DIRECTORY include/${PROJECT_NAME}
        DESTINATION include
    )

    install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION lib/${PROJECT_NAME}
        ARCHIVE DESTINATION lib/${PROJECT_NAME}
        RUNTIME DESTINATION bin/${PROJECT_NAME}
    )

    install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION lib/${PROJECT_NAME}/cmake
    )

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    configure_package_config_file(
        "${CMAKE_CURRENT_LIST_DIR}/Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
    )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" DESTINATION lib/${PROJECT_NAME}/cmake
    )
else()
    find_library(EGL-lib EGL)
    find_library(GLESv3-lib GLESv3)

    target_link_libraries(${PROJECT_NAME} PRIVATE ${log-lib} ${android-lib} ${EGL-lib} ${GLESv3-lib})
endif()

unset(GR_COMPILE_LINUX CACHE)
