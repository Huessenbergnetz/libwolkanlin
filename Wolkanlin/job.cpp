/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "job_p.h"
#include "logging.h"
#include "abstractnamfactory.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QReadWriteLock>
#include <QGlobalStatic>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>

#if defined(QT_DEBUG)
Q_LOGGING_CATEGORY(wlCore, "wolkanlin.core")
#else
Q_LOGGING_CATEGORY(wlCore, "wolkanlin.core", QtInfoMsg)
#endif

using namespace Wolkanlin;

class DefaultValues
{
public:
    mutable QReadWriteLock lock;

    AbstractConfiguration *configuration() const
    {
        return m_configuration;
    }

    void setConfiguration(AbstractConfiguration *config)
    {
        m_configuration = config;
    }

    AbstractNamFactory *namFactory() const
    {
        return m_namFactory;
    }

    void setNamFactory(AbstractNamFactory *factory)
    {
        m_namFactory = factory;
    }

private:
    AbstractConfiguration *m_configuration = nullptr;
    AbstractNamFactory *m_namFactory = nullptr;
};
Q_GLOBAL_STATIC(DefaultValues, defVals)

AbstractConfiguration *Wolkanlin::defaultConfiguration()
{
    const DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    defs->lock.lockForRead();
    AbstractConfiguration *config = defs->configuration();
    defs->lock.unlock();

    return config;
}

void Wolkanlin::setDefaultConfiguration(AbstractConfiguration *configuration)
{
    DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    QWriteLocker locker(&defs->lock);
    qCDebug(wlCore) << "Setting defaultConfiguration to" << configuration;
    defs->setConfiguration(configuration);
}

AbstractNamFactory *Wolkanlin::networkAccessManagerFactory()
{
    const DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    defs->lock.lockForRead();
    AbstractNamFactory *factory = defs->namFactory();
    defs->lock.unlock();

    return factory;
}

void Wolkanlin::setNetworkAccessManagerFactory(AbstractNamFactory *factory)
{
    DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    QWriteLocker locker(&defs->lock);
    qCDebug(wlCore) << "Setting networkAccessManagerFactory to" << factory;
    defs->setNamFactory(factory);
}

JobPrivate::JobPrivate(Job *q)
    : q_ptr(q)
{

}

JobPrivate::~JobPrivate() = default;

void JobPrivate::handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    if (configuration->ignoreSslErrors()) {
        if (wlCore().isWarningEnabled()) {
            for (const QSslError &e : errors) {
                qCWarning(wlCore) << "Ignoring SSL error:" << e.errorString();
            }
        }
        reply->ignoreSslErrors();
    } else {
        Q_Q(Job);
        q->setError(SslError);
        if (!errors.empty()) {
            q->setErrorText(errors.first().errorString());
        } else {
            //: Error message
            //% "Can not perform API request. An unknown SSL error has occured."
            q->setErrorText(qtTrId("libwolkanlin-error-unknown-ssl"));
        }
        qCCritical(wlCore) << "SSL error:" << errors.first().errorString();
        reply->abort();
    }
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
void JobPrivate::requestTimedOut()
{
    Q_Q(Job);
    QNetworkReply *nr = reply;
    reply = nullptr;
    delete nr;

    q->setError(RequestTimedOut);
    q->setErrorText(QString::number(requestTimeout));
    q->emitResult();
}
#endif

void JobPrivate::requestFinished()
{
    Q_Q(Job);

    //: Job info message to display state information
    //% "Checking reply"
    Q_EMIT q->infoMessage(q, qtTrId("libwolkanlin-info-msg-req-checking"));
    qCDebug(wlCore) << "Request finished, checking reply.";
    qCDebug(wlCore) << "HTTP status code:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    const QByteArray replyData = reply->readAll();

    qCDebug(wlCore) << "Reply data:" << replyData;

#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
    if (Q_LIKELY(timeoutTimer && timeoutTimer->isActive())) {
        qCDebug(wlCore) << "Stopping request timeout timer with" << (d->timeoutTimer->remainingTime()/1000)) << "seconds left.";
        timeoutTimer->stop();
    }
