# SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: LGPL-3.0-or-later

set(wolkanlin_SRCS
    abstractconfiguration.cpp
    abstractnamfactory.cpp
    job.cpp
    job_p.h
    getuserjob.cpp
    getuserjob_p.h
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
)

set(wolkanlin_HEADERS
    wolkanlin_global.h
    abstractconfiguration.h
    AbstractConfiguration
    abstractnamfactory.h
    AbstractNamFactory
    job.h
    getuserjob.h
    GetUserJob
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
)

set(wolkanlin_PRIVATE_HEADERS
    logging.h
)

if (NOT WITH_KDE)
    list(APPEND wolkanlin_HEADERS wjob.h)
    list(APPEND wolkanlin_SRCS wjob.cpp wjob_p.h)
endif (NOT WITH_KDE)

add_library(wolkanlin
    ${wolkanlin_SRCS}
    ${wolkanlin_HEADERS}
    ${wolkanlin_PRIVATE_HEADERS}
)

add_library(WolkanlinQt${QTVERMAJ}::Core ALIAS wolkanlin)

if(CMAKE_VERSION GREATER_EQUAL "3.16.0")
    target_precompile_headers(wolkanlin
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

target_compile_features(wolkanlin PUBLIC cxx_std_14)

target_link_libraries(wolkanlin
    PUBLIC
        Qt5::Core
    PRIVATE
        Qt5::Network
)

target_compile_definitions(wolkanlin
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
)

if (WITH_KDE)
    message(STATUS "KDE support enabled")
    target_compile_definitions(wolkanlin
        PUBLIC
            WOLKANLIN_WITH_KDE
    )
    target_link_libraries(wolkanlin
        PUBLIC
            KF5::CoreAddons
    )
endif (WITH_KDE)

if(ENABLE_MAINTAINER_FLAGS)
    target_compile_definitions(wolkanlin
        PRIVATE
            QT_DEPRECATED_WARNINGS
    )
    target_compile_options(wolkanlin
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

target_include_directories(wolkanlin PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/wolkanlin-qt${QTVERMAJ}>
)

set_target_properties(wolkanlin PROPERTIES
    EXPORT_NAME Core
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    OUTPUT_NAME wolkanlin-qt${QTVERMAJ}
    PUBLIC_HEADER "${wolkanlin_HEADERS}"
)

install(TARGETS wolkanlin
    EXPORT WolkanlinTargets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT libs
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libs
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libs
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wolkanlin-qt${QTVERMAJ}/Wolkanlin COMPONENT devel
)
