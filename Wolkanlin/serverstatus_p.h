/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_SERVERSTATUS_P_H
#define WOLKANLIN_SERVERSTATUS_P_H

#include "serverstatus.h"

namespace Wolkanlin {

class ServerStatusPrivate
{
public:
    void setInstalled(bool _installed);
    void setMaintenance(bool _maintenance);
    void setNeedsDbUpgrade(bool _needsDbUpgrade);
    void setVersion(const QString &_version);
    void setVersionstring(const QString &_versionstring);
    void setEdition(const QString &_edition);
    void setProductname(const QString &_productname);
    void setExtendedSupport(bool _extendedSupport);
    void setIsLoading(bool _isLoading);

    void onGetServerStatusSucceeded(const QJsonDocument &json);

    ServerStatus *q_ptr = nullptr;
    QString version;
    QString versionstring;
    QString edition;
    QString productname;
    bool installed = false;
    bool maintenance = false;
    bool needsDbUpgrade = false;
    bool extendedSupport = false;
    bool isLoading = false;

private:
    Q_DECLARE_PUBLIC(ServerStatus)
};

}

#endif // WOLKANLIN_SERVERSTATUS_P_H
