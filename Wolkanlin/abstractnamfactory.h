/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_ABSTRACTNAMFACTORY_H
#define WOLKANLIN_ABSTRACTNAMFACTORY_H

#include <QObject>
#include "wolkanlin_global.h"

class QNetworkAccessManager;

namespace Wolkanlin {

class WOLKANLIN_LIBRARY AbstractNamFactory
{
public:
    virtual ~AbstractNamFactory();

    virtual QNetworkAccessManager *create(QObject *parent) = 0;
};

}

#endif // WOLKANLIN_ABSTRACTNAMFACTORY_H
