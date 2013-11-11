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
* Description:  Implementation of Location Requestor
*
*/



#include "lbtlogger.h"
#include "lbtlocationrequestor.h"
#include "lbtpsychangelistner.h"

_LIT( KServer, "LocationTriggering" );

// CONSTANTS
const TInt64 KMaxUpdateAgeForNpp( 7200000000 ); // 120 minutes or 2 hours


// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CLbtLocationRequestor::NewL
// CLbtLocationRequestor instantiation method.
// -----------------------------------------------------------------------------
//
CLbtLocationRequestor* CLbtLocationRequestor::NewL( )
    {
    CLbtLocationRequestor* self = new ( ELeave ) CLbtLocationRequestor;
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop();
    return self;
    }
    

// -----------------------------------------------------------------------------
// CLbtLocationRequestor::~CLbtLocationRequestor
// Destructor.
// -----------------------------------------------------------------------------
//
CLbtLocationRequestor::~CLbtLocationRequestor()
    {
    if( iValidNppHandle )
    	{
    	iNppPositioner.Close();
    	}    
    delete iNbpChangeListener;
    iPositioner.Close();
    iPosServer.Close();    
    }
	
	
// -----------------------------------------------------------------------------
// CLbtLocationRequestor::CurrentLocation
// Method used by CLbtStrategyEngine to obtain position information
// -----------------------------------------------------------------------------
//
void CLbtLocationRequestor::CurrentLocation( TPositionInfo& aPosInfo, 
											  TRequestStatus& aStatus, 
											  TPositionModuleId aPosID,
											  TTimeIntervalMicroSeconds aTimeOut )
    {
    FUNC_ENTER("CLbtLocationRequestor::CurrentLocation");
    if( aPosID != iPosID )
        {
        iPositioner.Close();
        TRAPD( err, OpenConnectionL( aPosID ) );
        if( KErrNone != err )
            {
            TRequestStatus* lStatus = &aStatus;
            User::RequestComplete( lStatus, err );
            return;
            }
        }
    
    // Setting update options
    TPositionUpdateOptions updateOptions;
    updateOptions.SetUpdateTimeOut( aTimeOut );
    iPositioner.SetUpdateOptions( updateOptions );
    
    iPositioner.NotifyPositionUpdate( aPosInfo, aStatus );
    }

// -----------------------------------------------------------------------------
// CLbtLocationRequestor::CurrentLocationFromNpp
// Method used by CLbtStrategyEngine to obtain position information
// -----------------------------------------------------------------------------
//
void CLbtLocationRequestor::CurrentLocationFromNpp( TPositionInfo& aPosInfo, 
                                                    TRequestStatus& aStatus,
                                                    TTimeIntervalMicroSeconds /*aTimeOut*/ )
    {
    FUNC_ENTER("CLbtLocationRequestor::CurrentLocationFromNpp");
    LOG("CLbtLocationRequestor::CurrentLocationFromNpp");
    if( !iValidNppHandle )
        {
        TRequestStatus* statusPtr = &aStatus;
        User::RequestComplete( statusPtr,KErrNotFound );
        return;
        }
    
    // Setting update options
    TPositionUpdateOptions updateOptions;
    updateOptions.SetMaxUpdateAge( TTimeIntervalMicroSeconds(KMaxUpdateAgeForNpp) );
    updateOptions.SetUpdateTimeOut( 30000000 );
    iNppPositioner.SetUpdateOptions( updateOptions );
    
    iNppPositioner.NotifyPositionUpdate( aPosInfo, aStatus );
    }
	

// -----------------------------------------------------------------------------
// CLbtLocationRequestor::GetModuleInfo
// Retrieves information about the positioning technology being used
// -----------------------------------------------------------------------------
//
void CLbtLocationRequestor::GetModuleInfo( TPositionModuleId aPosID )
    {
    TPositionModuleInfo moduleInfo;
    iPosServer.GetModuleInfoById( aPosID, moduleInfo );
    TPositionModuleInfo::ECapabilitySatellite & moduleInfo.Capabilities()?
        iGPSUsed = ETrue : iGPSUsed = EFalse;
    }


// -----------------------------------------------------------------------------
// CLbtLocationRequestor::GPSUsed
// Determines if GPS method is used to retrieve location information
// -----------------------------------------------------------------------------
//
TBool CLbtLocationRequestor::GPSUsed() const
    {
    return iGPSUsed;
    }
    
    
