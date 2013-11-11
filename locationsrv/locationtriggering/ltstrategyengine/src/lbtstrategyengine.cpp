/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation Location Acquisition Strategy Engine
*
*/


#include <commdb.h>
#include "lbtlogger.h"
#include "lbtstrategyengine.h"
#include "lbtstrategyengineobserver.h"
#include "lbtcellchangehandler.h"


// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CLbtStrategyEngine::NewL
// CLbtStrategyEngine instantiation method
// -----------------------------------------------------------------------------
//
CLbtStrategyEngine* CLbtStrategyEngine::NewL( TAny* aConstructionParameters, 
                                          MLbtStrategyEngineObsrvr* aObserver )
    {
    FUNC_ENTER("CLbtStrategyEngine::NewL");
    
    CLbtStrategyEngine* self = new ( ELeave ) CLbtStrategyEngine();
    CleanupStack::PushL( self );
    self->ConstructL( aConstructionParameters, aObserver );
    CleanupStack::Pop();
    return self;
    
    }


// -----------------------------------------------------------------------------
// CLbtStrategyEngine::~CLbtStrategyEngine
// Default destructor
// -----------------------------------------------------------------------------
//
CLbtStrategyEngine::~CLbtStrategyEngine()
    {
    FUNC_ENTER("CLbtStrategyEngine::~CLbtStrategyEngine");
    
    // This is just done as a precautionary measure!
    StopSupervision();
    if( iView )
    	{
    	iView->RemContainerObserver();
    	}
    delete iView;
    
    iMPhone.Close();
    iTelServer.Close();
    
    
    delete iCoOrdinateSupervisor;
    delete iCellSupervisor;
    delete iCellChangeHandler;
    }	


// -----------------------------------------------------------------------------
// CLbtStrategyEngine::StartSupervision
// 
// -----------------------------------------------------------------------------
//
void CLbtStrategyEngine::StartSupervision()
    {
    FUNC_ENTER("CLbtStrategyEngine::StartSupervision");
    
    if( iSupervisionInitiated && iViewInitialised )
        {
        iCoOrdinateSupervisor->StartSupervision();
        iCellSupervisor->StartSupervision();
        }
    iSupervisionInitiated = ETrue;
    }

    
// -----------------------------------------------------------------------------
// CLbtStrategyEngine::StopSupervision
// 
// -----------------------------------------------------------------------------
//
void CLbtStrategyEngine::StopSupervision()
    {
    FUNC_ENTER("CLbtStrategyEngine::StopSupervision");
    
    iCellChangeHandler->Cancel();
    iCoOrdinateSupervisor->StopSupervision();
    iCellSupervisor->StopSupervision();
    }
    

// -----------------------------------------------------------------------------
// CLbtStrategyEngine::SettingsChanged
// Handles notification of triggering settings change provided by server
// -----------------------------------------------------------------------------
//
void CLbtStrategyEngine::SettingsChanged()
    {
    FUNC_ENTER("CLbtStrategyEngine::SettingsChanged");

    iCoOrdinateSupervisor->SettingsChanged();
    iCellSupervisor->SettingsChanged();
    }

    
// -----------------------------------------------------------------------------
// CLbtStrategyEngine::OperationComplete
// Handles notification from CLbtTriggerView when a refresh operation is  
// completed
// -----------------------------------------------------------------------------
//
void CLbtStrategyEngine::OperationComplete( 
                            MLbtTriggerViewObserver::TOperation aOperation )
    {
    FUNC_ENTER("CLbtStrategyEngine::OperationComplete");
    if( MLbtTriggerViewObserver::EViewRefreshed == aOperation )
        {
        if( ( iChangedAreaType & 
              MLbtContainerChangeEventObserver::ETypeCircularTriggerChange ) || 
              ( iChangedAreaType & MLbtContainerChangeEventObserver::ETypeAll ) )
            {
            if( iView->CoordTriggerCount() )
                {
                iCoOrdinateSupervisor->Resume();
                }
            else
                {
                iCoOrdinateSupervisor->Reset();
                }
            }
            
        if( ( iChangedAreaType & 
              MLbtContainerChangeEventObserver::ETypeCellularTriggerChange ) ||
              ( iChangedAreaType & MLbtContainerChangeEventObserver::ETypeAll ) )
            {
            if( iView->CellTriggerCount() )
                {
                iCellSupervisor->Resume();
                }
            else
                {
                iCellSupervisor->Reset();   
                }
            }
            
        iChangedAreaType = MLbtContainerChangeEventObserver::ETypeUnKnown;   
        }
        
    else if( MLbtTriggerViewObserver::EViewInitialized == aOperation )
        {
        iViewInitialised = ETrue;
        if( iSupervisionInitiated )
            {
            if( iView->CellTriggerCount() )
                {
                iCellSupervisor->StartSupervision();
                }
            if( iView->CoordTriggerCount() )
                {
                iCoOrdinateSupervisor->StartSupervision();
                }
            }
        }
        
    else
        {
        // TODO: Panic probably!
        }    
    }


