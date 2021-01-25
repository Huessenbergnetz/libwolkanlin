/*
 * SPDX-FileCopyrightText: (C) 2021 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef WOLKANLIN_WJOB_H
#define WOLKANLIN_WJOB_H

#include <QObject>
#include "wolkanlin_global.h"

namespace Wolkanlin {

class WJobPrivate;

/*!
 * \brief Base class for all jobs.
 *
 * This is a reimplementation of <A HREF="https://api.kde.org/frameworks/kcoreaddons/html/classKJob.html">KJob from KDE Frameworks’ KCoreAddons</A>,
 * it is meant for platforms and systems where KJob is not available. If KCoreAddons are available, you should build
 * libwolkanlin with `cmake -DWITH_KDE` to use the original KJob implementation.
 */
class WOLKANLIN_LIBRARY WJob : public QObject
{
    Q_OBJECT
    /*!
     * \brief Error code.
     *
     * This property holds the error code after finishing the job.
     * I no error has occured, the error code will be \c 0.
     *
     * \par Access functions
     * \li int error() const
     *
     * \par Notifier signal
     * \li void result(WJob *job)
     *
     * \sa WJob::errorString, WJob::errorText
     */
    Q_PROPERTY(int error READ error NOTIFY result)
    /*!
     * \brief Additional error text.
     *
     * This property holds additional error text, usually some extra data
     * associated with the \link WJob::error error\endlink, such as a URL.
     *
     * \par Access functions
     * \li QString errorText() const
     *
     * \par Notifier signal
     * \li void result(WJob *job)
     *
     * \sa WJob::error, WJob::errorString
     */
    Q_PROPERTY(QString errorText READ errorText NOTIFY result)
    /*!
     * \brief Human-readable error message.
     *
     * This property holds a translated, human-readable description of the
     * \link WJob::error error\endlink.
     *
     * \par Access functions
     * \li QString errorText() const
     *
     * \par Notifier signal
     * \li void result(WJob *job)
     *
     * \sa WJob::error, WJob::errorText
     */
    Q_PROPERTY(QString errorString READ errorString NOTIFY result)
    /*!
     * \brief Overall progress of the job.
     *
     * \par Access functions
     * \li unsigned long percent() const
     *
     * \par Notifier signal
     * \li void percent(WJob *job, unsigned long percent)
     */
    Q_PROPERTY(ulong percent READ percent NOTIFY percent) // KF6 TODO: make "int", is enough
    /*!
     * \brief Capabilities of this job.
     *
     * \par Access functions
     * \li Capabilities capabilities() const
     */
    Q_PROPERTY(Wolkanlin::WJob::Capabilities capabilities READ capabilities CONSTANT)
public:
    /*!
     * \brief Error codes.
     */
    enum {
        NoError = 0,            /**< Indicates there is no error. */
        KilledJobError = 1,     /**< Indicates the job was killed. */
        UserDefinedError = 100  /**< Subclasses should define error codes starting at this value. */
    };

    /*!
     * \brief Describes the unit used in the methods that handle reporting the job progress info.
     * \see totalAmount()
     */
    enum Unit {
        Bytes,          /**< Directory and file sizes in bytes. */
        Files,          /**< The number of files handled by the job. */
        Directories,    /**< The number of directories handled by the job. */
        Items           /**< The number of items (e.g. both directories and files) handled by the job. */
    };
    Q_ENUM(Unit)

    enum KillVerbosity {
        Quietly,
        EmitResult
    };
    Q_ENUM(KillVerbosity)

    /*!
     * \brief Job capabilities.
     */
    enum Capability {
        NoCapabilities  = 0x0000,   /**< None of the capabilities exist. */
        Killable        = 0x0001,   /**< The job can be killed. */
        Suspendable     = 0x0002    /**< The job can be suspended. */
    };
    Q_ENUM(Capability)

    /*!
     * \brief Stores a combination of Capability values.
     */
    Q_DECLARE_FLAGS(Capabilities, Capability)
    Q_FLAG(Capabilities)

