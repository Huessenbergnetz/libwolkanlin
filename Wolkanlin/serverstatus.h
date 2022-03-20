/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_SERVERSTATUS_H
#define WOLKANLIN_SERVERSTATUS_H

#include "wolkanlin_exports.h"
#include <QObject>
#include <memory>

namespace Wolkanlin {

class AbstractConfiguration;
class ServerStatusPrivate;

/*!
 * \brief Stores information about a remote server.
 *
 * This object stores information like returned by GetServerStatusJob.
 *
 * <H3 id="server-status-json-example">JSON representation example</H3>
 * This is the JSON object like returend by the Nextcloud API. fromJson() expects
 * a JSON object like this while toJson() creates a JSON object like this.
 * \include server-status-example.json
 *
 * <H3 id="server-status-usage-examples">Usage examples</H3>
 * To get server status information from the Nextcloud server, you can use GetServerStatusJob
 * together with fromJson() to fetch the data asynchronously.
 * \include get-server-status-async.cpp
 * There is also the possibility to do it synchronously.
 * \include get-server-status-sync.cpp
 * To get data for an already created %ServerStatus object, use the get() function.
 * \include server-status-get-function.cpp
 *
 * \headerfile "" <Wolkanlin/ServerStatus>
 */
class WOLKANLIN_LIBRARY ServerStatus : public QObject
{
    Q_OBJECT
    /*!
     * \brief Returns \c true if Nextcloud is installed, otherwise returns \c false.
     *
     * \par Access methods
     * \li bool isInstalled() const
     *
     * \par Notifier signal
     * \li void installedChanged(bool installed)
     */
    Q_PROPERTY(bool installed READ isInstalled NOTIFY installedChanged)
    /*!
     * \brief Returns \c true if Nextcloud is in maintenance mode, otherwise return \c false.
     *
     * \par Access methods
     * \li bool isInMaintenance() const
     *
     * \par Notifier signal
     * \li void maintenanceChanged(bool maintenance)
     */
    Q_PROPERTY(bool maintenance READ isInMaintenance NOTIFY maintenanceChanged)
    /*!
     * \brief Returns \c true if Nextcloud needs a database upgrade, otherwise returns \c false:
     *
     * \par Access methods
     * \li bool needsDbUpgrade() const
     *
     * \par Notifier signal
     * \li void needsDbUpgradeChanged(bool needsDbUprage)
     */
    Q_PROPERTY(bool needsDbUpgrade READ needsDbUpgrade NOTIFY needsDbUpgradeChanged)
    /*!
     * \brief This property holds the version of the Nextcloud.
     *
     * There is also the \link ServerStatus::versionstring versionstring\endlink property.
     * Main difference seems to be that version holds a more exact version of up to four parts (like 20.0.5.2)
     * while \link ServerStatus::versionstring versionstring\endlink holds a version up to three
     * parts (like 20.0.5).
     *
     * \par Access methods
     * \li QString version() const
     *
     * \par Notifier signal
     * \li void versionChanged(const QString &version)
     */
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    /*!
     * \brief This property holds the version string of the Nextcloud.
     *
     * There is also the \link ServerStatus::version version\endlink property.
     * Main difference seems to be that \link ServerStatus::version version\endlink holds a more exact
     * version of up to four parts like (20.0.5.2) while versionstring holds a version up to three parts
     * (like 20.0.5).
     *
     * \par Access methods
     * \li QString versionstring() const
     *
     * \par Notifier signal
     * \li void versionstringChanged(const QString &versionstring)
     */
    Q_PROPERTY(QString versionstring READ versionstring NOTIFY versionstringChanged)
    /*!
     * \brief This property holds the edition of the Nextcloud.
     *
     * \par Access methods
     * \li QString edition() const
     *
     * \par Notifier signal
     * \li void editionChanged(const QString &edition)
     */
    Q_PROPERTY(QString edition READ edition NOTIFY editionChanged)
    /*!
     * \brief This property holds the product name of the Nextcloud.
     *
     * By default on fresh installations this is simply “Nexcloud“ but can be changed
     * in the Nextcloud configration to a custom name like “My little cloud“.
     *
     * \par Access methods
     * \li QString productname() const
     *
     * \par Notifier signal
     * \li void productnameChanged(const QString &productname)
     */
    Q_PROPERTY(QString productname READ productname NOTIFY productnameChanged)
    /*!
     * \brief Returns \c true if extended support is available for this Nextcloud, otherwise returns \c false.
     *
     * \par Access methods
     * \li bool hasExtendedSupport() const
     *
     * \par Notifier signal
     * \li void extendedSupportChanged(bool extendedSupport)
     */
    Q_PROPERTY(bool extendedSupport READ hasExtendedSupport NOTIFY extendedSupportChanged)
    /*!
     * \brief Returns \c true while the object is loading data.
     *
     * When calling get() this indicates the loading/request process is running.
     *
     * \par Access methods
     * \li bool isLoading() const
     *
     * \par Notifier signal
     * void isLoadingChanged(bool isLoading)
     */
    Q_PROPERTY(bool isLoading READ isLoading NOTIFY isLoadingChanged)
public:
    /*!
     * \brief Constructs a new empty %ServerStatus object with the given \a parent.
     *
     * isEmpty() will return \c true.
     */
    explicit ServerStatus(QObject *parent = nullptr);
    /*!
     * \brief Destroys the %ServerStatus object.
     */
    ~ServerStatus() override;

