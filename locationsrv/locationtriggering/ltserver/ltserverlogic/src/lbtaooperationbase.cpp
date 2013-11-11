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
* Description:  Implementation of Base class of Operation Object
*
*/


// INCLUDE FILES
#include "lbtaooperationbase.h"
#include "lbtlogger.h"

// ---------------------------------------------------------------------------
// CLbtAOOperationBase::CLbtAOOperationBase
// Default C++ Constructor
// ---------------------------------------------------------------------------
//
CLbtAOOperationBase::CLbtAOOperationBase(
    MLbtAOObserver& aObserver, 
    const RMessage2& aMessage,
    CLbtContainer& aContainer,
    TInt aClientType)
	: CActive( EPriorityUserInput ), iObserver(aObserver),  
	iContainer(aContainer), iClientType(aClientType)
	{
	FUNC_ENTER("CLbtAOOperationBase::CLbtAOOperationBase");
	iMessage = aMessage;
    // These values cannot be accessed once the RMessage::Complete is called
    // on the iMessage. Hence we store these values for later access.
	iSecureId = iMessage.SecureId();
	
	// Save the session pointer
	iSession = iMessage.Session();
	
	// Save the service id
	iFunction = iMessage.Function();
	}


// ---------------------------------------------------------------------------
// CLbtAOOperationBase::CLbtAOOperationBase
// C++ Destructor
// ---------------------------------------------------------------------------
//
CLbtAOOperationBase::~CLbtAOOperationBase()
	{
	FUNC_ENTER("CLbtAOOperationBase::~CLbtAOOperationBase");
	iManagerUidArray.Close();
	}

// ---------------------------------------------------------------------------
// CLbtAOOperationBase::GetSecureId
// ---------------------------------------------------------------------------
//
const TSecureId& CLbtAOOperationBase::GetSecureId()
	{
	FUNC_ENTER("CLbtAOOperationBase::GetSecureId");
	return iSecureId;
	}

// ---------------------------------------------------------------------------
// CLbtAOOperationBase::GetSession
// ---------------------------------------------------------------------------
//
const CSession2* CLbtAOOperationBase::GetSession()
	{
	FUNC_ENTER("CLbtAOOperationBase::GetSession");
	return iMessage.Session();
	}

// ---------------------------------------------------------------------------
// CLbtCreateTriggerAOOperation::GetFunction
// ---------------------------------------------------------------------------
//
TInt CLbtAOOperationBase::GetFunction()
	{
	FUNC_ENTER("CLbtAOOperationBase::GetFunction");
	return iFunction;
	}

// ---------------------------------------------------------------------------
// CLbtAOOperationBase::GetClientType
// ---------------------------------------------------------------------------
//
TInt CLbtAOOperationBase::GetClientType()
	{
	FUNC_ENTER("CLbtAOOperationBase::GetClientType");
	return iClientType;
	}

// ---------------------------------------------------------------------------
// CLbtAOOperationBase::GetSubSessionHandle
// ---------------------------------------------------------------------------
//
TInt CLbtAOOperationBase::GetSubSessionHandle()
    {
    FUNC_ENTER("CLbtAOOperationBase::GetSubSessionHandle");
    return iMessage.Int3();
    }

// ---------------------------------------------------------------------------
// CLbtAOOperationBase::TriggerStoreChanged
// ---------------------------------------------------------------------------
//
void CLbtAOOperationBase::TriggerStoreChanged( RArray<TLbtTriggerId>& /*aTriggerIds*/,
                                  TLbtTriggerEventMask /*aEventMask*/,
                                  TLbtContainerChangedAreaType /*aAreaType*/,
                                  RArray<TUid>& aManagerUids )
    {
    for( TInt i=0;i<aManagerUids.Count();i++ )
        {
        TInt error = iManagerUidArray.Append( aManagerUids[i] );
        if( error != KErrNone )
            {
            LOG1("Failed to append manager uids to array:%d",error);
            return;
            }
        }
    }

// ---------------------------------------------------------------------------
// CLbtAOOperationBase::GetManagerArray
// ---------------------------------------------------------------------------
//
const RArray<TUid>& CLbtAOOperationBase::GetManagerArray()
    {
    return iManagerUidArray;
    }

// ---------------------------------------------------------------------------
// CLbtAOOperationBase::RunError
// ---------------------------------------------------------------------------
//
TInt CLbtAOOperationBase::RunError( TInt aError )
    {
    iMessage.Complete( aError );
    //iObserver.HandleOperationClosureL(this,aError);
    return KErrNone;
    }
// end of file
