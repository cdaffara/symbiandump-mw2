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
* Description:  Handles CallControl request that need SAT perform
*
*/


#include <etelmm.h>

#include "ccallcontrolrequesthandler.h"
#include "CCallControlHandler.h"
#include "SatLog.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCallControlRequestHandler::CCallControlRequestHandler
// The class constructor.
// -----------------------------------------------------------------------------
//
CCallControlRequestHandler::CCallControlRequestHandler(
                MSatMultiModeApi& aPhone, CCallControlHandler* aDispatcher )
    : CActive( EPriorityStandard ), iPhone( aPhone ), 
      iDispatcher( aDispatcher )
    {
    LOG( SIMPLE,
        "CALLCONTROL: CCallControlRequestHandler::CCallControlRequestHandler \
        calling" )

    CActiveScheduler::Add( this );

    LOG( SIMPLE,
        "CALLCONTROL: CCallControlRequestHandler::CCallControlRequestHandler \
        exiting" )
    }

// -----------------------------------------------------------------------------
// CCallControlRequestHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCallControlRequestHandler* CCallControlRequestHandler::NewL(
    MSatMultiModeApi& aPhone, CCallControlHandler* aDispatcher )
    {
    LOG( SIMPLE, "CALLCONTROL: CCallControlRequestHandler::NewL calling" )

    CCallControlRequestHandler* self =
        new ( ELeave ) CCallControlRequestHandler( aPhone, aDispatcher );
            
    LOG( SIMPLE, "CALLCONTROL: CCallControlRequestHandler::NewL exiting" )
    return self;
    }


// -----------------------------------------------------------------------------
// CCallControlRequestHandler::~CCallControlRequestHandler
// Destructor
// -----------------------------------------------------------------------------
//
CCallControlRequestHandler::~CCallControlRequestHandler()
    {
    LOG( SIMPLE, "CALLCONTROL: \
            CCallControlRequestHandler::~CCallControlRequestHandler calling" )

    Cancel();
    iDispatcher = NULL;

    LOG( SIMPLE, "CALLCONTROL: \
            CCallControlRequestHandler::~CCallControlRequestHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CCallControlRequestHandler::DialNumber
// -----------------------------------------------------------------------------
//
void CCallControlRequestHandler::DialNumber( const TDesC8& aCallParams,
                                             const TDesC& aTelNumber )
    {
    LOG( SIMPLE, "CALLCONTROL: CCallControlRequestHandler::DialNumber calling" )

    if ( !IsActive() )
        {
        iPhone.DialNoFdnCheck( iStatus, aCallParams, aTelNumber );
        SetActive();
        }
    else
        {
        iDispatcher->DispatchCcRequestComplete( KErrInUse );
        }

    LOG( SIMPLE, "CALLCONTROL: CCallControlRequestHandler::DialNumber exiting" )
    }

// -----------------------------------------------------------------------------
// CCallControlRequestHandler::SendSs
// -----------------------------------------------------------------------------
//
void CCallControlRequestHandler::SendSs( const TDesC& aServiceString )
    {
    LOG( SIMPLE, "CALLCONTROL: CCallControlRequestHandler::SendSs calling" )

    if ( !IsActive() )
        {        
        iPhone.SendNetworkServiceRequestNoFdnCheck( iStatus, aServiceString );
        SetActive();
        }
    else
        {
        iDispatcher->DispatchCcRequestComplete( KErrInUse );
        }

    LOG( SIMPLE, "CALLCONTROL: CCallControlRequestHandler::SendSs exiting" )
    }

// -----------------------------------------------------------------------------
// CCallControlRequestHandler::SendUssd
// -----------------------------------------------------------------------------
//
void CCallControlRequestHandler::SendUssd( const TDesC8& aMsgData,
		const TDesC8& aMsgAttributes )
    {
    LOG( SIMPLE, "CALLCONTROL: CCallControlRequestHandler::SendUssd calling" )

    if ( !IsActive() )
        {
        iPhone.SendMessageNoFdnCheck( iStatus, aMsgData, aMsgAttributes );
        SetActive();
        }
    else
        {
        iDispatcher->DispatchCcRequestComplete( KErrInUse );
        }

    LOG( SIMPLE, "CALLCONTROL: CCallControlRequestHandler::SendUssd exiting" )
    }


// -----------------------------------------------------------------------------
// CCallControlRequestHandler::RunL
// Handles the command.
// -----------------------------------------------------------------------------
//
void CCallControlRequestHandler::RunL()
    {
    LOG( SIMPLE, "CALLCONTROL: CCallControlRequestHandler::RunL calling" )
    
    if ( iStatus == KErrNone )
        {
        LOG( NORMAL, "CALLCONTROL: CCallControlRequestHandler::RunL\
             iStatus == KErrNone" )
        iDispatcher->DispatchCcRequestComplete( iCcStatus );
        }
    else
        {
        LOG2( NORMAL, "CALLCONTROL: CCallControlRequestHandler::RunL\
              iStatus == %i", iStatus.Int() )
        iDispatcher->DispatchCcRequestComplete( KErrGeneral );
        }
    
    LOG( SIMPLE, "CALLCONTROL: CCallControlRequestHandler::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CCallControlRequestHandler::CancelOperation
// -----------------------------------------------------------------------------
//
void CCallControlRequestHandler::CancelOperation( TInt aOperationType )
    {
    LOG( SIMPLE, "CALLCONTROL: \
                  CCallControlRequestHandler::CancelOperation calling" )

    LOG2( NORMAL, "CALLCONTROL: \
        CCallControlRequestHandler::CancelOperation callingOperation Type %d",
        aOperationType )

    switch( aOperationType )
        {
        case CCallControlHandler::ECcDialWaiting:
            {
            iPhone.DialCancel();
            break;
            }
        case CCallControlHandler::ECcSendSsWaiting:
            {
            iPhone.CancelAsyncRequest( 
                EMobilePhoneSendNetworkServiceRequestNoFdnCheck );
            break;
            }
        case CCallControlHandler::ECcSendUssdWaiting:
            {
            iPhone.CancelAsyncRequest( 
                EMobileUssdMessagingSendMessageNoFdnCheck );
            break;
            }
        }
    LOG( SIMPLE, 
        "CALLCONTROL: CCallControlRequestHandler::CancelOperation exiting" )
    }

// -----------------------------------------------------------------------------
// From class CActive.
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CCallControlRequestHandler::DoCancel()
    {
    LOG( SIMPLE, "CALLCONTROL: CCallControlRequestHandler::DoCancel calling" )

    LOG( SIMPLE, "CALLCONTROL: CCallControlRequestHandler::DoCancel exiting" )
    }


//  End of File