// -----------------------------------------------------------------------------
// CLbtStrategyEngine::TriggerStoreChanged
// Handles notification from Location Triggering Container for trigger 
// creation / modification events
// -----------------------------------------------------------------------------
//
void CLbtStrategyEngine::TriggerStoreChanged( RArray<TLbtTriggerId>& aTriggerIds,
		                                      TLbtTriggerEventMask aEventMask,
		                                      TLbtContainerChangedAreaType aAreaType,
		                                      RArray<TUid>& /*aManagerUids*/ )
	{
	FUNC_ENTER("CLbtStrategyEngine::TriggerStoreChanged");
    if( aAreaType & MLbtContainerChangeEventObserver::ETypeCircularTriggerChange )
        {
        iChangedAreaType = aAreaType;
        iCoOrdinateSupervisor->Suspend();
        }
        
    else if( aAreaType & MLbtContainerChangeEventObserver::ETypeCellularTriggerChange )
        {
        iChangedAreaType = aAreaType;
        iCellSupervisor->StopSupervision();
        }
        
    else if( aAreaType & MLbtContainerChangeEventObserver::ETypeAll )
        {
        iChangedAreaType = aAreaType;
        iCoOrdinateSupervisor->Suspend();
        iCellSupervisor->StopSupervision();
        }
    
    // This is dummy position info, this is no significance.
    TPositionInfo dummy;
    iView->Refresh( dummy, aEventMask, aTriggerIds );
    }


// -----------------------------------------------------------------------------
// CLbtStrategyEngine::CLbtStrategyEngine
// C++ default constructor.
// -----------------------------------------------------------------------------
//
CLbtStrategyEngine::CLbtStrategyEngine()
    {
    
    }

    
// -----------------------------------------------------------------------------
// CLbtStrategyEngine::ConstructL
// Symbian 2nd phase constructor.
// -----------------------------------------------------------------------------
//
void CLbtStrategyEngine::ConstructL( TAny* aConstructionParameters,
                                        MLbtStrategyEngineObsrvr* aObserver )
    {	
    FUNC_ENTER("CLbtStrategyEngine::ConstructL");
	iView = CLbtTriggerView::NewL( this );
	iView->SetTriggerStatusObserver( aObserver );
	iView->SetContainerObserver( this );
	
	InitialiseEtelL();
	iCellChangeHandler = CLbtCellChangeHandler::NewL( iMPhone );
	
	iCoOrdinateSupervisor = CLbtCoOrdinateSupervisor::NewL( aConstructionParameters,iView,aObserver,iCellChangeHandler );
	
	iCellSupervisor = CLbtCellSupervisor::NewL(  aConstructionParameters,iView,aObserver,iCellChangeHandler );
	
	iViewInitialised = EFalse;
    iSupervisionInitiated = EFalse;
	}	


// ----------------------------------------------------------------------------
// CLbtStrategyEngine::InitialiseEtelL
// ----------------------------------------------------------------------------
//
void CLbtStrategyEngine::InitialiseEtelL()
    {
    FUNC_ENTER("CLbtCellSupervisor::InitialiseEtelL");
    CCommsDatabase* const db = CCommsDatabase::NewL(ETrue); 
	CleanupStack::PushL(db); 

	TUint32 modemId = 0; 
	db->GetGlobalSettingL(TPtrC(MODEM_PHONE_SERVICES_SMS), modemId); 
	CCommsDbTableView* const view =
	    db->OpenViewMatchingUintLC(TPtrC(MODEM), TPtrC(COMMDB_ID), modemId); 

	TInt err = view->GotoFirstRecord();
	if(err != KErrNone)
	    {
	    User::Leave(err);
	    }

    HBufC* nameBuf = NULL;
    nameBuf = view->ReadLongTextLC(TPtrC(MODEM_TSY_NAME));

    User::LeaveIfError(iTelServer.Connect());
	err = iTelServer.LoadPhoneModule(*nameBuf);
	
	if(err != KErrNone)
	    {
	    User::Leave(err);
	    }
		    
	// For the phone information
	RTelServer::TPhoneInfo info;

	iTelServer.GetPhoneInfo(0, info); 
	err = iMPhone.Open(iTelServer, info.iName);

	if(err != KErrNone)
	    {
	    User::Leave(err);
	    }

    CleanupStack::PopAndDestroy(3); // nameBuf, view and db 
	}
