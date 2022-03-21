/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_GLOBAL_H
#define WOLKANLIN_GLOBAL_H

#include "wolkanlin_exports.h"

/*!
 * \brief The root namespace for libwolkanlin.
 */
namespace Wolkanlin {

class AbstractConfiguration;
class AbstractNamFactory;

/*!
 * \brief Sets a pointer to a global default \a configuration.
 * \sa Wolkanlin::defaultConfiguration()
 */
WOLKANLIN_LIBRARY void setDefaultConfiguration(AbstractConfiguration *configuration);

/*!
 * \brief Returns a pointer to a global default configuration.
 * \sa Wolkanlin::setDefaultConfiguration()
 */
WOLKANLIN_LIBRARY AbstractConfiguration* defaultConfiguration();

/*!
 * \brief Sets a pointer to a global network access manager \a factory.
 * \sa Wolkanlin::networkAccessManagerFactory()
 */
WOLKANLIN_LIBRARY void setNetworkAccessManagerFactory(AbstractNamFactory *factory);

/*!
 * \brief Returns a pointer to a global network access manager factory.
 * \sa Wolkanlin::setNetworkAccessManagerFactory()
 */
WOLKANLIN_LIBRARY AbstractNamFactory* networkAccessManagerFactory();

}

#endif // WOLKANLIN_GLOBAL_H
