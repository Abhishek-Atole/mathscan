cmake_minimum_required(VERSION 3.16)

project(mathscan VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable debug information
set(CMAKE_BUILD_TYPE Debug)

# Set Qt installation prefix paths for common locations
if(WIN32)
    # Common Qt installation paths on Windows
    list(APPEND CMAKE_PREFIX_PATH 
        "C:/Qt/6.9.1/mingw_64"
        "C:/Qt/6.7.0/mingw_64"
        "C:/Qt/6.6.0/mingw_64" 
        "C:/Qt/6.5.0/mingw_64"
        "C:/Qt/6.4.0/mingw_64"
        "C:/Qt6"
    )
elseif(UNIX AND NOT APPLE)
    # Common Qt installation paths on Linux
    list(APPEND CMAKE_PREFIX_PATH 
        "/usr/lib/x86_64-linux-gnu/cmake/Qt6"
        "/usr/lib64/cmake/Qt6"
        "/opt/Qt/6.7.0/gcc_64"
        "/opt/Qt/6.6.0/gcc_64"
        "/opt/Qt/6.5.0/gcc_64"
    )
endif()

# Find Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# Check if Qt6 was found
if(NOT Qt6_FOUND)
    message(FATAL_ERROR "Qt6 not found! Please install Qt6 or set CMAKE_PREFIX_PATH")
endif()

# Display Qt information
message(STATUS "Qt6 version: ${Qt6_VERSION}")
message(STATUS "Qt6 installation path: ${Qt6_DIR}")

# Enable Qt MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Main application executable
add_executable(mathscan src/main.cpp)
target_link_libraries(mathscan Qt6::Core Qt6::Widgets Qt6::Gui)

# OCR & PPT Automation Tool executable
add_executable(ocr_tool src/ocr_main.cpp src/mainwindow.cpp include/mainwindow.h)
target_include_directories(ocr_tool PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(ocr_tool Qt6::Core Qt6::Widgets Qt6::Gui)

# Qt checker utility executable
add_executable(qt_checker src/qt_checker.cpp)
target_link_libraries(qt_checker Qt6::Core Qt6::Widgets Qt6::Gui)

# Project cleanup utility executable (C++17 standard library only, no Qt)
add_executable(cleanup_tool src/cleanup_main.cpp)
set_target_properties(cleanup_tool PROPERTIES 
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    set_target_properties(mathscan PROPERTIES WIN32_EXECUTABLE TRUE)
    set_target_properties(ocr_tool PROPERTIES WIN32_EXECUTABLE TRUE)
    set_target_properties(qt_checker PROPERTIES WIN32_EXECUTABLE FALSE)  # Console app
    set_target_properties(cleanup_tool PROPERTIES WIN32_EXECUTABLE FALSE)  # Console app
elseif(UNIX AND NOT APPLE)
    # Linux-specific settings
    find_package(PkgConfig REQUIRED)
    # Add any Linux-specific libraries here
endif()

# Copy Qt DLLs and plugins to output directory on Windows (only if Qt6 was found)
if(WIN32 AND Qt6_FOUND)
    # Get Qt installation path
    get_target_property(QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_WINDEPLOYQT_EXECUTABLE ${QT_QMAKE_EXECUTABLE} PATH)
    set(QT_WINDEPLOYQT_EXECUTABLE "${QT_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")

    # Find Qt installation directory for manual plugin copying
    get_filename_component(QT_BIN_DIR ${QT_QMAKE_EXECUTABLE} DIRECTORY)
    get_filename_component(QT_INSTALL_DIR ${QT_BIN_DIR} DIRECTORY)
    set(QT_PLUGINS_DIR "${QT_INSTALL_DIR}/plugins")

    # Copy Qt DLLs and plugins for main application
    add_custom_command(TARGET mathscan POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt6::Core>
        $<TARGET_FILE:Qt6::Widgets>
        $<TARGET_FILE:Qt6::Gui>
        $<TARGET_FILE_DIR:mathscan>
        
        # Create platforms plugin directory
        COMMAND ${CMAKE_COMMAND} -E make_directory 
        $<TARGET_FILE_DIR:mathscan>/platforms
        
        # Copy platform plugins
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${QT_PLUGINS_DIR}/platforms/qwindows.dll"
        "${QT_PLUGINS_DIR}/platforms/qminimal.dll"
        $<TARGET_FILE_DIR:mathscan>/platforms/
        
        COMMENT "Copying Qt libraries and plugins for mathscan"
    )
    
    # Copy Qt DLLs and plugins for OCR tool
    add_custom_command(TARGET ocr_tool POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt6::Core>
        $<TARGET_FILE:Qt6::Widgets>
        $<TARGET_FILE:Qt6::Gui>
        $<TARGET_FILE_DIR:ocr_tool>
        
        # Create platforms plugin directory
        COMMAND ${CMAKE_COMMAND} -E make_directory 
        $<TARGET_FILE_DIR:ocr_tool>/platforms
        
        # Copy platform plugins
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${QT_PLUGINS_DIR}/platforms/qwindows.dll"
        "${QT_PLUGINS_DIR}/platforms/qminimal.dll"
        $<TARGET_FILE_DIR:ocr_tool>/platforms/
        
        COMMENT "Copying Qt libraries and plugins for ocr_tool"
    )
    
    # Copy Qt DLLs and plugins for Qt checker
    add_custom_command(TARGET qt_checker POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt6::Core>
        $<TARGET_FILE:Qt6::Widgets>
        $<TARGET_FILE:Qt6::Gui>
        $<TARGET_FILE_DIR:qt_checker>
        
        # Create platforms plugin directory
        COMMAND ${CMAKE_COMMAND} -E make_directory 
        $<TARGET_FILE_DIR:qt_checker>/platforms
        
        # Copy platform plugins
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${QT_PLUGINS_DIR}/platforms/qwindows.dll"
        "${QT_PLUGINS_DIR}/platforms/qminimal.dll"
        $<TARGET_FILE_DIR:qt_checker>/platforms/
        
        COMMENT "Copying Qt libraries and plugins for qt_checker"
    )
endif()
