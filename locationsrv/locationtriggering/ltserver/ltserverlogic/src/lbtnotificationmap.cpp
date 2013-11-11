/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Defines the notification map data structure used by Location 
*				 Triggering Server Logic to store notification request messages
*
*/


//  INCLUDE FILES
#include <e32base.h>
#include "lbtnotificationmap.h"
#include "lbtglobal.h"
#include "lbtlogger.h"

// ======== LOCAL FUNCTIONS ========

TInt OrderNotifReqs(
    const CLbtNotificationMap::TLbtNotificationReq& aLhs, 
    const CLbtNotificationMap::TLbtNotificationReq& aRhs)
	{
	FUNC_ENTER("CLbtNotificationMap::OrderNotifReqs");
	const RMessage2& lhs = aLhs.iMsg;
	const RMessage2& rhs = aRhs.iMsg;

	if(lhs.SecureId() == rhs.SecureId())
		{
		return 0;
		}
	if(lhs.SecureId() < rhs.SecureId())
		{
		return -1;
		}
	else
		{
		return 1;
		}
	}


TInt SecureIdCompare(CSession2** aSession, RMessage2& aMessage)
	{
	FUNC_ENTER("CLbtNotificationMap::SecureIdCompare");
	if(aMessage.Session() == (*aSession))
		{
		return 0;
		}
	if(aMessage.Session() < (*aSession))
		{
		return -1;
		}
	else
		{
		return 1;
		}
	}

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtNotificationMap::NewL
// Symbian Two - phase constructor
// ---------------------------------------------------------------------------
//
CLbtNotificationMap* CLbtNotificationMap::NewL()
	{
	FUNC_ENTER("CLbtNotificationMap::NewL");
	CLbtNotificationMap* self = new ( ELeave ) CLbtNotificationMap;
    CleanupStack::PushL( self );
    self->ConstructL();    
    CleanupStack::Pop();
    return self;
	}

// ---------------------------------------------------------------------------
// CLbtNotificationMap::~CLbtNotificationMap
// Destructor
// ---------------------------------------------------------------------------
//
CLbtNotificationMap::~CLbtNotificationMap()
	{
	FUNC_ENTER("CLbtNotificationMap::~CLbtNotificationMap");
	Reset(KErrServerBusy);
	iMessageArray.Close();
	}

// ---------------------------------------------------------------------------
// CLbtNotificationMap::InsertL
// ---------------------------------------------------------------------------
//
void CLbtNotificationMap::InsertL(
    const RMessage2& aMessage,
    CLbtServerLogicBase::TSubSessionType aType)	
	{
	FUNC_ENTER("CLbtNotificationMap::InsertL");
	TLbtNotificationReq req = { aMessage, aType };

	//Insert in sorted order into to the message array
	TLinearOrder<TLbtNotificationReq> OrderPredicate(OrderNotifReqs);
	iMessageArray.InsertInOrderAllowRepeats(req, OrderPredicate);
//	iMessageArray.Append(req);
	}

// ---------------------------------------------------------------------------
// CLbtNotificationMap::Retreive
// ---------------------------------------------------------------------------
//
TInt CLbtNotificationMap::Retreive(RMessage2& aMessage, 
				    			   TUint32 aSubSessionHandle, 
				    			   TSecureId aSecureId,
				    			   TInt aServiceId,
				    			   CSession2* aSession)
	{
	FUNC_ENTER("CLbtNotificationMap::Retreive");
	for(TInt i=0; i<iMessageArray.Count(); ++i)
		{
		
		RMessage2& msg = iMessageArray[i].iMsg;
		if( msg.SecureId() == aSecureId &&
		    msg.Int3() == aSubSessionHandle && 
		    msg.Function() == aServiceId && 
		    msg.Session() == aSession )
			{
			aMessage = msg;
			iMessageArray.Remove(i);
			return KErrNone;
			}
		}

	return KErrNotFound;
	}

