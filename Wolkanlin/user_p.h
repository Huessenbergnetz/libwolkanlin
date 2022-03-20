/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_USER_P_H
#define WOLKANLIN_USER_P_H

#include "user.h"

namespace Wolkanlin {

class UserPrivate
{
public:
    void setEnabled(bool _enabled);
    void setStorageLocation(const QString &_storageLocation);
    void setId(const QString &_id);
    void setLastLogin(const QDateTime &_lastLogin);
    void setBackend(const QString &_backend);
    void setSubadmin(const QStringList &_subadmin);
    void setQuota(const Quota &_quota);
    void setEmail(const QString &_email);
    void setDisplayname(const QString &_displayname);
    void setPhone(const QString &_phone);
    void setAddress(const QString &_address);
    void setWebsite(const QUrl &_website);
    void setTwitter(const QString &_twitter);
    void setGroups(const QStringList &_groups);
    void setLanguage(const QString &language);
    void setLocale(const QString &_locale);
    void setBackendCapabilities(User::Capabilities _backendCapabilties);

    void onGetUserSucceeded(const QJsonDocument &json);

    static QStringList jsonArrayToStringList(const QJsonArray &array);
    static QStringList jsonArrayToStringList(const QJsonValue &value);

    Quota quota;
    User *q_ptr = nullptr;
    QString storageLocation;
    QString id;
    QString backend;
    QString email;
    QString displayname;
    QString phone;
    QString address;
    QString twitter;
    QString language;
    QString locale;
    QStringList subadmin;
    QStringList groups;
    QUrl website;
    QDateTime lastLogin;
    User::Capabilities backendCapabilities;
    bool enabled = false;

private:
    Q_DECLARE_PUBLIC(User)
};

}

#endif // WOLKANLIN_USER_P_H
