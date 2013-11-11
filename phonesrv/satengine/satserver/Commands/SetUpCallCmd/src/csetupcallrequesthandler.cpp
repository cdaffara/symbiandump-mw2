/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Create the call and emergency call to ETelMM
*
*/


#include <etelmm.h>
#include <mmtsy_names.h>

#include "csetupcallrequesthandler.h"
#include "CSetUpCallHandler.h"
#include "csetupcalldtmfsender.h"
#include "msatasynctosync.h"
#include "SatLog.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CSetupCallRequestHandler::CSetupCallRequestHandler
// The class constructor.
// -----------------------------------------------------------------------------
//
CSetupCallRequestHandler::CSetupCallRequestHandler(
                MSatMultiModeApi& aPhone, CSetUpCallHandler* aDispatcher )
    : CActive( EPriorityStandard ), iPhone( aPhone ), 
      iDispatcher( aDispatcher )
    {
    LOG( SIMPLE,
        "SETUPCALL: CSetupCallRequestHandler::CSetupCallRequestHandler \
        calling" )

    CActiveScheduler::Add( this );

    LOG( SIMPLE,
        "SETUPCALL: CSetupCallRequestHandler::CSetupCallRequestHandler \
        exiting" )
    }

// -----------------------------------------------------------------------------
// CSetupCallRequestHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSetupCallRequestHandler* CSetupCallRequestHandler::NewL(
    MSatMultiModeApi& aPhone, CSetUpCallHandler* aDispatcher )
    {
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::NewL calling" )

    CSetupCallRequestHandler* self =
        new ( ELeave ) CSetupCallRequestHandler( aPhone, aDispatcher );
    
    self->ConstructL();
 
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSetupCallRequestHandler::~CSetupCallRequestHandler
// Destructor
// -----------------------------------------------------------------------------
//
CSetupCallRequestHandler::~CSetupCallRequestHandler()
    {
    LOG( SIMPLE, "SETUPCALL: \
            CSetupCallRequestHandler::~CSetupCallRequestHandler calling" )
    Cancel();
    iDispatcher = NULL;
    
    delete iDtmfSender;
    iDtmfSender = NULL;

    LOG( SIMPLE, "SETUPCALL: \
            CSetupCallRequestHandler::~CSetupCallRequestHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSetupCallRequestHandler::ConstructL
// -----------------------------------------------------------------------------
//
void CSetupCallRequestHandler::ConstructL()
{
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::ConstructL calling" )
    iDtmfSender = CSetupCallDtmfSender::NewL( iPhone );
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::ConstructL exiting" )
}

// -----------------------------------------------------------------------------
// CSetupCallRequestHandler::DialNumber
// -----------------------------------------------------------------------------
//
void CSetupCallRequestHandler::DialNumber( const TDesC8& aCallParams,
                  TDes& aTelNumber, TBool aTerminateOtherCall,
                  MSatAsyncToSync* aAsyncToSync )
    {
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::DialNumber calling" )
 
    TInt terminateRes ( KErrNone );
    
    if( aTerminateOtherCall )
        {
        if( aAsyncToSync )
            {
            iPhone.TerminateActiveCalls( aAsyncToSync->RequestStatus() );
            terminateRes = aAsyncToSync->SetActiveAndWait();
            }
        else
            {
            LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::DialNumber \
                    bad MSatAsyncToSync pointer" )
            terminateRes  = KErrGeneral;
            }
        }
    
    if( !IsActive() )
        {
        if( KErrNone == terminateRes )
            {
            // Separate dtmf string from whole number and store in iDtmfString
            // the dtmf string will be removed from aTelNumber 
            iDtmfSender->SeparateDtmfFromTelNumber(aTelNumber);
            // Dial the pure tel number
            iPhone.DialNoFdnCheck( iStatus, aCallParams, aTelNumber );
            SetActive();
            }
        else
            {
            iDispatcher->SetupCallRequestComplete( terminateRes );
            }
        }
    else
        {
        iDispatcher->SetupCallRequestComplete( KErrInUse );
        }
    
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::DialNumber exiting" )
    }

// -----------------------------------------------------------------------------
// CSetupCallRequestHandler::DialEmergencyCall
// -----------------------------------------------------------------------------
//
void CSetupCallRequestHandler::DialEmergencyCall( const TDesC& aTelNumber )
    {
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::DialNumber calling" )
     
    if( !IsActive() )
        {
        iPhone.DialEmergencyCall( iStatus, aTelNumber );
        SetActive();
        }
    else
        {
        iDispatcher->SetupCallRequestComplete( KErrInUse );
        }
    
    LOG( SIMPLE, 
        "SETUPCALL: CSetupCallRequestHandler::DialEmergencyCall exiting" )
    }

// -----------------------------------------------------------------------------
// CSetupCallRequestHandler::RunL
// Handles the command.
// -----------------------------------------------------------------------------
//
void CSetupCallRequestHandler::RunL()
    {
    LOG2( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::RunL calling\
        iStatus=%i", iStatus.Int() )
    
    iDispatcher->SetupCallRequestComplete( iStatus.Int() );
    
    if ( KErrNone == iStatus.Int() )
        {
        iDtmfSender->SendDtmfString();
        }
    
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSetupCallRequestHandler::CancelOperation
// -----------------------------------------------------------------------------
//
void CSetupCallRequestHandler::CancelOperation()
    {
    LOG( SIMPLE, "SETUPCALL: \
                  CSetupCallRequestHandler::CancelOperation calling" )
    iPhone.DialCancel();
    iDtmfSender->Cancel();
    LOG( SIMPLE, 
        "SETUPCALL: CSetupCallRequestHandler::CancelOperation exiting" )
    }

// -----------------------------------------------------------------------------
// From class CActive.
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CSetupCallRequestHandler::DoCancel()
    {
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::DoCancel calling" )
    CancelOperation();
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::DoCancel exiting" )
    }

//  End of File
