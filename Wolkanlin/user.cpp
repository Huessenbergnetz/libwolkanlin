/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "user_p.h"
#include "logging.h"
#include "abstractconfiguration.h"
#include "getuserjob.h"
#include <QDebug>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace Wolkanlin;

User::User(QObject *parent) : QObject(parent), wl_ptr(new UserPrivate)
{
    Q_D(User);
    d->q_ptr = this;
}

User::~User() = default;

bool User::isEnabled() const
{
    Q_D(const User);
    return d->enabled;
}

QString User::storageLocation() const
{
    Q_D(const User);
    return d->storageLocation;
}

QString User::id() const
{
    Q_D(const User);
    return d->id;
}

QDateTime User::lastLogin() const
{
    Q_D(const User);
    return d->lastLogin;
}

QString User::backend() const
{
    Q_D(const User);
    return d->backend;
}

QStringList User::subadmin() const
{
    Q_D(const User);
    return d->subadmin;
}

Quota User::quota() const
{
    Q_D(const User);
    return d->quota;
}

QString User::email() const
{
    Q_D(const User);
    return d->email;
}

QString User::displayname() const
{
    Q_D(const User);
    return d->displayname;
}

QString User::phone() const
{
    Q_D(const User);
    return d->phone;
}

QString User::address() const
{
    Q_D(const User);
    return d->address;
}

QUrl User::website() const
{
    Q_D(const User);
    return d->website;
}

QString User::twitter() const
{
    Q_D(const User);
    return d->twitter;
}

QStringList User::groups() const
{
    Q_D(const User);
    return d->groups;
}

QString User::language() const
{
    Q_D(const User);
    return d->language;
}

QString User::locale() const
{
    Q_D(const User);
    return d->locale;
}

User::Capabilities User::backendCapabilities() const
{
    Q_D(const User);
    return d->backendCapabilities;
}

bool User::isLoading() const
{
    Q_D(const User);
    return d->isLoading;
}

bool User::isEmpty() const
{
    Q_D(const User);
    return d->id.isEmpty();
}

QJsonObject User::toJson() const
{
    QJsonObject o;
    if (Q_LIKELY(!isEmpty())) {
        Q_D(const User);
        o.insert(QStringLiteral("enabled"), d->enabled);
        o.insert(QStringLiteral("storageLocation"), d->storageLocation);
        o.insert(QStringLiteral("id"), d->id);
        o.insert(QStringLiteral("lastLogin"), d->lastLogin.toMSecsSinceEpoch());
        o.insert(QStringLiteral("backend"), d->backend);
        o.insert(QStringLiteral("subadmin"), QJsonArray::fromStringList(d->subadmin));
        o.insert(QStringLiteral("quota"), d->quota.toJson());
        o.insert(QStringLiteral("email"), d->email);
        o.insert(QStringLiteral("displayname"), d->displayname);
        o.insert(QStringLiteral("phone"), d->phone);
        o.insert(QStringLiteral("address"), d->address);
        o.insert(QStringLiteral("website"), d->website.toString());
        o.insert(QStringLiteral("twitter"), d->twitter);
        o.insert(QStringLiteral("groups"), QJsonArray::fromStringList(d->groups));
        o.insert(QStringLiteral("language"), d->language);
        o.insert(QStringLiteral("locale"), d->locale);
        QJsonObject backendCaps;
        backendCaps.insert(QStringLiteral("setDisplayName"), d->backendCapabilities.testFlag(User::CanSetDisplayName));
        backendCaps.insert(QStringLiteral("setPassword"), d->backendCapabilities.testFlag(User::CanSetPassword));
        o.insert(QStringLiteral("backendCapabilities"), backendCaps);
    } else {
        qCWarning(wlCore) << "Wolkanlin::User is empty, created QJsonObject will be empty, too.";
    }
    return o;
}

User *User::fromJson(const QJsonDocument &json, QObject *parent)
{
    if (json.isObject()) {
        return User::fromJson(json.object(), parent);
    } else {
        qCWarning(wlCore) << "JSON document is not an object, creating empty Wolkanlin::User.";
        return new User(parent);
    }
}

