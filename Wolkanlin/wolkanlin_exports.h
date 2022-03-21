/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_EXPORTS_H
#define WOLKANLIN_EXPORTS_H

#include <QtCore/QtGlobal>

#if defined(WolkanlinQt5_EXPORTS) || defined(WolkanlinQt6_EXPORTS)
#  define WOLKANLIN_LIBRARY Q_DECL_EXPORT
#else
#  define WOLKANLIN_LIBRARY Q_DECL_IMPORT
#endif

#endif // WOLKANLIN_EXPORTS_H
