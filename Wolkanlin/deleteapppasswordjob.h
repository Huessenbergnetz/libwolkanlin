/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_DELETEAPPPASSWORD_H
#define WOLKANLIN_DELETEAPPPASSWORD_H

#include "wolkanlin_global.h"
#include "job.h"
#include <QObject>

namespace Wolkanlin {

class DeleteAppPasswordJobPrivate;

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
