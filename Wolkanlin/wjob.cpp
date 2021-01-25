/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "wjob_p.h"

#include <QEventLoop>
#include <QTimer>

using namespace Wolkanlin;

WJobPrivate::WJobPrivate()
{

}

WJobPrivate::~WJobPrivate()
{

}

void WJobPrivate::_k_speedTimeout()
{
    Q_Q(WJob);
    Q_EMIT q->speed(q, 0);
    speedTimer->stop();
}

WJob::WJob(QObject *parent)
    : QObject(parent), d_ptr(new WJobPrivate)
{
     d_ptr->q_ptr = this;
}

WJob::WJob(WJobPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

WJob::~WJob()
{
    if (!d_ptr->isFinished) {
        d_ptr->isFinished = true;
        Q_EMIT finished(this, QPrivateSignal());
    }

    delete d_ptr->speedTimer;
    delete d_ptr;
}

WJob::Capabilities WJob::capabilities() const
{
    return d_func()->capabilities;
}

bool WJob::isSuspended() const
{
    return d_func()->suspended;
}

void WJob::finishJob(bool emitResult)
{
    Q_D(WJob);
    Q_ASSERT(!d->isFinished);
    d->isFinished = true;

    if (d->eventLoop) {
        d->eventLoop->quit();
    }

    // If we are displaying a progress dialog, remove it first.
    Q_EMIT finished(this, QPrivateSignal());

    if (emitResult) {
        Q_EMIT result(this, QPrivateSignal());
    }

    if (isAutoDelete()) {
        deleteLater();
    }
}

bool WJob::kill(KillVerbosity verbosity)
{
    Q_D(WJob);
    if (d->isFinished) {
        return true;
    }

    if (doKill()) {
        // A subclass can (but should not) call emitResult() or kill()
        // from doKill() and thus set isFinished to true.
        if (!d->isFinished) {
            setError(KilledJobError);
            finishJob(verbosity != Quietly);
        }
        return true;
    } else {
        return false;
    }
}

bool WJob::suspend()
{
    Q_D(WJob);
    if (!d->suspended) {
        if (doSuspend()) {
            d->suspended = true;
            Q_EMIT suspended(this, QPrivateSignal());
            return true;
        }
    }

    return false;
}

bool WJob::resume()
{
    Q_D(WJob);
    if (d->suspended) {
        if (doResume()) {
            d->suspended = false;
            Q_EMIT resumed(this, QPrivateSignal());

            return true;
        }
    }

    return false;
}

bool WJob::doKill()
{
    return false;
}

bool WJob::doSuspend()
{
    return false;
}

bool WJob::doResume()
{
    return false;
}

void WJob::setCapabilities(WJob::Capabilities capabilities)
{
    Q_D(WJob);
    d->capabilities = capabilities;
}

bool WJob::exec()
{
    Q_D(WJob);
    // Usually this job would delete itself, via deleteLater() just after
    // emitting result() (unless configured otherwise). Since we use an event
    // loop below, that event loop will process the deletion event and we'll
    // have been deleted when exec() returns. This crashes, so temporarily
    // suspend autodeletion and manually do it afterwards.
    const bool wasAutoDelete = isAutoDelete();
    setAutoDelete(false);

    Q_ASSERT(!d->eventLoop);

    QEventLoop loop(this);
    d->eventLoop = &loop;

    start();
    if (!d->isFinished) {
        d->eventLoop->exec(QEventLoop::ExcludeUserInputEvents);
    }
    d->eventLoop = nullptr;

    if (wasAutoDelete) {
        deleteLater();
    }
    return (d->error == NoError);
}

int WJob::error() const
{
    return d_func()->error;
}

QString WJob::errorText() const
{
    return d_func()->errorText;
}

QString WJob::errorString() const
{
    return d_func()->errorText;
}

qulonglong WJob::processedAmount(Unit unit) const
{
    return d_func()->processedAmount[unit];
}

qulonglong WJob::totalAmount(Unit unit) const
{
    return d_func()->totalAmount[unit];
}

unsigned long WJob::percent() const
{
    return d_func()->percentage;
}

bool WJob::isFinished() const
{
    return d_func()->isFinished;
}

void WJob::setError(int errorCode)
{
    Q_D(WJob);
    d->error = errorCode;
}

void WJob::setErrorText(const QString &errorText)
{
    Q_D(WJob);
    d->errorText = errorText;
}

void WJob::setProcessedAmount(Unit unit, qulonglong amount)
{
    Q_D(WJob);
    bool should_emit = (d->processedAmount[unit] != amount);

    d->processedAmount[unit] = amount;

    if (should_emit) {
        Q_EMIT processedAmount(this, unit, amount);
        if (unit == d->progressUnit) {
            Q_EMIT processedSize(this, amount);
            emitPercent(d->processedAmount[unit], d->totalAmount[unit]);
        }
    }
}

void WJob::setTotalAmount(Unit unit, qulonglong amount)
{
    Q_D(WJob);
    bool should_emit = (d->totalAmount[unit] != amount);

    d->totalAmount[unit] = amount;

    if (should_emit) {
        Q_EMIT totalAmount(this, unit, amount);
        if (unit == d->progressUnit) {
            Q_EMIT totalSize(this, amount);
            emitPercent(d->processedAmount[unit], d->totalAmount[unit]);
        }
    }
}

void WJob::setProgressUnit(Unit unit)
{
    Q_D(WJob);
    d->progressUnit = unit;
}

void WJob::setPercent(unsigned long percentage)
{
    Q_D(WJob);
    if (d->percentage != percentage) {
        d->percentage = percentage;
        Q_EMIT percent(this, percentage);
    }
}

void WJob::emitResult()
{
    if (!d_func()->isFinished) {
        finishJob(true);
    }
}

void WJob::emitPercent(qulonglong processedAmount, qulonglong totalAmount)
{
    Q_D(WJob);
    // calculate percents
    if (totalAmount) {
        unsigned long oldPercentage = d->percentage;
        d->percentage = 100.0 * processedAmount / totalAmount;
        if (d->percentage != oldPercentage) {
            Q_EMIT percent(this, d->percentage);
        }
    }
}

void WJob::emitSpeed(unsigned long value)
{
    Q_D(WJob);
    if (!d->speedTimer) {
        d->speedTimer = new QTimer(this);
        connect(d->speedTimer, SIGNAL(timeout()), SLOT(_k_speedTimeout()));
    }

    Q_EMIT speed(this, value);
    d->speedTimer->start(5000);   // 5 seconds interval should be enough
}


bool WJob::isAutoDelete() const
{
    Q_D(const WJob);
    return d->isAutoDelete;
}

void WJob::setAutoDelete(bool autodelete)
{
    Q_D(WJob);
    d->isAutoDelete = autodelete;
}


#include "moc_wjob.cpp"
