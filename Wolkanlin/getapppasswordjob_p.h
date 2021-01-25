/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_GETAPPPASSWORDJOB_P_H
#define WOLKANLIN_GETAPPPASSWORDJOB_P_H

#include "getapppasswordjob.h"
#include "job_p.h"

namespace Wolkanlin {

class GetAppPasswordJobPrivate : public JobPrivate
{
public:
    GetAppPasswordJobPrivate(GetAppPasswordJob *q);
    ~GetAppPasswordJobPrivate() override;

    QString buildUrlPath() const override;

    void emitDescription() override;

    void extractError() override;

private:
    Q_DISABLE_COPY(GetAppPasswordJobPrivate)
    Q_DECLARE_PUBLIC(GetAppPasswordJob)
};

}

#endif // WOLKANLIN_GETAPPPASSWORDJOB_P_H
