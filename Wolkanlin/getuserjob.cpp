/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "getuserjob_p.h"
#include "logging.h"
#include <QTimer>

using namespace Wolkanlin;

GetUserJobPrivate::GetUserJobPrivate(GetUserJob *q)
    : JobPrivate(q)
{
    namOperation = NetworkOperation::Get;
    expectedContentType = ExpectedContentType::JsonObject;
}

GetUserJobPrivate::~GetUserJobPrivate() = default;

QString GetUserJobPrivate::buildUrlPath() const
{
    const QString path = JobPrivate::buildUrlPath() + QLatin1String("/ocs/v1.php/cloud/users/") + id;
    return path;
}

bool GetUserJobPrivate::checkInput()
{
    if (Q_UNLIKELY(!JobPrivate::checkInput())) {
        return false;
    }

    if (Q_UNLIKELY(id.isEmpty())) {
        emitError(EmptyUser);
        qCCritical(wlCore) << "Can not get user data with empty user name/id.";
        return false;
    }

    return true;
}

void GetUserJobPrivate::emitDescription()
{
    Q_Q(GetUserJob);

    //: Job title
    //% "Requesting user data"
    const QString _title = qtTrId("libwolkanlin-job-desc-get-user-title");

    //: Job description field name, means the user name metadata should be requested for
    //% "User name"
    const QString _userField = qtTrId("libwolkanlin-job-desc-get-user-field1");

    Q_EMIT q->description(q, _title, qMakePair(_userField, id));
}

bool GetUserJobPrivate::checkOutput(const QByteArray &data)
{
    if (Q_UNLIKELY(!JobPrivate::checkOutput(data))) {
        return false;
    }

    Q_Q(GetUserJob);

    if (statusCode > 0) {
        if (statusCode == 404) {
            q->setError(NotFound);
            q->setErrorText(id);
        } else {
            q->setError(UnknownError);
        }
        return false;
    }

    return true;
}

GetUserJob::GetUserJob(QObject *parent)
    : Job(* new GetUserJobPrivate(this), parent)
{

}

GetUserJob::GetUserJob(const QString &id, QObject *parent)
    : Job(* new GetUserJobPrivate(this), parent)
{
    Q_D(GetUserJob);
    d->id = id;
}

GetUserJob::~GetUserJob() = default;

void GetUserJob::start()
{
    QTimer::singleShot(0, this, &GetUserJob::sendRequest);
}

QString GetUserJob::errorString() const
{
    if (error() == EmptyUser) {
        //: Error message if the user name has not been set.
        //% "Can not get user data for empty user name."
        return qtTrId("libwolkanlin-error-get-user-empty-id");
    } else if (error() == NotFound) {
        //: Error message if the user was not found, %1 will be replaced by the user name.
        //% "Cannot get user information for %1. The user was not found."
        return qtTrId("libwolkanlin-error-get-user-not-found").arg(errorText());
    } else {
        return Job::errorString();
    }
}

QString GetUserJob::id() const
{
    Q_D(const GetUserJob);
    return d->id;
}

void GetUserJob::setId(const QString &id)
{
    Q_D(GetUserJob);
    if (d->id != id) {
        qCDebug(wlCore) << "Changing id from" << d->id << "to" << id;
        d->id = id;
        Q_EMIT idChanged(d->id);
    }
}

#include "moc_getuserjob.cpp"
