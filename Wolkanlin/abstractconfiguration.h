/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_ABSTRACTCONFIGURATION_H
#define WOLKANLIN_ABSTRACTCONFIGURATION_H

#include <QObject>
#include "wolkanlin_global.h"

class QUrl;
class QJsonDocument;
class QJsonObject;

namespace Wolkanlin {

/*!
 * \brief Stores configuration for API requests.
 *
 * This class stores information used to perform API requests, especially the
 * url of the remote host and the username and pasword. Reimplement this class
 * to provide these information.
 *
 * All API classes have a configuration property that takes subclasses of AbstractConfiguration.
 * There is also Wolkanlin::setDefaultConfiguration() to set a global default configuration
 * that will be used if no local configuration is available.
 *
 * The complete remote URL will be build from useSsl(), host(), port() and installPath().
 *
 * \headerfile "" <Wolkanlin/AbstractConfiguration>
 */
class WOLKANLIN_LIBRARY AbstractConfiguration : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new %AbstractConfiguratoin object with the given \a parent.
     */
    explicit AbstractConfiguration(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the %AbstractConfiguration object.
     */
    ~AbstractConfiguration() override;

    /*!
     * \brief Returns the username.
     *
     * Reimplement this to return the username used for authentication.
     *
     * \sa setUsername()
     */
    virtual QString username() const = 0;

    /*!
     * \brief Set the username.
     *
     * Reimplement this to set the \a username used for authentication.
     * The default implementation does nothing.
     *
     * \sa username()
     */
    virtual void setUsername(const QString &username);

    /*!
     * \brief Returns the password.
     *
     * Reimplement this to return the password used for authentication.
     *
     * \sa setPassword()
     */
    virtual QString password() const = 0;

    /*!
     * \brief Sets the password.
     *
     * Reimplement this to set the \a password used for authentication.
     * The default implementation does nothing.
     *
     * \sa password()
     */
    virtual void setPassword(const QString &password);

    /*!
     * \brief Returns the remote host name.
     *
     * Reimplement this to return the remote host name to connect to.
     *
     * \sa setHost()
     */
    virtual QString host() const = 0;

    /*!
     * \brief Set the host name.
     *
     * Reimplement this to set the remote \a host name to connect to.
     * The default implementation does nothing.
     *
     * \sa host()
     */
    virtual void setHost(const QString &host);

    /*!
     * \brief Returns the port.
     *
     * Reimplement this to return the port the remote host listens on.
     *
     * \sa setPort()
     */
    virtual int port() const = 0;

    /*!
     * \brief Sets the port.
     *
     * Reimplement this to set the \a port the remote host listens on.
     * The default implementation does nothing.
     *
     * Setting the \a port to \c 0 will omit the port when building the
     * URL and uses the protocol default port.
     */
    virtual void setPort(int port);

    /*!
     * \brief Returns the installation path.
     *
     * Reimplement this to return an optional installation path for the remote host.
     * The default implementation returns an empty string.
     *
     * \sa setInstallPath()
     */
    virtual QString installPath() const;

    /*!
     * \brief Sets the installation path.
     *
     * Reimplement this to set an optional installation path for the remote host.
     * The default implementation does nothing.
     *
     * \sa installPath()
     */
    virtual void setInstallPath(const QString &installPath);

    /*!
     * \brief Returns \c true if HTTPS should be used.
     *
     * The default implementation returns \c true.
     *
     * \sa setUseSsl()
     */
    virtual bool useSsl() const;

    /*!
     * \brief Set \a useSsl to \c true to use HTTPS.
     *
     * The default implementation does nothing.
     *
     * \sa useSsl()
     */
    virtual void setUseSsl(bool useSsl);

    /*!
     * \brief Returns \c true if SSL errors should be ignored.
     *
     * The default implementation returns \c false.
     *
     * \warning It is a security risk to ignore SSL errors.
     *
     * \sa setIgnoreSslErrors()
     */
    virtual bool ignoreSslErrors() const;

    /*!
     * \brief Set \a ignoreSslErrors to \c true to ignore SSL erros.
     *
     * The default implementation does nothing.
     *
     * \warning It is a security risk to ignore SSL errors.
     *
     * \sa ignoreSslErrors()
     */
    virtual void setIgnoreSslErrors(bool ignoreSslErrors);

    /*!
     * \brief Returns a user agent string.
     *
     * Reimplement this to return a custom user agent string that describes
     * your application. The default implementation returns "libwolkanlin $VERSION".
     */
    Q_INVOKABLE virtual QString userAgent() const;

    /*!
     * \brief Returns a user agent for the login flow.
     *
     * In the login flow API, the user agent is used to show the user the application that requests a new
     * application password and will also be used for the identifier of the generated application password.
     *
     * See also https://docs.nextcloud.com/server/latest/developer_manual/client_apis/LoginFlow/index.html
     *
     * Reimplement this in a subclass. The default implementation simply returns the same as getUserAgent(),
     * the string "libwolkanlin $VERSION", and that is not really meaningful to a user.
     */
    Q_INVOKABLE virtual QString loginFlowUserAgent() const;

public Q_SLOTS:
    /*!
     * \brief Sets login credentials requested from the login flow API and returns \c true on success.
     *
     * See also https://docs.nextcloud.com/server/latest/developer_manual/client_apis/LoginFlow/index.html
     *
     * Reimplement this in a subclass. The default implementation does nothing.
     */
    virtual bool setLoginFlowCredentials(const QUrl &credentialUrl);

    /*!
     * \brief Sets the login credentials requested from the login flow v2 API and returns \c true on success.
     *
     * This has to be a valid JSON object requested from the polling endpoint.
     * See also https://docs.nextcloud.com/server/latest/developer_manual/client_apis/LoginFlow/index.html
     */
    bool setLoginFlowCredentials(const QByteArray &credentials);

    /*!
     * \brief Sets the login credentials requested from the login flow v2 API and returns \c true on success.
     *
     * This has to be a valid JSON object requested from the polling endpoint.
     * See also https://docs.nextcloud.com/server/latest/developer_manual/client_apis/LoginFlow/index.html
     */
    bool setLoginFlowCredentials(const QJsonDocument &credentials);

    /*!
     * \brief Sets the login credentials requested from the login flow v2 API and returns \c true on success.
     *
     * This has to be a valid JSON object requested from the polling endpoint.
     * See also https://docs.nextcloud.com/server/latest/developer_manual/client_apis/LoginFlow/index.html
     */
    bool setLoginFlowCredentials(const QJsonObject &credentials);

    /*!
     * \brief Sets the server data via \a url and returns \c true on success, otherwise \c false.
     *
     * This takes the scheme and the host and if available the port and the path from the \a url to
     * set them to setUseSSL(), setHost(), setServerPort() and setInstallPath().
     */
    bool setServerUrl(const QUrl &url);

    /*!
     * \brief Sets the server data via \a url and returns \c true on success, otherwise \c false.
     *
     * This takes the scheme and the host and if available the port and the path from the \a url to
     * set them to setUseSSL(), setHost(), setServerPort() and setInstallPath().
     */
    bool setServerUrl(const QString &url);

    bool setApplicationPassword(const QJsonDocument &json);

    bool setApplicationPassword(const QJsonObject &json);

private:
    Q_DISABLE_COPY(AbstractConfiguration)
};

}

#endif // WOLKANLIN_ABSTRACTCONFIGURATION_H
