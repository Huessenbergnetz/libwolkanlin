/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "serverstatus_p.h"
#include "logging.h"
#include "getserverstatusjob.h"
#include <QDebug>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>

using namespace Wolkanlin;

ServerStatus::ServerStatus(QObject *parent) : QObject(parent), wl_ptr(new ServerStatusPrivate)
{
    Q_D(ServerStatus);
    d->q_ptr = this;
}

ServerStatus::~ServerStatus() = default;

bool ServerStatus::isInstalled() const
{
    Q_D(const ServerStatus);
    return d->installed;
}

bool ServerStatus::isInMaintenance() const
{
    Q_D(const ServerStatus);
    return d->maintenance;
}

bool ServerStatus::needsDbUpgrade() const
{
    Q_D(const ServerStatus);
    return d->needsDbUpgrade;
}

QString ServerStatus::version() const
{
    Q_D(const ServerStatus);
    return d->version;
}

QString ServerStatus::versionstring() const
{
    Q_D(const ServerStatus);
    return d->versionstring;
}

QString ServerStatus::edition() const
{
    Q_D(const ServerStatus);
    return d->edition;
}

QString ServerStatus::productname() const
{
    Q_D(const ServerStatus);
    return d->productname;
}

bool ServerStatus::hasExtendedSupport() const
{
    Q_D(const ServerStatus);
    return d->extendedSupport;
}

bool ServerStatus::isLoading() const
{
    Q_D(const ServerStatus);
    return d->isLoading;
}

bool ServerStatus::isEmpty() const
{
    Q_D(const ServerStatus);
    return d->version.isEmpty();
}

QJsonObject ServerStatus::toJson() const
{
    QJsonObject o;
    if (Q_LIKELY(!isEmpty())) {
        Q_D(const ServerStatus);
        o.insert(QStringLiteral("installed"), d->installed);
        o.insert(QStringLiteral("maintenance"), d->maintenance);
        o.insert(QStringLiteral("needsDbUpgrade"), d->needsDbUpgrade);
        o.insert(QStringLiteral("version"), d->version);
        o.insert(QStringLiteral("versionstring"), d->versionstring);
        o.insert(QStringLiteral("edition"), d->edition);
        o.insert(QStringLiteral("productname"), d->productname);
        o.insert(QStringLiteral("extendedSupport"), d->extendedSupport);
    }
    return o;
}

ServerStatus *ServerStatus::fromJson(const QJsonDocument &json, QObject *parent)
{
    if (json.isObject()) {
        return ServerStatus::fromJson(json.object(), parent);
    } else {
        qCWarning(wlCore) << "JSON document is not an object, creating empty Wolkanlin::ServerStatus object.";
        return new ServerStatus(parent);
    }
}

ServerStatus *ServerStatus::fromJson(const QJsonObject &json, QObject *parent)
{
    if (json.isEmpty()) {
        qCWarning(wlCore) << "JSON object is empty, creating empty Wolkanlin::Serverstatus object.";
        return new ServerStatus(parent);
    }

    auto status = new ServerStatus(parent);
    auto d = status->d_func();
    d->installed = json.value(QStringLiteral("installed")).toBool();
    d->maintenance = json.value(QStringLiteral("maintenance")).toBool();
    d->needsDbUpgrade = json.value(QStringLiteral("needsDbUpgrade")).toBool();
    d->version = json.value(QStringLiteral("version")).toString();
    d->versionstring = json.value(QStringLiteral("versionstring")).toString();
    d->edition = json.value(QStringLiteral("edition")).toString();
    d->productname = json.value(QStringLiteral("productname")).toString();
    d->extendedSupport = json.value(QStringLiteral("extendedSupport")).toBool();
    return status;
}

bool ServerStatus::get(bool async, AbstractConfiguration *config)
{
    Q_D(ServerStatus);

    auto job = new GetServerStatusJob(this);
    if (config) {
        job->setConfiguration(config);
    }

    if (async) {
        connect(job, &GetServerStatusJob::succeeded, this, [d](const QJsonDocument &json) {
            d->onGetServerStatusSucceeded(json);
        });
        connect(job, &GetServerStatusJob::failed, this, &ServerStatus::failed);
        job->start();
        return true;
    } else {
        if (Q_LIKELY(job->exec())) {
            d->onGetServerStatusSucceeded(job->replyData());
            job->deleteLater();
            return true;
        } else {
            Q_EMIT failed(job->error(), job->errorString());
            job->deleteLater();
            return false;
        }
    }
}

void ServerStatusPrivate::setInstalled(bool _installed)
{
    if (installed != _installed) {
        qCDebug(wlCore) << "Changing installed from" << installed << "to" << _installed;
        installed = _installed;
        Q_Q(ServerStatus);
        Q_EMIT q->installedChanged(installed);
    }
}

void ServerStatusPrivate::setMaintenance(bool _maintenance)
{
    if (maintenance != _maintenance) {
        qCDebug(wlCore) << "Changing maintenance from" << maintenance << "to" << _maintenance;
        maintenance = _maintenance;
        Q_Q(ServerStatus);
        Q_EMIT q->maintenanceChanged(maintenance);
    }
}

