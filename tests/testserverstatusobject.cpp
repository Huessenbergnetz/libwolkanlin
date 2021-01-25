/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <QTest>
#include <QObject>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include "serverstatus.h"

using namespace Wolkanlin;

class ServerStatusObjectTest : public QObject
{
    Q_OBJECT
public:
    ServerStatusObjectTest(QObject *parent = nullptr);
    ~ServerStatusObjectTest() override;

private slots:
    void initTestCase();

    void testDefaultConstructor();
    void testJsonConverters();
    void testDataStreamConverters();

private:
    QJsonDocument m_json;
};

ServerStatusObjectTest::ServerStatusObjectTest(QObject *parent) : QObject(parent)
{

}

ServerStatusObjectTest::~ServerStatusObjectTest() = default;

void ServerStatusObjectTest::initTestCase()
{
    QJsonParseError jsonError;
    m_json = QJsonDocument::fromJson(QByteArrayLiteral("{\"installed\":true,\"maintenance\":true,\"needsDbUpgrade\":true,\"version\":\"20.0.5.2\",\"versionstring\":\"20.0.5\",\"edition\":\"Test edition\",\"productname\":\"My little cloud\",\"extendedSupport\":true}"), &jsonError);
    QCOMPARE(jsonError.error, QJsonParseError::NoError);
}

void ServerStatusObjectTest::testDefaultConstructor()
{
    auto s = new ServerStatus(this);
    QVERIFY(s->isEmpty());
    QVERIFY(!s->isInstalled());
    QVERIFY(!s->isInMaintenance());
    QVERIFY(!s->needsDbUpgrade());
    QVERIFY(s->version().isEmpty());
    QVERIFY(s->versionstring().isEmpty());
    QVERIFY(s->edition().isEmpty());
    QVERIFY(s->productname().isEmpty());
    QVERIFY(!s->hasExtendedSupport());
}

void ServerStatusObjectTest::testJsonConverters()
{
    auto s1 = ServerStatus::fromJson(m_json, this);
    QVERIFY(!s1->isEmpty());
    QVERIFY(s1->isInstalled());
    QVERIFY(s1->isInMaintenance());
    QVERIFY(s1->needsDbUpgrade());
    QCOMPARE(s1->version(), QStringLiteral("20.0.5.2"));
    QCOMPARE(s1->versionstring(), QStringLiteral("20.0.5"));
    QCOMPARE(s1->edition(), QStringLiteral("Test edition"));
    QCOMPARE(s1->productname(), QStringLiteral("My little cloud"));
    QVERIFY(s1->hasExtendedSupport());

    auto s2 = ServerStatus::fromJson(QJsonDocument());
    QVERIFY(s2->isEmpty());

    auto s3 = ServerStatus::fromJson(QJsonObject());
    QVERIFY(s3->isEmpty());

    const QJsonObject convertedJson = s1->toJson();
    QCOMPARE(convertedJson, m_json.object());
}

void ServerStatusObjectTest::testDataStreamConverters()
{
    auto s1 = ServerStatus::fromJson(m_json, this);

    QByteArray outBa;
    QDataStream out(&outBa, QIODevice::WriteOnly);
    out << *s1;

    const QByteArray inBa = outBa;
    QDataStream in(inBa);
    auto s2 = new ServerStatus(this);
    in >> *s2;

    QCOMPARE(s1->isInstalled(), s2->isInstalled());
    QCOMPARE(s1->isInMaintenance(), s2->isInMaintenance());
    QCOMPARE(s1->needsDbUpgrade(), s2->needsDbUpgrade());
    QCOMPARE(s1->version(), s2->version());
    QCOMPARE(s1->versionstring(), s2->versionstring());
    QCOMPARE(s1->edition(), s2->edition());
    QCOMPARE(s1->productname(), s2->productname());
    QCOMPARE(s1->hasExtendedSupport(), s2->hasExtendedSupport());
}

QTEST_MAIN(ServerStatusObjectTest)

#include "testserverstatusobject.moc"