    /*!
     * \brief Constructs a new %WJob object with the given \a parent.
     */
    explicit WJob(QObject *parent = nullptr);

    /*!
     * \brief Desconstructs the %WJob obejct.
     */
    ~WJob() override;

    /*!
     * \brief Returns the capabilities of this job.
     * \sa setCapabilities()
     */
    Capabilities capabilities() const;

    /*!
     * \brief Returns \c true if the job was suspended with the suspend() call.
     * \sa suspend(), resume()
     */
    bool isSuspended() const;

    /*!
     * \brief Starts the job asynchronously.
     *
     * When the job is finished, result() is emitted.
     *
     * \warning Never implement any synchronous workload in this method. This method should
     * just trigger the job startup, not do any work itself. It is expected to be non-blocking.
     *
     * This is the method all subclasses need to implement. It should setup and trigger the
     * workload of the job. It should not do any work itself. This includes all signals and
     * terminating the job, e.g. by emitResult(). The workload, which could be another method
     * of the subclass, is to be triggered using the event loop, e.g. by code like:
     *
     * \code
     * void ExampleJob::start()
     * {
     *     QTimer:singleShot(0, this, &ExampleJob::doWork);
     * }
     * \endcode
     */
    Q_SCRIPTABLE virtual void start() = 0;

    /*!
     * \brief Executes the job synchronously.
     *
     * This will start a nested QEventLoop internally. Nested event loop can be dangerous and
     * can have unintended side effects, you should avoid calling exec() whenever you can and
     * use the asynchronous interface of WJob instead.
     *
     * Should you indeed call this method, you need to make sure that all callers are reentrant,
     * so that events delivered by the inner event loop don't cause non-reentrant functions
     * to be called, which usually wreaks havoc.
     *
     * Note that the event loop started by this method does not process user input events,
     * which means your user interface will effectively be blocked. Other events like paint
     * or network events are still being processed. The advantage of not processing user input
     * events is that the chance of accidental reentrance is greatly reduced. Still you should
     * avoid calling this function.
     *
     * Returns \c if the job has been executed without eror, \c false otherwise.
     */
    bool exec();

    /*!
     * \brief Returns the error code, if there has been an error.
     *
     * Only call this method from the slot connected to result().
     * If there has been no error, \c 0 will be returned.
     */
    int error() const;

    /*!
     * \brief Returns the error text if there has been an error.
     *
     * Only call if error is not \c 0.
     *
     * This is usually some extra data associated with the error, such as a URL.
     * Use errorString() to get a human-readable, translated message.
     */
    QString errorText() const;

    /*!
     * \brief A human-readable error message.
     *
     * This provides a translated, human-readable description of the error.
     * Only call if error is not \c 0.
     *
     * Subclasses should implement this to create a translated error message from
     * the error code and error text. For example:
     *
     * \code
     * if (error() == ReadFailed) {
     *     tr("Could not read \"%1\"", errorText());
     * }
     * \endcode
     */
    virtual QString errorString() const;

    /*!
     * \brief Returns the processed amount of a given unit for this job.
     * \param unit the unit of the requested amount
     */
    Q_SCRIPTABLE qulonglong processedAmount(Unit unit) const;

    /*!
     * \brief Returns the total amount of a given unit for this job.
     * \param unit the unit of the requested amount
     */
    Q_SCRIPTABLE qulonglong totalAmount(Unit unit) const;

    /*!
     * \brief Returns the overall progress of this job.
     */
    unsigned long percent() const;

    /*!
     * \brief Sets the auto-delete property of the job.
     *
     * If \a autodelete is set to \c false the job will not delete itself once it is finished.
     *
     * The default for any %WJob is to automatically delete itself.
     *
     * \sa isAutoDelete()
     */
    void setAutoDelete(bool autodelete);

