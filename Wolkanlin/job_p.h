/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_JOB_P_H
#define WOLKANLIN_JOB_P_H

#include "job.h"
#include <QMap>
#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
#include <QTimer>
#endif
#include <QUrlQuery>
#include <QSslError>
#include <utility>

class QNetworkReply;
class QNetworkAccessManager;
#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
class QTimer;
#endif

namespace Wolkanlin {

enum class ExpectedContentType : qint8 {
    Invalid     = -1,
    Empty       = 0,
    JsonArray   = 1,
    JsonObject  = 2,
    Html        = 3,
    Image       = 4
};

enum class NetworkOperation : qint8 {
    Invalid = 0,
    Head    = 1,
    Get     = 2,
    Put     = 3,
    Post    = 4,
    Delete  = 5,
    Custom  = 6
};

class JobPrivate
{
public:
    JobPrivate(Job *q);
    virtual ~JobPrivate();

    QJsonDocument jsonResult;
    QNetworkAccessManager *nam = nullptr;
#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
    QTimer *timeoutTimer = nullptr;
#endif
    QNetworkReply *reply = nullptr;
    AbstractConfiguration *configuration = nullptr;
    NetworkOperation namOperation = NetworkOperation::Invalid;
    ExpectedContentType expectedContentType = ExpectedContentType::Invalid;
    int statusCode = 0;
    quint16 requestTimeout = 300;
    quint8 retryCount = 0;
    bool requiresAuth = true;
    bool checkForWipe = true;

    void handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);

#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
    void requestTimedOut();
#endif

    void requestFinished();

    void emitError(int errorCode, const QString &errorText = QString());

    virtual QString buildUrlPath() const;

    virtual QUrlQuery buildUrlQuery() const;

    virtual QMap<QByteArray, QByteArray> buildRequestHeaders() const;

    virtual std::pair<QByteArray, QByteArray> buildPayload() const;

    virtual bool checkInput();

    virtual bool checkOutput(const QByteArray &data);

    virtual void extractError();

    virtual void emitDescription();

protected:
    Job *q_ptr = nullptr;

private:
    Q_DISABLE_COPY(JobPrivate)
    Q_DECLARE_PUBLIC(Job)
};

}

#endif // WOLKANLIN_JOB_P_H
