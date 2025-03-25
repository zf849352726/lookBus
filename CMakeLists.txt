cmake_minimum_required(VERSION 3.16)

project(loveBus VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2 Core)
find_package(LibXml2 REQUIRED)  # 查找 libxml2

# 确保 libxml2 已找到
if (NOT LIBXML2_FOUND)
    message(FATAL_ERROR "libxml2 not found!")
endif()

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(apploveBus
    main.cpp
    htmlparse.cpp
    htmlparse.h
    pipecls.cpp
    pipecls.h
    slotscls.cpp
    slotscls.h
    webscraper.cpp
    webscraper.h
    itemmodel.cpp
    itemmodel.h
)

qt_add_qml_module(apploveBus
    URI loveBus
    VERSION 1.0
    QML_FILES
        Main.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(apploveBus PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apploveBus
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(apploveBus
    PRIVATE Qt6::Quick
    Qt6::QuickControls2
    Qt6::Core
    ${LIBXML2_LIBRARIES}  # 链接 libxml2 库
)

include(GNUInstallDirs)
install(TARGETS apploveBus
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

file(GLOB_RECURSE HEADERS "include/*.h")  # 假设你的头文件在 include 目录下

# 为头文件设置虚拟目录
source_group("Header Files" FILES ${HEADERS})

target_include_directories(apploveBus
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${LIBXML2_INCLUDE_DIR}
)
