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
* Description:  test source
*
*/


#include <spnotifychange.h>
#include "stubobserver.h"
#include <spsettings.h>
#include <spentry.h>
#include <spproperty.h>



// CONSTRUCTION

CStubObserver* CStubObserver::NewL(CActiveSchedulerWait* aWait)
    {
    CStubObserver* self = new( ELeave ) CStubObserver( aWait );
    CleanupStack::PushL( self );

    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// Destructor (virtual by CBase)
CStubObserver::~CStubObserver()
    {
    delete iNotifyChange;
    }

// Default constructor
CStubObserver::CStubObserver(CActiveSchedulerWait* aWait) : iWait( aWait )
    {
    }

// Second phase construct
void CStubObserver::ConstructL()
    {
    iNotifyChange = CSPNotifyChange::NewL( *this );
    }
    
    
void CStubObserver::HandleNotifyChange( TServiceId aServiceId )
	{
    if (iLeave)
        {
        User::Leave(KErrGeneral);
        }
	    
	iCalledTimes++;
	iServiceId = aServiceId;

	CSPSettings* settings = CSPSettings::NewLC();
    CSPEntry* entry = CSPEntry::NewLC();
    
    TInt err = settings->FindEntryL( aServiceId, *entry );
    if ( KErrNoMemory == err ) { User::Leave(err); }
    
    CleanupStack::PopAndDestroy( entry );
    CleanupStack::PopAndDestroy( settings );
    
    if( err == KErrNone )
    	{
    	iFound = ETrue;
    	}
    else
    	{
    	iFound = EFalse;
    	}
    
	iWait->AsyncStop();
	}
	
void CStubObserver::HandleError( TInt  aError  )
	{
		iWait->AsyncStop();
	}

void CStubObserver::NotifyChangeL( RIdArray& aIdArray )
	{
	iNotifyChange->NotifyChangeL( aIdArray );
	}
	
TInt CStubObserver::GetServiceId()
	{
	return iServiceId;
	}
void CStubObserver::LeaveWhenNotified(TBool aLeave)
    {  
      iLeave = aLeave;
    }
TInt CStubObserver::GetCalledTimes()
	{
	return iCalledTimes;
	}
	
TInt CStubObserver::AddNewEntryL()
	{
	CSPSettings* settings = CSPSettings::NewLC();
    CSPEntry* entry = CSPEntry::NewLC();
    _LIT( KTestName, "test123" );
    TInt err = entry->SetServiceName( KTestName );
    if ( err == KErrNoMemory ) { User::Leave( err ); }
    
    err = settings->AddEntryL( *entry );
    if ( err == KErrNoMemory ) { User::Leave( err ); }
    iWait->Start();
    
    TServiceId id = entry->GetServiceId();
    
    CleanupStack::PopAndDestroy( entry );
    CleanupStack::PopAndDestroy( settings );
    
    return id;
	}
	
TBool CStubObserver::GetFound()
	{
	return iFound;
	}
    
void CStubObserver::CancelNotify()
	{
	iNotifyChange->NotifyChangeCancel();
	}

