/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_GETUSERJOB_P_H
#define WOLKANLIN_GETUSERJOB_P_H

#include "getuserjob.h"
#include "job_p.h"

namespace Wolkanlin {

class GetUserJobPrivate : public JobPrivate
{
public:
    explicit GetUserJobPrivate(GetUserJob *q);
    ~GetUserJobPrivate() override;

    QString buildUrlPath() const override;

    bool checkInput() override;

    void emitDescription() override;

    bool checkOutput(const QByteArray &data) override;

    QString id;

private:
    Q_DISABLE_COPY(GetUserJobPrivate)
    Q_DECLARE_PUBLIC(GetUserJob)
};

}

#endif // WOLKANLIN_GETUSERJOB_P_H