User *User::fromJson(const QJsonObject &json, QObject *parent)
{

    if (json.isEmpty()) {
        qCWarning(wlCore) << "JSON object is empty, creating empty Wolkanlin::User object.";
        return new User(parent);
    }

    QJsonObject data;

    if (json.contains(QStringLiteral("data"))) {
        data = json.value(QStringLiteral("data")).toObject();
    } else if (json.contains(QStringLiteral("ocs"))) {
        data = json.value(QStringLiteral("ocs")).toObject().value(QStringLiteral("data")).toObject();
    } else {
        data = json;
    }

    const QString id = data.value(QStringLiteral("id")).toString();

    if (Q_UNLIKELY(id.isEmpty())) {
        qCWarning(wlCore) << "JSON does not contain a valid user id, creating empty Wolkandlin::User object.";
        return new User(parent);
    }

    auto user = new User(parent);
    auto d = user->d_func();
    d->enabled = data.value(QStringLiteral("enabled")).toBool();
    d->storageLocation = data.value(QStringLiteral("storageLocation")).toString();
    d->id = id;
    d->lastLogin = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(data.value(QStringLiteral("lastLogin")).toDouble()));
    d->backend = data.value(QStringLiteral("backend")).toString();
    d->subadmin = UserPrivate::jsonArrayToStringList(data.value(QStringLiteral("subadmin")));
    d->quota = Quota::fromJson(data.value(QStringLiteral("quota")).toObject());
    d->email = data.value(QStringLiteral("email")).toString();
    d->displayname = data.value(QStringLiteral("displayname")).toString();
    d->phone = data.value(QStringLiteral("phone")).toString();
    d->address = data.value(QStringLiteral("address")).toString();
    d->website = QUrl(data.value(QStringLiteral("website")).toString());
    d->twitter = data.value(QStringLiteral("twitter")).toString();
    d->groups = UserPrivate::jsonArrayToStringList(data.value(QStringLiteral("groups")));
    d->language = data.value(QStringLiteral("language")).toString();
    d->locale = data.value(QStringLiteral("locale")).toString();

    const QJsonObject backendCaps = data.value(QStringLiteral("backendCapabilities")).toObject();
    if (backendCaps.value(QStringLiteral("setDisplayName")).toBool()) {
        d->backendCapabilities |= User::CanSetDisplayName;
    }
    if (backendCaps.value(QStringLiteral("setPassword")).toBool()) {
        d->backendCapabilities |= User::CanSetPassword;
    }

    return user;
}

bool User::get(const QString &id, bool async, AbstractConfiguration *config)
{
    Q_D(User);

    d->setIsLoading(true);

    auto getUserJob = new GetUserJob(id, this);
    if (config) {
        getUserJob->setConfiguration(config);
    }

    if (async) {
        connect(getUserJob, &GetUserJob::succeeded, this, [d](const QJsonDocument &json) {
            d->onGetUserSucceeded(json);
        });
        connect(getUserJob, &GetUserJob::failed, this, &User::failed);
        connect(getUserJob, &GetUserJob::failed, this, [d](){
            d->setIsLoading(false);
        });
        getUserJob->start();
        return true;
    } else {
        if (Q_LIKELY(getUserJob->exec())) {
            d->onGetUserSucceeded(getUserJob->replyData());
            getUserJob->deleteLater();
            return true;
        } else {
            Q_EMIT failed(getUserJob->error(), getUserJob->errorString());
            d->setIsLoading(false);
            getUserJob->deleteLater();
            return false;
        }
    }
}

void UserPrivate::setEnabled(bool _enabled)
{
    if (enabled != _enabled) {
        qCDebug(wlCore) << "Changing enabled from" << enabled << "to" << _enabled;
        enabled = _enabled;
        Q_Q(User);
        Q_EMIT q->enabledChanged(enabled);
    }
}

void UserPrivate::setStorageLocation(const QString &_storageLocation)
{
    if (storageLocation != _storageLocation) {
        qCDebug(wlCore) << "Changing storageLocation from" << storageLocation << "to" << _storageLocation;
        storageLocation = _storageLocation;
        Q_Q(User);
        Q_EMIT q->storageLocationChanged(storageLocation);
    }
}

void UserPrivate::setId(const QString &_id)
{
    if (id != _id) {
        qCDebug(wlCore) << "Changing id from" << id << "to" << _id;
        id = _id;
        Q_Q(User);
        Q_EMIT q->idChanged(id);
    }
}

void UserPrivate::setLastLogin(const QDateTime &_lastLogin)
{
    if (lastLogin != _lastLogin) {
        qCDebug(wlCore) << "Changing lastLogin from" << lastLogin << "to" << _lastLogin;
        lastLogin = _lastLogin;
        Q_Q(User);
        Q_EMIT q->lastLoginChanged(lastLogin);
    }
}

