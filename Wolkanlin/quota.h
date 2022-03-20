/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_QUOTA_H
#define WOLKANLIN_QUOTA_H

#include "wolkanlin_exports.h"
#include <QObject>
#include <QSharedDataPointer>
#include <QJsonObject>

namespace Wolkanlin {

class QuotaPrivate;

/*!
 * \brief Stores quota information.
 *
 * \headerfile "" <Wolkanlin/Quota>
 */
class WOLKANLIN_LIBRARY Quota
{
    Q_GADGET
    /*!
     * \brief This property holds the available free space in bytes.
     *
     * The value is computed by \link Quota::total total\endlink - \link Quota::used used\endlink.
     * It will not be negative. If \a used is greater than \a total, this holds \c 0.
     *
     * If %Quota isNull(), this holds \c 0.
     *
     * \par Access methods
     * \li qint64 free() const
     */
    Q_PROPERTY(qint64 free READ free CONSTANT)
    /*!
     * \brief This property holds the used space in bytes.
     *
     * If %Quota isNull(), this holds \c 0.
     *
     * \par Access methods
     * \li qint64 used() const
     */
    Q_PROPERTY(qint64 used READ used CONSTANT)
    /*!
     * \brief This property holds the set quota in bytes.
     *
     * If the quota is unlimited, this value will be below \c 0.
     *
     * If %Quota isNull(), this holds \c 0.
     *
     * \par Access methods
     * \li qint64 quota() const
     */
    Q_PROPERTY(qint64 quota READ quota CONSTANT)
    /*!
     * \brief This property holds the total available space in bytes.
     *
     * If the \link Quota::quota quota\endlink is not limited (not below 0), this will
     * be the full available free disk space. If the quota is set, this will be the same
     * as quota. If the quota is set to \c 0, this will be the same as \link Quota::used used\endlink.
     *
     * If %Quota isNull(), this holds \c 0.
     *
     * \par Access methods
     * \li qint64 total() const
     */
    Q_PROPERTY(qint64 total READ total CONSTANT)
    /*!
     * \brief This property holds the relative available free space.
     *
     * This is the relation between \link Quota::used used\endlink and \link Quota::total total\endlink
     * as percent value. It will never be greater than \c 100 or lower than \c 0.
     *
     * If %Quota isNull(), this holds \c 0.0.
     *
     * \par Access methods
     * \li double relative() const
     */
    Q_PROPERTY(double relative READ relative CONSTANT)
public:
    /*!
     * \brief Constructs a null %Quota.
     * \sa isNull()
     */
    Quota();
    /*!
     * \brief Constructs a copy of \a other.
     */
    Quota(const Quota &other);
    /*!
     * \brief Move-constructs a %Quota instance, making it point at the same object that \a other was pointing to.
     */
    Quota(Quota &&other) noexcept;
    /*!
     * \brief Constructs a new %Quota object with the given parameters.
     * \param free      the available free space in bytes
     * \param used      the used space in bytes
     * \param quota     the set quota in bytes
     * \param total     the total available space in bytes
     * \param relative  the relative available space
     */
    Quota(qint64 free, qint64 used, qint64 quota, qint64 total, double relative);

    /*!
     * \brief Destroys the %Quota object.
     */
    virtual ~Quota();

    /*!
     * \brief Assigns \a other to this %Quota and returns a reference to this instance.
     */
    Quota &operator=(const Quota &other);
    /*!
     * \brief Move-assigns \a other to this %Quota instance.
     */
    Quota &operator=(Quota &&other) noexcept;

    /*!
     * \brief Returns \c true if \a this and \a other have the same content; otherwise returns \c false.
     */
    bool operator==(const Quota &other) const noexcept;
    /*!
     * \brief Returns \c true if \a this and \a other have not the same content; otherwise returns \c false.
     */
    inline bool operator!=(const Quota &other) const noexcept { return !operator==(other); }

    /*!
     * \brief Returns \c true if this %Quota is null; otherwise returns \c false.
     *
     * A null %Quota is a %Quota that has no values set.
     */
    bool isNull() const;

    /*!
     * \brief Getter function for the \link Quota::free free\endlink property.
     */
    qint64 free() const;
    /*!
     * \brief Getter function for the \link Quota::used used\endlink property.
     */
    qint64 used() const;
    /*!
     * \brief Getter function for the \link Quota::quota quota\endlink property.
     */
    qint64 quota() const;
    /*!
     * \brief Getter function for the \link Quota::total total\endlink property.
     */
    qint64 total() const;
    /*!
     * \brief Getter function for the \link Quota::relative relative\endlink property.
     */
    double relative() const;

    /*!
     * \brief Returns the content of the %Quota as JSON object.
     *
     * \code{.json}
     * {
     *    "free": 209639130,
     *    "used": 76070,
     *    "total": 209715200,
     *    "relative": 0.04,
     *    "quota": 209715200
     * }
     * \endcode
     *
     * If isNull() returns \c true, an empty JSON object will be returned.
     *
     * \sa fromJson()
     */
    QJsonObject toJson() const;

    /*!
     * \brief Creates a %Quota object from \a json.
     *
     * If \a json is empty, a \link isNull() null\endlink %Quota will be returned.
     * See also toJson() for information about the JSON object keys.
     */
    static Quota fromJson(const QJsonObject &json);

private:
    QSharedDataPointer<QuotaPrivate> d;

    friend QDataStream &operator>>(QDataStream &stream, Quota &q);
};

/*!
 * \relates Wolkanlin::Quota
 * \brief Reads a %Quota vom \a stream and stores it to \a q.
 */
WOLKANLIN_LIBRARY QDataStream &operator>>(QDataStream &stream, Quota &q);

}

Q_DECLARE_METATYPE(Wolkanlin::Quota)

/*!
 * \relates Wolkanlin::Quota
 * \brief Writes the \a quota to the \a dbg stream and returns the stream.
 */
WOLKANLIN_LIBRARY QDebug operator<<(QDebug dbg, const Wolkanlin::Quota &quota);

/*!
 * \relates Wolkanlin::Quota
 * \brief Writes %Quota \a q to the \a stream.
 */
WOLKANLIN_LIBRARY QDataStream &operator<<(QDataStream &stream, const Wolkanlin::Quota &q);

#endif // WOLKANLIN_QUOTA_H
