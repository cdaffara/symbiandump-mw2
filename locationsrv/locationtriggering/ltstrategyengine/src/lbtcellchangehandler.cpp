/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class definition of cell change handler.
*
*/


#include "lbtcellchangehandler.h"
#include "lbtlogger.h"

// ======== MEMBER FUNCTIONS ========

// --------------------------------------------------------------------------
// CLbtCellChangeHandler::NewL
// CLbtCellChangeHandler instantiation method
// --------------------------------------------------------------------------
//
CLbtCellChangeHandler* CLbtCellChangeHandler::NewL(RMobilePhone& aMPhone)
    {
    FUNC_ENTER("CLbtCellChangeHandler::NewL");
    CLbtCellChangeHandler* self = new (ELeave) CLbtCellChangeHandler(aMPhone);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CLbtCellChangeHandler::CLbtCellChangeHandler
// 
// -----------------------------------------------------------------------------
//
CLbtCellChangeHandler::CLbtCellChangeHandler(RMobilePhone& aMPhone):
    CActive(EPriorityStandard), iMPhone(aMPhone),
    iCommandId(-1), iNwInfo(), iNwInfoPckg(iNwInfo), iLocArea()
    {
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CLbtCellChangeHandler::~CLbtCellChangeHandler
// 
// -----------------------------------------------------------------------------
//
CLbtCellChangeHandler::~CLbtCellChangeHandler()
    {
    FUNC_ENTER("CLbtCellChangeHandler::~CLbtCellChangeHandler");
    if (IsActive())
        {
        Cancel();
        }
    iObserverArray.Close();
    }


// -----------------------------------------------------------------------------
// CLbtCellChangeHandler::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CLbtCellChangeHandler::ConstructL()
    {
   
    }
        

// -----------------------------------------------------------------------------
// CLbtCellChangeHandler::RunL
// 
// -----------------------------------------------------------------------------
//
void CLbtCellChangeHandler::RunL()
    {
    FUNC_ENTER("CLbtCellChangeHandler::RunL");
    if( iStatus.Int() == KErrNone )
        {
        for( TInt i=0;i<iObserverArray.Count();i++ )
            {
            iObserverArray[i]->HandleCellChangeEvent( KErrNone, iNwInfo, iLocArea );
            }
        }
    iLastStatusInfo = iStatus.Int();
    
    if( !IsActive() )
        {
        iMPhone.NotifyCurrentNetworkChange( iStatus, iNwInfoPckg, iLocArea );
	    SetActive();
	    iCommandId = EMobilePhoneNotifyCurrentNetworkChange;   
        }
	}

// -----------------------------------------------------------------------------
// CLbtCellChangeHandler::DoCancel
// 
// -----------------------------------------------------------------------------
//
void CLbtCellChangeHandler::DoCancel()
    {
    FUNC_ENTER("CLbtCellChangeHandler::DoCancel");
    iMPhone.CancelAsyncRequest( iCommandId );
    }

// -----------------------------------------------------------------------------
// CLbtCellChangeHandler::GetNetworkInfo
// 
// -----------------------------------------------------------------------------
//
void CLbtCellChangeHandler::GetNetworkInfo()
    {
    FUNC_ENTER("CLbtCellChangeHandler::GetNetworkInfo");
    if(!IsActive())
        {
        iMPhone.GetCurrentNetwork( iStatus,iNwInfoPckg,iLocArea );
        SetActive();
        iCommandId = EMobilePhoneGetCurrentNetwork;
        }
    }

// -----------------------------------------------------------------------------
// CLbtCellChangeHandler::SetObserver
// 
// -----------------------------------------------------------------------------
//
void CLbtCellChangeHandler::SetObserver( MCellChangeObserver* aObserver )
    {
    FUNC_ENTER("CLbtCellChangeHandler::SetObserver");
    TInt error = iObserverArray.Append( aObserver );
    if( error != KErrNone )
        {
        LOG1("Failed to append observer to the array:%d",error);
        return;
        }
    // If cell change handler already has cell information,update it to the observer
    if( iLocArea.iCellId && iLastStatusInfo == KErrNone )
        {
        aObserver->HandleCellChangeEvent( iLastStatusInfo ,iNwInfo, iLocArea );
        }
    if( !IsActive() )
        {
        GetNetworkInfo();
        }
    }

// -----------------------------------------------------------------------------
// CLbtCellChangeHandler::Remove
// 
// -----------------------------------------------------------------------------
//
void CLbtCellChangeHandler::Remove( MCellChangeObserver* aObserver )
	{
	FUNC_ENTER("CLbtCellChangeHandler::Remove");
	if( iObserverArray.Count() )
	    {
	    TInt position = iObserverArray.Find( aObserver );	
    	if( position != KErrNotFound )
    		{
        	iObserverArray.Remove( position );
        	if( !iObserverArray.Count() )
        	    {
        	    Cancel();
        		}
    	    }
	    }
	}	
// End of File

