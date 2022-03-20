/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_DELETEAPPPASSWORD_H
#define WOLKANLIN_DELETEAPPPASSWORD_H

#include "wolkanlin_exports.h"
#include "job.h"
#include <QObject>

namespace Wolkanlin {

class DeleteAppPasswordJobPrivate;

/*!
 * \brief Deletes an application password/token on the server.
 *
 * When an account on a client is removed for housekeeping it is desired
 * to destroy the application token in use. The token to delete is taken
 * from the user authentication data provided in the \link Job::configuration
 * configuration\endlink property.
 *
 * \par Mandatory properties
 * \li Job::configuration
 *
 * \par API method
 * DELETE
 *
 * \par API rout
 * /ocs/v2.php/core/apppassword
 *
 * \par API docs
 * https://docs.nextcloud.com/server/latest/developer_manual/client_apis/LoginFlow/index.html#deleting-an-app-password
 *
 * \headerfile "" <Wolkanlin/DeleteAppPasswordJob>
 */
class DeleteAppPasswordJob : public Job
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new %DeleteAppPasswordJob object with the given \a parent.
     */
    explicit DeleteAppPasswordJob(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %DeleteAppPasswordJob object.
     */
    ~DeleteAppPasswordJob() override;

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
    Q_DISABLE_COPY(DeleteAppPasswordJob)
    Q_DECLARE_PRIVATE_D(wl_ptr, DeleteAppPasswordJob)
};

}

#endif // WOLKANLIN_DELETEAPPPASSWORD_H
