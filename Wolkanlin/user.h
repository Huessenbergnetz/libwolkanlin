/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_USER_H
#define WOLKANLIN_USER_H

#include "wolkanlin_exports.h"
#include "quota.h"
#include <QObject>
#include <QDateTime>
#include <QUrl>
#include <memory>

namespace Wolkanlin {

class AbstractConfiguration;
class UserPrivate;

/*!
 * \brief Stores information about a single user.
 *
 * This object stores information like returned by GetUserJob.
 *
 * <H3 id="user-json-example">JSON representation example</H3>
 * This is the JSON object like it is returned by the Nextcloud API. fromJson() loads
 * either from the whole content or directly from the “ocs“ or “data“ objects. When
 * using toJson() only the content of the “data“ object is returned. See the
 * <A HREF="https://docs.nextcloud.com/server/latest/developer_manual/client_apis/OCS/ocs-api-overview.html#user-metadata" rel="external noopener">OCS API overview</A>
 * for a maybe more recent description of this data.
 * \include userdata-example.json
 *
 * <H3 id="user-usage-examples">Usage examples</H3>
 * To get user meta data from the Nextcloud server, you can use GetUserJob together with
 * fromJson() to fetch the data asynchronously.
 * \include get-user-async.cpp
 * There is also the possibility to do it synchronously.
 * \include get-user-sync.cpp
 * To get data for an already created %User object, use the get() function.
 * \include user-get-function.cpp
 *
 * \headerfile "" <Wolkanlin/User>
 */
class WOLKANLIN_LIBRARY User : public QObject
{
    Q_OBJECT
    /*!
     * \brief Returns \c true if the user is enabled, otherwise returns \c false.
     *
     * \par Access methods
     * \li bool isEnabled() const
     *
     * \par Notifier signal
     * \li void enabledChanged(bool enabled)
     */
    Q_PROPERTY(bool enabled READ isEnabled NOTIFY enabledChanged)
    /*!
     * \brief This property holds the storage location of the user’s data on ther server.
     *
     * \par Access methods
     * \li QString storageLocation() const
     *
     * \par Notifier signal
     * \li void storageLocationChanged(const QString &storageLocation)
     */
    Q_PROPERTY(QString storageLocation READ storageLocation NOTIFY storageLocationChanged)
    /*!
     * \brief This property holds the id/user name of the user.
     *
     * \par Access methods
     * \li QString id() const
     *
     * \par Notifier signal
     * \li void idChanged(const QString &id)
     */
    Q_PROPERTY(QString id READ id NOTIFY idChanged)
    /*!
     * \brief This property holds the date and time the user logged in the last time.
     *
     * The datetime ist store in UTC.
     *
     * \par Access methods
     * \li QDateTime() lastLogin() const
     *
     * \par Notifier signal
     * \li void lastLoginChanged(const QDateTime & lastLogin)
     */
    Q_PROPERTY(QDateTime lastLogin READ lastLogin NOTIFY lastLoginChanged)
    /*!
     * \brief This property holds the backend the user is stored in.
     *
     * \par Access methods
     * \li QString backend() const
     *
     * \par Notifier signal
     * \li void backendChanged(const QString &backend)
     */
    Q_PROPERTY(QString backend READ backend NOTIFY backendChanged)
    /*!
     * \brief This property holds a list of groups the user is admin of.
     *
     * \par Access methods
     * \li QStringList subadmin() const
     *
     * \par Notifier signal
     * \li void subadminChanged(const QStringList &subadmin)
     */
    Q_PROPERTY(QStringList subadmin READ subadmin NOTIFY subadminChanged)
    /*!
     * \brief This property holds information about the user’s quota.
     *
     * \par Access methods
     * \li Quota quota() const
     *
     * \li void quotaChanged(const Wolkanlin::Quota &quota)
     */
    Q_PROPERTY(Wolkanlin::Quota quota READ quota NOTIFY quotaChanged)
    /*!
     * \brief This property holds the user’s email address.
     *
     * \par Access methods
     * \li QString email() const
     *
     * \par Notifier signal
     * \li void emailChanged(const QString &email)
     */
    Q_PROPERTY(QString email READ email NOTIFY emailChanged)
    /*!
     * \brief This property holds the user’s display name.
     *
     * \par Access methods
     * \li QString displayname() const
     *
     * \par Notifier signal
     * \li void displaynameChanged(const QString &displayname)
     */
    Q_PROPERTY(QString displayname READ displayname NOTIFY displaynameChanged)
    /*!
     * \brief This property holds the user’s phone number.
     *
     * \par Access methods
     * \li QString phone() const
     *
     * \par Notifier signal
     * \li void phoneChanged(const QString &phone)
     */
    Q_PROPERTY(QString phone READ phone NOTIFY phoneChanged)
    /*!
     * \brief This property holds the user’s address.
     *
     * \par Access methods
     * \li QString address() const
     *
     * \par Notifier signal
     * \li void addressChanged(const QString &address)
     */
    Q_PROPERTY(QString address READ address NOTIFY addressChanged)
    /*!
     * \brief This property holds the user’s website URL.
     *
     * \par Access methods
     * \li QUrl website() const
     *
     * \par Notifier signal
     * \li void websiteChanged(const QUrl &website)
     */
    Q_PROPERTY(QUrl website READ website NOTIFY websiteChanged)
    /*!
     * \brief This property holds the user’s twitter user name.
     *
     * \par Access methods
     * \li QString twitter() const
     *
     * \par Notifier signal
     * \li void twitterChanged(const QString &twitter)
     */
    Q_PROPERTY(QString twitter READ twitter NOTIFY twitterChanged)
    /*!
     * \brief This property holds a list of groups the user is member of.
     *
     * \par Access methods
     * \li QStringList groups() const
     *
     * \par Notifier signal
     * \li void groupsChanged(const QStringList &groups)
     */
    Q_PROPERTY(QStringList groups READ groups NOTIFY groupsChanged)
    /*!
     * \brief This property holds the language the user uses as BCP 47 language tag.
     *
     * The language means the translation of texts. For the used locale scheme, like
     * start of the week etc., see \link User::locale locale\endlink.
     *
     * \par Access methods
     * \li QString language() const
     *
     * \par Notifier signal
     * \li void languageChanged(const QString &language)
     */
    Q_PROPERTY(QString language READ language NOTIFY languageChanged)
    /*!
     * \brief This property holds the locale scheme the user uses as BCP 47 language tag.
     *
     * The locale scheme contains settings like start of the week etc. For the text translations
     * language see \link User::language language\endlink.
     *
     * \par Access methods
     * \li QString locale() const
     *
     * \par Notifier signal
     * \li void localeChanged(const QString &locale)
     */
    Q_PROPERTY(QString locale READ locale NOTIFY localeChanged)
    /*!
     * \brief This property holds the capabilities the user has on the backend.
     *
     * \par Access methods
     * \li Capabilities backendCapabilities() const
     *
     * \par Notifier signal
     * \li void backendCapabilitiesChanged(Wolkanlin::User::Capabilities backendCapabilities)
     */
    Q_PROPERTY(Wolkanlin::User::Capabilities backendCapabilities READ backendCapabilities NOTIFY backendCapabilitiesChanged)
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
     * \brief This enum describes the capabilities the user has on the backend.
     *
     * They are used in the Capabilities flags.
     *
     * \sa User::backendCapabilities
     */
    enum Capability : quint32 {
        CanSetDisplayName   = 0x0001,   /**< the user can change his display name */
        CanSetPassword      = 0x0002    /**< the user can change his password */
    };
    Q_DECLARE_FLAGS(Capabilities, Capability)
    Q_FLAG(Capabilities)

