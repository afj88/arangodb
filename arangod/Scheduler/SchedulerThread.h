////////////////////////////////////////////////////////////////////////////////
/// @brief scheduler thread
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2014 ArangoDB GmbH, Cologne, Germany
/// Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Dr. Frank Celler
/// @author Martin Schoenert
/// @author Copyright 2014, ArangoDB GmbH, Cologne, Germany
/// @author Copyright 2009-2013, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef ARANGODB_SCHEDULER_SCHEDULER_THREAD_H
#define ARANGODB_SCHEDULER_SCHEDULER_THREAD_H 1

#include "Basics/Common.h"
#include "Basics/Mutex.h"
#include "Basics/SpinLock.h"
#include "Basics/Thread.h"
#include "Scheduler/Task.h"
#include "Scheduler/TaskManager.h"

// #define TRI_USE_SPIN_LOCK_SCHEDULER_THREAD 1

// -----------------------------------------------------------------------------
// --SECTION--                                              forward declarations
// -----------------------------------------------------------------------------

namespace triagens {
  namespace rest {
    class Scheduler;

// -----------------------------------------------------------------------------
// --SECTION--                                             class SchedulerThread
// -----------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
/// @brief job scheduler thread
/////////////////////////////////////////////////////////////////////////////

    class SchedulerThread : public basics::Thread, private TaskManager {
      private:
        SchedulerThread (SchedulerThread const&);
        SchedulerThread& operator= (SchedulerThread const&);

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief constructor
////////////////////////////////////////////////////////////////////////////////

        SchedulerThread (Scheduler*, EventLoop, bool defaultLoop);

////////////////////////////////////////////////////////////////////////////////
/// @brief destructor
////////////////////////////////////////////////////////////////////////////////

        ~SchedulerThread ();

// -----------------------------------------------------------------------------
// --SECTION--                                                    public methods
// -----------------------------------------------------------------------------

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief checks if the scheduler thread is up and running
////////////////////////////////////////////////////////////////////////////////

        bool isStarted ();

////////////////////////////////////////////////////////////////////////////////
/// @brief opens the scheduler thread for business
////////////////////////////////////////////////////////////////////////////////

        bool open ();

////////////////////////////////////////////////////////////////////////////////
/// @brief begin shutdown sequence
////////////////////////////////////////////////////////////////////////////////

        void beginShutdown ();

////////////////////////////////////////////////////////////////////////////////
/// @brief registers a task
////////////////////////////////////////////////////////////////////////////////

        bool registerTask (Scheduler*, Task*);

////////////////////////////////////////////////////////////////////////////////
/// @brief unregisters a task
////////////////////////////////////////////////////////////////////////////////

        void unregisterTask (Task*);

////////////////////////////////////////////////////////////////////////////////
/// @brief destroys a task
////////////////////////////////////////////////////////////////////////////////

        void destroyTask (Task*);

// -----------------------------------------------------------------------------
// --SECTION--                                                    Thread methods
// -----------------------------------------------------------------------------

      protected:

////////////////////////////////////////////////////////////////////////////////
/// {@inheritDoc}
////////////////////////////////////////////////////////////////////////////////

        void run ();

// -----------------------------------------------------------------------------
// --SECTION--                                                     private types
// -----------------------------------------------------------------------------

      private:

////////////////////////////////////////////////////////////////////////////////
/// @brief what to do with the task
////////////////////////////////////////////////////////////////////////////////

        enum work_e {
          INVALID,
          CLEANUP,
          DESTROY,
          SETUP
        };

////////////////////////////////////////////////////////////////////////////////
/// @brief what to do with the thread
////////////////////////////////////////////////////////////////////////////////

        class Work {
          public:
            Work ()
              : work(INVALID), scheduler(nullptr), task(nullptr) {
            }

            Work (work_e w, Scheduler* scheduler, Task* task)
              : work(w), scheduler(scheduler), task(task) {
            }

            work_e work;
            Scheduler* scheduler;
            Task* task;
        };

// -----------------------------------------------------------------------------
// --SECTION--                                                   private methods
// -----------------------------------------------------------------------------

      private:

////////////////////////////////////////////////////////////////////////////////
/// @brief underlying scheduler
////////////////////////////////////////////////////////////////////////////////

        Scheduler* _scheduler;

////////////////////////////////////////////////////////////////////////////////
/// @brief if true, this is the default loop
////////////////////////////////////////////////////////////////////////////////

        bool _defaultLoop;

////////////////////////////////////////////////////////////////////////////////
/// @brief event loop
////////////////////////////////////////////////////////////////////////////////

        EventLoop _loop;

////////////////////////////////////////////////////////////////////////////////
/// @brief true if scheduler threads is shutting down
////////////////////////////////////////////////////////////////////////////////

        volatile sig_atomic_t _stopping;

////////////////////////////////////////////////////////////////////////////////
/// @brief true if scheduler threads has stopped
////////////////////////////////////////////////////////////////////////////////

        volatile sig_atomic_t _stopped;

////////////////////////////////////////////////////////////////////////////////
/// @brief queue lock
////////////////////////////////////////////////////////////////////////////////

#ifdef TRI_USE_SPIN_LOCK_SCHEDULER_THREAD
        triagens::basics::SpinLock _queueLock;
#else
        triagens::basics::Mutex _queueLock;
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief work queue
////////////////////////////////////////////////////////////////////////////////

        std::deque<Work> _queue;

////////////////////////////////////////////////////////////////////////////////
/// @brief open for business
////////////////////////////////////////////////////////////////////////////////

        volatile sig_atomic_t _open;

////////////////////////////////////////////////////////////////////////////////
/// @brief work indicator
////////////////////////////////////////////////////////////////////////////////

        bool _hasWork;

    };
  }
}

#endif

// -----------------------------------------------------------------------------
// --SECTION--                                                       END-OF-FILE
// -----------------------------------------------------------------------------

// Local Variables:
// mode: outline-minor
// outline-regexp: "/// @brief\\|/// {@inheritDoc}\\|/// @page\\|// --SECTION--\\|/// @\\}"
// End: