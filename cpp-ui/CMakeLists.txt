cmake_minimum_required(VERSION 3.16)

project(CyberMD
    VERSION 0.1.0
    LANGUAGES CXX
)

# =========================
# C++ configuration
# =========================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =========================
# Qt detection (Qt6 → Qt5)
# =========================
find_package(Qt6 COMPONENTS Core Gui Widgets QUIET)

if (Qt6_FOUND)
    set(QT_VERSION_MAJOR 6)
    set(QT_LIBS Qt6::Core Qt6::Gui Qt6::Widgets)
else()
    message(STATUS "Qt6 not found, falling back to Qt5")
    find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
    set(QT_VERSION_MAJOR 5)
    set(QT_LIBS Qt5::Core Qt5::Gui Qt5::Widgets)
endif()

message(STATUS "Using Qt${QT_VERSION_MAJOR}")

# =========================
# Source files
# =========================
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/codeeditor.cpp
    src/settings.cpp
    src/markdownpreview.cpp
    src/searchdialog.cpp
    src/regexhelper.cpp
    src/commandhelper.cpp
    src/shellchecker.cpp
    src/vimmode.cpp
    src/filetree.cpp
    src/featurepanel.cpp
    src/syntaxhighlighter.cpp
    src/theme.cpp
    src/codefolding.cpp
    src/tabwidget.cpp
    src/fuzzyfinder.cpp
    src/linenumberarea.cpp
    src/foldingarea.cpp
)

set(HEADERS
    include/mainwindow.h
    include/codeeditor.h
    include/settings.h
    include/markdownpreview.h
    include/searchdialog.h
    include/regexhelper.h
    include/commandhelper.h
    include/shellchecker.h
    include/vimmode.h
    include/filetree.h
    include/featurepanel.h
    include/syntaxhighlighter.h
    include/theme.h
    include/codefolding.h
    include/tabwidget.h
    include/fuzzyfinder.h
    include/rustbridge.h
    include/linenumberarea.h
    include/foldingarea.h
)

# =========================
# Executable
# =========================
add_executable(cybermd
    ${SOURCES}
    ${HEADERS}
)

# =========================
# Include directories
# =========================
target_include_directories(cybermd PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/../rust-core/cybermd-ffi
)

# =========================
# Rust FFI library
# =========================
set(RUST_FFI_LIB
    ${CMAKE_SOURCE_DIR}/../rust-core/target/release/libcybermd_ffi.so
)

# =========================
# Link libraries
# =========================
target_link_libraries(cybermd PRIVATE
    ${QT_LIBS}
    ${RUST_FFI_LIB}
)

# =========================
# Export Qt version to C++
# =========================
target_compile_definitions(cybermd PRIVATE
    QT_MAJOR_VERSION=${QT_VERSION_MAJOR}
)

# =========================
# Output & install
# =========================
set_target_properties(cybermd PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

install(TARGETS cybermd
    RUNTIME DESTINATION bin
)
