cmake_minimum_required(VERSION 3.16)

project(BankingManagementSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
endif()

set(PROJECT_SOURCES
    src/main.cpp
    src/Account.cpp
    src/Transaction.cpp
    src/Bank.cpp
    src/MainWindow.cpp
)

set(PROJECT_HEADERS
    include/Account.h
    include/Transaction.h
    include/Bank.h
    include/Exceptions.h
    include/DashboardSummary.h
    include/MainWindow.h
)

add_executable(BankingManagementSystem
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
)

target_include_directories(BankingManagementSystem PRIVATE include)

if(Qt6_FOUND)
    target_link_libraries(BankingManagementSystem PRIVATE Qt6::Widgets)
else()
    target_link_libraries(BankingManagementSystem PRIVATE Qt5::Widgets)
endif()