    /*!
     * \brief Returns \c true if this job automatically deletes itself once the job is finished.
     *
     * \sa setAutoDelete()
     */
    bool isAutoDelete() const;

public Q_SLOTS:
    /*!
     * \brief Aborts this job.
     *
     * This kills and deletes the job.
     *
     * \param verbosity if equals to EmitResult, job will emit signal result()
     * and ask uiserver to close the progress window.
     * \a verbosity is set to EmitResult for subjobs. Whether applications
     * should call with Quietly or EmitResult depends on whether they rely
     * on result being emitted or not. Please notice that if \a verbosity is
     * set to Quietly, signal result will NOT be emitted.
     *
     * \return \c true if the operation is supported and succeeded, \c false otherwise
     */
    bool kill(Wolkanlin::WJob::KillVerbosity verbosity = Quietly);

    /*!
     * \brief Suspends this job.
     *
     * The job should be kept in a state in which it is possible to resume it.
     *
     * \sa suspended(), doSuspend()
     *
     * \return \c true if the operation is supported and succeeded, \c false otherwise
     */
    bool suspend();

    /*!
     * \brief Resumes this job.
     *
     * \sa resumed(), doResume()
     *
     * \return \c true if the operation is supported and succeeded, \c false otherwise.
     */
    bool resume();

Q_SIGNALS:
    /*!
     * \brief Emitted when the job is finished, in any case.
     *
     * It is used to notify observers that the job is terminated and that progress can be hidden.
     *
     * \note This is a private signal, it can’t be emitted directly by subclasses of %WJob, use emitResult() instead.
     *
     * In general, to be notified of a job’s completion, client code should connect to result()
     * rather than finished(), so that kill(Quietly) is indeed quiet. However if you store a list
     * of jobs and they might get killed silently, then you must connect to this instead of result(),
     * to avoid dangling pointers in your list.
     *
     * \sa result()
     *
     * \param job the job that emitted this signal
     */
    void finished(Wolkanlin::WJob *job
#if !defined(W_DOXYGEN)
                  , QPrivateSignal
#endif
                  );

    /*!
     * \brief Emitted when the job is suspended.
     *
     * \note This is a private signal, it can’t be emitted directly by subclasses of %WJob.
     *
     * \sa suspend(), doSuspend()
     *
     * \param job the job that emitted this signal
     */
    void suspended(Wolkanlin::WJob *job
#if !defined(W_DOXYGEN)
                  , QPrivateSignal
#endif
                  );

    /*!
     * \brief Emitted when the job is resumed.
     *
     * \note This is a private signal, it can’t be emitted directly by subclasses of %WJob.
     *
     * \sa resume(), doResume()
     *
     * \param job the job that emitted this signal
     */
    void resumed(Wolkanlin::WJob *job
#if !defined(W_DOXYGEN)
                 , QPrivateSignal
#endif
                 );

    /*!
     * \brief Emitted when the job is finished (except when killed with WJob::Quietly).
     *
     * Use error() to know if the job was finished with error.
     *
     * \note This is a private signal, it can’t be emitted directly by subclasses of %WJob, use emitResult() instead.
     *
     * Please connect to this signal instead of finished.
     *
     * \sa kill()
     *
     * \param job the job that emitted this signal
     */
    void result(Wolkanlin::WJob *job
#if !defined(W_DOXYGEN)
                , QPrivateSignal
#endif
                );

    /*!
     * \brief Emitted to display general description of this job.
     *
     * A description has a title and tow optional fields which can be used to complete the description.
     *
     * Examples of titles are “Copying“, “Creating resource”, etc. The fields of the description can be “Source“
     * with an URL and “Destination“ with an URL for a “Copying“ description.
     *
     * \param job       the job that emitted the signal
     * \param title     the general description of the job
     * \param field1    first field (localized name and value)
     * \param field2    second field (localized name and value)
     */
    void description(Wolkanlin::WJob *job, const QString &title,
                     const QPair<QString, QString> &field1 = QPair<QString, QString>(),
                     const QPair<QString, QString> &field2 = QPair<QString, QString>());