void UserPrivate::setBackend(const QString &_backend)
{
    if (backend != _backend) {
        qCDebug(wlCore) << "Changing backend from" << backend << "to" << _backend;
        backend = _backend;
        Q_Q(User);
        Q_EMIT q->backendChanged(backend);
    }
}

void UserPrivate::setSubadmin(const QStringList &_subadmin)
{
    if (subadmin != _subadmin) {
        qCDebug(wlCore) << "Changing subadmin from" << subadmin << "to" << _subadmin;
        subadmin = _subadmin;
        Q_Q(User);
        Q_EMIT q->subadminChanged(subadmin);
    }
}

void UserPrivate::setQuota(const Quota &_quota)
{
    if (quota != _quota) {
        qCDebug(wlCore) << "Changing quota from" << quota << "to" << _quota;
        quota = _quota;
        Q_Q(User);
        Q_EMIT q->quotaChanged(quota);
    }
}

void UserPrivate::setEmail(const QString &_email)
{
    if (email != _email) {
        qCDebug(wlCore) << "Changnig email from" << email << "to" << _email;
        email = _email;
        Q_Q(User);
        Q_EMIT q->emailChanged(email);
    }
}

void UserPrivate::setDisplayname(const QString &_displayname)
{
    if (displayname != _displayname) {
        qCDebug(wlCore) << "Changing displayname from" << displayname << "to" << _displayname;
        displayname = _displayname;
        Q_Q(User);
        Q_EMIT q->displaynameChanged(displayname);
    }
}

void UserPrivate::setPhone(const QString &_phone)
{
    if (phone != _phone) {
        qCDebug(wlCore) << "Changing phone from" << phone << "to" << _phone;
        phone = _phone;
        Q_Q(User);
        Q_EMIT q->phoneChanged(phone);
    }
}

void UserPrivate::setAddress(const QString &_address)
{
    if (address != _address) {
        qCDebug(wlCore) << "Changing address from" << address << "to" << _address;
        address = _address;
        Q_Q(User);
        Q_EMIT q->addressChanged(address);
    }
}

void UserPrivate::setWebsite(const QUrl &_website)
{
    if (website != _website) {
        qCDebug(wlCore) << "Changing website from" << website << "to" << _website;
        website = _website;
        Q_Q(User);
        Q_EMIT q->websiteChanged(website);
    }
}

void UserPrivate::setTwitter(const QString &_twitter)
{
    if (twitter != _twitter) {
        qCDebug(wlCore) << "Changing twitter from" << twitter << "to" << _twitter;
        twitter = _twitter;
        Q_Q(User);
        Q_EMIT q->twitterChanged(twitter);
    }
}

void UserPrivate::setGroups(const QStringList &_groups)
{
    if (groups != _groups) {
        qCDebug(wlCore) << "Changing groups from" << groups << "to" << _groups;
        groups = _groups;
        Q_Q(User);
        Q_EMIT q->groupsChanged(groups);
    }
}

void UserPrivate::setLanguage(const QString &_language)
{
    if (language != _language) {
        qCDebug(wlCore) << "Changing language from" << language << "to" << _language;
        language = _language;
        Q_Q(User);
        Q_EMIT q->languageChanged(language);
    }
}

void UserPrivate::setLocale(const QString &_locale)
{
    if (locale != _locale) {
        qCDebug(wlCore) << "Changing locale from" << locale << "to" << _locale;
        locale = _locale;
        Q_Q(User);
        Q_EMIT q->localeChanged(locale);
    }
}

void UserPrivate::setBackendCapabilities(User::Capabilities _backendCapabilties)
{
    if (backendCapabilities != _backendCapabilties) {
        qCDebug(wlCore) << "Changing backendCapabilties from" << backendCapabilities << "to" << _backendCapabilties;
        backendCapabilities = _backendCapabilties;
        Q_Q(User);
        Q_EMIT q->backendCapabilitiesChanged(backendCapabilities);
    }
}

void UserPrivate::setIsLoading(bool _isLoading)
{
    if (isLoading != _isLoading) {
        qCDebug(wlCore) << "Changing isLoading from" << isLoading << "to" << _isLoading;
        isLoading = _isLoading;
        Q_Q(User);
        Q_EMIT q->isLoadingChanged(isLoading);
    }
}

