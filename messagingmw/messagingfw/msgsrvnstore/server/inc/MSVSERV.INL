// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// CMsvServer
// 
//

inline RFs& CMsvServer::FileSession()
	{
	return iFs;
	}

inline TInt CMsvServer::SessionId()
	{
	return iSessionNumber++;
	}

inline TInt CMsvServer::CheckEntries(const CMsvEntrySelection& aEntries)
	{
	return CheckEntries(aEntries, 0, aEntries.Count()-1);
	}

inline void CMsvServer::AddSessionIdToQueueL(CMsvMtmOperationQueue& aQueue, TInt aSessionId)
//
// Adds the session id to the queue session id array if it is not already on it
//
	{
	if (!SessionIdInQueue(aQueue, aSessionId))
		aQueue.iSessionIdArray.AppendL(aSessionId);
	}

inline CMsvTimer& CMsvServer::Delay() const
	{
	return *iDelayTimer;
	}

inline TBool CMsvServer::HasContext() const
	{
	return iContext != NULL;
	}

inline CMsvIndexContext& CMsvServer::Context() const
	{
	__ASSERT_DEBUG(iContext, User::Invariant());
	return *iContext;
	}

inline CMsvIndexContext* CMsvServer::NewContext() const
	{
	return iNewContext;
	}

inline void CMsvServer::DeleteNewContext()
	{
	delete iNewContext;
	iNewContext = NULL;
	}

inline TMsvServerChangeNotificationType CMsvServer::StartupState() const
	{
	return iStartupState;
	}

inline MRegisteredMtmDllObserver& CMsvServer::MtmObserver() const
	{
	return *iMtmRegControl;
	}

inline const CServerMtmDllRegistry& CMsvServer::Registry() const
	{
	return *iServerMtmReg;
	}

inline CMsvIndexAdapter& CMsvServer::IndexAdapter()
	{
	__ASSERT_DEBUG(iContext && iContext->IndexAdapter(), User::Invariant());
	return *iContext->IndexAdapter();
	}

inline const CMsvIndexAdapter& CMsvServer::IndexAdapter() const
	{
	__ASSERT_DEBUG(iContext && iContext->IndexAdapter(), User::Invariant());
	return *iContext->IndexAdapter();
	}


#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
inline const CMsvMessageDBAdapter& CMsvServer::MessageDBAdapter() const
	{
	__ASSERT_DEBUG(iMessageDBAdapter, User::Invariant());
	return *iMessageDBAdapter;
	}


inline CMsvMessageDBAdapter& CMsvServer::MessageDBAdapter()
	{
	__ASSERT_DEBUG(iMessageDBAdapter, User::Invariant());
	return *iMessageDBAdapter;	
	}
#endif


//**********************************
// CMsvMtmOperationQueue
//**********************************

inline TUid CMsvMtmOperationQueue::MtmUid() const
	{
	return iMtmUid;
	}

inline TMsvId CMsvMtmOperationQueue::ServiceId() const
	{
	return iServiceId;
	}

//**********************************
// CMsvOperationData
//**********************************

inline TMsvOp CMsvOperationData::OperationId() const
	{
	return iOpId;
	}

//**********************************
// CMsvServerSession
//**********************************

inline void CMsvServerSession::ReadMainBufferL(const RMessage2& aMessage, const TInt aParam)
	{
	ReadBufferL(aMessage, aParam, iBuffer);
	}

inline TBool CMsvServerSession::IsAnObserver()
	{
	return iObserverOnly;
	}

inline TInt CMsvServerSession::SessionId()
	{
	return iSessionId;
	}

inline TInt CMsvServerSession::HaveOutstandingOperations()
	{
	return iOperations.Count();
	}

inline const TDesC& CMsvServerSession::ProcessName() const
	{
	return iProcessName;
	}


inline TBool CMsvServerSession::ReceiveEntryEvents() const
	{
	return iReceiveEntryEvents;
	}


