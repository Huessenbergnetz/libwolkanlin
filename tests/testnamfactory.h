/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TESTNAMFACTORY_H
#define TESTNAMFACTORY_H

#include <Wolkanlin/AbstractNamFactory>

class TestNamFactory : public Wolkanlin::AbstractNamFactory
{
public:
    ~TestNamFactory() override;

    QNetworkAccessManager *create(QObject *parent = nullptr) override;
};

#endif // TESTNAMFACTORY_H
