/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_QUOTA_P_H
#define WOLKANLIN_QUOTA_P_H

#include "quota.h"
#include <QSharedData>

namespace Wolkanlin {

class QuotaPrivate : public QSharedData
{
public:
    double relative = 0.0;
    qint64 free = 0;
    qint64 used = 0;
    qint64 total = 0;
    qint64 quota = 0;
};

}

#endif // WOLKANLIN_QUOTA_P_H