// -----------------------------------------------------------------------------
// CLbtLocationRequestor::Cancel
// Cancels all outstanding requests to the location server. This method 
// -----------------------------------------------------------------------------
//
void CLbtLocationRequestor::Cancel()
    {
    iPositioner.CancelRequest( EPositionerNotifyPositionUpdate );
    }

// -----------------------------------------------------------------------------
// CLbtLocationRequestor::CancelFixFromNpp
// Cancels all outstanding requests to the location server. This method 
// -----------------------------------------------------------------------------
//
void CLbtLocationRequestor::CancelFixFromNpp()
    {
    if( iValidNppHandle )
    	{
    	iNppPositioner.CancelRequest( EPositionerNotifyPositionUpdate );
    	}    
    }
	

// -----------------------------------------------------------------------------
// CLbtLocationRequestor::CLbtLocationRequestor
// Default constructor.
// -----------------------------------------------------------------------------
//
CLbtLocationRequestor::CLbtLocationRequestor()
    {
    }
	

// -----------------------------------------------------------------------------
// CLbtLocationRequestor::ConstructL
// Symbian 2nd phase constructor.
// -----------------------------------------------------------------------------
//
void CLbtLocationRequestor::ConstructL()
    {
    User::LeaveIfError( iPosServer.Connect() );
    
    iPosID = KPositionNullModuleId;
    
    // TODO: Read this from CenRep
    TPositionModuleId nppPsyID = { 0x10206915 };
    TInt error = iNppPositioner.Open( iPosServer,nppPsyID ) ;
    
    if( error == KErrNone )
        {
        LOG("NBP Session Opened");
        iNbpChangeListener = NULL;
        User::LeaveIfError( iNppPositioner.SetRequestor( 
                                            CRequestor::ERequestorService,
                                            CRequestor::EFormatApplication,
                                            KServer ) );
        iValidNppHandle = ETrue;
        }
    else
        {
        iValidNppHandle = EFalse;
        iNbpChangeListener = CLbtPsyChangeListner::NewL(*this);
        iNbpChangeListener->StartToListen( nppPsyID );
        }
    }


// -----------------------------------------------------------------------------
// CLbtLocationRequestor::OpenConnectionL
// Resets the position method used for acquiring location information. This is 
// done only when the module ID specified differs from the the existing ID. 
// -----------------------------------------------------------------------------
//
void CLbtLocationRequestor::OpenConnectionL( TPositionModuleId aPosID )
    {
    FUNC_ENTER("CLbtLocationRequestor::OpenConnectionL");  	
    
    if( aPosID != iPosID )
        {
        iPosID = aPosID;
        TUid defaultID = { 0XFFFFFFFF };
        if( iPosID == defaultID )
            {
            User::LeaveIfError( iPositioner.Open( iPosServer) );
            }
        else
            {
            User::LeaveIfError( iPositioner.Open( iPosServer, aPosID ) );
            }        

        User::LeaveIfError( iPositioner.SetRequestor( 
                                    CRequestor::ERequestorService,
                                    CRequestor::EFormatApplication,
                                    KServer ) );
        }
    }

// -----------------------------------------------------------------------------
// CLbtLocationRequestor::HandlePsyChangeEvent
// -----------------------------------------------------------------------------
//
void CLbtLocationRequestor::HandlePsyChangeEvent( TPositionModuleId aModuleId )
	{
	FUNC_ENTER("CLbtLocationRequestor::HandlePsyChangeEvent");
	// TODO: Read this from CenRep
	TPositionModuleId nppPsyID = { 0x10206915 };
	
	if( aModuleId == nppPsyID )
		{
		TInt error = iNppPositioner.Open( iPosServer,nppPsyID ) ;
    
	    if( error == KErrNone )
	        {
	        LOG("PSY Change Listener: NBP Session Opened");
	        delete iNbpChangeListener;
	        iNbpChangeListener = NULL;
	        TInt error = iNppPositioner.SetRequestor( 
	                                            CRequestor::ERequestorService,
	                                            CRequestor::EFormatApplication,
	                                            KServer );
	        if( error == KErrNone )
	            {
	            iValidNppHandle = ETrue;
	            }
	        else
	            {
	            iValidNppHandle = EFalse;
	            }
	        }	    
		}
	}

// -----------------------------------------------------------------------------
// CLbtLocationRequestor::GetPositionModuleId
// 
// -----------------------------------------------------------------------------
//
TPositionModuleId CLbtLocationRequestor::GetPositionModuleId()
    {
    return iPosID;
    }

// end of file
