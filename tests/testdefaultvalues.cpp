/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "testconfig.h"
#include <Wolkanlin/Global>
#include <QObject>
#include <QNetworkAccessManager>
#include <QTest>
#include <memory>

class DefaultValuesTest : public QObject
{
    Q_OBJECT
public:
    explicit DefaultValuesTest(QObject *parent = nullptr);
    ~DefaultValuesTest() override;

private slots:
    void testDefaultConfiguration();
    void testDefaultNetworkAccessManager();
};

DefaultValuesTest::DefaultValuesTest(QObject *parent)
    : QObject(parent)
{

}

DefaultValuesTest::~DefaultValuesTest() = default;

void DefaultValuesTest::testDefaultConfiguration()
{
    auto c = new TestConfig(false, this);
    Wolkanlin::setDefaultConfiguration(c);
    QCOMPARE(Wolkanlin::defaultConfiguration(), c);
}

void DefaultValuesTest::testDefaultNetworkAccessManager()
{
    auto nam = new QNetworkAccessManager(this);
    Wolkanlin::setDefaultNetworkAccessManager(nam);
    QCOMPARE(Wolkanlin::defaultNetworkAccessManager(), nam);
}

QTEST_MAIN(DefaultValuesTest)

#include "testdefaultvalues.moc"
