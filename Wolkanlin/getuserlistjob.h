/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_GETUSERLISTJOB_H
#define WOLKANLIN_GETUSERLISTJOB_H

#include "wolkanlin_export.h"
#include "job.h"
#include <QObject>

namespace Wolkanlin {

class GetUserListJobPrivate;

/*!
 * \brief Requests a list of user IDs from the remote server.
 *
 * The request has to be performed with the authorization of an admin user ID.
 *
 * \par Mandatory properties
 * \li Job::configuration
 *
 * \par API method
 * GET
 *
 * \par API route
 * /ocs/v1.php/cloud/users
 *
 * \par API docs
 * https://docs.nextcloud.com/server/latest/developer_manual/client_apis/OCS/ocs-api-overview.html#user-metadata-list-user-ids
 *
 * <H3 id="getuserlistjob-json-example">JSON representation example</H3>
 * This is the JSON object like it is returned by the Nextcloud API. On a successful request
 * it will be emitted in the Job::succeeded() signal or can be get via the Job::replyData() function.
 * See the <A HREF="https://docs.nextcloud.com/server/latest/developer_manual/client_apis/OCS/ocs-api-overview.html#user-metadata-list-user-ids" rel="external noopener">OCS API overview</A>
 * for a maybe more recent description of this data.
 * \include userlist-example.json
 *
 * <H3 id="getuserlistjob-usage-examples">Usage examples</H3>
 * Fetch the user list from the Nextcloud server asynchronously.
 * \include get-user-async.cpp
 * There is also the possibility to do it synchronously.
 * \include get-user-sync.cpp
 *
 * \headerfile "" <Wolkanlin/GetUserListJob>
 */
class WOLKANLIN_EXPORT GetUserListJob : public Job
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new %GetUserListJob object with the given \a parent.
     */
    explicit GetUserListJob(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %GetUserListJob object.
     */
    ~GetUserListJob() override;

    /*!
     * \brief Starts the job asynchronously.
     */
    void start() override;

private:
    Q_DECLARE_PRIVATE_D(wl_ptr, GetUserListJob)
    Q_DISABLE_COPY(GetUserListJob)
};

}

#endif // WOLKANLIN_GETUSERLISTJOB_H
