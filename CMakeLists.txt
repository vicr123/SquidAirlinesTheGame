cmake_minimum_required(VERSION 3.22)
project(SquidAirlinesTheGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        OpenGL
        OpenGLWidgets
        REQUIRED)

add_executable(SquidAirlinesTheGame WIN32 MACOSX_BUNDLE
        resources.qrc
        main.cpp
        MainWindow.cpp MainWindow.h
        menus/AbstractMenu.cpp menus/AbstractMenu.h
        menus/MainMenu.cpp menus/MainMenu.h
        menus/nav/NavigationManager.cpp menus/nav/NavigationManager.h
        menus/nav/NavigationElement.cpp menus/nav/NavigationElement.h
        menus/nav/NavigationButton.h menus/nav/NavigationButton.cpp
        game/GameSession.cpp game/GameSession.h
        game/Player.cpp game/Player.h
        game/objects/gameobject.h game/objects/gameobject.cpp
        game/objects/buildingobject.h game/objects/buildingobject.cpp
        game/objects/fuelobject.h game/objects/fuelobject.cpp
        game/objects/tankobject.h game/objects/tankobject.cpp

        Info.plist.in
    )

target_link_libraries(SquidAirlinesTheGame
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL
        Qt::OpenGLWidgets
        )

set_target_properties(SquidAirlinesTheGame PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/Info.plist.in
)

if (EDIT_AND_CONTINUE)
    if(MSVC)
      target_compile_options(SquidAirlinesTheGame PUBLIC "/ZI")
      target_link_options(SquidAirlinesTheGame PUBLIC "/INCREMENTAL")
    endif()
endif()

if(NOT DEFINED ENV{CI})
    if (NOT NO_COPY)
        if (WIN32)
            set(DEBUG_SUFFIX)
            if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
                set(DEBUG_SUFFIX "d")
            endif ()
            set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
            if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
                set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
                if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
                    set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
                endif ()
            endif ()
            if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E make_directory
                        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy
                        "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
            endif ()
            foreach (QT_LIB Core Gui Widgets)
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy
                        "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                        "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
            endforeach (QT_LIB)
        endif ()
    endif()
endif()
