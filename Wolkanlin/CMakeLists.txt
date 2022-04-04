# SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: LGPL-3.0-or-later

set(wolkanlin_SRCS
    abstractconfiguration.cpp
    abstractnamfactory.cpp
    job.cpp
    job_p.h
    getuserjob.cpp
    getuserjob_p.h
    getuserlistjob.cpp
    getuserlistjob_p.h
    quota.cpp
    quota_p.h
    user.cpp
    user_p.h
    getapppasswordjob.cpp
    getapppasswordjob_p.h
    deleteapppasswordjob.cpp
    deleteapppasswordjob_p.h
    getserverstatusjob.cpp
    getserverstatusjob_p.h
    serverstatus.cpp
    serverstatus_p.h
    getwipestatusjob.cpp
    getwipestatusjob_p.h
    global.cpp
)

set(wolkanlin_HEADERS
    wolkanlin_exports.h
    abstractconfiguration.h
    AbstractConfiguration
    abstractnamfactory.h
    AbstractNamFactory
    job.h
    getuserjob.h
    GetUserJob
    getuserlistjob.h
    GetUserListJob
    quota.h
    Quota
    user.h
    User
    getapppasswordjob.h
    GetAppPasswordJob
    deleteapppasswordjob.h
    DeleteAppPasswordJob
    getserverstatusjob.h
    GetServerStatusJob
    serverstatus.h
    ServerStatus
    getwipestatusjob.h
    GetWipeStatusJob
    global.h
    Global
)

set(wolkanlin_PRIVATE_HEADERS
    logging.h
)

if (NOT WITH_KDE)
    list(APPEND wolkanlin_HEADERS wjob.h)
    list(APPEND wolkanlin_SRCS wjob.cpp wjob_p.h)
endif (NOT WITH_KDE)

add_library(WolkanlinQt${QT_VERSION_MAJOR}
    ${wolkanlin_SRCS}
    ${wolkanlin_HEADERS}
    ${wolkanlin_PRIVATE_HEADERS}
)

add_library(WolkanlinQt${QT_VERSION_MAJOR}::Core ALIAS WolkanlinQt${QT_VERSION_MAJOR})

if(CMAKE_VERSION GREATER_EQUAL "3.16.0")
    target_precompile_headers(WolkanlinQt${QT_VERSION_MAJOR}
        PRIVATE
            <QString>
            <QStringList>
            <QTimer>
            <QJsonDocument>
            <QJsonObject>
            <QJsonArray>
            <QJsonValue>
    )
endif(CMAKE_VERSION GREATER_EQUAL "3.16.0")

target_compile_features(WolkanlinQt${QT_VERSION_MAJOR} PUBLIC cxx_std_14)

target_link_libraries(WolkanlinQt${QT_VERSION_MAJOR}
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Network
)

GNUInstallDirs_get_absolute_install_dir(LIBWOLKANLIN_ABS_I18NDIR LIBWOLKANLIN_I18NDIR)

target_compile_definitions(WolkanlinQt${QT_VERSION_MAJOR}
    PRIVATE
        QT_NO_KEYWORDS
        QT_NO_CAST_TO_ASCII
        QT_NO_CAST_FROM_ASCII
        QT_STRICT_ITERATORS
        QT_NO_URL_CAST_FROM_STRING
        QT_NO_CAST_FROM_BYTEARRAY
        QT_USE_QSTRINGBUILDER
        QT_NO_SIGNALS_SLOTS_KEYWORDS
        QT_USE_FAST_OPERATOR_PLUS
        QT_DISABLE_DEPRECATED_BEFORE=0x050500
        WOLKANLIN_VERSION="${PROJECT_VERSION}"
        WOLKANLIN_I18NDIR="${LIBWOLKANLIN_ABS_I18NDIR}"
)

if (WITH_KDE)
    message(STATUS "KDE support enabled")
    target_compile_definitions(WolkanlinQt${QT_VERSION_MAJOR}
        PUBLIC
            WOLKANLIN_WITH_KDE
    )
    target_link_libraries(WolkanlinQt${QT_VERSION_MAJOR}
        PUBLIC
            KF5::CoreAddons
    )
endif (WITH_KDE)

if(ENABLE_MAINTAINER_FLAGS)
    target_compile_definitions(WolkanlinQt${QT_VERSION_MAJOR}
        PRIVATE
            QT_DEPRECATED_WARNINGS
    )
    target_compile_options(WolkanlinQt${QT_VERSION_MAJOR}
        PRIVATE
            -Wall
            -Wcast-align
            -Wno-uninitialized
            -Wempty-body
            -Wformat-security
            -Wformat
            -Winit-self
    )
endif(ENABLE_MAINTAINER_FLAGS)

get_property(_current_parent_dir DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY PARENT_DIRECTORY)

target_include_directories(WolkanlinQt${QT_VERSION_MAJOR} PUBLIC
    $<BUILD_INTERFACE:${_current_parent_dir}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/wolkanlin-qt${QT_VERSION_MAJOR}>
)

set_target_properties(WolkanlinQt${QT_VERSION_MAJOR} PROPERTIES
    EXPORT_NAME Core
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "${wolkanlin_HEADERS}"
)

install(TARGETS WolkanlinQt${QT_VERSION_MAJOR}
    EXPORT WolkanlinTargets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT libs
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libs
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libs
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wolkanlin-qt${QT_VERSION_MAJOR}/Wolkanlin COMPONENT devel
)