void ServerStatusPrivate::setNeedsDbUpgrade(bool _needsDbUpgrade)
{
    if (needsDbUpgrade != _needsDbUpgrade) {
        qCDebug(wlCore) << "Changing needsDbUpgrade from" << needsDbUpgrade << "to" << _needsDbUpgrade;
        needsDbUpgrade = _needsDbUpgrade;
        Q_Q(ServerStatus);
        Q_EMIT q->needsDbUpgradeChanged(needsDbUpgrade);
    }
}

void ServerStatusPrivate::setVersion(const QString &_version)
{
    if (version != _version) {
        qCDebug(wlCore) << "Changing version from" << version << "to" << _version;
        version = _version;
        Q_Q(ServerStatus);
        Q_EMIT q->versionChanged(version);
    }
}

void ServerStatusPrivate::setVersionstring(const QString &_versionstring)
{
    if (versionstring != _versionstring) {
        qCDebug(wlCore) << "Changing versionstring from" << versionstring << "to" << _versionstring;
        versionstring = _versionstring;
        Q_Q(ServerStatus);
        Q_EMIT q->versionstringChanged(versionstring);
    }
}

void ServerStatusPrivate::setEdition(const QString &_edition)
{
    if (edition != _edition) {
        qCDebug(wlCore) << "Changing edition from" << edition << "to" << _edition;
        edition = _edition;
        Q_Q(ServerStatus);
        Q_EMIT q->editionChanged(edition);
    }
}

void ServerStatusPrivate::setProductname(const QString &_productname)
{
    if (productname != _productname) {
        qCDebug(wlCore) << "Changing productname from" << productname << "to" << _productname;
        productname = _productname;
        Q_Q(ServerStatus);
        Q_EMIT q->productnameChanged(productname);
    }
}

void ServerStatusPrivate::setExtendedSupport(bool _extendedSupport)
{
    if (extendedSupport != _extendedSupport) {
        qCDebug(wlCore) << "Changing extendedSupport from" << extendedSupport << "to" << _extendedSupport;
        extendedSupport = _extendedSupport;
        Q_Q(ServerStatus);
        Q_EMIT q->extendedSupportChanged(extendedSupport);
    }
}

void ServerStatusPrivate::setIsLoading(bool _isLoading)
{
    if (isLoading != _isLoading) {
        qCDebug(wlCore) << "Changing isLoading from" << isLoading << "to" << _isLoading;
        isLoading = _isLoading;
        Q_Q(ServerStatus);
        Q_EMIT q->isLoadingChanged(isLoading);
    }
}

void ServerStatusPrivate::onGetServerStatusSucceeded(const QJsonDocument &json)
{
    const QJsonObject status = json.object();

    setInstalled(status.value(QStringLiteral("installed")).toBool());
    setMaintenance(status.value(QStringLiteral("maintenance")).toBool());
    setNeedsDbUpgrade(status.value(QStringLiteral("needsDbUpgrade")).toBool());
    setVersion(status.value(QStringLiteral("version")).toString());
    setVersionstring(status.value(QStringLiteral("versionstring")).toString());
    setEdition(status.value(QStringLiteral("edition")).toString());
    setProductname(status.value(QStringLiteral("productname")).toString());
    setExtendedSupport(status.value(QStringLiteral("needsDbUpgrade")).toBool());

    Q_Q(ServerStatus);
    Q_EMIT q->finished();
}

QDebug operator<<(QDebug dbg, const Wolkanlin::ServerStatus &serverStatus)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "Wolkanlin::ServerStatus(";
    dbg << "productname: " << serverStatus.productname();
    dbg << ", edition: " << serverStatus.edition();
    dbg << ", version: " << serverStatus.version();
    dbg << ", versionstring: " << serverStatus.versionstring();
    dbg << ", installed: " << serverStatus.isInstalled();
    dbg << ", maintenance: " << serverStatus.isInMaintenance();
    dbg << ", needsDbUpgrade: " << serverStatus.needsDbUpgrade();
    dbg << ", extendedSupport: " << serverStatus.hasExtendedSupport();
    dbg << ')';
    return dbg.maybeSpace();
}

QDataStream &Wolkanlin::operator>>(QDataStream &stream, Wolkanlin::ServerStatus &serverStatus)
{
    stream >> serverStatus.wl_ptr->productname;
    stream >> serverStatus.wl_ptr->edition;
    stream >> serverStatus.wl_ptr->version;
    stream >> serverStatus.wl_ptr->versionstring;
    stream >> serverStatus.wl_ptr->installed;
    stream >> serverStatus.wl_ptr->maintenance;
    stream >> serverStatus.wl_ptr->needsDbUpgrade;
    stream >> serverStatus.wl_ptr->extendedSupport;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const Wolkanlin::ServerStatus &serverStatus)
{
    stream << serverStatus.productname()
           << serverStatus.edition()
           << serverStatus.version()
           << serverStatus.versionstring()
           << serverStatus.isInstalled()
           << serverStatus.isInMaintenance()
           << serverStatus.needsDbUpgrade()
           << serverStatus.hasExtendedSupport();
    return stream;
}

#include "moc_serverstatus.cpp"

