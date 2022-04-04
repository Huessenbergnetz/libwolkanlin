/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "getuserlistjob_p.h"
#include <QTimer>

using namespace Wolkanlin;

GetUserListJobPrivate::GetUserListJobPrivate(GetUserListJob *q)
    : JobPrivate(q)
{
    namOperation= NetworkOperation::Get;
    expectedContentType = ExpectedContentType::JsonObject;
}

GetUserListJobPrivate::~GetUserListJobPrivate() = default;

QString GetUserListJobPrivate::buildUrlPath() const
{
    const QString path = JobPrivate::buildUrlPath() + QLatin1String("/ocs/v1.php/cloud/users");
    return path;
}

void GetUserListJobPrivate::emitDescription()
{
    Q_Q(GetUserListJob);

    //: Job title
    //% "Requesting user list"
    const QString _title = qtTrId("libwolkanlin-job-desc-get-users-title");

    Q_EMIT q->description(q, _title);
}

GetUserListJob::GetUserListJob(QObject *parent)
    : Job(* new GetUserListJobPrivate(this), parent)
{

}

GetUserListJob::~GetUserListJob() = default;

void GetUserListJob::start()
{
    QTimer::singleShot(0, this, &GetUserListJob::sendRequest);
}

#include "moc_getuserlistjob.cpp"
