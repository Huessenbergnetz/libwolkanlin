/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_GETUSERJOB_H
#define WOLKANLIN_GETUSERJOB_H

#include "wolkanlin_exports.h"
#include "job.h"
#include <QObject>

namespace Wolkanlin {

class GetUserJobPrivate;

/*!
 * \brief Requests information about a single user from the remote server.
 *
 * At least the \link GetUserJob::id id\endlink property has to be set to a valid user name.
 * After setting the mandatory properties, call start() to perform the request. See User for
 * an object that takes and represents data requested by this API job.
 *
 * \par Mandatory properties
 * \li Job::configuration
 * \li GetUserJob::id
 *
 * \par API method
 * GET
 *
 * \par API route
 * /ocs/v1.php/cloud/users/{\link GetUserJob::id id\endlink}
 *
 * \par API docs
 * https://docs.nextcloud.com/server/latest/developer_manual/client_apis/OCS/ocs-api-overview.html#user-metadata
 *
 * <H3 id="getuserjob-json-example">JSON representation example</H3>
 * This is the JSON object like it is returned by the Nextcloud API. On a successful request
 * it will be emitted in the Job::succeeded() signal or can be get via the Job::replyData() function.
 * See the <A HREF="https://docs.nextcloud.com/server/latest/developer_manual/client_apis/OCS/ocs-api-overview.html#user-metadata" rel="external noopener">OCS API overview</A>
 * for a maybe more recent description of this data.
 * \include userdata-example.json
 *
 * <H3 id="getuserjob-usage-examples">Usage examples</H3>
 * To get user meta data from the Nextcloud server, you can use %GetUserJob together with
 * User::fromJson() to fetch the data asynchronously.
 * \include get-user-async.cpp
 * There is also the possibility to do it synchronously.
 * \include get-user-sync.cpp
 * Another way is to use User::get() function.
 *
 * \headerfile "" <Wolkanlin/GetUserJob>
 */
class WOLKANLIN_LIBRARY GetUserJob : public Job
{
    Q_OBJECT
    /*!
     * \brief This property holds the ID / user name to get the information for.
     *
     * \par Access methods
     * \li void setId(const QString &id)
     * \li QString id() const
     *
     * \par Notifier signal
     * \li void idChanged(const QString &id)
     */
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
public:
    /*!
     * \brief Constructs a new %GetUserJob object with the given \a parent.
     */
    explicit GetUserJob(QObject *parent = nullptr);

    /*!
     * \brief Constructs a new %GetUserJob object with the given parameters.
     * \param id        the user name / id to get the information for
     * \param parent    pointer to a parent object
     */
    explicit GetUserJob(const QString &id, QObject *parent = nullptr);

    /*!
     * \brief Destroys the %GetUserJob object.
     */
    ~GetUserJob() override;

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

    /*!
     * \brief Getter function for the \link GetUserJob::id id\endlink property.
     * \sa setUser(), userChanged()
     */
    QString id() const;

    /*!
     * \brief Setter function for the \link GetUserJob::id id\endlink property.
     * \sa id(), idChanged()
     */
    void setId(const QString &id);

Q_SIGNALS:
    /*!
     * \brief Notifier signal for the \link GetUserJob::id id\endlink property.
     * \sa setId(), id()
     */
    void idChanged(const QString &id);

private:
    Q_DECLARE_PRIVATE_D(wl_ptr, GetUserJob)
    Q_DISABLE_COPY(GetUserJob)
};

}

#endif // WOLKANLIN_GETUSERJOB_H