    /*!
     * \brief Getter function for the \link ServerStatus::installed installed\endlink property.
     * \sa installedChanged()
     */
    bool isInstalled() const;
    /*!
     * \brief Getter function for the \link ServerStatus::maintenance maintenance\endlink property.
     * \sa maintenanceChanged()
     */
    bool isInMaintenance() const;
    /*!
     * \brief Getter function for the \link ServerStatus::needsDbUpgrade needsDbUpgrade\endlink property.
     * \sa needsDbUpgradeChanged()
     */
    bool needsDbUpgrade() const;
    /*!
     * \brief Getter function for the \link ServerStatus::version version\endlink property.
     * \sa versionChanged()
     */
    QString version() const;
    /*!
     * \brief Getter function for the \link ServerStatus::versionstring versionstring\endlink property.
     * \sa versionstringChanged()
     */
    QString versionstring() const;
    /*!
     * \brief Getter function for the \link ServerStatus::edition edition\endlink property.
     * \sa editionChanged()
     */
    QString edition() const;
    /*!
     * \brief Getter function for the \link ServerStatus::productname productname\endlink property.
     * \sa productnameChanged()
     */
    QString productname() const;
    /*!
     * \brief Getter function for the \link ServerStatus::extendedSupport extendedSupport\endlink property.
     * \sa extendedSupportChanged()
     */
    bool hasExtendedSupport() const;
    /*!
     * \brief Getter function for the \link ServerStatus::isLoading isLoading\endlink property.
     * \sa isLoadingChanged()
     */
    bool isLoading() const;
    /*!
     * \brief Returns \c true if the %ServerStatus object is empty, otherwise returns \c false.
     *
     * The %ServerStatus object is empty, if no \link ServerStatus::version version\endlink has been set.
     */
    bool isEmpty() const;

    /*!
     * \brief Converts the %ServerStatus object to a JSON object where the property names ar the keys.
     *
     * See also the <A href="#server-status-json-example">JSON representation example</A> to leran more about
     * the created JSON object structure.
     */
    QJsonObject toJson() const;

    /*!
     * \brief Creates a new %ServerStatus object with the given \a parent from \a json.
     *
     * Will convert the \a json into a JSON object at first. If the \a json is empty or
     * does not contain an object, a pointer to an empty %ServerStatus object will be returned.
     * See also the <A href="#server-status-json-example">JSON representation example</A> to learn
     * more about the expected JSON object structure.
     */
    static ServerStatus *fromJson(const QJsonDocument &json, QObject *parent = nullptr);

    /*!
     * \brief Creates a new %ServerStatus object with the given \a parent from \a json.
     *
     * If the \a json is empty, a pointer to an empty %ServerStatus object will be returned.
     * See also the <A href="#server-status-json-example">JSON representation example</A> to learn
     * more about the expected JSON object structure.
     */
    static ServerStatus *fromJson(const QJsonObject &json, QObject *parent = nullptr);