    /*!
     * \brief Constructs a new empty %User object with the given \a parent.
     *
     * isEmpty() will return \c true.
     */
    explicit User(QObject *parent = nullptr);
    /*!
     * \brief Destroys the %User object.
     */
    ~User() override;

    /*!
     * \brief Getter function for the \link User::enabled enabled\endlink property.
     * \sa void enabledChanged()
     */
    bool isEnabled() const;
    /*!
     * \brief Getter function for the \link User::storageLocation storageLocation\endlink property.
     * \sa void enabledChanged()
     */
    QString storageLocation() const;
    /*!
     * \brief Getter function for the \link User::id id\endlink property.
     * \sa void idChanged()
     */
    QString id() const;
    /*!
     * \brief Getter function for the \link User::lastLogin lastLogin\endlink property.
     * \sa void lastLoginChanged()
     */
    QDateTime lastLogin() const;
    /*!
     * \brief Getter function for the \link User::backend backend\endlink property.
     * \sa void backendChanged()
     */
    QString backend() const;
    /*!
     * \brief Getter function for the \link User::subadmin subadmin\endlink property.
     * \sa void quotaChanged()
     */
    QStringList subadmin() const;
    /*!
     * \brief Getter function for the \link User::quota quota\endlink property.
     * \sa void quotaChanged()
     */
    Quota quota() const;
    /*!
     * \brief Getter function for the \link User::email email\endlink property.
     * \sa void emailChanged()
     */
    QString email() const;
    /*!
     * \brief Getter function for the \link User::displayname displayname\endlink property.
     * \sa void displaynameChanged()
     */
    QString displayname() const;
    /*!
     * \brief Getter function for the \link User::phone phone\endlink property.
     * \sa void phoneChanged()
     */
    QString phone() const;
    /*!
     * \brief Getter function for the \link User::address address\endlink property.
     * \sa void addressChanged()
     */
    QString address() const;
    /*!
     * \brief Getter function for the \link User::website website\endlink property.
     * \sa void websiteChanged()
     */
    QUrl website() const;
    /*!
     * \brief Getter function for the \link User::twitter twitter\endlink property.
     * \sa void twitterChanged()
     */
    QString twitter() const;
    /*!
     * \brief Getter function for the \link User::groups groups\endlink property.
     * \sa void groupsChanged()
     */
    QStringList groups() const;
    /*!
     * \brief Getter function for the \link User::language language\endlink property.
     * \sa void languageChanged()
     */
    QString language() const;
    /*!
     * \brief Getter function for the \link User::locale locale\endlink property.
     * \sa void localeChanged()
     */
    QString locale() const;
    /*!
     * \brief Getter function for the \link User::backendCapabilities backendCapabilities\endlink property.
     * \sa void backendCapabilitiesChanged()
     */
    Capabilities backendCapabilities() const;
    /*!
     * \brief Getter function for the \link User::isLoading isLoading\endlink property.
     * \sa void isLoadingChanged()
     */
    bool isLoading() const;

