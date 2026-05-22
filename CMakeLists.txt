cmake_minimum_required(VERSION 3.16)

# Project name and programming language.
project(BankingManagementSystem LANGUAGES CXX)

# Use C++17 for the whole project.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt checks the __cplusplus macro. MSVC needs this flag to report C++17 correctly.
if(MSVC)
    add_compile_options(/Zc:__cplusplus /permissive-)
endif()

# Let CMake run Qt tools automatically when needed.
# AUTOMOC handles Q_OBJECT classes.
# AUTOUIC handles .ui form files.
# AUTORCC handles Qt resource files.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt Widgets. Prefer Qt 6, but allow Qt 5 if Qt 6 is installed instead.
find_package(Qt6 COMPONENTS Widgets QUIET)
if(Qt6_FOUND)
    set(QT_WIDGETS_LIBRARY Qt6::Widgets)
else()
    find_package(Qt5 COMPONENTS Widgets QUIET)
    if(Qt5_FOUND)
        set(QT_WIDGETS_LIBRARY Qt5::Widgets)
    endif()
endif()

# Backend source files shared by the GUI app and the test executable.
set(BACKEND_SOURCES
    src/Account.cpp
    src/Transaction.cpp
    src/Bank.cpp
)

# Source files compiled into the application.
set(PROJECT_SOURCES
    src/main.cpp
    ${BACKEND_SOURCES}
    src/MainWindow.cpp
)

# Header files listed here so IDEs show them inside the project.
set(PROJECT_HEADERS
    include/Account.h
    include/Transaction.h
    include/Bank.h
    include/Exceptions.h
    include/DashboardSummary.h
    include/MainWindow.h
)

# Create the desktop application executable when Qt Widgets is available.
if(QT_WIDGETS_LIBRARY)
    add_executable(BankingManagementSystem
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
    )

    # Allow source files to include headers like #include "Account.h".
    target_include_directories(BankingManagementSystem PRIVATE include)

    # Link the executable with the Qt Widgets library.
    target_link_libraries(BankingManagementSystem PRIVATE ${QT_WIDGETS_LIBRARY})
else()
    message(WARNING "Qt Widgets was not found. BankingManagementSystem will not be built, but BankTests can still be built.")
endif()

# Simple assert-based backend test executable.
# This target does not use Qt, so it can test the Bank logic separately from the GUI.
add_executable(BankTests
    tests/test_bank.cpp
    ${BACKEND_SOURCES}
    ${PROJECT_HEADERS}
)

target_include_directories(BankTests PRIVATE include)

# The backend tests do not need Qt's automatic code generation tools.
set_target_properties(BankTests PROPERTIES
    AUTOMOC OFF
    AUTOUIC OFF
    AUTORCC OFF
)

enable_testing()
add_test(NAME BankTests COMMAND BankTests)

# Small console demo for trying the backend without the Qt GUI.
add_executable(BankConsoleDemo
    src/console_demo.cpp
    ${BACKEND_SOURCES}
    ${PROJECT_HEADERS}
)

target_include_directories(BankConsoleDemo PRIVATE include)

set_target_properties(BankConsoleDemo PROPERTIES
    AUTOMOC OFF
    AUTOUIC OFF
    AUTORCC OFF
)
