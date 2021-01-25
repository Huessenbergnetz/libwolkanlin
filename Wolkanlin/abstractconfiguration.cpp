/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "abstractconfiguration.h"
#include "logging.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QUrl>

using namespace Wolkanlin;

AbstractConfiguration::AbstractConfiguration(QObject *parent) : QObject(parent)
{

}

AbstractConfiguration::~AbstractConfiguration() = default;

void AbstractConfiguration::setUsername(const QString &username)
{
    Q_UNUSED(username)
}

void AbstractConfiguration::setPassword(const QString &password)
{
    Q_UNUSED(password);
}

void AbstractConfiguration::setHost(const QString &host)
{
    Q_UNUSED(host)
}

void AbstractConfiguration::setPort(int port)
{
    Q_UNUSED(port)
}

QString AbstractConfiguration::installPath() const
{
    return QString();
}

void AbstractConfiguration::setInstallPath(const QString &installPath)
{
    Q_UNUSED(installPath)
}

bool AbstractConfiguration::useSsl() const
{
    return true;
}

void AbstractConfiguration::setUseSsl(bool useSsl)
{
    Q_UNUSED(useSsl)
}

bool AbstractConfiguration::ignoreSslErrors() const
{
    return false;
}

void AbstractConfiguration::setIgnoreSslErrors(bool ignoreSslErrors)
{
    Q_UNUSED(ignoreSslErrors);
}

QString AbstractConfiguration::userAgent() const
{
    return QStringLiteral("libwolkanlin %1").arg(QStringLiteral(WOLKANLIN_VERSION));
}

QString AbstractConfiguration::loginFlowUserAgent() const
{
    return userAgent();
}

bool AbstractConfiguration::setLoginFlowCredentials(const QUrl &credentialUrl)
{
    Q_UNUSED(credentialUrl);
    return false;
}

bool AbstractConfiguration::setLoginFlowCredentials(const QByteArray &credentials)
{
    if (Q_UNLIKELY(credentials.isEmpty())) {
        qCCritical(wlCore, "%s", "Failed to set login flow credentials from empty data.");
        return false;
    }

    QJsonParseError jsonError;
    const QJsonDocument json = QJsonDocument::fromJson(credentials, &jsonError);
    if (Q_UNLIKELY(jsonError.error != QJsonParseError::NoError)) {
        qCCritical(wlCore, "Failed to parse the login flow credential JSON data: %s", qUtf8Printable(jsonError.errorString()));
        return false;
    }

    return setLoginFlowCredentials(json);
}

bool AbstractConfiguration::setLoginFlowCredentials(const QJsonDocument &credentials)
{
    if (Q_UNLIKELY(credentials.isEmpty() || credentials.isNull())) {
        qCCritical(wlCore, "%s", "Failed to set login flow credentials from empty JSON document.");
        return false;
    }

    if (Q_UNLIKELY(!credentials.isObject())) {
        qCCritical(wlCore, "%s", "Failed to set login flow credentials from a non JSON object.");
        return false;
    }

    return setLoginFlowCredentials(credentials.object());
}

bool AbstractConfiguration::setLoginFlowCredentials(const QJsonObject &credentials)
{
    if (Q_UNLIKELY(credentials.empty())) {
        qCCritical(wlCore, "%s", "Failed to set login flow credentials from empty JSON object.");
        return false;
    }

    const QUrl server = QUrl(credentials.value(QStringLiteral("server")).toString(), QUrl::StrictMode);
    if (Q_UNLIKELY(!setServerUrl(server))) {
        qCCritical(wlCore, "Failed to set login flow credentials with invalid server URL: %s", qUtf8Printable(credentials.value(QStringLiteral("server")).toString()));
        return false;
    }

    const QString loginName = credentials.value(QStringLiteral("loginName")).toString();
    if (Q_UNLIKELY(loginName.isEmpty())) {
        qCCritical(wlCore, "%s", "Failed to set login flow credentials with empty login name.");
        return false;
    }

    const QString appPassword = credentials.value(QStringLiteral("appPassword")).toString();
    if (Q_UNLIKELY(appPassword.isEmpty())) {
        qCCritical(wlCore, "%s", "Failed to set login flow credentials with empty application passsword.");
        return false;
    }

    setUsername(loginName);
    setPassword(appPassword);

    return false;
}

bool AbstractConfiguration::setServerUrl(const QUrl &url)
{
    if (Q_UNLIKELY(!url.isValid())) {
        qCCritical(wlCore, "Invalid server url: %s", qUtf8Printable(url.toString()));
        return false;
    }

    bool ssl = true;
    if (url.scheme().isEmpty() || url.scheme() == QLatin1String("https")) {
        ssl = true;
    } else if (url.scheme() == QLatin1String("http")) {
        ssl = false;
    } else {
        qCCritical(wlCore, "Invalid server URL scheme: %s", qUtf8Printable(url.scheme()));
        return false;
    }

    const QString host = url.host();
    if (Q_UNLIKELY(host.isEmpty())) {
        qCCritical(wlCore, "%s", "Empty server host name.");
        return false;
    }

    const int port = url.port(0);

    QString path = url.path();
    while (path.endsWith(QLatin1Char('/'))) {
        path.chop(1);
    }

    setUseSsl(ssl);
    setHost(host);
    setPort(port);
    setInstallPath(path);

    return true;
}

bool AbstractConfiguration::setServerUrl(const QString &url)
{
    const QString _url = url.trimmed();
    if (_url.isEmpty()) {
        qCCritical(wlCore, "%s", "Can not set connection data with empty server url.");
        return false;
    }
    return setServerUrl(QUrl(_url, QUrl::StrictMode));
}

bool AbstractConfiguration::setApplicationPassword(const QJsonDocument &json)
{
    if (Q_UNLIKELY(json.isEmpty() || json.isNull())) {
        qCCritical(wlCore, "%s", "Failed to set application password from empty JSON document.");
        return false;
    }

    if (Q_UNLIKELY(!json.isObject())) {
        qCCritical(wlCore, "%s", "Failed to set application password from a non JSON object.");
        return false;
    }

    return setApplicationPassword(json.object());
}

bool AbstractConfiguration::setApplicationPassword(const QJsonObject &json)
{
    if (Q_UNLIKELY(json.empty())) {
        qCCritical(wlCore, "%s", "Failed to set application password from empty JSON object.");
        return false;
    }

    const QJsonObject ocs = json.value(QStringLiteral("ocs")).toObject();
    if (Q_UNLIKELY(ocs.isEmpty())) {
        qCCritical(wlCore, "%s", "Failed to set application password from empty “ocs“ JSON object.");
        return false;
    }

    const QJsonObject data = ocs.value(QStringLiteral("data")).toObject();
    if (Q_UNLIKELY(ocs.isEmpty())) {
        qCCritical(wlCore, "%s", "Failed to set application password from empty “data“ JSON object.");
        return false;
    }

    const QString appPass = data.value(QStringLiteral("apppassword")).toString().trimmed();
    if (Q_UNLIKELY(appPass.isEmpty())) {
        qCCritical(wlCore, "%s", "Failed to set application password from empty “apppassword” JSON value.");
        return false;
    }

    setPassword(appPass);

    return true;
}

#include "moc_abstractconfiguration.cpp"
