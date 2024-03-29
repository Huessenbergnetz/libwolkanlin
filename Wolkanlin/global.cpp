/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "global.h"
#include "logging.h"
#include <QReadWriteLock>
#include <QCoreApplication>
#include <QTranslator>

#if defined(QT_DEBUG)
Q_LOGGING_CATEGORY(wlCore, "wolkanlin.core")
#else
Q_LOGGING_CATEGORY(wlCore, "wolkanlin.core", QtInfoMsg)
#endif

using namespace Wolkanlin;

class DefaultValues
{
public:
    mutable QReadWriteLock lock;

    AbstractConfiguration *configuration() const
    {
        return m_configuration;
    }

    void setConfiguration(AbstractConfiguration *config)
    {
        m_configuration = config;
    }

    QNetworkAccessManager *networkAccessManager() const
    {
        return m_nam;
    }

    void setNetworkAccessManager(QNetworkAccessManager *nam)
    {
        m_nam = nam;
    }

private:
    AbstractConfiguration *m_configuration = nullptr;
    QNetworkAccessManager *m_nam = nullptr;
};
Q_GLOBAL_STATIC(DefaultValues, defVals)

AbstractConfiguration *Wolkanlin::defaultConfiguration()
{
    const DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    defs->lock.lockForRead();
    AbstractConfiguration *config = defs->configuration();
    defs->lock.unlock();

    return config;
}

void Wolkanlin::setDefaultConfiguration(AbstractConfiguration *configuration)
{
    DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    QWriteLocker locker(&defs->lock);
    qCDebug(wlCore) << "Setting defaultConfiguration to" << configuration;
    defs->setConfiguration(configuration);
}

QNetworkAccessManager *Wolkanlin::defaultNetworkAccessManager()
{
    const DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    QReadLocker locker(&defs->lock);
    return defs->networkAccessManager();
}

void Wolkanlin::setDefaultNetworkAccessManager(QNetworkAccessManager *nam)
{
    DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    QWriteLocker locker(&defs->lock);
    qCDebug(wlCore) << "Setting defaultNetworkAccessManager to" << nam;
    defs->setNetworkAccessManager(nam);
}

QVersionNumber Wolkanlin::version()
{
    return QVersionNumber::fromString(QStringLiteral(WOLKANLIN_VERSION));
}

bool Wolkanlin::loadTranslations(const QLocale &locale)
{
    auto t = new QTranslator(QCoreApplication::instance());
    if (t->load(locale, QStringLiteral("libwolkanlin"), QStringLiteral("_"), QStringLiteral(WOLKANLIN_I18NDIR))) {
        if (QCoreApplication::installTranslator(t)) {
            return true;
        } else {
            qCWarning(wlCore) << "Failed to install translations for libwolkanlin for locale" << locale;
            return false;
        }
    } else {
        qCWarning(wlCore) << "Failed to load translations for libwolkanlin for locale" << locale;
        return false;
    }
}