    /*!
     * \brief Emitted to display state information about this job.
     *
     * Examples of message are “Resolving host”, “Connecting to host...”, etc.
     *
     * \param job   the job that emitted this signal
     * \param plain the info message
     * \param rich  the rich text version of the message, or QString() if none is available
     */
    void infoMessage(Wolkanlin::WJob *job, const QString &plain, const QString &rich = QString());

    /*!
     * \brief Emitted to display a warning about this job.
     *
     * \param job   the job that emitted this signal
     * \param plain the warning message
     * \param rich  the rich text version of the message, or QString() is none is available
     */
    void warning(Wolkanlin::WJob *job, const QString &plain, const QString &rich = QString());

    /*!
     * \brief Emitted when we know the amount the job will have to process.
     *
     * The unit of this amount is sent too. It can be emitted several times if the job manages several different units.
     *
     * \note This is a private signal, it shouldn’t be emitted directly by subclasses of %WJob, use setTotalAmount() instead.
     *
     * \param job       the job that emitted this signal
     * \param unit      the unit of the total amount
     * \param amount    the total amount
     */
    void totalAmount(Wolkanlin::WJob *job, WJob::Unit unit, qulonglong amount);

    /*!
     * \brief Regularly emitted to show the progress of this job by giving the current amount.
     *
     * The unit of this amount is sent too. It can be emitted several times if the job manages several different units.
     *
     * \note This is a private signal, it shouldn’t be emitted directly by subclasses of %WJob, use setProcessedAmount() instead.
     *
     * \param job       the job that emitted this signal
     * \param unit      the unit of the processed amount
     * \param amount    the processed amount
     */
    void processedAmount(Wolkanlin::WJob *job, WJob::Unit unit, qulonglong amount);

    /*!
     * \brief Emitted when we know the size of this job (data size in bytes for transfers, number of entries for listings, etc).
     *
     * \note This is a private signal, it shouldn’t be emitted directly by subclasses of %WJob, use setTotalAmount() instead.
     *
     * \param job   the job that emitted this signal
     * \param size  the total size
     */
    void totalSize(Wolkanlin::WJob *job, qulonglong size);

    /*!
     * \brief Regularly emitted to show the progress of this job (current data size in bytes for transfers, entries listed, etc.).
     *
     * \note This is a private signal, it shouldn’t be emitted directly by subclasses of %WJob, use setProcessedAmount() instead.
     *
     * \param job   the job that emitted this signal
     * \param size  the processed size
     */
    void processedSize(Wolkanlin::WJob *job, qulonglong size);

    /*!
     * \brief Progress signal showing the overall progress of the job This is valid for
     * any kind of job, and allows using a a progress bar very easily.
     *
     * \note This is a private signal, it shouldn’t be emitted directly by subclasses of %WJob,
     * use emitPercent(), setPercent(), setTotalAmount() or setProcessedAmount() instead.
     *
     * \param job       the job that emitted this signal
     * \param percent   the percentage
     */
    void percent(Wolkanlin::WJob *job, unsigned long percent);

    /*!
     * \brief Emitted to display information about the speed of this job.
     *
     * \note This is a private signal, it shouldn’t be emitted directly by subclasses
     * of %WJob, use emitSpeed() instead.
     *
     * \param job   the job that emitted this signal
     * \param speed the speed in bytes/s
     */
    void speed(Wolkanlin::WJob *job, unsigned long speed);

protected:
    /*!
     * \brief Aborts this job quietly.
     *
     * This simply kills the job, no error reporting or job deletion should be involved.
     * The default implementation does nothing and returns \c false.
     *
     * \sa kill()
     *
     * \return \c true if the operation is supported and succeedd, \c false otherwise.
     */
    virtual bool doKill();

    /*!
     * \brief Suspends this job.
     *
     * The default implementation does nothing and returns \c false.
     *
     * \sa suspend(), suspended()
     *
     * \return \c true if the operation is supported and succeeded, \c false otherwise.
     */
    virtual bool doSuspend();

