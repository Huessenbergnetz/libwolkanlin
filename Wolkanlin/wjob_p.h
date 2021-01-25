/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_WJOB_P_H
#define WOLKANLIN_WJOB_P_H

#include "wjob.h"
#include <QMap>
#include <QEventLoopLocker>

class QTimer;
class QEventLoop;

namespace Wolkanlin {

class WJobPrivate
{
public:
    WJobPrivate();
    virtual ~WJobPrivate();

    WJob *q_ptr = nullptr;

    QString errorText;
    int error = WJob::NoError;
    WJob::Unit progressUnit = WJob::Bytes;
    QMap<WJob::Unit, qulonglong> processedAmount;
    QMap<WJob::Unit, qulonglong> totalAmount;
    unsigned long percentage = 0;
    QTimer *speedTimer = nullptr;
    QEventLoop *eventLoop = nullptr;
    // eventLoopLocker prevents QCoreApplication from exiting when the last
    // window is closed until the job has finished running
    QEventLoopLocker eventLoopLocker;
    WJob::Capabilities capabilities = WJob::NoCapabilities;
    bool suspended = false;
    bool isAutoDelete = true;

    void _k_speedTimeout();

    bool isFinished = false;

    Q_DECLARE_PUBLIC(WJob)
};

}

#endif // WOLKANLIN_WJOB_P_H
