/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_GETWIPESTATUSJOB_H
#define WOLKANLIN_GETWIPESTATUSJOB_H

#include "wolkanlin_exports.h"
#include "job.h"
#include <QObject>

namespace Wolkanlin {

class GetWipeStatusJobPrivate;

/*!
 * \brief Obtains wipe status for a specific token from the Nextcloud server.
 *
 * Once you receive a 401 or 403 status response in another request, you should
 * use this job to obtain the wipe status from the Nextcloud server. This only
 * works if you have created a dedicated application token using the login flow.
 *
 * If the \link GetWipeStatusJob::token token\endlink property has not been set,
 * the value returned by AbstractConfiguration::password() will be used. After
 * setting the mandatory properties, call start() to perform the request.
 *
 * \par Mandatory properties
 * \li Job::configuration
 *
 * \par API method
 * POST
 *
 * \par API route
 * /index.php/core/wipe/check
 *
 * \par API docs
 * https://docs.nextcloud.com/server/latest/developer_manual/client_apis/RemoteWipe/index.html
 *
 * \headerfile "" <Wolkanlin/GetWipeStatusJob>
 */
class WOLKANLIN_LIBRARY GetWipeStatusJob : public Job
{
    Q_OBJECT
    /*!
     * \brief This property holds the token to obtain the wipe status for.
     *
     * If this property is not set, AbstractConfiguration::password() will be
     * used. By default, this property holds an empty string.
     *
     * \par Access functions
     * \li QString token() const
     * \li void setToken(const QString &token)
     *
     * \par Notifier signal
     * \li void tokenChanged(const QString &token)
     */
    Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged)
public:
    /*!
     * \brief Constructs a new %GetWipeStatusJob with the given \a parent.
     */
    explicit GetWipeStatusJob(QObject *parent = nullptr);

    /*!
     * \brief Constructs a new %GetWipeStatusJob with the given parameters.
     * \param token     the application to check the wipe status for
     * \param parent    pionter to a parent object
     */
    GetWipeStatusJob(const QString &token, QObject *parent = nullptr);

    /*!
     * \brief Destroys the %GetWipeStatusJob object.
     */
    ~GetWipeStatusJob() override;

    /*!
     * \brief Starts the job asnychronously.
     *
     * To start the job in a synchronous way, use exec().
     */
    void start() override;

    QString errorString() const override;

    /*!
     * \brief Getter function for the \link GetWipeStatusJob::token token\endlink property.
     * \sa setToken(), tokenChanged()
     */
    QString token() const;

    /*!
     * \brief Setter function for the \link GetWipeStatusJob::token token\endlink property.
     * \sa token(), tokenChanged()
     */
    void setToken(const QString &token);

Q_SIGNALS:
    /*!
     * \brief Notifier signal for the \link GetWipeStatusJob::token token\endlink property.
     * \sa setToken(), token()
     */
    void tokenChanged(const QString &token);

private:
    Q_DECLARE_PRIVATE_D(wl_ptr, GetWipeStatusJob)
    Q_DISABLE_COPY(GetWipeStatusJob)
};

}

#endif // WOLKANLIN_GETWIPESTATUSJOB_H
