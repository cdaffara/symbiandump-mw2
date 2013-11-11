/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This is NPP PSY Positioning Plug-in ( PSY ).
*
*/



// INCLUDE FILES
#include <e32std.h>            
#include <lbspositioninfo.h>      // TPositionInfoBase
#include <lbs/epos_cpositioner.h> // CPositioner

#include "npppsypositioner.h"
#include "npppsysingletonhandler.h"
#include "npppsysettinghandler.h"
#include "npppsystatushandler.h"
#include "npppsyrequester.h"
#include "npppsylogging.h"
#include "npppsypanic.h"
#include "npppsy.hrh"



// ========================== == MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CNppPsyRequester::CNppPsyRequester
// C++default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CNppPsyRequester::CNppPsyRequester( CNppPsyPositioner& aPositioner )
    : CActive( CActive::EPriorityStandard ),
      iNppPsyPositioner( aPositioner )
    {
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
// CNppPsyRequester::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CNppPsyRequester::ConstructL()
    {
    TRACESTRING( "CNppPsyRequester::ConstructL start" )
    
    //Singleton handler
    iSingletonHandler = CNppPsySingletonHandler::GetInstanceL( 
        iNppPsyPositioner.StatusReportInterface() );
    
    TRACESTRING( "CNppPsyRequester::ConstructL end" )
    }


// -----------------------------------------------------------------------------
// CNppPsyRequester::NewL
// Two - phased constructor.
// -----------------------------------------------------------------------------
//
CNppPsyRequester* CNppPsyRequester::NewL( CNppPsyPositioner& aPositioner )
    {
    CNppPsyRequester* self = new( ELeave ) CNppPsyRequester( aPositioner );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

    
// -----------------------------------------------------------------------------
// CNppPsyRequester::~CNppPsyRequester
// 
// -----------------------------------------------------------------------------
//
CNppPsyRequester::~CNppPsyRequester()
    {
    Cancel();
    
    iPsyList.Close();
    delete iPositioner;
    
    if ( iSingletonHandler )
        {
        iSingletonHandler->ReleaseInstance();
        }
        
    TRACESTRING( "CNppPsyRequester:: destructed" )
    }


// -----------------------------------------------------------------------------
// CNppPsyRequester::NotifyPositionUpdate
// 
// -----------------------------------------------------------------------------
//
void CNppPsyRequester::NotifyPositionUpdate ( 
    TPositionInfoBase& aPosInfo,  
    TRequestStatus&    aStatus )
    {
    TRACESTRING( "CNppPsyRequester::NotifyPositionUpdate start" )

    //NPP PSY will not handle simultaneous location request
    if ( iRequestStatus )
        {
        Panic( EPanicPositionRequestOngoing );
        }
    
    //Report the request is made
    iSingletonHandler->StatusHandler().IncRequestCount();

    aStatus = KRequestPending;
    iRequestStatus = &aStatus;
    iPositionInfo = &aPosInfo;
    
    //Get PSY list
    TRAPD( err, iSingletonHandler->SettingHandler().GetPsyListL( iPsyList ) );
    if ( err == KErrNone )
        {
        if ( iPsyList.Count() == 0 )
            {
            //Empty PSY List
            CompleteRequest( KPositionQualityLoss );
            }
        else
            {
            MakeLocationRequest();
            }
        }
    else
        {
        CompleteRequest( err );
        }
        
    TRACESTRING( "CNppPsyRequester::NotifyPositionUpdate end" )
    }



// -----------------------------------------------------------------------------
// CNppPsyRequester::RunL
// -----------------------------------------------------------------------------
//
void CNppPsyRequester::RunL()
    {
    //Partial update is not possible for network based positioning
    if ( iStatus.Int() == KErrNone )
        {
        CompleteRequest( KErrNone );
        }
    else
        {
        //Try next PSY
        if ( iStatus.Int() < 0 && iStatus != KErrCancel )
            {
            //Move Psy to last.
            iSingletonHandler->SettingHandler().MovePsyLast( iPsyList[0] );
            //iPsyList can't be empty
            }
            
            //Remove this PSY from list
            iPsyList.Remove( 0 );
            
            //Try next PSY
            MakeLocationRequest();
        }
    }

// -----------------------------------------------------------------------------
// CNppPsyRequester::DoCancel
// -----------------------------------------------------------------------------
//
void CNppPsyRequester::DoCancel()
    {
    if ( iRequestStatus )
        {
        CompleteRequest( KErrCancel );
        }
    
    if ( iPositioner )
        {
        iPositioner->CancelNotifyPositionUpdate();
        }
    }

// -----------------------------------------------------------------------------
// CNppPsyRequester::RunError
// -----------------------------------------------------------------------------
//
TInt CNppPsyRequester::RunError( TInt /*aErr*/ )
    {
    //shall not be called.
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CNppPsyRequester::MakeLocationRequest
// -----------------------------------------------------------------------------
//
void CNppPsyRequester::MakeLocationRequest()
    {
    //Get the uid of the first PSY
    if ( iPsyList.Count() == 0 )
        {
        //List is empty
        TInt preError = iStatus.Int();
        preError = preError == KErrNone?KErrNotFound:preError;
        CompleteRequest( preError );
        return;
        }

    TUid impUid = iPsyList[0];
    
    TInt err( KErrNone );
    
    //Load PSY
    if ( !iPositioner || iPositioner->ImplementationUid() != impUid )
        {
        if ( iPositioner )
            {
            iPositioner->CancelNotifyPositionUpdate();
            }
            
        delete iPositioner;
        iPositioner = NULL;
        
        TRAP( err,
            iPositioner =
                CPositioner::NewL( impUid, iNppPsyPositioner ) );
        
        if ( err == KErrNotFound )
            {
            //Psy uid is in the list but psy is not found in system
            iSingletonHandler->SettingHandler().RemovePsy( impUid ); 
            //error ignored.
            }
        }
    

    if ( err == KErrNone )
        {
        iStatus = KRequestPending;
        iPositioner->NotifyPositionUpdate( *iPositionInfo, iStatus );
        SetActive();
        }
    else
        {
        //Report the first psy failure
        iStatus = KRequestPending;
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, err );
        }
    }


// -----------------------------------------------------------------------------
// CNppPsyRequester::CompleteRequest
// -----------------------------------------------------------------------------
//
void CNppPsyRequester::CompleteRequest( TInt aErr )
    {
    //Set UID before complete the request
    iPositionInfo->SetModuleId( TUid::Uid( KNppPsyImplUid ) );
    User::RequestComplete( iRequestStatus, aErr );

    //Report the request is completed
    iSingletonHandler->StatusHandler().DecRequestCount();
    }

//  End of File  
