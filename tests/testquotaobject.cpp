/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <QTest>
#include <QObject>
#include <QDataStream>
#include "quota.h"

using namespace Wolkanlin;

class QuotaObjectTest : public QObject
{
    Q_OBJECT
public:
    QuotaObjectTest(QObject *parent = nullptr);
    ~QuotaObjectTest() override;

private slots:
    void testDefaultConstructor();
    void testConstructorWithArgs();
    void testCompare();
    void testMove();
    void testDataStream();
    void testJsonConvert();
};

QuotaObjectTest::QuotaObjectTest(QObject *parent) : QObject(parent)
{

}

QuotaObjectTest::~QuotaObjectTest() = default;

void QuotaObjectTest::testDefaultConstructor()
{
    Quota q;
    QVERIFY(q.isNull());
    QCOMPARE(q.free(), 0);
    QCOMPARE(q.used(), 0);
    QCOMPARE(q.quota(), 0);
    QCOMPARE(q.total(), 0);
    QCOMPARE(q.relative(), 0.0);
}

void QuotaObjectTest::testConstructorWithArgs()
{
    const qint64 free = 2068694953984;
    const qint64 used = 78805623983;
    const qint64 quota = -3;
    const qint64 total = 2147500577967;
    const double relative = 3.67;

    Quota q(free, used, quota, total, relative);
    QVERIFY(!q.isNull());
    QCOMPARE(q.free(), free);
    QCOMPARE(q.used(), used);
    QCOMPARE(q.quota(), quota);
    QCOMPARE(q.total(), total);
    QCOMPARE(q.relative(), relative);
}

void QuotaObjectTest::testCompare()
{
    const Quota q1(1, 2, 3, 4, 5.0);
    const Quota q2(1, 2, 3, 4, 5.0);
    const Quota q3 = q1;
    const Quota q4(2, 3, 4, 5, 6.0);
    const Quota q5;
    const Quota q6;

    QVERIFY(q1 == q2);
    QVERIFY(q1 == q3);
    QVERIFY(q1 != q4);
    QVERIFY(q2 != q4);
    QVERIFY(q3 != q4);
    QVERIFY(q4 != q5);
    QVERIFY(q5 != q4);
    QVERIFY(q5 == q6);
}

void QuotaObjectTest::testMove()
{
    // test move constructor
    {
        Quota q1(1, 2, 3, 4, 5.0);
        Quota q2(std::move(q1));
        QCOMPARE(q2.free(), 1);
        QCOMPARE(q2.used(), 2);
        QCOMPARE(q2.quota(), 3);
        QCOMPARE(q2.total(), 4);
        QCOMPARE(q2.relative(), 5.0);
    }

    // test move assignment
    {
        Quota q1(1, 2, 3, 4, 5.0);
        Quota q2(2, 3, 4, 5, 6.0);
        q2 = std::move(q1);
        QCOMPARE(q2.free(), 1);
        QCOMPARE(q2.used(), 2);
        QCOMPARE(q2.quota(), 3);
        QCOMPARE(q2.total(), 4);
        QCOMPARE(q2.relative(), 5.0);

        Quota q3;
        q3 = std::move(q2);
        QCOMPARE(q3.free(), 1);
        QCOMPARE(q3.used(), 2);
        QCOMPARE(q3.quota(), 3);
        QCOMPARE(q3.total(), 4);
        QCOMPARE(q3.relative(), 5.0);
    }
}

void QuotaObjectTest::testDataStream()
{
    Quota q1(1, 2, 3, 4, 5.0);

    QByteArray outBa;
    QDataStream out(&outBa, QIODevice::WriteOnly);
    out << q1;

    const QByteArray inBa = outBa;
    QDataStream in(inBa);
    Quota q2;
    in >> q2;

    QCOMPARE(q1, q2);
}

void QuotaObjectTest::testJsonConvert()
{
    Quota q1(1, 2, 3, 4, 5.0);
    const QJsonObject j1 = q1.toJson();
    QCOMPARE(q1.free(), static_cast<qint64>(j1.value(QStringLiteral("free")).toDouble()));
    QCOMPARE(q1.used(), static_cast<qint64>(j1.value(QStringLiteral("used")).toDouble()));
    QCOMPARE(q1.quota(), static_cast<qint64>(j1.value(QStringLiteral("quota")).toDouble()));
    QCOMPARE(q1.total(), static_cast<qint64>(j1.value(QStringLiteral("total")).toDouble()));
    QCOMPARE(q1.relative(), j1.value(QStringLiteral("relative")).toDouble());

    Quota q2;
    QVERIFY(q2.isNull());
    const QJsonObject j2 = q2.toJson();
    QVERIFY(j2.isEmpty());

    const Quota q3 = Quota::fromJson(j1);
    QCOMPARE(q3.free(), q1.free());
    QCOMPARE(q3.used(), q1.used());
    QCOMPARE(q3.quota(), q1.quota());
    QCOMPARE(q3.total(), q1.total());
    QCOMPARE(q3.relative(), q1.relative());

    const Quota q4 = Quota::fromJson(QJsonObject());
    QVERIFY(q4.isNull());
}

QTEST_MAIN(QuotaObjectTest)

#include "testquotaobject.moc"
