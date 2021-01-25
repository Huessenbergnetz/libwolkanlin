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
#include "user.h"

using namespace Wolkanlin;

class UserObjectTest : public QObject
{
    Q_OBJECT
public:
    UserObjectTest(QObject *parent = nullptr);
    ~UserObjectTest() override;

private slots:
    void initTestCase();

    void testDefaultConstructor();
    void testJsonConverters();

private:
    QJsonDocument m_json;
};

UserObjectTest::UserObjectTest(QObject *parent) : QObject(parent)
{

}

UserObjectTest::~UserObjectTest() = default;

void UserObjectTest::initTestCase()
{
    QJsonParseError jsonError;
    m_json = QJsonDocument::fromJson(QByteArrayLiteral("{\"ocs\":{\"meta\":{\"status\":\"ok\",\"statuscode\":100,\"message\":\"OK\",\"totalitems\":\"\",\"itemsperpage\":\"\"},\"data\":{\"enabled\":true,\"storageLocation\":\"/srv/www/nextcloud/data/tester\",\"id\":\"tester\",\"lastLogin\":1611134157000,\"backend\":\"Database\",\"subadmin\":[\"group1\"],\"quota\":{\"free\":209639130,\"used\":76070,\"total\":209715200,\"relative\":0.04,\"quota\":209715200},\"email\":\"tester@example.net\",\"displayname\":\"Tester\",\"phone\":\"+49123456789\",\"address\":\"Somewhere over the rainbow\",\"website\":\"https://example.net\",\"twitter\":\"@tester\",\"groups\":[\"group1\",\"group2\"],\"language\":\"de_DE\",\"locale\":\"de_DE\",\"backendCapabilities\":{\"setDisplayName\":false,\"setPassword\":true}}}}"), &jsonError);
    QVERIFY(jsonError.error == QJsonParseError::NoError);
}

void UserObjectTest::testDefaultConstructor()
{
    auto u = new User(this);
    QVERIFY(u->isEmpty());
    QVERIFY(!u->isEnabled());
    QVERIFY(u->storageLocation().isEmpty());
    QVERIFY(u->id().isEmpty());
    QVERIFY(u->lastLogin().isNull());
    QVERIFY(u->backend().isEmpty());
    QVERIFY(u->subadmin().empty());
    QVERIFY(u->quota().isNull());
    QVERIFY(u->email().isEmpty());
    QVERIFY(u->displayname().isEmpty());
    QVERIFY(u->phone().isEmpty());
    QVERIFY(u->address().isEmpty());
    QVERIFY(!u->website().isValid());
    QVERIFY(u->twitter().isEmpty());
    QVERIFY(u->groups().empty());
    QVERIFY(u->language().isEmpty());
    QVERIFY(u->locale().isEmpty());
    QVERIFY(u->backendCapabilities() == 0);
}

void UserObjectTest::testJsonConverters()
{
    auto u1 = User::fromJson(m_json, this);
    QVERIFY(!u1->isEmpty());
    QVERIFY(u1->isEnabled());
    QCOMPARE(u1->storageLocation(), QStringLiteral("/srv/www/nextcloud/data/tester"));
    QCOMPARE(u1->id(), QStringLiteral("tester"));
    QCOMPARE(u1->lastLogin(), QDateTime::fromMSecsSinceEpoch(1611134157000));
    QCOMPARE(u1->backend(), QStringLiteral("Database"));
    QCOMPARE(u1->subadmin(), QStringList{QStringLiteral("group1")});
    QCOMPARE(u1->quota(), Quota(209639130, 76070, 209715200, 209715200, 0.04));
    QCOMPARE(u1->email(), QStringLiteral("tester@example.net"));
    QCOMPARE(u1->displayname(), QStringLiteral("Tester"));
    QCOMPARE(u1->phone(), QStringLiteral("+49123456789"));
    QCOMPARE(u1->address(), QStringLiteral("Somewhere over the rainbow"));
    QCOMPARE(u1->website(), QUrl(QStringLiteral("https://example.net")));
    QCOMPARE(u1->twitter(), QStringLiteral("@tester"));
    QCOMPARE(u1->groups(), QStringList({QStringLiteral("group1"), QStringLiteral("group2")}));
    QCOMPARE(u1->language(), QStringLiteral("de_DE"));
    QCOMPARE(u1->locale(), QStringLiteral("de_DE"));
    QVERIFY((u1->backendCapabilities() & User::CanSetDisplayName) != User::CanSetDisplayName);
    QVERIFY((u1->backendCapabilities() & User::CanSetPassword) == User::CanSetPassword);

    auto u2 = User::fromJson(QJsonDocument(), this);
    QVERIFY(u2->isEmpty());

    auto u3 = User::fromJson(QJsonObject(), this);
    QVERIFY(u3->isEmpty());

    const QJsonObject ocs = m_json.object().value(QStringLiteral("ocs")).toObject();
    auto u4 = User::fromJson(ocs, this);
    QCOMPARE(u4->id(), QStringLiteral("tester"));

    const QJsonObject data = ocs.value(QStringLiteral("data")).toObject();
    auto u5 = User::fromJson(data, this);
    QCOMPARE(u5->id(), QStringLiteral("tester"));

    auto u6 = User::fromJson(ocs.value(QStringLiteral("meta")).toObject(), this);
    QVERIFY(u6->isEmpty());

    const QJsonObject convertedJson = u1->toJson();
    QCOMPARE(data, convertedJson);
}

QTEST_MAIN(UserObjectTest)

#include "testuserobject.moc"
