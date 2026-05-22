cmake_minimum_required(VERSION 3.16)

# Project name and programming language.
project(BankingManagementSystem LANGUAGES CXX)

# Use C++17 for the whole project.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Let CMake run Qt tools automatically when needed.
# AUTOMOC handles Q_OBJECT classes.
# AUTOUIC handles .ui form files.
# AUTORCC handles Qt resource files.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt Widgets. Prefer Qt 6, but allow Qt 5 if Qt 6 is not installed.
find_package(Qt6 COMPONENTS Widgets QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
endif()

# Source files compiled into the application.
set(PROJECT_SOURCES
    src/main.cpp
    src/Account.cpp
    src/Transaction.cpp
    src/Bank.cpp
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

# Create the desktop application executable.
add_executable(BankingManagementSystem
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
)

# Allow source files to include headers like #include "Account.h".
target_include_directories(BankingManagementSystem PRIVATE include)

# Link the executable with the Qt Widgets library.
if(Qt6_FOUND)
    target_link_libraries(BankingManagementSystem PRIVATE Qt6::Widgets)
else()
    target_link_libraries(BankingManagementSystem PRIVATE Qt5::Widgets)
endif()
