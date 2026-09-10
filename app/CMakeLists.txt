cmake_minimum_required(VERSION 3.21)

project(Lebowski VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(Lebowski
    src/main.cpp
    src/ui/MainWindow.cpp
    src/ui/MainWindow.h
    src/ui/TopBar.cpp
    src/ui/TopBar.h
    src/ui/NavigationPanel.cpp
    src/ui/NavigationPanel.h
    src/ui/BottomStatusBar.cpp
    src/ui/BottomStatusBar.h
    src/ui/widgets/Panel.cpp
    src/ui/widgets/Panel.h
    src/ui/widgets/StatusIndicator.cpp
    src/ui/widgets/StatusIndicator.h
    src/ui/widgets/ValueRow.cpp
    src/ui/widgets/ValueRow.h
    src/ui/widgets/JointStatusWidget.cpp
    src/ui/widgets/JointStatusWidget.h
    src/ui/widgets/JointControlWidget.cpp
    src/ui/widgets/JointControlWidget.h
    src/ui/widgets/RobotViewWidget.cpp
    src/ui/widgets/RobotViewWidget.h
    src/pages/OverviewPage.cpp
    src/pages/OverviewPage.h
    src/pages/PlaceholderPage.cpp
    src/pages/PlaceholderPage.h
    src/pages/ControlPage.cpp
    src/pages/ControlPage.h
    src/pages/ProgramPage.cpp
    src/pages/ProgramPage.h
    src/pages/TeachPage.cpp
    src/pages/TeachPage.h
    src/pages/AIPage.cpp
    src/pages/AIPage.h
    src/pages/MonitorPage.cpp
    src/pages/MonitorPage.h
    src/pages/SettingsPage.cpp
    src/pages/SettingsPage.h
    resources/resources.qrc
)

target_link_libraries(Lebowski PRIVATE Qt6::Widgets)

target_include_directories(Lebowski PRIVATE src)
