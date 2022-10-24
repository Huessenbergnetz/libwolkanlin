/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "testconfig.h"
#include <Wolkanlin/Global>
#include <Wolkanlin/GetServerStatusJob>
#include <Wolkanlin/ServerStatus>
#include <Wolkanlin/GetAppPasswordJob>
#include <Wolkanlin/DeleteAppPasswordJob>
#include <Wolkanlin/GetUserJob>
#include <Wolkanlin/User>
#include <Wolkanlin/GetWipeStatusJob>
#include <QTest>

using namespace Wolkanlin;

class ApiCallsTest : public QObject
{
    Q_OBJECT
public:
    explicit ApiCallsTest(QObject *parent = nullptr);
    ~ApiCallsTest() override;

private slots:
    void initTestCase();

    void testGetServerStatusJob();

    void testGetAppPassword();
    void testGetAppPasswordInvalidUser();
    void testGetAppPasswordInvalidPassword();
    void testGetAppPasswordAlreadyConverted();

    void testGetWipeStatusJobMissingToken();
    void testGetWipeStatusJobInvalidPassword();
    void testGetWipeStatusJobNoWipe();

    void testGetUserJob();
    void testGetUserJobNotFound();

    void testDeleteAppPasswordInvalidUser();
    void testDeleteAppPasswordInvalidPassword();
    void testDeleteAppPassword();

    void cleanupTestCase();

private:
    TestConfig *m_adminConfig = nullptr;
    TestConfig *m_userConfig = nullptr;
};

ApiCallsTest::ApiCallsTest(QObject *parent)
    : QObject(parent)
{

}

ApiCallsTest::~ApiCallsTest() = default;

void ApiCallsTest::initTestCase()
{
    m_userConfig = new TestConfig(false, this);
    QVERIFY(m_userConfig->loadConfig());
    Wolkanlin::setDefaultConfiguration(m_userConfig);

    m_adminConfig = new TestConfig(true, this);
    QVERIFY(m_adminConfig->loadConfig());
}

void ApiCallsTest::testGetServerStatusJob()
{
    auto job = new GetServerStatusJob(this);
    QVERIFY(job->exec());
    auto s1 = ServerStatus::fromJson(job->replyData());
    QVERIFY(!s1->isEmpty());

    auto s2 = new ServerStatus(this);
    s2->get(false);
    QVERIFY(!s2->isEmpty());
}

void ApiCallsTest::testGetAppPassword()
{
    auto job = new GetAppPasswordJob(this);
    QVERIFY(job->exec());
    QVERIFY(m_userConfig->setApplicationPassword(job->replyData()));
}

void ApiCallsTest::testGetAppPasswordInvalidUser()
{
    auto conf = new TestConfig(false, this);
    QVERIFY(conf->loadConfig());
    const QString invalidUserName = conf->username() + conf->username() + conf->username();
    conf->setUsername(invalidUserName);
    auto job = new GetAppPasswordJob(this);
    job->setConfiguration(conf);
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Wolkanlin::AuthNFailed));
}

void ApiCallsTest::testGetAppPasswordInvalidPassword()
{
    auto conf = new TestConfig(false, this);
    QVERIFY(conf->loadConfig());
    const QString invalidPassword = QStringLiteral(WOLKANLIN_TESTS_USER_PASSWORD) + QStringLiteral(WOLKANLIN_TESTS_USER_PASSWORD);
    conf->setPassword(invalidPassword);
    auto job = new GetAppPasswordJob(this);
    job->setConfiguration(conf);
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Wolkanlin::AuthNFailed));
}

void ApiCallsTest::testGetAppPasswordAlreadyConverted()
{
    auto job = new GetAppPasswordJob(this);
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Wolkanlin::AlreadyAppPassword));
}

void ApiCallsTest::testGetWipeStatusJobMissingToken()
{
    auto conf = new TestConfig(false, this);
    QVERIFY(conf->loadConfig());
    conf->setPassword(QString());
    auto job = new GetWipeStatusJob(this);
    job->setConfiguration(conf);
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Wolkanlin::MissingPassword));
}

void ApiCallsTest::testGetWipeStatusJobInvalidPassword()
{
    auto conf = new TestConfig(false, this);
    QVERIFY(conf->loadConfig());
    conf->setPassword(m_userConfig->password() + m_userConfig->password());
    auto job = new GetWipeStatusJob(this);
    job->setConfiguration(conf);
    QVERIFY(job->exec());
    QCOMPARE(job->error(), 0);
    QVERIFY(job->replyData().isEmpty());
}

void ApiCallsTest::testGetWipeStatusJobNoWipe()
{
    auto job = new GetWipeStatusJob(this);
    QVERIFY(job->exec());
    QCOMPARE(job->error(), 0);
    QVERIFY(job->replyData().isEmpty());
}

void ApiCallsTest::testGetUserJob()
{
    auto getUserJob = new GetUserJob(this);
    getUserJob->setId(QStringLiteral(WOLKANLIN_TESTS_USER_USERNAME));
    QVERIFY(getUserJob->exec());
    auto u1 = User::fromJson(getUserJob->replyData());
    QCOMPARE(u1->id(), QStringLiteral(WOLKANLIN_TESTS_USER_USERNAME));

    auto u2 = new User(this);
    QVERIFY(u2->get(QStringLiteral(WOLKANLIN_TESTS_USER_USERNAME), false));
    QCOMPARE(u2->id(), QStringLiteral(WOLKANLIN_TESTS_USER_USERNAME));
}

void ApiCallsTest::testGetUserJobNotFound()
{
    auto getUserJob = new GetUserJob(QStringLiteral(WOLKANLIN_TESTS_USER_USERNAME) + QStringLiteral(WOLKANLIN_TESTS_USER_USERNAME), this);
    QVERIFY(!getUserJob->exec());

    auto u = new User(this);
    QVERIFY(!u->get(QStringLiteral(WOLKANLIN_TESTS_USER_USERNAME) + QStringLiteral(WOLKANLIN_TESTS_USER_USERNAME), false));
}

void ApiCallsTest::testDeleteAppPasswordInvalidUser()
{
    auto conf = new TestConfig(false, this);
    QVERIFY(conf->loadConfig());
    const QString invalidUser = conf->username() + conf->username() + conf->username();
    conf->setUsername(invalidUser);
    auto job = new DeleteAppPasswordJob(this);
    job->setConfiguration(conf);
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Wolkanlin::AuthNFailed));
}

void ApiCallsTest::testDeleteAppPasswordInvalidPassword()
{
    auto conf = new TestConfig(false, this);
    QVERIFY(conf->loadConfig());
    const QString invalidPassword = QStringLiteral(WOLKANLIN_TESTS_USER_PASSWORD) + QStringLiteral(WOLKANLIN_TESTS_USER_PASSWORD);
    conf->setPassword(invalidPassword);
    auto job = new DeleteAppPasswordJob(this);
    job->setConfiguration(conf);
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Wolkanlin::AuthNFailed));
}

void ApiCallsTest::testDeleteAppPassword()
{
    auto job = new DeleteAppPasswordJob(this);
    QVERIFY(job->exec());
}

void ApiCallsTest::cleanupTestCase()
{

}

QTEST_MAIN(ApiCallsTest)

#include "testapicalls.moc"
