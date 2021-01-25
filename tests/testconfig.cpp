/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "testconfig.h"

TestConfig::TestConfig(bool useAdmin, QObject *parent)
    : Wolkanlin::AbstractConfiguration(parent), m_useAdmin(useAdmin)
{

}

TestConfig::~TestConfig() = default;

QString TestConfig::username() const
{
    return m_username;
}

void TestConfig::setUsername(const QString &username)
{
    m_username = username;
}

QString TestConfig::password() const
{
    return m_password;
}

void TestConfig::setPassword(const QString &password)
{
    m_password = password;
}

QString TestConfig::host() const
{
    return m_host;
}

void TestConfig::setHost(const QString &host)
{
    m_host = host;
}

int TestConfig::port() const
{
    return m_port;
}

void TestConfig::setPort(int port)
{
    m_port = port;
}

QString TestConfig::installPath() const
{
    return m_installPath;
}

void TestConfig::setInstallPath(const QString &installPath)
{
    m_installPath = installPath;
}

bool TestConfig::useSsl() const
{
    return m_useSsl;
}

void TestConfig::setUseSsl(bool useSsl)
{
    m_useSsl = useSsl;
}

bool TestConfig::ignoreSslErrors() const
{
    return m_ignoreSslErrors;
}

void TestConfig::setIgnoreSslErrors(bool ignoreSslErrors)
{
    m_ignoreSslErrors = ignoreSslErrors;
}

QString TestConfig::userAgent() const
{
    return QStringLiteral("libwolkanlin test suite");
}

bool TestConfig::loadConfig()
{
    QString serverUrl = QString::fromLocal8Bit(qgetenv("WOLKANLIN_TESTS_SERVER_URL")).trimmed();
#if defined(WOLKANLIN_TESTS_SERVER_URL)
    if (serverUrl.isEmpty()) {
        serverUrl = QStringLiteral(WOLKANLIN_TESTS_SERVER_URL);
    }
#endif
    if (serverUrl.isEmpty()) {
        qCritical("%s", "Can not run API tests without valid server url. Either use cmake -DTESTS_SERVER_URL:STRING or set the WOLKANLIN_TESTS_SERVER_URL environment variable before running tests.");
        return false;
    }
    if (!setServerUrl(serverUrl)) {
        return false;
    }

    QString username;
    if (m_useAdmin) {
        username = QString::fromLocal8Bit(qgetenv("WOLKANLIN_TESTS_ADMIN_USERNAME")).trimmed();
#if defined(WOLKANLIN_TESTS_ADMIN_USERNAME)
        if (username.isEmpty()) {
            username = QStringLiteral(WOLKANLIN_TESTS_ADMIN_USERNAME);
        }
#endif
        if (username.isEmpty()) {
            qCritical("%s", "Can not run API tests without valid admin user name. Either use cmake -DTESTS_ADMIN_USERNAME:STRING or set the WOLKANLIN_TESTS_ADMIN_USERNAME environment variable before running tests.");
            return false;
        }
    } else {
        username = QString::fromLocal8Bit(qgetenv("WOLKANLIN_TESTS_USER_USERNAME")).trimmed();
#if defined(WOLKANLIN_TESTS_USER_USERNAME)
        if (username.isEmpty()) {
            username = QStringLiteral(WOLKANLIN_TESTS_USER_USERNAME);
        }
#endif
        if (username.isEmpty()) {
            qCritical("%s", "Can not run API tests without valid normal user name. Either use cmake -DTESTS_USER_USERNAME:STRING or set the WOLKANLIN_TESTS_USER_USERNAME environment variable before running tests.");
            return false;
        }
    }
    setUsername(username);

    QString password;
    if (m_useAdmin) {
        password = QString::fromLocal8Bit(qgetenv("WOLKANLIN_TESTS_ADMIN_PASSWORD")).trimmed();
#if defined(WOLKANLIN_TESTS_ADMIN_PASSWORD)
        if (password.isEmpty()) {
            password = QStringLiteral(WOLKANLIN_TESTS_ADMIN_PASSWORD);
        }
#endif
        if (password.isEmpty()) {
            qCritical("%s", "Can not run API tests without valid admin user password. Either use cmake -DTESTS_ADMIN_PASSWORD:STRING or set the WOLKANLIN_TESTS_ADMIN_PASSWORD environment variable before running tests.");
            return false;
        }
    } else {
        password = QString::fromLocal8Bit(qgetenv("WOLKANLIN_TESTS_USER_PASSWORD")).trimmed();
#if defined(WOLKANLIN_TESTS_USER_PASSWORD)
        if (password.isEmpty()) {
            password = QStringLiteral(WOLKANLIN_TESTS_USER_PASSWORD);
        }
#endif
        if (password.isEmpty()) {
            qCritical("%s", "Can not run API tests without valid normal user password. Either use cmake -DTESTS_USER_PASSWORD:STRING or set the WOLKANLIN_TESTS_USER_PASSWORD environment variable before running tests.");
            return false;
        }
    }
    setPassword(password);

    QString ignoreSslErrors = QString::fromLocal8Bit(qgetenv("WOLKANLIN_TESTS_IGNORE_SSL_ERRORS")).trimmed();
#if defined(WOLKANLIN_TESTS_IGNORE_SSL_ERRORS)
    if (ignoreSslErrors.isEmpty()) {
        ignoreSslErrors = QStringLiteral(WOLKANLIN_TESTS_IGNORE_SSL_ERRORS);
    }
#endif
    if (ignoreSslErrors == QLatin1String("1") || QString::compare(ignoreSslErrors, QLatin1String("true"), Qt::CaseInsensitive) || QString::compare(ignoreSslErrors, QLatin1String("on"), Qt::CaseInsensitive)) {
        setIgnoreSslErrors(true);
    } else {
        setIgnoreSslErrors(false);
    }

    return true;
}

#include "moc_testconfig.cpp"