#endif

    if (Q_LIKELY(reply->error() == QNetworkReply::NoError)) {
        if (checkOutput(replyData)) {
            Q_EMIT q->succeeded(jsonResult);
        } else {
            qCDebug(wlCore) << "Error code:" << q->error();
            Q_EMIT q->failed(q->error(), q->errorString());
        }
    } else {
        extractError();
        if (Q_UNLIKELY(q->error() == WJob::NoError)) {
            Q_EMIT q->succeeded(jsonResult);
        } else {
            qCDebug(wlCore) << "Error code:" << q->error();
            Q_EMIT q->failed(q->error(), q->errorString());
        }
    }

    reply->deleteLater();
    reply = nullptr;

    q->emitResult();
}

void JobPrivate::extractError()
{
    Q_ASSERT(reply);
    Q_Q(Job);
    if (q->error() == WJob::NoError) {
        const int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (httpStatusCode == 401) {
            qCCritical(wlCore) << "Authentication failed, please check your user name and password.";
            q->setError(AuthNFailed);
        } else if (httpStatusCode == 403) {
            qCCritical(wlCore) << "Authorization failed, you are not allowed to perform this request.";
            q->setError(AuthZFailed);
        } else {
            qCCritical(wlCore) << "Network error:" << reply->errorString();
            q->setError(NetworkError);
            q->setErrorText(reply->errorString());
        }
    }
}

void JobPrivate::emitError(int errorCode, const QString &errorText)
{
    Q_Q(Job);
    q->setError(errorCode);
    q->setErrorText(errorText);
    q->emitResult();
    Q_EMIT q->failed(errorCode, q->errorString());
}

QString JobPrivate::buildUrlPath() const
{
    return configuration->installPath();
}

QUrlQuery JobPrivate::buildUrlQuery() const
{
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("format"), QStringLiteral("json"));
    return query;
}

QMap<QByteArray, QByteArray> JobPrivate::buildRequestHeaders() const
{
    QMap<QByteArray, QByteArray> headers;
    headers.insert(QByteArrayLiteral("OCS-APIRequest"), QByteArrayLiteral("true"));
    headers.insert(QByteArrayLiteral("User-Agent"), configuration->userAgent().toLatin1());
    return headers;
}

std::pair<QByteArray, QByteArray> JobPrivate::buildPayload() const
{
    return std::make_pair(QByteArray(), QByteArray());
}

bool JobPrivate::checkInput()
{
    if (Q_UNLIKELY(configuration->host().isEmpty())) {
        emitError(MissingConfig);
        qCCritical(wlCore) << "Can not send request: missing host.";
        return false;
    }

    if (Q_UNLIKELY(requiresAuth && configuration->username().isEmpty())) {
        emitError(MissingUser);
        qCCritical(wlCore) << "Can not send request: missing username.";
        return false;
    }

    if (Q_UNLIKELY(requiresAuth && configuration->password().isEmpty())) {
        emitError(MissingPassword);
        qCCritical(wlCore) << "Can not send request: missing password.";
        return false;
    }

    return true;
}

