/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "deleteapppasswordjob_p.h"
#include "logging.h"
#include <QTimer>

using namespace Wolkanlin;

DeleteAppPasswordJobPrivate::DeleteAppPasswordJobPrivate(DeleteAppPasswordJob *q)
    : JobPrivate(q)
{
    namOperation = NetworkOperation::Delete;
    expectedContentType = ExpectedContentType::JsonObject;
}

DeleteAppPasswordJobPrivate::~DeleteAppPasswordJobPrivate() = default;

QString DeleteAppPasswordJobPrivate::buildUrlPath() const
{
    const QString path = JobPrivate::buildUrlPath() + QLatin1String("/ocs/v2.php/core/apppassword");
    return path;
}

void DeleteAppPasswordJobPrivate::emitDescription()
{
    Q_Q(DeleteAppPasswordJob);
    //: Job title
    //% "Deleting application password"
    Q_EMIT q->description(q, qtTrId("libwolkanlin-job-desc-del-apppass-title"));
}

DeleteAppPasswordJob::DeleteAppPasswordJob(QObject *parent)
    : Job(* new DeleteAppPasswordJobPrivate(this), parent)
{

}

DeleteAppPasswordJob::~DeleteAppPasswordJob() = default;

void DeleteAppPasswordJob::start()
{
    QTimer::singleShot(0, this, &DeleteAppPasswordJob::sendRequest);
}

QString DeleteAppPasswordJob::errorString() const
{
    return Job::errorString();
}
