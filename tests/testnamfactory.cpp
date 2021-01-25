/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "testnamfactory.h"
#include <QNetworkAccessManager>

TestNamFactory::~TestNamFactory() = default;

QNetworkAccessManager *TestNamFactory::create(QObject *parent)
{
    return new QNetworkAccessManager(parent);
}