bool JobPrivate::checkOutput(const QByteArray &data)
{
    Q_Q(Job);

    if (expectedContentType != ExpectedContentType::Empty && data.isEmpty()) {
        q->setError(EmptyReply);
        qCCritical(wlCore) << "Invalid reply: content expected, but reply is empty.";
        return false;
    }

    if (expectedContentType == ExpectedContentType::JsonArray || expectedContentType == ExpectedContentType::JsonObject) {
        QJsonParseError jsonError;
        jsonResult = QJsonDocument::fromJson(data, &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {
            q->setError(JsonParseError);
            q->setErrorText(jsonError.errorString());
            qCCritical(wlCore) << "Invalid JSON data in reply at offset" << jsonError.offset << ":" << jsonError.errorString();
            return false;
        }
    }

    if ((expectedContentType == ExpectedContentType::JsonArray || expectedContentType == ExpectedContentType::JsonObject) && (jsonResult.isNull() || jsonResult.isEmpty())) {
        q->setError(EmptyJson);
        qCCritical(wlCore) << "Invalid reply: content expected, but reply is empty.";
        return false;
    }

    if (expectedContentType == ExpectedContentType::JsonArray && !jsonResult.isArray()) {
        q->setError(WrongOutputType);
        qCCritical(wlCore) << "Invalid reply: JSON array expected, but got something different.";
        return false;
    }

    if (expectedContentType == ExpectedContentType::JsonObject && !jsonResult.isObject()) {
        q->setError(WrongOutputType);
        qCCritical(wlCore) << "Invalid reply: JSON object expected, but got something different.";
        return false;
    }

    if (expectedContentType == ExpectedContentType::Image) {
        const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
        if (!contentType.contains(QLatin1String("image"))) {
            q->setError(InvalidImageType);
            q->setErrorText(contentType);
            qCCritical(wlCore) << "Invalid reply: the returned content type" << contentType << "is not an image expected image type.";
            return false;
        }
    }

    if (expectedContentType == ExpectedContentType::JsonObject) {
        const QJsonObject o = jsonResult.object();
        if (o.contains(QStringLiteral("ocs"))) {
            const QJsonObject ocs = o.value(QStringLiteral("ocs")).toObject();
            if (ocs.contains(QStringLiteral("meta"))) {
                const QJsonObject meta = ocs.value(QStringLiteral("meta")).toObject();
                const QString status = meta.value(QStringLiteral("status")).toString();
                if (status.compare(QLatin1String("failure"), Qt::CaseInsensitive) == 0) {
                    statusCode = meta.value(QStringLiteral("statuscode")).toInt();
                    qCDebug(wlCore) << "JSON status code:" << statusCode;
                }
            }
        }
    }

    return true;
}

void JobPrivate::emitDescription()
{

}

Job::Job(QObject *parent)
    : WJob(parent), wl_ptr(new JobPrivate(this))
{

}

Job::Job(JobPrivate &dd, QObject *parent)
    : WJob(parent), wl_ptr(&dd)
{

}

Job::~Job() = default;

void Job::sendRequest()
{
    Q_D(Job);

    d->emitDescription();

    //: Job info message to display state information
    //% "Setting up request"
    Q_EMIT infoMessage(this, qtTrId("libwolkanlin-info-msg-req-setup"));
    qCDebug(wlCore) << "Setting up network request.";

    if (!d->configuration) {
        d->configuration = Wolkanlin::defaultConfiguration();
        if (d->configuration) {
            qCDebug(wlCore) << "Using default configuration" << d->configuration;
            Q_EMIT configurationChanged(d->configuration);
        } else {
            d->emitError(MissingConfig);
            qCCritical(wlCore) << "Can not send request: missing configuration.";
            return;
        }
    }

    if (Q_UNLIKELY(!d->checkInput())) {
        return;
    }

    QUrl url;
    if (d->configuration->useSsl()) {
        url.setScheme(QStringLiteral("https"));
    } else {
        url.setScheme(QStringLiteral("http"));
    }

    if (d->configuration->port() != 0) {
        url.setPort(d->configuration->port());
    }

    url.setHost(d->configuration->host());
    url.setPath(d->buildUrlPath());
    url.setQuery(d->buildUrlQuery());

    if (Q_UNLIKELY(!url.isValid())) {
        d->emitError(InvalidRequestUrl, url.toString());
        return;
    }

    if (!d->nam) {
        auto namf = Wolkanlin::networkAccessManagerFactory();
        if (namf) {
            d->nam = namf->create(this);
        } else {
            d->nam = new QNetworkAccessManager(this);
            qCDebug(wlCore) << "Using default created" << d->nam;
        }

        connect(d->nam, &QNetworkAccessManager::sslErrors, this, [d](QNetworkReply *reply, const QList<QSslError> &errors){
            d->handleSslErrors(reply, errors);
        });
    }

    QNetworkRequest nr(url);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    if (Q_LIKELY(d->requestTimeout > 0)) {
        nr.setTransferTimeout(static_cast<int>(d->requestTimeout) * 1000);
    }
#endif

    switch (d->expectedContentType) {
    case ExpectedContentType::JsonObject:
    case ExpectedContentType::JsonArray:
        nr.setRawHeader(QByteArrayLiteral("Accept"), QByteArrayLiteral("application/json"));
        break;
    case ExpectedContentType::Image:
        nr.setRawHeader(QByteArrayLiteral("Accept"), QByteArrayLiteral("image/png,image/gif,image/vnd.microsoft.icon"));
        break;
    case ExpectedContentType::Html:
        nr.setRawHeader(QByteArrayLiteral("Accept"), QByteArrayLiteral("text/html,application/xhtml+xml,application/xml"));
        break;
    case ExpectedContentType::Invalid:
        Q_ASSERT_X(false, "sending request", "invalid exepected content type");
        break;
    default:
        break;
    }

    const QMap<QByteArray, QByteArray> reqHeaders = d->buildRequestHeaders();
    if (!reqHeaders.empty()) {
        auto i = reqHeaders.constBegin();
        while (i != reqHeaders.constEnd()) {
            nr.setRawHeader(i.key(), i.value());
            ++i;
        }
    }

    const auto payload = d->buildPayload();

    if (!payload.second.isEmpty()) {
        nr.setRawHeader(QByteArrayLiteral("Content-Type"), payload.second);
    }

    if (d->requiresAuth) {
        const QString auth = d->configuration->username() + QLatin1Char(':') + d->configuration->password();
        const QByteArray authHeader = QByteArrayLiteral("Basic ") + auth.toUtf8().toBase64();
        nr.setRawHeader(QByteArrayLiteral("Authorization"), authHeader);
    }

    if (wlCore().isDebugEnabled()) {
        QString opName;
        switch(d->namOperation) {
        case NetworkOperation::Head:
            opName = QStringLiteral("HEAD");
            break;
        case NetworkOperation::Post:
            opName = QStringLiteral("POST");
            break;
        case NetworkOperation::Put:
            opName = QStringLiteral("PUT");
            break;
        case NetworkOperation::Delete:
            opName = QStringLiteral("DELETE");
            break;
        case NetworkOperation::Get:
            opName = QStringLiteral("GET");
            break;
        default:
            Q_ASSERT_X(false, "sending request", "invalid network operation");
            break;
        }
        qCDebug(wlCore) << "Start performing" << opName << "network operation.";
        qCDebug(wlCore) << "API URL:" << url;
        const auto rhl = nr.rawHeaderList();
        for (const QByteArray &h : rhl) {
            if (h == QByteArrayLiteral("Authorization")) {
                qCDebug(wlCore, "%s: **************", h.constData());
            } else {
                qCDebug(wlCore, "%s: %s", h.constData(), nr.rawHeader(h).constData());
            }
        }
        if (!payload.first.isEmpty()) {
            qCDebug(wlCore) << "Payload:" << payload.first;
        }
    }

#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
    if (Q_LIKELY(d->requestTimeout > 0)) {
        if (!d->timeoutTimer) {
            d->timeoutTimer = new QTimer(this);
            d->timeoutTimer->setSingleShot(true);
            d->timeoutTimer->setTimerType(Qt::VeryCoarseTimer);
            connect(d->timeoutTimer, &QTimer::timeout, this, [d](){
                d->requestTimedOut();
            });
        }
        d->timeoutTimer->start(static_cast<int>(d->requestTimeout) * 1000);
        qCDebug(wlCore) << "Started request timeout timer with" << d->requestTimeout << "seconds.";
    }
#endif

    //: Job info message to display state information
    //% "Sending request"
    Q_EMIT infoMessage(this, qtTrId("libwolkanlin-info-msg-req-send"));
    qCDebug(wlCore) << "Sending network request.";

    switch(d->namOperation) {
    case NetworkOperation::Head:
        d->reply = d->nam->head(nr);
        break;
    case NetworkOperation::Post:
        d->reply = d->nam->post(nr, payload.first);
        break;
    case NetworkOperation::Put:
        d->reply = d->nam->put(nr, payload.first);
        break;
    case NetworkOperation::Delete:
        d->reply = d->nam->deleteResource(nr);
        break;
    case NetworkOperation::Get:
        d->reply = d->nam->get(nr);
        break;
    default:
        Q_ASSERT_X(false, "sending request", "invalid network operation");
        break;
    }

    connect(d->reply, &QNetworkReply::finished, this, [d](){
        d->requestFinished();
    });
}

AbstractConfiguration* Job::configuration() const
{
    Q_D(const Job);
    return d->configuration;
}

void Job::setConfiguration(AbstractConfiguration *configuration)
{
    Q_D(Job);
    if (configuration != d->configuration) {
        qCDebug(wlCore) << "Changing configuration from" << d->configuration << "to" << configuration;
        d->configuration = configuration;
        Q_EMIT configurationChanged(d->configuration);
    }
}

QString Job::errorString() const
{
    switch (error()) {
    case MissingConfig:
        //: Error message
        //% "No configuration set."
        return qtTrId("libwolkanlin-error-missing-config");
    case MissingHost:
        //: Error message
        //% "Missing remote host name."
        return qtTrId("libwolkanlin-error-missing-host");
    case MissingUser:
        //: Error message
        //% "Missing username."
        return qtTrId("libwolkanlin-error-missing-user");
    case MissingPassword:
        //: Error message
        //% "Missing user password."
        return qtTrId("libwolkanlin-error-missing-password");
    case AuthNFailed:
        //: Error message
        //% "Authentication failed at the remote server, please check your username and password."
        return qtTrId("libwolkanlin-error-authn-failed");
    case AuthZFailed:
        //: Error message
        //% "Authorization failed, you are not allowed to perform this request."
        return qtTrId("libwolkanlin-error-authz-failed");
    case InvalidRequestUrl:
        //: Error message, %1 will be the invalid URL string.
        //% "The URL (%1) generated to perform the request is not valid, please check your input values."
        return qtTrId("libwolkanlin-error-invalid-req-url").arg(errorText());
    case RequestTimedOut:
        //: Error message, %1 will be the request timeout in seconds
        //% "The request timed out after %1 seconds."
        return qtTrId("libwolkanlin-error-request-timeout").arg(errorText());
    case JsonParseError:
        //: Error message, %1 will be the JSON parser error string.
        //% "Failed to parse the received JSON data: %1"
        return qtTrId("libwolkanlin-error-json-parser").arg(errorText());
    case WrongOutputType:
        //: Error message
        //% "Unexpected JSON type in received data."
        return qtTrId("libwolkanlin-error-invalid-output-type");
    case EmptyJson:
    case EmptyReply:
        //: Error message
        //% "Unexpected empty reply data."
        return qtTrId("libwolkanlin-error-empty-json");
    case InvalidImageType:
        //: Error message
        //% "Expected reply content of type image, but received content of type “%1”."
        return qtTrId("libwolkanlin-error-invalid-image-type").arg(errorText());
    case NetworkError:
    case SslError:
        return errorText();
    default:
        //: Error message
        //% "Sorry, but unfortunately an unknown error has occurred."
        return qtTrId("libwolkanlin-error-unknown");
    }
}

QJsonDocument Job::result() const
{
    Q_D(const Job);
    return d->jsonResult;
}

#include "moc_job.cpp"
