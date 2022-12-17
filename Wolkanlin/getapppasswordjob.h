/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_GETAPPPASSWORDJOB_H
#define WOLKANLIN_GETAPPPASSWORDJOB_H

#include "wolkanlin_export.h"
#include "job.h"
#include <QObject>

namespace Wolkanlin {

class GetAppPasswordJobPrivate;

/*!
 * \brief Converts “normal” passwords to application passwords.
 *
 * Nextcloud’s OCS API supports the automatic upgrade from “normal“ passwords to
 * application specific passwords. This class implements this API call. Running
 * start() or exec() will send a GET request to the /ocs/v2.php/core/getapppassword API
 * endpoint with the currently configured username and password for authorization.
 *
 * If a 403 HTTP status is returned, the current password is already an application
 * password.
 *
 * If a 200 HTTP status code is returned, the used password was not an application
 * password and the response contains a newly generated application password.
 *
 * \note
 * This API call will use AbstractConfiguration::getLoginFlowUserAgent() to set the User-Agent
 * header of the request to generate a user friendly application name user agent that will
 * be shown in the configuration backend of the Nextcloud.
 *
 * \par Mandatory properties
 * \li Job::configuration
 *
 * \par API method
 * GET
 *
 * \par API route
 * /ocs/v2.php/core/getapppassword
 *
 * \par API docs
 * https://docs.nextcloud.com/server/latest/developer_manual/client_apis/LoginFlow/index.html#converting-to-app-passwords
 */
class WOLKANLIN_EXPORT GetAppPasswordJob : public Job
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new %GetAppPasswordJob object with the given \a parent.
     */
    explicit GetAppPasswordJob(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %GetAppPasswordJob object.
     */
    ~GetAppPasswordJob() override;

    /*!
     * \brief Starts the job asynchronously.
     */
    void start() override;

    /*!
     * \brief Returns a human readable and translated error string.
     *
     * If WJob::error() returns not \c 0, an error has occured and the human
     * readable description can be returned by this function.
     */
    QString errorString() const override;

private:
    Q_DECLARE_PRIVATE_D(wl_ptr, GetAppPasswordJob)
    Q_DISABLE_COPY(GetAppPasswordJob)
};

}

#endif // WOLKANLIN_GETAPPPASSWORDJOB_H
