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
* Description:  Definition of CLbtCellIdMvmtDetGetCellData class.
*
*/



#include "lbtcellidmvmtdetgetcelldata.h"
#include "lbtlogger.h"


// Class methods

//------------------------------------------------------------
// CLbtCellIdMvmtDetGetCellData::NewL
// Symbian Two - phase constructor
//------------------------------------------------------------
//
CLbtCellIdMvmtDetGetCellData* CLbtCellIdMvmtDetGetCellData::NewL(MLbtCellIdMvmtDetCellOberserver& aObserver,
												 RMmCustomAPI& aMnCustomApi)
	{
	FUNC_ENTER("CLbtCellIdMvmtDetGetCellData::NewL");
	CLbtCellIdMvmtDetGetCellData* self=new ( ELeave )
									  CLbtCellIdMvmtDetGetCellData( aObserver, aMnCustomApi );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

	
//------------------------------------------------------------
// CLbtCellIdMvmtDetGetCellData::~CLbtCellIdMvmtDetGetCellData
// Destructor
//------------------------------------------------------------
//	
CLbtCellIdMvmtDetGetCellData::~CLbtCellIdMvmtDetGetCellData()
	{
	FUNC_ENTER("CLbtCellIdMvmtDetGetCellData::~CLbtCellIdMvmtDetGetCellData");
	Cancel();
	iTimer.Close();
	}


//------------------------------------------------------------
// CLbtCellIdMvmtDetGetCellData::Start
//------------------------------------------------------------
//	
void CLbtCellIdMvmtDetGetCellData::Start()
    {
    FUNC_ENTER("CLbtCellIdMvmtDetGetCellData::Start");
    iStopNotification = EFalse;
    GetCellInfo();
    }

	
//------------------------------------------------------------
// CLbtCellIdMvmtDetGetCellData::GetCellInfo
//------------------------------------------------------------
//	
void CLbtCellIdMvmtDetGetCellData::GetCellInfo( )
	{
	FUNC_ENTER("CLbtCellIdMvmtDetGetCellData::GetCellInfo");
    if( !IsActive() )
        {
        iState = EProcessCellInfo;
        iCApi.GetCellInfo( iStatus, iCellPckg );
        iReqID = ECustomGetCellInfoIPC;
        SetActive();
        }
	}
	

//------------------------------------------------------------
// CLbtCellIdMvmtDetGetCellData::Stop
//------------------------------------------------------------
//
void CLbtCellIdMvmtDetGetCellData::Stop()
    {
    FUNC_ENTER("CLbtCellIdMvmtDetGetCellData::Stop");
    iStopNotification = ETrue;	
    iTimer.Cancel();
    iCApi.CancelAsyncRequest( iReqID );
    }
    	
	
//------------------------------------------------------------
// CLbtCellIdMvmtDetGetCellData::RunL
//------------------------------------------------------------
//	
void CLbtCellIdMvmtDetGetCellData::RunL()
	{
	FUNC_ENTER("CLbtCellIdMvmtDetGetCellData::RunL");
	TInt err = KErrNone;
	if( iStopNotification )
	    {
	    return;
	    }
    switch( iState )
        {
        case ETimerOperation:
            GetCellInfo();
            break;
            
        case EProcessCellInfo:
            err = iStatus.Int();
            iObserver.HandleCellDataL( iCellInfo, err );
            iState = ETimerOperation;
            iTimer.After( iStatus, 1000000 );
            SetActive();
            break;
            
        default:
            break;
        }
	}


//------------------------------------------------------------
// CLbtCellIdMvmtDetGetCellData::DoCancel
//------------------------------------------------------------
//	
void CLbtCellIdMvmtDetGetCellData::DoCancel()
	{
	FUNC_ENTER("CLbtCellIdMvmtDetGetCellData::DoCancel");
	iTimer.Cancel();
    iCApi.CancelAsyncRequest( iReqID );
	}	


//------------------------------------------------------------
// CLbtCellIdMvmtDetGetCellData::CLbtCellIdMvmtDetGetCellData
// C++ Default constructor
//------------------------------------------------------------
//	
CLbtCellIdMvmtDetGetCellData::CLbtCellIdMvmtDetGetCellData( 
                    MLbtCellIdMvmtDetCellOberserver& aObserver,
				    RMmCustomAPI& aMnCustomApi ) : CActive( EPriorityNormal),
					iObserver(aObserver),iCApi( aMnCustomApi ),iCellInfo(),
                    iCellPckg( iCellInfo )
	{
	CActiveScheduler::Add( this );										
	}
	    
	
//------------------------------------------------------------
// CLbtCellIdMvmtDetGetCellData::ConstructL
//------------------------------------------------------------
//	
void CLbtCellIdMvmtDetGetCellData::ConstructL()
	{
	FUNC_ENTER("CLbtCellIdMvmtDetGetCellData::ConstructL");
	iStopNotification = EFalse;
	iTimer.CreateLocal();
	}