    /*!
     * \brief Resumes this job.
     *
     * The default implementation does nothing and returns \c false.
     *
     * \sa resume(), resumed()
     *
     * \return \c true if the operation is supported and succeeded, \c false otherwise.
     */
    virtual bool doResume();

    /*!
     * \brief Sets the capabilities for this job.
     * \sa capabilities()
     * \param capabilities the capabilities supported by this job
     */
    void setCapabilities(Capabilities capabilities);

    /*!
     * \brief Returns \c true if the job has been finished and has emitted the finished() signal.
     */
    bool isFinished() const;

    /*!
     * \brief Sets the error code.
     *
     * It should be called when an error is encountered in the job, just before calling emitResult().
     *
     * You should define an (anonymous) enum of error codes, with values starting at WJob::UserDefinedError,
     * and use those. For example:
     *
     * \code
     * enum {
     *     InvalidFoo = WJob::UserDefinedError,
     *     BarNotFound
     * }
     * \endcode
     *
     * \param errorCode
     */
    void setError(int errorCode);

    /*!
     * \brief Sets the error text.
     *
     * It should be called when an error is encountered in the job, just before calling emitResult().
     *
     * Provides extra information about the error that cannot be determined directly from the error
     * code. For example, a URL or filename. This string is not normally translatable.
     *
     * \sa errorString(), setError()
     *
     * \param errorText the error text
     */
    void setErrorText(const QString &errorText);

    /*!
     * \brief Sets the processed size.
     *
     * The processedAmount() and percent() signals are emitted if the values changed.
     * The percent() signal is emitted only for the progress unit.
     *
     * \param unit      the unit of the new processed amount
     * \param amount    the new processed amount
     */
    void setProcessedAmount(Unit unit, qulonglong amount);

    /*!
     * \brief Sets the total size.
     *
     * The totalSize() and percent() signals are emitted if the values changed.
     * The percent() signal is emitted only for the progress unit.
     *
     * \param unit      the unit of the new total amount
     * \param amount    the new total amount
     */
    void setTotalAmount(Unit unit, qulonglong amount);

    /*!
     * \brief Sets the unit that will be used internally to calculate the progress percentage.
     *
     * The default progress unit is Bytes.
     *
     * \param unit  the unit that will be used
     */
    void setProgressUnit(Unit unit);

    /*!
     * \brief Sets the overall progress of the job.
     *
     * The percent() signal is emitted if the value changed.
     *
     * The job takes care of this if you call setProcessedAmount
     * in Bytes (or the unit set by setProgressUnit).
     * This method allows you to set your own progress, as an alternative.
     *
     * \param percentage the new overall progress
     */
    void setPercent(unsigned long percentage);

    /*!
     * \brief Utility function to emit the result signal, and suicide this job.
     *
     * It first notifies the observers to hide the progress for this job using the finished() signal.
     *
     * \note Deletes this job using deleteLater().
     *
     * \sa result(), finished()
     */
    void emitResult();

    /*!
     * \brief Utility function for inherited jobs.
     *
     * Emits the percent signal if bigger than previous value, after calculating it from the parameters.
     *
     * \sa percent()
     *
     * \param processedAmount   the processed amount
     * \param totalAmount       the total amount
     */
    void emitPercent(qulonglong processedAmount, qulonglong totalAmount);

    /*!
     * \brief Utility function for inherited jobs.
     *
     * Emits the speed signal and starts the timer for removing that info
     *
     * \param speed the speed in bytes/s
     */
    void emitSpeed(unsigned long speed);

    WJobPrivate *const d_ptr;
    WJob(WJobPrivate &dd, QObject *parent = nullptr);

private:
    void finishJob(bool emitResult);

    Q_PRIVATE_SLOT(d_func(), void _k_speedTimeout());
    Q_DECLARE_PRIVATE(WJob)
    Q_DISABLE_COPY(WJob)
};

}

Q_DECLARE_METATYPE(Wolkanlin::WJob::Unit)
Q_DECLARE_OPERATORS_FOR_FLAGS(Wolkanlin::WJob::Capabilities)

#endif // WOLKANLIN_WJOB_H
