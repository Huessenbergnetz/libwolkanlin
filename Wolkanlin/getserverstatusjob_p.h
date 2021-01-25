/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_GETSERVERSTATUSJOB_P_H
#define WOLKANLIN_GETSERVERSTATUSJOB_P_H

#include "getserverstatusjob.h"
#include "job_p.h"

namespace Wolkanlin {

class GetServerStatusJobPrivate : public JobPrivate
{
public:
    GetServerStatusJobPrivate(GetServerStatusJob *q);
    ~GetServerStatusJobPrivate() override;

    QString buildUrlPath() const override;

    void emitDescription() override;

    QUrlQuery buildUrlQuery() const override;

    QMap<QByteArray, QByteArray> buildRequestHeaders() const override;

private:
    Q_DISABLE_COPY(GetServerStatusJobPrivate)
    Q_DECLARE_PUBLIC(GetServerStatusJob)
};

}

#endif // WOLKANLIN_GETSERVERSTATUSJOB_P_H
