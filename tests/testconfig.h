/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_TESTCONFIG_H
#define WOLKANLIN_TESTCONFIG_H

#include "abstractconfiguration.h"

class TestConfig : public Wolkanlin::AbstractConfiguration
{
    Q_OBJECT
public:
    TestConfig(bool useAdmin = false, QObject *parent = nullptr);
    ~TestConfig() override;

    QString username() const override;

    void setUsername(const QString &username) override;

    QString password() const override;

    void setPassword(const QString &password) override;

    QString host() const override;

    void setHost(const QString &host) override;

    int port() const override;

    void setPort(int port) override;

    QString installPath() const override;

    void setInstallPath(const QString &installPath) override;

    bool useSsl() const override;

    void setUseSsl(bool useSsl) override;

    bool ignoreSslErrors() const override;

    void setIgnoreSslErrors(bool ignoreSslErrors) override;

    QString userAgent() const override;

    bool loadConfig();

private:
    QString m_username;
    QString m_password;
    QString m_host;
    QString m_installPath;
    int m_port;
    bool m_useSsl = true;
    bool m_ignoreSslErrors = false;
    bool m_useAdmin = false;
    Q_DISABLE_COPY(TestConfig)
};

#endif // WOLKANLIN_TESTCONFIG_H