void UserPrivate::onGetUserSucceeded(const QJsonDocument &json)
{
    const QJsonObject data = json.object().value(QStringLiteral("ocs")).toObject().value(QStringLiteral("data")).toObject();

    setEnabled(data.value(QStringLiteral("enabled")).toBool());
    setStorageLocation(data.value(QStringLiteral("storageLocation")).toString());
    setId(data.value(QStringLiteral("id")).toString());
    setLastLogin(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(data.value(QStringLiteral("lastLogin")).toDouble()), Qt::UTC));
    setBackend(data.value(QStringLiteral("backend")).toString());
    setSubadmin(jsonArrayToStringList(data.value(QStringLiteral("subadmin"))));
    setQuota(Quota::fromJson(data.value(QStringLiteral("quota")).toObject()));
    setEmail(data.value(QStringLiteral("email")).toString());
    setDisplayname(data.value(QStringLiteral("displayname")).toString());
    setPhone(data.value(QStringLiteral("phone")).toString());
    setAddress(data.value(QStringLiteral("address")).toString());
    setWebsite(QUrl(data.value(QStringLiteral("website")).toString()));
    setTwitter(data.value(QStringLiteral("twitter")).toString());
    setGroups(jsonArrayToStringList(data.value(QStringLiteral("groups"))));
    setLanguage(data.value(QStringLiteral("language")).toString());
    setLocale(data.value(QStringLiteral("locale")).toString());

    const QJsonObject backendCaps = data.value(QStringLiteral("backendCapabilities")).toObject();
    User::Capabilities caps;
    if (backendCaps.value(QStringLiteral("setDisplayName")).toBool()) {
        caps |= User::CanSetDisplayName;
    }
    if (backendCaps.value(QStringLiteral("setPassword")).toBool()) {
        caps |= User::CanSetPassword;
    }
    setBackendCapabilities(caps);

    setIsLoading(false);
    Q_Q(User);
    Q_EMIT q->finished();
}

QStringList UserPrivate::jsonArrayToStringList(const QJsonArray &array)
{
    QStringList list;
    if (!array.empty()) {
        list.reserve(array.size());
        for (const QJsonValue &v : array) {
            list << v.toString();
        }
    }
    return list;
}

QStringList UserPrivate::jsonArrayToStringList(const QJsonValue &value)
{
    return jsonArrayToStringList(value.toArray());
}

QDebug operator<<(QDebug dbg, const Wolkanlin::User &user)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "Wolkanlin::User(";
    dbg << "id: " << user.id();
    dbg << ", enabled: " << user.isEnabled();
    dbg << ", lastLogin: " << user.lastLogin();
    dbg << ", storageLocation: " << user.storageLocation();
    dbg << ", backend: " << user.backend();
    dbg << ", subadmin: " << user.subadmin();
    dbg << ", quota: " << user.quota();
    dbg << ", email: " << user.email();
    dbg << ", displayname: " << user.displayname();
    dbg << ", phone: " << user.phone();
    dbg << ", address: " << user.address();
    dbg << ", website: " << user.website();
    dbg << ", twitter: " << user.twitter();
    dbg << ", groups: " << user.groups();
    dbg << ", language: " << user.language();
    dbg << ", locale: " << user.locale();
    dbg << ", backendCapabilities: " << user.backendCapabilities();
    dbg << ')';
    return dbg.maybeSpace();
}

QDataStream &Wolkanlin::operator>>(QDataStream &stream, Wolkanlin::User &user)
{
    stream >> user.wl_ptr->id;
    stream >> user.wl_ptr->enabled;
    stream >> user.wl_ptr->lastLogin;
    stream >> user.wl_ptr->storageLocation;
    stream >> user.wl_ptr->backend;
    stream >> user.wl_ptr->subadmin;
    stream >> user.wl_ptr->quota;
    stream >> user.wl_ptr->email;
    stream >> user.wl_ptr->displayname;
    stream >> user.wl_ptr->phone;
    stream >> user.wl_ptr->address;
    stream >> user.wl_ptr->website;
    stream >> user.wl_ptr->twitter;
    stream >> user.wl_ptr->groups;
    stream >> user.wl_ptr->language;
    stream >> user.wl_ptr->locale;
    stream >> user.wl_ptr->backendCapabilities;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const Wolkanlin::User &user)
{
    stream << user.id()
           << user.isEnabled()
           << user.lastLogin()
           << user.storageLocation()
           << user.backend()
           << user.subadmin()
           << user.quota()
           << user.email()
           << user.displayname()
           << user.phone()
           << user.address()
           << user.website()
           << user.twitter()
           << user.groups()
           << user.language()
           << user.locale()
           << user.backendCapabilities();
    return stream;
}

#include "moc_user.cpp"
