/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "getwipestatusjob_p.h"
#include "logging.h"
#include <QTimer>
#include <QNetworkReply>

using namespace Wolkanlin;

GetWipeStatusJobPrivate::GetWipeStatusJobPrivate(GetWipeStatusJob *q)
    : JobPrivate(q)
{
    namOperation = NetworkOperation::Post;
    expectedContentType = ExpectedContentType::JsonObject;
    requiresAuth = false;
}

GetWipeStatusJobPrivate::~GetWipeStatusJobPrivate() = default;

QString GetWipeStatusJobPrivate::buildUrlPath() const
{
    const QString path = JobPrivate::buildUrlPath() + QLatin1String("/index.php/core/wipe/check");
    return path;
}

void GetWipeStatusJobPrivate::emitDescription()
{
    Q_Q(GetWipeStatusJob);
    //: Job title
    //% "Requesting wipe status"
    Q_EMIT q->description(q, qtTrId("libwolkanlin-job-desc-get-wipe-status-title"));
}

QUrlQuery GetWipeStatusJobPrivate::buildUrlQuery() const
{
    return QUrlQuery();
}

QMap<QByteArray, QByteArray> GetWipeStatusJobPrivate::buildRequestHeaders() const
{
    auto map = JobPrivate::buildRequestHeaders();
    map.remove(QByteArrayLiteral("OCS-APIRequest"));
    return map;
}

std::pair<QByteArray, QByteArray> GetWipeStatusJobPrivate::buildPayload() const
{
    const QByteArray _token = !token.isEmpty() ? token.toUtf8() : configuration->password().toUtf8();
    const QByteArray tokenKey = QByteArrayLiteral("token=")  + _token;
    return std::make_pair(tokenKey, QByteArrayLiteral("application/x-www-form-urlencoded"));
}

bool GetWipeStatusJobPrivate::checkInput()
{
    if (Q_UNLIKELY(!JobPrivate::checkInput())) {
        return false;
    }

    if (Q_UNLIKELY(token.isEmpty() && configuration->password().isEmpty())) {
        emitError(MissingPassword);
        qCCritical(wlCore) << "Can not get wipe status with empty application password/token.";
        return false;
    }

    return true;
}

void GetWipeStatusJobPrivate::extractError()
{
    Q_ASSERT(reply);
    Q_Q(GetWipeStatusJob);
    if (q->error() == WJob::NoError) {
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 404) {
            JobPrivate::extractError();
        }
    }
}

GetWipeStatusJob::GetWipeStatusJob(QObject *parent)
    : Job(* new GetWipeStatusJobPrivate(this), parent)
{

}

GetWipeStatusJob::GetWipeStatusJob(const QString &token, QObject *parent)
    : Job(* new GetWipeStatusJobPrivate(this), parent)
{
    Q_D(GetWipeStatusJob);
    d->token = token;
}

GetWipeStatusJob::~GetWipeStatusJob() = default;

void GetWipeStatusJob::start()
{
    QTimer::singleShot(0, this, &GetWipeStatusJob::sendRequest);
}

QString GetWipeStatusJob::errorString() const
{
    if (error() == MissingPassword) {
        //: Error message if the password/token has not been set.
        //% "Can not get wipe status with empty application password/token."
        return qtTrId("libwolkanlin-error-get-wipe-status-missing-token");
    } else {
        return Job::errorString();
    }
}

QString GetWipeStatusJob::token() const
{
    Q_D(const GetWipeStatusJob);
    return d->token;
}

void GetWipeStatusJob::setToken(const QString &token)
{
    Q_D(GetWipeStatusJob);
    if (d->token != token) {
        qCDebug(wlCore) << "Changing token from" << d->token << "to" << token;
        d->token = token;
        Q_EMIT tokenChanged(d->token);
    }
}

#include "moc_getwipestatusjob.cpp"
