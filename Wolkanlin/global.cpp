/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "global.h"
#include "logging.h"
#include <QReadWriteLock>

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

    AbstractNamFactory *namFactory() const
    {
        return m_namFactory;
    }

    void setNamFactory(AbstractNamFactory *factory)
    {
        m_namFactory = factory;
    }

private:
    AbstractConfiguration *m_configuration = nullptr;
    AbstractNamFactory *m_namFactory = nullptr;
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

AbstractNamFactory *Wolkanlin::networkAccessManagerFactory()
{
    const DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    defs->lock.lockForRead();
    AbstractNamFactory *factory = defs->namFactory();
    defs->lock.unlock();

    return factory;
}

void Wolkanlin::setNetworkAccessManagerFactory(AbstractNamFactory *factory)
{
    DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    QWriteLocker locker(&defs->lock);
    qCDebug(wlCore) << "Setting networkAccessManagerFactory to" << factory;
    defs->setNamFactory(factory);
}

QVersionNumber Wolkanlin::version()
{
    return QVersionNumber::fromString(QStringLiteral(WOLKANLIN_VERSION));
}
