# Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
# SPDX-License-Identifier: 0BSD

cmake_minimum_required(VERSION 3.18)
cmake_policy(VERSION 3.18)

project(CustomPackage LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

option(BUILD_WITH_HOLON "Build with Holon support" OFF)

if (BUILD_WITH_HOLON)
    set(CMAKE_AUTOMOC ON)
    find_package(QT NAMES Qt6)
    find_package(Qt${QT_VERSION_MAJOR} 6.10 REQUIRED COMPONENTS Core)
    find_package(Qt${QT_VERSION_MAJOR}Loader)
    find_package(Qt${QT_VERSION_MAJOR}Holon)
endif()

if(NOT APPLE)
    set(CMAKE_INSTALL_RPATH $ORIGIN)
endif()

# CUSTOM_PACKAGE_IMPL_LIBRARY -----------------------------------------------------------------
set(CUSTOM_PACKAGE_IMPL_LIBRARY "libCustomPackage")

add_library(${CUSTOM_PACKAGE_IMPL_LIBRARY} SHARED)

target_sources(${CUSTOM_PACKAGE_IMPL_LIBRARY}
    PRIVATE src/custompackageglobal.h
            src/customtask.cpp
            src/customtask.h
)

target_include_directories(${CUSTOM_PACKAGE_IMPL_LIBRARY}
    PRIVATE src
)

set_target_properties(${CUSTOM_PACKAGE_IMPL_LIBRARY} PROPERTIES
    PREFIX ""
)

target_compile_definitions(${CUSTOM_PACKAGE_IMPL_LIBRARY}
    PRIVATE BUILD_CUSTOM_PACKAGE_IMPL_LIB
)

if (BUILD_WITH_HOLON)
    target_compile_definitions(${CUSTOM_PACKAGE_IMPL_LIBRARY}
        PRIVATE BUILD_WITH_HOLON=1
    )

    target_link_libraries(${CUSTOM_PACKAGE_IMPL_LIBRARY}
        PRIVATE Qt${QT_VERSION_MAJOR}::Core
        PRIVATE Qt${QT_VERSION_MAJOR}Loader
        PRIVATE Qt${QT_VERSION_MAJOR}Holon
    )
endif()

include(GNUInstallDirs)
install(TARGETS ${CUSTOM_PACKAGE_IMPL_LIBRARY}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT CustomPackage_Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT CustomPackage_Runtime
            NAMELINK_COMPONENT CustomPackage_Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT CustomPackage_Development
)

# CUSTOM_PACKAGE_BINDINGS_LIBRARY -------------------------------------------------------------
set(CUSTOM_PACKAGE_BINDINGS_LIBRARY "CustomPackage")

find_package(Python COMPONENTS Interpreter Development REQUIRED)

execute_process(
    COMMAND ${Python_EXECUTABLE} -c
    "import site; print(next(p for p in site.getsitepackages() if 'site-packages' in p))"
    OUTPUT_VARIABLE Python_SITELIB
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "Python site-packages directory: ${Python_SITELIB}")

list(APPEND CMAKE_PREFIX_PATH
    "${Python_SITELIB}/shiboken6_generator/lib/cmake"
)

find_package(Shiboken6Tools REQUIRED)

set(generated_sources
    ${CMAKE_CURRENT_BINARY_DIR}/${CUSTOM_PACKAGE_BINDINGS_LIBRARY}/custompackage_module_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/${CUSTOM_PACKAGE_BINDINGS_LIBRARY}/customtask_wrapper.cpp
)

set(wrapped_header ${CMAKE_SOURCE_DIR}/src/bindings.h)
set(typesystem_file ${CMAKE_SOURCE_DIR}/src/bindings.xml)

shiboken_generator_create_binding(
    EXTENSION_TARGET ${CUSTOM_PACKAGE_BINDINGS_LIBRARY}
    GENERATED_SOURCES ${generated_sources}
    HEADERS ${wrapped_header}
    TYPESYSTEM_FILE ${typesystem_file}
    LIBRARY_TARGET ${CUSTOM_PACKAGE_IMPL_LIBRARY}
    FORCE_LIMITED_API
)

install(TARGETS ${CUSTOM_PACKAGE_BINDINGS_LIBRARY}
    RUNTIME DESTINATION ${Python_SITELIB}
            COMPONENT CustomPackage_Runtime
    LIBRARY DESTINATION ${Python_SITELIB}
            COMPONENT CustomPackage_Runtime
            NAMELINK_COMPONENT CustomPackage_Development
)

if(WIN32)
    # Copyright (C) 2023 The Qt Company Ltd. --------------------------------------------------
    # SPDX-License-Identifier: BSD-3-Clause
    # =========================================================================================
    # !!! (The section below is deployment related, so in a real world application you will
    # want to take care of this properly (this is simply to eliminate errors that users usually
    # encounter.
    # =========================================================================================
    # Circumvent some "#pragma comment(lib)"s in "include/pyconfig.h" which might force to link
    # against a wrong python shared library.

    set(windows_shiboken_shared_libraries)
    set(python_versions_list 3 36 37 38 39)
    set(python_additional_link_flags "")
    foreach(ver ${python_versions_list})
        set(python_additional_link_flags
            "${python_additional_link_flags} /NODEFAULTLIB:\"python${ver}_d.lib\"")
        set(python_additional_link_flags
            "${python_additional_link_flags} /NODEFAULTLIB:\"python${ver}.lib\"")
    endforeach()

    set_target_properties(${CUSTOM_PACKAGE_BINDINGS_LIBRARY}
                           PROPERTIES LINK_FLAGS "${python_additional_link_flags}")

    # Get the correct DLL path for the current build type
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        get_target_property(dll_path Shiboken6::libshiboken IMPORTED_LOCATION_DEBUG)
    else()
        get_target_property(dll_path Shiboken6::libshiboken IMPORTED_LOCATION_RELEASE)
    endif()
    file(TO_CMAKE_PATH "${dll_path}" dll_path)
    set(windows_shiboken_shared_libraries "${dll_path}")
    install(FILES ${windows_shiboken_shared_libraries} DESTINATION ${CMAKE_INSTALL_LIBDIR})
    # =========================================================================================
    # !!! End of dubious section.
    # =========================================================================================
endif()