    /*!
     * \brief Get data from the Nextcloud server configured in \a config.
     *
     * This will use GetServerStatusJob internally to get the data and update the properties
     * according to the result. You can connect to the finished() signal to handle the data after
     * getting it. To handle errors, connect to the failed() signal. While the request is running,
     * \link ServerStatus::isLoading isLoading\endlink returns \c true.
     *
     * If \a config is not set, Wolkanlin::defaultConfiguration() will be used.
     *
     * By default, the request will be performed asynchronously. Set \a async to \c false to get
     * the data synchronously.
     *
     * If \a async is set to \c true (the default), this function will always return \c true. If
     * \a async is set to \c false, \c true will be retruned if the request succeeded, otherwise
     * \c false will be returned.
     */
    bool get(bool async = true, AbstractConfiguration *config = nullptr);

Q_SIGNALS:
    /*!
     * \brief Notifier signal for the \link ServerStatus::installed installed\endlink property.
     * \sa isInstalled()
     */
    void installedChanged(bool installed);
    /*!
     * \brief Notifier signal for the \link ServerStatus::maintenance maintenance\endlink property.
     * \sa isInMaintenance()
     */
    void maintenanceChanged(bool maintenance);
    /*!
     * \brief Notifier signal for the \link ServerStatus::needsDbUpgrade needsDbUpgrade\endlink property.
     * \sa needsDbUpgrade()
     */
    void needsDbUpgradeChanged(bool needsDbUpgrade);
    /*!
     * \brief Notifier signal for the \link ServerStatus::version version\endlink property.
     * \sa version()
     */
    void versionChanged(const QString &version);
    /*!
     * \brief Notifier signal for the \link ServerStatus::versionstring versionstring\endlink property.
     * \sa versionstring()
     */
    void versionstringChanged(const QString &versionstring);
    /*!
     * \brief Notifier signal for the \link ServerStatus::edition edition\endlink property.
     * \sa edition()
     */
    void editionChanged(const QString &edition);
    /*!
     * \brief Notifier signal for the \link ServerStatus::productname productname\endlink property.
     * \sa productname()
     */
    void productnameChanged(const QString &productname);
    /*!
     * \brief Notifier signal for the \link ServerStatus::extendedSupport extendedSupport\endlink property.
     * \sa hasExtendedSupport()
     */
    void extendedSupportChanged(bool extendedSupport);
    /*!
     * \brief Notifier signal for the \link ServerStatus::isLoading isLoading\endlink property.
     * \sa isLoading()
     */
    void isLoadingChanged(bool isLoading);

    /*!
     * \brief Emitted when getting server status data succeeded.
     *
     * When using get(), this will be emitted after the data has been set to the object.
     *
     * \sa failed()
     */
    void finished();

    /*!
     * \brief Emitten when getting server status data failed.
     *
     * When using get(), this will be emitted after requesting the data failed.
     *
     * \sa finished()
     */
    void failed(int error, const QString &errorString);

private:
    const std::unique_ptr<ServerStatusPrivate> wl_ptr;

    friend QDataStream &operator>>(QDataStream &stream, ServerStatus &serverStatus);

    Q_DECLARE_PRIVATE_D(wl_ptr, ServerStatus)
    Q_DISABLE_COPY(ServerStatus)
};

/*!
 * \relates Wolkanlin::ServerStatus
 * \brief Reads a %ServerStatus from \a stream and stores it to \a serverStatus.
 */
WOLKANLIN_LIBRARY QDataStream &operator>>(QDataStream &stream, ServerStatus &serverStatus);

}

/*!
 * \relates Wolkanlin::ServerStatus
 * \brief Writes the \a serverStatus to the \a dbg stream and returns the stream.
 */
WOLKANLIN_LIBRARY QDebug operator<<(QDebug dbg, const Wolkanlin::ServerStatus &serverStatus);

/*!
 * \relates Wolkanlin::ServerStatus
 * \brief Writes \a serverStatus to the \a stream.
 */
WOLKANLIN_LIBRARY QDataStream &operator<<(QDataStream &stream, const Wolkanlin::ServerStatus &serverStatus);

#endif // WOLKANLIN_SERVERSTATUS_H
