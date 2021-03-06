
/* BEGIN_COMMON_COPYRIGHT_HEADER
 *
 * TOra - An Oracle Toolkit for DBA's and developers
 * 
 * Shared/mixed copyright is held throughout files in this product
 * 
 * Portions Copyright (C) 2000-2001 Underscore AB
 * Portions Copyright (C) 2003-2005 Quest Software, Inc.
 * Portions Copyright (C) 2004-2013 Numerous Other Contributors
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation;  only version 2 of
 * the License is valid for this program.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program as the file COPYING.txt; if not, please see
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt.
 * 
 *      As a special exception, you have permission to link this program
 *      with the Oracle Client libraries and distribute executables, as long
 *      as you follow the requirements of the GNU GPL in regard to all of the
 *      software in the executable aside from Oracle client libraries.
 * 
 * All trademarks belong to their respective owners.
 *
 * END_COMMON_COPYRIGHT_HEADER */

#ifndef TORUNNABLE_H
#define TORUNNABLE_H

#include <QtGui/QObject>
#include <QtGui/QEvent>
#include <QtGui/QThread>


class toRunnableThread;


/**
 * Custom event class for deleting thread
 *
 */
class FinishedEvent : public QEvent
{
    toRunnableThread *Thread;

public:
    FinishedEvent(toRunnableThread *t) : QEvent(QEvent::User)
    {
        Thread = t;
    }


    toRunnableThread* thread() const
    {
        return Thread;
    }
};


/**
 * An abstract class designed to be run from a toRunnableThread. This
 * allows you to send signals and use slots from a thread in an
 * intuitive fashion.
 *
 * NOTE: at this moment it is subclassed only by toEventQueryTask
 * and toConnectionPool
 */
class toRunnable : public QObject
{
    Q_OBJECT;

	friend class toRunnableThread;      // there is cyclic pointer reference between these two classes
    toRunnableThread *Thread;			// This value is initialized at toRunnableThread::run()
public:

    /**
     * Create new runnable object. Does not accept a QObject parent as
     * this class will be deleted by toRunnableThread.
     *
     */
    toRunnable() : QObject(0), Thread(0)
    {
    }

    virtual ~toRunnable()
    {
    }


    /**
     * Override to provide executable. Thread will exit when run does.
     *
     */
    virtual void run(void) = 0;


    /**
     * Set by toRunnableThread before run()
     *
     */
    void setThread(toRunnableThread *thread)
    {
        Thread = thread;
    }

    toRunnableThread* thread(void) const
    {
        return Thread;
    }
};


/**
 * A QThread subclass that uses toRunnable
 *
 */
class toRunnableThread : public QThread
{
    Q_OBJECT;

    toRunnable *Runner;

    /** override from QThread */
    virtual void run(void);

public:

    /**
     * Creates new thread. Call start to begin.
     * and then delete the runner, callee should not delete it
     */
    toRunnableThread(toRunnable *runner) : QThread(0)
    {
        Runner = runner;
        Runner->moveToThread(this);
        setObjectName(QString::fromLatin1("Task:") + Runner->objectName());
    }

    virtual ~toRunnableThread();

    /**
     * Redeclares public
     *
     */
    int exec()
    {
        return QThread::exec();
    }
};


/**
 * Can't delete thread from event handler
 *
 */
class toRunnableDeleter : public QObject
{
    Q_OBJECT;

public:
    toRunnableDeleter()
    {
    }


protected:

    /**
     * Overrides qobject and redeclares non-virtual. It's not a good
     * idea to override this.
     *
     */
    void customEvent(QEvent *event);
};

#endif
