/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_GLOBAL_H
#define WOLKANLIN_GLOBAL_H

#include <QtCore/QtGlobal>

/*!
 * \namespace Wolkanlin
 * \brief The root namespace for libwolkanlin.
 */

#if defined(wolkanlin_EXPORTS)
#  define WOLKANLIN_LIBRARY Q_DECL_EXPORT
#else
#  define WOLKANLIN_LIBRARY Q_DECL_IMPORT
#endif

#endif // WOLKANLIN_GLOBAL_H
