/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_GETUSERLISTJOB_P_H
#define WOLKANLIN_GETUSERLISTJOB_P_H

#include "getuserlistjob.h"
#include "job_p.h"

namespace Wolkanlin {

class GetUserListJobPrivate : public JobPrivate
{
public:
    explicit GetUserListJobPrivate(GetUserListJob *q);
    ~GetUserListJobPrivate() override;

    QString buildUrlPath() const override;

    void emitDescription() override;

private:
    Q_DECLARE_PUBLIC(GetUserListJob)
    Q_DISABLE_COPY(GetUserListJobPrivate)
};

}

#endif // WOLKANLIN_GETUSERLISTJOB_P_H
