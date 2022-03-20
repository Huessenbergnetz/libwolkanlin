/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "quota_p.h"
#include "logging.h"
#include <QDebug>
#include <QDataStream>

using namespace Wolkanlin;

Quota::Quota()
{

}

Quota::Quota(qint64 free, qint64 used, qint64 quota, qint64 total, double relative) : d(new QuotaPrivate)
{
    d->free = free < 0 ? 0 : free;
    d->used = used;
    d->quota = quota;
    d->total = total;
    if (relative < 0.0) {
        d->relative = 0.0;
    } else if (relative > 100.0) {
        d->relative = 100.0;
    } else {
        d->relative = relative;
    }
}

Quota::Quota(const Quota &other) = default;
Quota::Quota(Quota &&other) noexcept = default;
Quota& Quota::operator=(const Quota &other) = default;
Quota& Quota::operator=(Quota &&other) noexcept = default;
Quota::~Quota() = default;

bool Quota::operator==(const Quota &other) const noexcept
{
    if (d == other.d) {
        return true;
    } else {
        if (d && other.d) {
            if (d->free == other.d->free && d->used == other.d->used && d->quota == other.d->quota && d->total == other.d->total && d->relative == other.d->relative) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
}

bool Quota::isNull() const
{
    return d == nullptr;
}

qint64 Quota::free() const
{
    return d ? d->free : 0;
}

qint64 Quota::used() const
{
    return d ? d->used : 0;
}

qint64 Quota::quota() const
{
    return d ? d->quota : 0;
}

qint64 Quota::total() const
{
    return d ? d->total : 0;
}

double Quota::relative() const
{
    return d ? d->relative : 0.0;
}

QJsonObject Quota::toJson() const
{
    QJsonObject o;
    if (Q_LIKELY(!isNull())) {
        o.insert(QStringLiteral("free"), d->free);
        o.insert(QStringLiteral("used"), d->used);
        o.insert(QStringLiteral("quota"), d->quota);
        o.insert(QStringLiteral("total"), d->total);
        o.insert(QStringLiteral("relative"), d->relative);
    } else {
        qCWarning(wlCore) << "Wolkanlin::Quota is null, created QJsonObject will be empty.";
    }
    return o;
}

Quota Quota::fromJson(const QJsonObject &json)
{
    if (Q_LIKELY(!json.isEmpty())) {
        const qint64 free = static_cast<qint64>(json.value(QStringLiteral("free")).toDouble());
        const qint64 used = static_cast<qint64>(json.value(QStringLiteral("used")).toDouble());
        const qint64 quota = static_cast<qint64>(json.value(QStringLiteral("quota")).toDouble());
        const qint64 total = static_cast<qint64>(json.value(QStringLiteral("total")).toDouble());
        const double relative = json.value(QStringLiteral("relative")).toDouble();
        return Quota(free, used, quota, total, relative);
    } else {
        qCWarning(wlCore) << "JSON object is empty, creating null Wolkanlin::Quota.";
        return Quota();
    }
}

QDebug operator<<(QDebug dbg, const Wolkanlin::Quota &quota)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "Wolkanlin::Quota(";
    dbg << "Free: " << quota.free();
    dbg << ", Used: " << quota.used();
    dbg << ", Quota: " << quota.quota();
    dbg << ", Total: " << quota.total();
    dbg << ", Relative: " << quota.relative();
    dbg << ')';
    return dbg.maybeSpace();
}

QDataStream &Wolkanlin::operator>>(QDataStream &stream, Wolkanlin::Quota &q)
{
    if (!q.d) {
        q.d = new Wolkanlin::QuotaPrivate;
    }
    stream >> q.d->free;
    stream >> q.d->used;
    stream >> q.d->quota;
    stream >> q.d->total;
    stream >> q.d->relative;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const Wolkanlin::Quota &q)
{
    stream << q.free() << q.used() << q.quota() << q.total() << q.relative();
    return stream;
}
