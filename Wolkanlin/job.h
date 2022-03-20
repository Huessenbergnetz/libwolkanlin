/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_JOB_H
#define WOLKANLIN_JOB_H

#include "wolkanlin_exports.h"
#if defined(WOLKANLIN_WITH_KDE)
#include <KF5/KCoreAddons/KJob>
#else
#include "wjob.h"
#endif
#include "abstractconfiguration.h"
#include <QObject>
#include <QJsonDocument>
#include <memory>

namespace Wolkanlin {

#if defined(WOLKANLIN_WITH_KDE)
using WJob = KJob;
#endif

class JobPrivate;
class AbstractNamFactory;

/*!
 * \brief Error codes for Job.
 */
enum {
    MissingConfig = WJob::UserDefinedError + 1, /**< AbstractConfiguration is missing on this API class. */
    MissingHost,            /**< The remote host is missing. */
    MissingUser,            /**< The username is missing. */
    MissingPassword,        /**< The password is missing. */
    AuthNFailed,            /**< Authentication failed, check user name and password. */
    AuthZFailed,            /**< Authorization failed. */
    InvalidRequestUrl,      /**< The generated request URL is not valid. */
    RequestTimedOut,        /**< The request has been timed out. */
    JsonParseError,         /**< Failed to parse JSON response message. */
    WrongOutputType,        /**< The output type is not the expected one. */
    EmptyJson,              /**< The response data is empty but that was not expected. */
    EmptyReply,             /**< The response data is empty but that was not expected. */
    NetworkError,           /**< Network related error. */
    SslError,               /**< Error while performing SSL/TLS encryption. */
    InvalidImageType,       /**< The content type of the reply data is not an image type. */
    EmptyUser,              /**< No user defined to get data for. */
    NotFound,               /**< The requested data could not be found. */
    AlreadyAppPassword,     /**< The password in use is already an application password. */
    UnknownError            /**< An unknown error. */
};

/*!
 * \brief Base class for all API jobs.
 *
 * This class is used by all classes that perform API requests. It is not meant to be used
 * by itself. It provides basic properties and functions used by all classes that perform
 * API requests.
 */
class WOLKANLIN_LIBRARY Job : public WJob
{
    Q_OBJECT
    /*!
     * \brief Pointer to an object providing configuration data.
     *
     * This property holds a pointer to an object of a class derived from AbstractConfiguration
     * that provides configuration information for the API request. It especially provides the
     * remote host and username and password. See AbstractConfiguration for more information.
     *
     * You can also set a global default configuration via Wolkanlin::setDefaultConfiguration()
     * that will be used if no local configuration is available.
     *
     * \par Access functions
     * \li AbstractConfiguration *configuration() const
     * \li void setConfiguration(AbstractConfiguration *configuratoin)
     *
     * \par Notifier signal
     * \li void configurationChanged(AbstractConfiguration *configuration)
     */
    Q_PROPERTY(Wolkanlin::AbstractConfiguration *configuration READ configuration WRITE setConfiguration NOTIFY configurationChanged)
public:
    /*!
     * Destroys the %Job object.
     */
    ~Job() override;

    /*!
     * \brief Returns a human readable and translated error string.
     *
     * If WJob::error() returns not \c 0, an error has occured and the human readable
     * description can be returned by this function.
     */
    QString errorString() const override;

    /*!
     * \brief Getter function for the \link Job::configuration configuration\endlink property.
     * \sa setConfiguration(), configurationChanged()
     */
    AbstractConfiguration *configuration() const;

    /*!
     * \brief Setter function for the \link Job::configuration configuration\endlink property.
     * \sa configuration(), configurationChanged()
     */
    void setConfiguration(AbstractConfiguration *configuration);

    /*!
     * \brief Returns the API result after successful request.
     *
     * If the API request has been successful and WJob::error() returns \c 0, this
     * function returns the requested JSON data (if any).
     *
     * \sa succeeded()
     */
    QJsonDocument result() const;

protected:
    const std::unique_ptr<JobPrivate> wl_ptr;

    /*!
     * \brief Constructs a new %Job object with the given \a parent.
     */
    explicit Job(QObject *parent = nullptr);

    explicit Job(JobPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Performs basic checks and sets up and sends the request.
     *
     * This will be called in the reimplementation of WJob::start() by
     * class that are derived from %Job.
     */
    void sendRequest();

Q_SIGNALS:
    /*!
     * \brief Notifier signal for the \link Job::configuration configuration\endlink property.
     * \sa setConfiguration(), configuration()
     */
    void configurationChanged(Wolkanlin::AbstractConfiguration *configuration);

    /*!
     * \brief Emitted when the API request has been successful finished.
     *
     * This signal is triggered together with WJob::finished() and WJob::result()
     * if the API request was successful. \a json will contain the data requested
     * from the remote server.
     *
     * \sa result()
     */
    void succeeded(const QJsonDocument &json);

    /*!
     * \brief Emitted when the API request has been failed.
     *
     * This signal is triggered together with WJob::finished() and WJob::result()
     * if the API request has been failed. \a errorCode wile have the code of the
     * occured error, \a errorString will contain a human-readable error message.
     *
     * \sa error(), errorString()
     */
    void failed(int errorCode, const QString &errorString);

private:
    Q_DECLARE_PRIVATE_D(wl_ptr, Job)
    Q_DISABLE_COPY(Job)
};

/*!
 * \brief Sets a pointer to a global default \a configuration.
 * \sa Wolkanlin::defaultConfiguration()
 */
WOLKANLIN_LIBRARY void setDefaultConfiguration(AbstractConfiguration *configuration);

/*!
 * \brief Returns a pointer to a global default configuration.
 * \sa Wolkanlin::setDefaultConfiguration()
 */
WOLKANLIN_LIBRARY AbstractConfiguration* defaultConfiguration();

/*!
 * \brief Sets a pointer to a global network access manager \a factory.
 * \sa Wolkanlin::networkAccessManagerFactory()
 */
WOLKANLIN_LIBRARY void setNetworkAccessManagerFactory(AbstractNamFactory *factory);

/*!
 * \brief Returns a pointer to a global network access manager factory.
 * \sa Wolkanlin::setNetworkAccessManagerFactory()
 */
WOLKANLIN_LIBRARY AbstractNamFactory* networkAccessManagerFactory();

}

#endif // WOLKANLIN_JOB_H
