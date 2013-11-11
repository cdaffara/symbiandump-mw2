/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CEventStorage class implementation
*
*/
/*
* ============================================================================
*  Name:        ceventstorage.cpp
* 
*  Description: CEventStorage class implementation
*
* ============================================================================
*/

// Include files

#include <e32base.h>
#include <e32debug.h>
#include "ceventstorage.h"

// Forward declarations

// ---------------------------------------------------------------
// CEventStorage Constructor & Destructor
// ---------------------------------------------------------------

CEventStorage::CEventStorage()
	{
	}

CEventStorage::~CEventStorage()
	{
	iReceivedEvents.Reset();
	iReceivedEvents.Close();
	
	if ( iWait->IsStarted() ) iWait->AsyncStop();
	delete iWait;
	}


TBool TEvent::operator==(const TEvent aEvent) const
	{

	if ( this->iCallID == aEvent.iCallID  &&
	     this->iEventID == aEvent.iEventID  &&
	     this->iEventType == aEvent.iEventType &&
	     this->iParam1 == aEvent.iParam1 &&
	     this->iParam2 == aEvent.iParam2 )
		{
		return ETrue;
		}
		
	return EFalse;
	}

	
// ---------------------------------------------------------------
// CEventStorage::NewL()
//
// Allocate CEventStorage from heap
// ---------------------------------------------------------------
CEventStorage* CEventStorage::NewL()
	{
	CEventStorage* self = new (ELeave) CEventStorage();
	
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	
	return self;
	}
	
// ---------------------------------------------------------------
// CEventStorage::ConstructL()
// ---------------------------------------------------------------
void CEventStorage::ConstructL()
	{
	iWait = new (ELeave) CActiveSchedulerWait();
	}

	
void CEventStorage::StoreEvent( TEvent aEvent )
	{
	iMessageQueueCount--;
	
	if ( iWait->IsStarted() )
	    {
	    iWait->AsyncStop();
	    }
	
	__ASSERT_ALWAYS( iReceivedEvents.Append( aEvent ) == KErrNone, User::Panic( _L("CEventStorage::StoreEvent storing event failed"), -2) );
	
	RDebug::Print(_L("[CCETEST] CEventStorage::StoreEvent Event received (number:%d)"), iReceivedEvents.Count() );
	RDebug::Print(_L("[CCETEST] ### callID=%d eventType=%d eventID=%d par1=%d par2=%d"), 
	        aEvent.iCallID,
	        aEvent.iEventType,
	        aEvent.iEventID,
	        aEvent.iParam1,
	        aEvent.iParam2 );
	}

void CEventStorage::GetEvent()
    {
    iMessageQueueCount++;
    
    while ( iMessageQueueCount > 0 )
        {
        if ( !iWait->IsStarted() )
            {
            iWait->Start();
            }
        }
    }
// end of file