    /*!
     * \brief Returns \c true if the %User object is empty, otherwise returns \c false.
     *
     * The %User object is empty, if no \link User::id id\endlink has been set.
     */
    bool isEmpty() const;

    /*!
     * \brief Convertes the %User object to a JSON object where the property names are the keys.
     *
     * See also the <A href="#user-json-example">JSON representation example</A> to learn more about the
     * created JSON object structure. Only the content of the “data“ key will be created.
     */
    QJsonObject toJson() const;

    /*!
     * \brief Creates a new %User object with the given \a parent from \a json.
     *
     * Will convert the \a json into a JSON object at first. If the \a json is empty
     * or does not contain an object, a pointer to an empty %ServerStatus object will be returned.
     * See also the <A href="#user-json-example">JSON representation example</A> to learn
     * more about the expected JSON object structure.
     */
    static User *fromJson(const QJsonDocument &json, QObject *parent = nullptr);
    /*!
     * \brief Creates a new %User object with the given \a parent from \a json.
     *
     * Will read the data from a \a json object as describe in the <A href="#user-json-example">JSON representation example</A>.
     * Can read from the root object as well as from the “ocs“ and “data“ keys content.
     * If \a json is empty, a pointer to an empty %User object will be returned.
     */
    static User *fromJson(const QJsonObject &json, QObject *parent = nullptr);

