/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Sim State Monitoring class methods implementation
*
*/



// INCLUDE FILES
#include "CNWNetworkViagBaseEngine.h"
#include "CNWNetworkSystemEventMonitor.h"
#include "NWLogger.h"

#include <e32property.h>
#include <PSVariables.h>


// EXTERNAL DATA STRUCTURES
// None

// EXTERNAL FUNCTION PROTOTYPES  
// None

// CONSTANTS
// None

// MACROS
// None

// LOCAL CONSTANTS AND MACROS
// None

// MODULE DATA STRUCTURES
// None

// LOCAL FUNCTION PROTOTYPES
// None

// FORWARD DECLARATIONS
// None

// ==================== LOCAL FUNCTIONS =======================================
// None

// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// CNWNetworkSystemEventMonitor::CNWNetworkSystemEventMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWNetworkSystemEventMonitor::CNWNetworkSystemEventMonitor( 
    CNWNetworkViagBaseEngine& aBase,  
    TUid aUid,
    TInt aStatusValue
    )
   : CActive( EPriorityStandard ),
     iBase( aBase ),
     iState( 0 ),
     iUid( aUid ),
     iStatusValue( aStatusValue )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSystemEventMonitor::\
        CNWNetworkSystemEventMonitor() Begin " );
    
    CActiveScheduler::Add( this );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSystemEventMonitor::\
        CNWNetworkSystemEventMonitor() End " );
    }    

// ----------------------------------------------------------------------------
// CNWNetworkSystemEventMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CNWNetworkSystemEventMonitor::ConstructL(  )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSystemEventMonitor::ConstructL Begin." );
    // Get current state and start monitoring.
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSystemEventMonitor::ConstructL ATTACHING..." );
    User::LeaveIfError( iProperty.Attach( iUid, iStatusValue  ));
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSystemEventMonitor::ConstructL Attaching END" );

    RunL();
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSystemEventMonitor::ConstructL END." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkSystemEventMonitor::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
 CNWNetworkSystemEventMonitor* CNWNetworkSystemEventMonitor::NewL( 
            CNWNetworkViagBaseEngine& aBase,
            TUid aUid,
            TInt aStatusValue )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSystemEventMonitor::NewL() Begin." );
    
    CNWNetworkSystemEventMonitor* self = new (
            ELeave) CNWNetworkSystemEventMonitor( 
                aBase,  
                aUid, 
                aStatusValue );
                
    CleanupStack::PushL( self );
    self->ConstructL(  );
    CleanupStack::Pop( self );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSystemEventMonitor::NewL() Begin." );
    return self;
    }
// Destructor
CNWNetworkSystemEventMonitor::~CNWNetworkSystemEventMonitor()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSystemEventMonitor::\
        ~CNWNetworkSystemEventMonitor() Begin." );
    
    Cancel();
    iProperty.Close();
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSystemEventMonitor::\
        ~CNWNetworkSystemEventMonitor() End." );
    }


// ----------------------------------------------------------------------------
// CNWNetworkSystemEventMonitor::DoCancel
// Callback method from CActive.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkSystemEventMonitor::DoCancel()
    { 
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSystemEventMonitor::DoCancel Begin." );
    iProperty.Cancel();
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSystemEventMonitor::DoCancel End." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkSystemEventMonitor::Start
// In startup reads Sim state from System, otherwise it is called 
// when Sim state is changed.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkSystemEventMonitor::RunL()
    {    
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkSystemEventMonitor::RunL() Begin,\
        iStatus.Int() = %d", iStatus.Int() );
    
    if ( iStatus == KErrNone ) 
        {
        NWLOGSTRING3( KNWOBJECT, 
            "NW: CNWNetworkSystemEventMonitor::RunL() \
            UID= %x Value = %x", iUid,iStatusValue );
        iProperty.Subscribe( iStatus );
        TInt errorCode = iProperty.Get( iUid, iStatusValue, iState );
        if ( errorCode != KErrNone )
            {
            NWLOGSTRING2( KNWERROR, 
                "NW: CNWNetworkSystemEventMonitor::RunL() \
                ,GET Failed error: %d", errorCode );
            }
        iBase.HandleSystemEventL( iStatusValue, iState );
        SetActive();
        }
    else if( iStatus != KErrCancel  &&  iStatus != KErrServerTerminated )
        {
        NWLOGSTRING2( KNWOBJECT, 
           "NW: CNWNetworkSystemEventMonitor::RunL() ERROR %d",iStatus.Int() );
        }
    
    NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkSystemEventMonitor::RunL() END." );
    }

// ========================== OTHER EXPORTED FUNCTIONS ========================

// None

//  End of File
