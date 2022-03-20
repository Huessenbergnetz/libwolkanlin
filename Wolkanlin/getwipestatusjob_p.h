/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_GETWIPESTATUSJOB_P_H
#define WOLKANLIN_GETWIPESTATUSJOB_P_H

#include "getwipestatusjob.h"
#include "job_p.h"

namespace Wolkanlin {

class GetWipeStatusJobPrivate : public JobPrivate
{
public:
    GetWipeStatusJobPrivate(GetWipeStatusJob *q);
    ~GetWipeStatusJobPrivate() override;

    QString buildUrlPath() const override;

    void emitDescription() override;

    QUrlQuery buildUrlQuery() const override;

    QMap<QByteArray, QByteArray> buildRequestHeaders() const override;

    std::pair<QByteArray, QByteArray> buildPayload() const override;

    bool checkInput() override;

    void extractError() override;

    QString token;

private:
    Q_DISABLE_COPY(GetWipeStatusJobPrivate)
    Q_DECLARE_PUBLIC(GetWipeStatusJob)
};

}

#endif // WOLKANLIN_GETWIPESTATUSJOB_P_H
