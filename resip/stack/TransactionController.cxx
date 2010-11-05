#if defined(HAVE_CONFIG_H)
#include "resip/stack/config.hxx"
#endif

#include "resip/stack/AbandonServerTransaction.hxx"
#include "resip/stack/ApplicationMessage.hxx"
#include "resip/stack/CancelClientInviteTransaction.hxx"
#include "resip/stack/ShutdownMessage.hxx"
#include "resip/stack/SipMessage.hxx"
#include "resip/stack/TransactionController.hxx"
#include "resip/stack/TransactionState.hxx"
#ifdef USE_SSL
#include "resip/stack/ssl/Security.hxx"
#endif
#include "rutil/Logger.hxx"
#include "resip/stack/SipStack.hxx"
#include "rutil/WinLeakCheck.hxx"
#include <thread>
#include <boost/shared_ptr.hpp>
#include <resip/stack/TransactionStateThread.hxx>

using namespace resip;

#define RESIPROCATE_SUBSYSTEM Subsystem::TRANSACTION

#if defined(WIN32) && !defined (__GNUC__)
#pragma warning( disable : 4355 ) // using this in base member initializer list 
#endif

unsigned int TransactionController::MaxTUFifoSize = 0;
unsigned int TransactionController::MaxTUFifoTimeDepthSecs = 0;

TransactionController::TransactionController(SipStack& stack) :
   mStack(stack),
   mDiscardStrayResponses(true),
   mFixBadDialogIdentifiers(true),
   mFixBadCSeqNumbers(true),
   mStateMacFifo(),
   mTuSelector(stack.mTuSelector),
   mTransportSelector(mStateMacFifo,
                      stack.getSecurity(),
                      stack.getDnsStub(),
                      stack.getCompression()),
   mClientTransactionMaps(numberOfThreads()),
   mServerTransactionMaps(numberOfThreads()),
   mTimers(mStateMacFifo),
   mShuttingDown(false),
   mStatsManager(stack.mStatsManager)
{
      for (int i = 0; i < numberOfThreads(); ++i)
      {
	mThreads.push_back(boost::shared_ptr<TransactionStateThread>(new TransactionStateThread(*this,i)));
      }
      
      
}

#if defined(WIN32) && !defined(__GNUC__)
#pragma warning( default : 4355 )
#endif

TransactionController::~TransactionController()
{
}


std::size_t resip::TransactionController::numberOfThreads()
{
  return std::thread::hardware_concurrency() == 0 ? 4u : std::thread::hardware_concurrency();
}


bool 
TransactionController::isTUOverloaded() const
{
   return !mTuSelector.wouldAccept(TimeLimitFifo<Message>::EnforceTimeDepth);
}

void
TransactionController::shutdown()
{
   mShuttingDown = true;
   mTransportSelector.shutdown();
}

void
TransactionController::process(FdSet& fdset)
{
   if (mShuttingDown && 
       //mTimers.empty() && 
       !mStateMacFifo.messageAvailable() && // !dcm! -- see below 
       !mStack.mTUFifo.messageAvailable() &&
       mTransportSelector.isFinished())
// !dcm! -- why would one wait for the Tu's fifo to be empty before delivering a
// shutdown message?
   {
      //!dcm! -- send to all?
      mTuSelector.add(new ShutdownMessage, TimeLimitFifo<Message>::InternalElement);
   }
   else
   {
      mTransportSelector.process(fdset);
      mTimers.process();
      
       

      TransactionMessage *msg(0);
      while ((msg = mStateMacFifo.getNext(-1)))
      { 
	 try
	 {
	   
	    Data const & tid = msg->getTransactionId();
	    std::size_t threadNum = tid.hash() % numberOfThreads();
	    mThreads[threadNum]->fifo().push(msg);
	 }
	 catch(SipMessage::Exception&)
	 {
            // .bwc This is not our error. Do not ErrLog.
	    DebugLog( << "TransactionController::process dropping message with invalid tid " << msg->brief());
	    delete msg;
	 }
      }
   }
}

unsigned int 
TransactionController::getTimeTillNextProcessMS()
{
   if ( mStateMacFifo.messageAvailable() ) 
   {
      return 0;
   }
   else if ( mTransportSelector.hasDataToSend() )
   {
      return 0;
   }

   return resipMin(mTimers.msTillNextTimer(), mTransportSelector.getTimeTillNextProcessMS());   
} 
   
void 
TransactionController::buildFdSet( FdSet& fdset)
{
   mTransportSelector.buildFdSet( fdset );
}

void
TransactionController::send(SipMessage* msg)
{
   mStateMacFifo.add(msg);
}


unsigned int 
TransactionController::getTuFifoSize() const
{
   return mTuSelector.size();
}

unsigned int 
TransactionController::sumTransportFifoSizes() const
{
   return mTransportSelector.sumTransportFifoSizes();
}

unsigned int 
TransactionController::getTransactionFifoSize() const
{
   return mStateMacFifo.size();
}

unsigned int 
TransactionController::getNumClientTransactions() const
{
   std::size_t transactions(0);
   for (auto i = mClientTransactionMaps.begin(); i != mClientTransactionMaps.end(); ++i)
   {
    transactions += i->size();
   }
   return transactions;
}

unsigned int 
TransactionController::getNumServerTransactions() const
{
   std::size_t transactions(0);
   for (auto i = mServerTransactionMaps.begin(); i != mServerTransactionMaps.end(); ++i)
   {
    transactions += i->size();
   }
   return transactions;
}

unsigned int 
TransactionController::getTimerQueueSize() const
{
   return mTimers.size();
}

void
TransactionController::registerMarkListener(MarkListener* listener)
{
   mTransportSelector.registerMarkListener(listener);
}

void TransactionController::unregisterMarkListener(MarkListener* listener)
{
   mTransportSelector.unregisterMarkListener(listener);
}

void 
TransactionController::abandonServerTransaction(const Data& tid)
{
   mStateMacFifo.add(new AbandonServerTransaction(tid));
}

void 
TransactionController::cancelClientInviteTransaction(const Data& tid)
{
   mStateMacFifo.add(new CancelClientInviteTransaction(tid));
}


/* ====================================================================
 * The Vovida Software License, Version 1.0 
 * 
 * Copyright (c) 2004 Vovida Networks, Inc.  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * 3. The names "VOCAL", "Vovida Open Communication Application Library",
 *    and "Vovida Open Communication Application Library (VOCAL)" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact vocal@vovida.org.
 *
 * 4. Products derived from this software may not be called "VOCAL", nor
 *    may "VOCAL" appear in their name, without prior written
 *    permission of Vovida Networks, Inc.
 * 
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL VOVIDA
 * NETWORKS, INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT DAMAGES
 * IN EXCESS OF $1,000, NOR FOR ANY INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * 
 * ====================================================================
 * 
 * This software consists of voluntary contributions made by Vovida
 * Networks, Inc. and many individuals on behalf of Vovida Networks,
 * Inc.  For more information on Vovida Networks, Inc., please see
 * <http://www.vovida.org/>.
 *
 */
