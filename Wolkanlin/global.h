/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_GLOBAL_H
#define WOLKANLIN_GLOBAL_H

#include "wolkanlin_export.h"
#include <QVersionNumber>
#include <QLocale>

class QNetworkAccessManager;

/*!
 * \brief The root namespace for libwolkanlin.
 */
namespace Wolkanlin {

class AbstractConfiguration;

/*!
 * \brief Sets a pointer to a global default \a configuration.
 * \sa Wolkanlin::defaultConfiguration()
 */
WOLKANLIN_EXPORT void setDefaultConfiguration(AbstractConfiguration *configuration);

/*!
 * \brief Returns a pointer to a global default configuration.
 * \sa Wolkanlin::setDefaultConfiguration()
 */
WOLKANLIN_EXPORT AbstractConfiguration* defaultConfiguration();

/*!
 * \brief Sets a pointer to a global default network access manager \a nam.
 * \sa Wolkanlin::defaultNetworkAccessManager()
 */
WOLKANLIN_EXPORT void setDefaultNetworkAccessManager(QNetworkAccessManager *nam);

/*!
 * \brief Returns a pointer to a global default network access manager.
 * \sa Wolkanlin::defaultNetworkAccessManager()
 */
WOLKANLIN_EXPORT QNetworkAccessManager* defaultNetworkAccessManager();

/*!
 * \brief Returns the version number of the currently used libwolkanlin.
 */
WOLKANLIN_EXPORT QVersionNumber version();

/*!
 * \brief Load and install the translations for libwolkanlin for the given \a locale.
 *
 * Returns \c true on success, otherwise \c false.
 */
WOLKANLIN_EXPORT bool loadTranslations(const QLocale &locale = QLocale());

}

#endif // WOLKANLIN_GLOBAL_H
