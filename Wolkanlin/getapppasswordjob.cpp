/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "getapppasswordjob_p.h"
#include "logging.h"
#include <QTimer>
#include <QNetworkReply>

using namespace Wolkanlin;

GetAppPasswordJobPrivate::GetAppPasswordJobPrivate(GetAppPasswordJob *q)
    : JobPrivate(q)
{
    namOperation = NetworkOperation::Get;
    expectedContentType = ExpectedContentType::JsonObject;
}

GetAppPasswordJobPrivate::~GetAppPasswordJobPrivate() = default;

QString GetAppPasswordJobPrivate::buildUrlPath() const
{
    const QString path = JobPrivate::buildUrlPath() + QLatin1String("/ocs/v2.php/core/getapppassword");
    return path;
}

void GetAppPasswordJobPrivate::emitDescription()
{
    Q_Q(GetAppPasswordJob);
    //: Job title
    //% "Check for possibility to convert to application password"
    Q_EMIT q->description(q, qtTrId("libwolkanlin-job-desc-get-apppass-title"));
}

void GetAppPasswordJobPrivate::extractError()
{
    Q_ASSERT(reply);
    Q_Q(GetAppPasswordJob);
    if (q->error() == WJob::NoError) {
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 403) {
            qCDebug(wlCore) << "Application password is already in use";
            q->setError(AlreadyAppPassword);
        } else {
            JobPrivate::extractError();
        }
    }
}

GetAppPasswordJob::GetAppPasswordJob(QObject *parent)
    : Job(* new GetAppPasswordJobPrivate(this), parent)
{

}

GetAppPasswordJob::~GetAppPasswordJob() = default;

void GetAppPasswordJob::start()
{
    QTimer::singleShot(0, this, &GetAppPasswordJob::sendRequest);
}

QString GetAppPasswordJob::errorString() const
{
    if (error() == AlreadyAppPassword) {
        //: Error message if app pass conversion fails because an app password is already in use
        //% "The password used is already an application password."
        return qtTrId("libwolkanlin-error-get-apppass-already-converted");
    } else {
        return Job::errorString();
    }
}