// ---------------------------------------------------------------------------
// CLbtNotificationMap::Retreive
// ---------------------------------------------------------------------------
//
TInt CLbtNotificationMap::Retreive(RMessage2& aMessage, 
				    			   TSecureId aSecureId,
				    			   TInt aServiceId)
	{
	FUNC_ENTER("CLbtNotificationMap::Retreive");
	for(TInt i=0; i<iMessageArray.Count(); ++i)
		{
		RMessage2& msg = iMessageArray[i].iMsg;
		if( msg.SecureId() == aSecureId &&
			msg.Function() == aServiceId )
			{
			aMessage = msg;
			iMessageArray.Remove(i);
			return KErrNone;
			}
		}
	return KErrNotFound;
	}

// ---------------------------------------------------------------------------
// CLbtNotificationMap::Retreive
// ---------------------------------------------------------------------------
//
TInt CLbtNotificationMap::Retreive(RMessage2& aMessage, 
                                   TInt aSubSessionHandle,
                                   TSecureId aSecureId)
    {
    FUNC_ENTER("CLbtNotificationMap::Retreive");
    for(TInt i=0; i<iMessageArray.Count(); ++i)
        {
        RMessage2& msg = iMessageArray[i].iMsg;
        if( msg.SecureId() == aSecureId && 
            msg.Int3() == aSubSessionHandle )
            {
            aMessage = msg;
            iMessageArray.Remove(i);
            return KErrNone;
            }
        }
    return KErrNotFound;
    }


// ---------------------------------------------------------------------------
// CLbtNotificationMap::Retreive
// ---------------------------------------------------------------------------
//
TInt CLbtNotificationMap::Retreive(RMessage2& aMessage, 
				    			   TInt aServiceId,
				    			   CLbtServerLogicBase::TSubSessionType aType)
	{
	FUNC_ENTER("CLbtNotificationMap::Retreive");
	for(TInt i=0; i<iMessageArray.Count(); ++i)
		{
		RMessage2& msg = iMessageArray[i].iMsg;
		if( msg.Function() == aServiceId &&
			iMessageArray[i].iType == aType )
			{
			aMessage = msg;
			iMessageArray.Remove(i);
			return KErrNone;
			}
		}
	return KErrNotFound;
	}

// ---------------------------------------------------------------------------
// CLbtNotificationMap::CompleteMessagesOfSubsession
// ---------------------------------------------------------------------------
//
void CLbtNotificationMap::CompleteMessagesOfSubsession(const CSession2* aSession, const TInt aSubSessionHandle, TInt aErrorCode)
	{
	FUNC_ENTER("CLbtNotificationMap::CompleteMessagesOfSubsession");
	for(TInt i=0;i<iMessageArray.Count();++i)
		{
		RMessage2& message = iMessageArray[i].iMsg;
		if(message.Session() == aSession && 
		   message.Int3() == aSubSessionHandle)
			{
			LbtGlobal::RequestComplete(message, aErrorCode);
			iMessageArray.Remove(i);
			}
		}
	}
// ---------------------------------------------------------------------------
// CLbtNotificationMap::CompleteMessagesOfSubsession
// ---------------------------------------------------------------------------
//
void CLbtNotificationMap::RemoveAllClientMessages(TSecureId aSecureId)	
	{
	FUNC_ENTER("CLbtNotificationMap::RemoveAllClientMessages");
	for(TInt i=0;i<iMessageArray.Count();i++)
		{
		RMessage2& message = iMessageArray[i].iMsg;
		if(message.SecureId() == aSecureId)
			{
			LbtGlobal::RequestComplete(message,KErrDied);
			iMessageArray.Remove(i);
			}
		}
	}

// ---------------------------------------------------------------------------
// CLbtNotificationMap::Reset
// ---------------------------------------------------------------------------
//
void CLbtNotificationMap::Reset(TInt aErrorCode)
	{
	FUNC_ENTER("CLbtNotificationMap::Reset");
	for(TInt i=0;i<iMessageArray.Count();++i)
		{
		const RMessage2& message = iMessageArray[i].iMsg;
		LbtGlobal::RequestComplete(message, aErrorCode);
		}
	iMessageArray.Reset();
	}

// ---------------------------------------------------------------------------
// CLbtNotificationMap::ConstructL
// ---------------------------------------------------------------------------
//
void CLbtNotificationMap::ConstructL()
	{
	FUNC_ENTER("CLbtNotificationMap::ConstructL");
	}

// end of file
