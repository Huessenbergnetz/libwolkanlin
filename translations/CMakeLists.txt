# SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: LGPL-3.0-or-later

find_program(LRELEASE_CMD_PATH NAMES lrelease-qt5 lrelease REQUIRED)
set(LRELEASE_CMD ${LRELEASE_CMD_PATH})

message(STATUS "Found lrelease at ${LRELEASE_CMD}")

set(LANGS de)

list(APPEND QM_FILES ${CMAKE_CURRENT_BINARY_DIR}/libwolkanlin.qm)
list(APPEND TS_FILES libwolkanlin.ts)

set(LRELEASE_OPTIONS "-idbased" "-silent")

foreach(l ${LANGS})
    set(TRANS_ARGS ${TRANS_ARGS} COMMAND ${LRELEASE_CMD} ${LRELEASE_OPTIONS} libwolkanlin_${l}.ts -qm ${CMAKE_CURRENT_BINARY_DIR}/libwolkanlin_${l}.qm)
    list(APPEND QM_FILES ${CMAKE_CURRENT_BINARY_DIR}/libwolkanlin_${l}.qm)
    list(APPEND TS_FILES libwolkanlin_${l}.ts)
endforeach(l ${LANGS})

add_custom_target(translations ALL ${LRELEASE_CMD} ${LRELEASE_OPTIONS} libwolkanlin.ts -qm ${CMAKE_CURRENT_BINARY_DIR}/libwolkanlin.qm
    ${TRANS_ARGS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Compiling translation files"
    SOURCES ${TS_FILES}
)

install(FILES ${QM_FILES} DESTINATION ${CMAKE_INSTALL_LOCALEDIR})

