////////////////////////////////////////////////////////////////////////////////
/// @brief dispatcher queue
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2010-2011 triagens GmbH, Cologne, Germany
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
/// Copyright holder is triAGENS GmbH, Cologne, Germany
///
/// @author Dr. Frank Celler
/// @author Martin Schoenert
/// @author Copyright 2009-2011, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_FYN_DISPATCHER_DISPATCHER_QUEUE_H
#define TRIAGENS_FYN_DISPATCHER_DISPATCHER_QUEUE_H 1

#include <Basics/Common.h>

#include <Basics/ConditionVariable.h>
#include <Rest/Dispatcher.h>

#include "Dispatcher/DispatcherImpl.h"

namespace triagens {
  namespace rest {
    class DispatcherThread;
    class Job;

    /////////////////////////////////////////////////////////////////////////////
    /// @brief dispatcher queue
    /////////////////////////////////////////////////////////////////////////////

    class DispatcherQueue : noncopyable {
      public:

        /////////////////////////////////////////////////////////////////////////////
        /// @brief constructs a new dispatcher queue
        /////////////////////////////////////////////////////////////////////////////

        DispatcherQueue (Dispatcher* dispatcher,
                         DispatcherImpl::newDispatcherThread_fptr,
                         size_t nrThreads);

        /////////////////////////////////////////////////////////////////////////////
        /// @brief destructor
        /////////////////////////////////////////////////////////////////////////////

        ~DispatcherQueue ();

      public:

        /////////////////////////////////////////////////////////////////////////////
        /// @brief adds a job
        /////////////////////////////////////////////////////////////////////////////

        void addJob (Job*);

        /////////////////////////////////////////////////////////////////////////////
        /// @brief shut downs the queue
        /////////////////////////////////////////////////////////////////////////////

        void beginShutdown ();

        /////////////////////////////////////////////////////////////////////////
        /// @brief downgrades the thread to special
        /////////////////////////////////////////////////////////////////////////

        void specializeThread (DispatcherThread*);

        /////////////////////////////////////////////////////////////////////////////
        /// @brief starts a queue
        /////////////////////////////////////////////////////////////////////////////

        bool start ();

        /////////////////////////////////////////////////////////////////////////////
        /// @brief is the queue still active
        /////////////////////////////////////////////////////////////////////////////

        bool isRunning ();

        /////////////////////////////////////////////////////////////////////////////
        /// @brief starts a new queue thread
        /////////////////////////////////////////////////////////////////////////////

        bool startQueueThread ();

      public:

        /////////////////////////////////////////////////////////////////////////////
        /// @brief monopolistic jobs
        /////////////////////////////////////////////////////////////////////////////

        basics::ConditionVariable accessQueue;

        /////////////////////////////////////////////////////////////////////////////
        /// @brief list of ready jobs
        /////////////////////////////////////////////////////////////////////////////

        list<Job*> readyJobs;

        /////////////////////////////////////////////////////////////////////////////
        /// @brief queue is shutting down
        /////////////////////////////////////////////////////////////////////////////

        volatile sig_atomic_t stopping;

        /////////////////////////////////////////////////////////////////////////////
        /// @brief monopolistic job
        /////////////////////////////////////////////////////////////////////////////

        DispatcherThread * monopolizer;

        /////////////////////////////////////////////////////////////////////////////
        /// @brief list of started threads
        /////////////////////////////////////////////////////////////////////////////

        set<DispatcherThread*> startedThreads;

        /////////////////////////////////////////////////////////////////////////////
        /// @brief list of stopped threads
        /////////////////////////////////////////////////////////////////////////////

        list<DispatcherThread*> stoppedThreads;

        /////////////////////////////////////////////////////////////////////////////
        /// @brief number of started jobs
        /////////////////////////////////////////////////////////////////////////////

        size_t nrStarted;

        /////////////////////////////////////////////////////////////////////////////
        /// @brief number of running jobs
        /////////////////////////////////////////////////////////////////////////////

        size_t nrRunning;

        /////////////////////////////////////////////////////////////////////////////
        /// @brief number of waiting jobs
        /////////////////////////////////////////////////////////////////////////////

        size_t nrWaiting;

        /////////////////////////////////////////////////////////////////////////////
        /// @brief number of stopped jobs
        /////////////////////////////////////////////////////////////////////////////

        size_t nrStopped;

        /////////////////////////////////////////////////////////////////////////////
        /// @brief number of special jobs
        /////////////////////////////////////////////////////////////////////////////

        size_t nrSpecial;

        /////////////////////////////////////////////////////////////////////////////
        /// @brief total number of threads
        /////////////////////////////////////////////////////////////////////////////

        size_t nrThreads;

        /////////////////////////////////////////////////////////////////////////////
        /// @brief dispatcher
        /////////////////////////////////////////////////////////////////////////////

        Dispatcher* dispatcher;

      private:
        DispatcherImpl::newDispatcherThread_fptr createDispatcherThread;
    };
  }
}

#endif