    /*!
     * \brief Get data from the Nextcloud server for the user identified by \a id.
     *
     * This will use GetUserJob internally to get the data and update the properties according
     * to the result. You can connect to the finished() signal to handle the data after getting it.
     * To handle errors, connect to the failed() signal. While the request is running,
     * \link User::isLoading isLoading\endlink returns \c true.
     *
     * By default, the request will be performed asynchronously. Set \a async to \c false to get
     * the data synchronously. If you do not want to use the Wolkanlin::defaultConfiguration() you
     * can set a custom \a config.
     *
     * If \a async is set to \c true (the default), this function will always return \c true. If
     * \a async is set to \c false, \c true will be returned if the request succeeded, otherwise
     * \c false will be returned.
     *
     * \include user-get-function.cpp
     */
    bool get(const QString &id, bool async = true, AbstractConfiguration *config = nullptr);

Q_SIGNALS:
    /*!
     * \brief Notifier signal for the \link User::enabled enabled\endlink property.
     * \sa isEnabled()
     */
    void enabledChanged(bool enabled);
    /*!
     * \brief Notifier signal for the \link User::storageLocation storageLocation\endlink property.
     * \sa storageLocation()
     */
    void storageLocationChanged(const QString &storageLocation);
    /*!
     * \brief Notifier signal for the \link User::id id\endlink property.
     * \sa id()
     */
    void idChanged(const QString &id);
    /*!
     * \brief Notifier signal for the \link User::lastLogin lastLogin\endlink property.
     * \sa lastLogin()
     */
    void lastLoginChanged(const QDateTime &lastLogin);
    /*!
     * \brief Notifier signal for the \link User::backend backend\endlink property.
     * \sa backend()
     */
    void backendChanged(const QString &backend);
    /*!
     * \brief Notifier signal for the \link User::subadmin subadmin\endlink property.
     * \sa subadmin()
     */
    void subadminChanged(const QStringList &subadmin);
    /*!
     * \brief Notifier signal for the \link User::quota quota\endlink property.
     * \sa quota()
     */
    void quotaChanged(const Wolkanlin::Quota &quota);
    /*!
     * \brief Notifier signal for the \link User::email email\endlink property.
     * \sa email()
     */
    void emailChanged(const QString &email);
    /*!
     * \brief Notifier signal for the \link User::displayname displayname\endlink property.
     * \sa displayname()
     */
    void displaynameChanged(const QString &displayname);
    /*!
     * \brief Notifier signal for the \link User::phone phone\endlink property.
     * \sa phone()
     */
    void phoneChanged(const QString &phone);
    /*!
     * \brief Notifier signal for the \link User::address address\endlink property.
     * \sa address()
     */
    void addressChanged(const QString &address);
    /*!
     * \brief Notifier signal for the \link User::website website\endlink property.
     * \sa website()
     */
    void websiteChanged(const QUrl &website);
    /*!
     * \brief Notifier signal for the \link User::storageLocation twitter\endlink property.
     * \sa twitter()
     */
    void twitterChanged(const QString &twitter);
    /*!
     * \brief Notifier signal for the \link User::groups groups\endlink property.
     * \sa groups()
     */
    void groupsChanged(const QStringList &groups);
    /*!
     * \brief Notifier signal for the \link User::language language\endlink property.
     * \sa language()
     */
    void languageChanged(const QString &language);
    /*!
     * \brief Notifier signal for the \link User::locale locale\endlink property.
     * \sa locale()
     */
    void localeChanged(const QString &locale);
    /*!
     * \brief Notifier signal for the \link User::backendCapabilities backendCapabilities\endlink property.
     * \sa backendCapabilities()
     */
    void backendCapabilitiesChanged(Wolkanlin::User::Capabilities backendCapabilities);
    /*!
     * \brief Notifier signal for the \link User::isLoading isLoading\endlink property.
     * \sa isLoading()
     */
    void isLoadingChanged(bool isLoading);

    /*!
     * \brief Emitted when getting user meta data succeeded.
     *
     * When using get(), this will be emitted after the data has been set to the object.
     *
     * \sa failed()
     */
    void finished();

    /*!
     * \brief Emitted when getting user meta data failed.
     *
     * When using get(), this will be emitted after requesting the data failed.
     *
     * \sa finished()
     */
    void failed(int error, const QString &errorString);

private:
    const std::unique_ptr<UserPrivate> wl_ptr;

    friend QDataStream &operator>>(QDataStream &stream, User &user);

    Q_DECLARE_PRIVATE_D(wl_ptr, User)
    Q_DISABLE_COPY(User)
};

/*!
 * \relates Wolkanlin::User
 * \brief Reads a %User from \a stream and stores it to \a user.
 */
WOLKANLIN_LIBRARY QDataStream &operator>>(QDataStream &stream, User &user);

}

/*!
 * \relates Wolkanlin::User
 * \brief Writes the \a user to the \a dbg stream and returns the stream.
 */
WOLKANLIN_LIBRARY QDebug operator<<(QDebug dbg, const Wolkanlin::User &user);

/*!
 * \relates Wolkanlin::User
 * \brief Writes \a user to the \a stream.
 */
WOLKANLIN_LIBRARY QDataStream &operator<<(QDataStream &stream, const Wolkanlin::User &user);

Q_DECLARE_OPERATORS_FOR_FLAGS(Wolkanlin::User::Capabilities)

#endif // WOLKANLIN_USER_H
