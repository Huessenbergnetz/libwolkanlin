/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "testconfig.h"
#include "testnamfactory.h"
#include <Wolkanlin/Global>
#include <QObject>
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
    void testNetworkAccessManagerFactory();
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

void DefaultValuesTest::testNetworkAccessManagerFactory()
{
    auto nf = std::make_unique<TestNamFactory>();
    Wolkanlin::setNetworkAccessManagerFactory(nf.get());
    QCOMPARE(Wolkanlin::networkAccessManagerFactory(), nf.get());
}

QTEST_MAIN(DefaultValuesTest)

#include "testdefaultvalues.moc"
