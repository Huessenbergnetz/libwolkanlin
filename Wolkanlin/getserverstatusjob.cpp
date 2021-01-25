/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "getserverstatusjob_p.h"
#include "logging.h"
#include <QTimer>

using namespace Wolkanlin;

GetServerStatusJobPrivate::GetServerStatusJobPrivate(GetServerStatusJob *q)
    : JobPrivate(q)
{
    namOperation = NetworkOperation::Get;
    expectedContentType = ExpectedContentType::JsonObject;
    requiresAuth = false;
}

GetServerStatusJobPrivate::~GetServerStatusJobPrivate() = default;

QString GetServerStatusJobPrivate::buildUrlPath() const
{
    const QString path = JobPrivate::buildUrlPath() + QLatin1String("/status.php");
    return path;
}

void GetServerStatusJobPrivate::emitDescription()
{
    Q_Q(GetServerStatusJob);
    //: Job title
    //% "Requesting server status information"
    Q_EMIT q->description(q, qtTrId("libwolkanlin-job-desc-get-server-status-title"));
}

QUrlQuery GetServerStatusJobPrivate::buildUrlQuery() const
{
    return QUrlQuery();
}

QMap<QByteArray, QByteArray> GetServerStatusJobPrivate::buildRequestHeaders() const
{
    auto map = JobPrivate::buildRequestHeaders();
    map.remove(QByteArrayLiteral("OCS-APIRequest"));
    return map;
}

GetServerStatusJob::GetServerStatusJob(QObject *parent)
    : Job(* new GetServerStatusJobPrivate(this), parent)
{

}

GetServerStatusJob::~GetServerStatusJob() = default;

void GetServerStatusJob::start()
{
    QTimer::singleShot(0, this, &GetServerStatusJob::sendRequest);
}
