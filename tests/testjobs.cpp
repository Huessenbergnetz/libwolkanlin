/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "testconfig.h"
#include <QTest>
#include <QObject>
#include <QSignalSpy>
#include <Wolkanlin/GetUserJob>
#include <Wolkanlin/GetWipeStatusJob>

using namespace Wolkanlin;

class JobsTest : public QObject
{
    Q_OBJECT
public:
    JobsTest(QObject *parent = nullptr) : QObject(parent) {}

    ~JobsTest() override = default;

private slots:
    void initTestCase();

    void testSetConfiguration();
    void testMissingConfiguration();
    void testMissingHost();
    void testMissingUsername();
    void testMissingPassword();
    void testGetUserJob();
    void testGetWipeStatusJob();

private:
    TestConfig *m_config = nullptr;
};

void JobsTest::initTestCase()
{
    m_config = new TestConfig(false, this);
    m_config->setHost(QStringLiteral("localhost"));
    m_config->setUsername(QStringLiteral("user"));
    m_config->setPassword(QStringLiteral("password"));

}

void JobsTest::testSetConfiguration()
{
    auto job = new GetUserJob(this);
    QSignalSpy spy(job, &Job::configurationChanged);
    QCOMPARE(job->configuration(), nullptr);
    auto conf = new TestConfig(false, this);
    job->setConfiguration(conf);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<TestConfig*>(), conf);
    QCOMPARE(job->configuration(), conf);
}

void JobsTest::testMissingConfiguration()
{
    auto job = new GetUserJob(this);
    QSignalSpy failedSpy(job, &Job::failed);
    QSignalSpy resultSpy(job, &WJob::result);
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), static_cast<int>(Wolkanlin::MissingConfig));
    QCOMPARE(failedSpy.count(), 1);
    QCOMPARE(failedSpy.at(0).at(0).toInt(), static_cast<int>(Wolkanlin::MissingConfig));
    QCOMPARE(resultSpy.count(), 1);
}

void JobsTest::testMissingHost()
{
    const int expectedErrorCode = static_cast<int>(Wolkanlin::MissingHost);
    auto job = new GetUserJob(this);
    auto conf = new TestConfig(false, this);
    conf->setUsername(QStringLiteral("user"));
    conf->setPassword(QStringLiteral("password"));
    job->setConfiguration(conf);
    QSignalSpy failedSpy(job, &Job::failed);
    QSignalSpy resultSpy(job, &WJob::result);
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), expectedErrorCode);
    QCOMPARE(failedSpy.count(), 1);
    QCOMPARE(failedSpy.at(0).at(0).toInt(), expectedErrorCode);
    QCOMPARE(resultSpy.count(), 1);
}

void JobsTest::testMissingUsername()
{
    const int expectedErrorCode = static_cast<int>(Wolkanlin::MissingUser);
    auto job = new GetUserJob(this);
    auto conf = new TestConfig(false, this);
    conf->setHost(QStringLiteral("localhost"));
    conf->setPassword(QStringLiteral("password"));
    job->setConfiguration(conf);
    QSignalSpy failedSpy(job, &Job::failed);
    QSignalSpy resultSpy(job, &WJob::result);
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), expectedErrorCode);
    QCOMPARE(failedSpy.count(), 1);
    QCOMPARE(failedSpy.at(0).at(0).toInt(), expectedErrorCode);
    QCOMPARE(resultSpy.count(), 1);
}

void JobsTest::testMissingPassword()
{
    const int expectedErrorCode = static_cast<int>(Wolkanlin::MissingPassword);
    auto job = new GetUserJob(this);
    auto conf = new TestConfig(false, this);
    conf->setHost(QStringLiteral("localhost"));
    conf->setUsername(QStringLiteral("user"));
    job->setConfiguration(conf);
    QSignalSpy failedSpy(job, &Job::failed);
    QSignalSpy resultSpy(job, &WJob::result);
    QVERIFY(!job->exec());
    QCOMPARE(job->error(), expectedErrorCode);
    QCOMPARE(failedSpy.count(), 1);
    QCOMPARE(failedSpy.at(0).at(0).toInt(), expectedErrorCode);
    QCOMPARE(resultSpy.count(), 1);
}

void JobsTest::testGetUserJob()
{
    // test constructor
    auto job = new GetUserJob(QStringLiteral("userId"), this);
    QCOMPARE(job->id(), QStringLiteral("userId"));

    job = new GetUserJob(this);
    job->setConfiguration(m_config);

    // test missing user id
    {
        const int expectedErrorCode = static_cast<int>(Wolkanlin::EmptyUser);
        QSignalSpy failedSpy(job, &Job::failed);
        QSignalSpy resultSpy(job, &WJob::result);
        QVERIFY(!job->exec());
        QCOMPARE(job->error(), expectedErrorCode);
        QCOMPARE(failedSpy.count(), 1);
        QCOMPARE(failedSpy.at(0).at(0).toInt(), expectedErrorCode);
        QCOMPARE(resultSpy.count(), 1);
    }

    // test id property
    {
        const QString id = QStringLiteral("userId");
        QSignalSpy spy(job, &GetUserJob::idChanged);
        QVERIFY(job->id().isEmpty()); // default value
        job->setId(id);
        QCOMPARE(job->id(), id);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toString(), id);
    }
}

void JobsTest::testGetWipeStatusJob()
{
    // test constructor
    auto job = new GetWipeStatusJob(QStringLiteral("my-token"), this);
    QCOMPARE(job->token(), QStringLiteral("my-token"));

    job = new GetWipeStatusJob(this);
    auto conf = new TestConfig(false, this);
    conf->setHost(QStringLiteral("localhost"));
    job->setConfiguration(conf);

    // test missing token
    {
        const int expectedErrorCode = static_cast<int>(Wolkanlin::MissingPassword);
        QSignalSpy failedSpy(job, &Job::failed);
        QSignalSpy resultSpy(job, &WJob::result);
        QVERIFY(!job->exec());
        QCOMPARE(job->error(), expectedErrorCode);
        QCOMPARE(failedSpy.count(), 1);
        QCOMPARE(failedSpy.at(0).at(0).toInt(), expectedErrorCode);
        QCOMPARE(resultSpy.count(), 1);
    }

    // test token property
    {
        const QString token = QStringLiteral("my-token");
        QSignalSpy spy(job, &GetWipeStatusJob::tokenChanged);
        QVERIFY(job->token().isEmpty()); // default value
        job->setToken(token);
        QCOMPARE(job->token(), token);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toString(), token);
    }
}

QTEST_MAIN(JobsTest)

#include "testjobs.moc"
