/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_DELETEAPPPASSWORD_P_H
#define WOLKANLIN_DELETEAPPPASSWORD_P_H

#include "deleteapppasswordjob.h"
#include "job_p.h"

namespace Wolkanlin {

class DeleteAppPasswordJobPrivate : public JobPrivate
{
public:
    DeleteAppPasswordJobPrivate(DeleteAppPasswordJob *q);
    ~DeleteAppPasswordJobPrivate() override;

    QString buildUrlPath() const override;

    void emitDescription() override;

private:
    Q_DISABLE_COPY(DeleteAppPasswordJobPrivate)
    Q_DECLARE_PUBLIC(DeleteAppPasswordJob)
};

}

#endif // WOLKANLIN_DELETEAPPPASSWORD_P_H
