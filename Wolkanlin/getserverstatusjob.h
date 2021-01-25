/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_GETSERVERSTATUSJOB_H
#define WOLKANLIN_GETSERVERSTATUSJOB_H

#include "wolkanlin_global.h"
#include "job.h"
#include <QObject>

namespace Wolkanlin {

class GetServerStatusJobPrivate;

/*!
 * \brief Requests information about the server status from the remote server.
 *
 * \par Mandatory properties
 * \li Job::configuration
 *
 * \par API method
 * GET
 *
 * \par API route
 * /status.php
 *
 * <H3 id="getserverstatusjob-json-example">JSON representation exmaple</H3>
 * This is the JSON object like it is returned by the Nextcloud API. On a successful
 * request it will be emitted in the Job::succeeded() signal or can be get via the
 * Job::result() function.
 * \include server-status-example.json
 *
 * <H3 id="geserverstatusjob-usage-examples">Usage examples</H3>
 * To get server status information from the Nextcloud server, you can use %GetServerStatusJob
 * together with ServerStatus::fromJson() to fetch data asynchronously.
 * \include get-server-status-async.cpp
 * There is also the possibility to do it synchronously.
 * \include get-server-status-sync.cpp
 * Another way is to use ServerStatus::get() function.
 *
 * \headerfile "" <Wolkanlin/GetServerStatusJob>
 */
class WOLKANLIN_LIBRARY GetServerStatusJob : public Job
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new %GetServerStatusJob with the given \a parent.
     */
    explicit GetServerStatusJob(QObject *parent = nullptr);
    /*!
     * \brief Destroys the %GetServerStatusJob object.
     */
    ~GetServerStatusJob() override;

    /*!
     * \brief Starts the job asynchronously.
     */
    void start() override;

private:
    Q_DECLARE_PRIVATE_D(wl_ptr, GetServerStatusJob)
    Q_DISABLE_COPY(GetServerStatusJob)
};

}

#endif // WOLKANLIN_GETSERVERSTATUSJOB_H
