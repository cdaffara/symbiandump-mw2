/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CCbsSatRefresh
*                class member functions.
*
*/



// INCLUDE FILES
#include "CCbsSatRefresh.h"
#include "CCbsServer.h"
#include <rsatrefresh.h>
#include <rsatsession.h>

#include "CbsLogger.h"

// Constants

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCbsSatRefresh::CCbsSatRefresh
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsSatRefresh::CCbsSatRefresh( CCbsServer& aCbsServer ): 
	iCbsServer ( aCbsServer )
    {
    CBSLOGSTRING( ">>> CBSSERVER: CCbsSatRefresh::CCbsSatRefresh" );
    CBSLOGSTRING( "<<< CBSSERVER: CCbsSatRefresh::CCbsSatRefresh" );
    }

// -----------------------------------------------------------------------------
// CCbsSatRefresh::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsSatRefresh::ConstructL()
    {
    CBSLOGSTRING( ">>> CBSSERVER: CCbsSatRefresh::ConstructL" );
    
    iSatSession = new ( ELeave ) RSatSession;
    iSatSession->ConnectL();
    iSatRefresh = new ( ELeave ) RSatRefresh( *this );
   	iSatRefresh->OpenL( *iSatSession );   	
   	
   	TSatRefreshFiles file;
   	file.Append( KCbmiEf );
   	iSatRefresh->NotifyFileChangeL( file );
   	
    CBSLOGSTRING( "<<< CBSSERVER: CCbsSatRefresh::ConstructL" );
    }

// -----------------------------------------------------------------------------
// CCbsSatRefresh::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsSatRefresh* CCbsSatRefresh::NewL( CCbsServer& aCbsServer )
    {    
    CCbsSatRefresh* self = new (ELeave) CCbsSatRefresh( aCbsServer );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

    
// Destructor
CCbsSatRefresh::~CCbsSatRefresh()
    {
    CBSLOGSTRING( ">>> CBSSERVER: CCbsSatRefresh::~CCbsSatRefresh" );
    
    if ( iSatRefresh )
        {
        // Cancel notifications.
        iSatRefresh->Cancel();	
        // Close SubSession.
        iSatRefresh->Close();
        }
	delete iSatRefresh;
	
	if ( iSatSession )
		{
		iSatSession->Close();
		}
	delete iSatSession;
	
    CBSLOGSTRING( "<<< CBSSERVER: CCbsSatRefresh::~CCbsSatRefresh" );
    }

// -----------------------------------------------------------------------------
// CCbsSatRefresh::AllowRefresh
// Refresh query. Determines whether it allow the refresh to happen. 
// -----------------------------------------------------------------------------
//
TBool CCbsSatRefresh::AllowRefresh( TSatRefreshType aType, const TSatRefreshFiles& /*aFiles*/ )
 	{
 	// Refresh is always allowed
    CBSLOGSTRING( ">>> CBSSERVER: CCbsSatRefresh::AllowRefresh()" );
    CBSLOGSTRING2( "CBSSERVER: CCbsSatRefresh::AllowRefresh() Refresh type: %d", aType );
    CBSLOGSTRING( "<<< CBSSERVER: CCbsSatRefresh::AllowRefresh complete, returning ETrue." );
    
    // To prevent ARMV5 compiler warnings
    if ( aType )
        {
        aType = ERefreshTypeNotSet;
        }
	return ETrue;
    }
 
// -----------------------------------------------------------------------------
// CCbsSatRefresh::Refresh
// Notification of refresh. In this method the client should not close 
// or cancel the RSatRefresh subsession. 
// -----------------------------------------------------------------------------
//     
void CCbsSatRefresh::Refresh( TSatRefreshType aType, const TSatRefreshFiles& /*aFiles*/ )
	{
	// Reload SIM Topics in case of refresh     		
    CBSLOGSTRING( ">>> CBSSERVER: CCbsSatRefresh::Refresh" );
    CBSLOGSTRING2( "CBSSERVER: CCbsSatRefresh::Refresh Type: %d", aType );
    
    TRAP_IGNORE( iCbsServer.ReLoadSimTopicsL() );
    
    // Notify SAT Server that refresh initiated file read is done
    iSatRefresh->RefreshEFRead( EFalse );
    
    // To prevent ARMV5 compiler warnings
    if ( aType )
        {
        aType = ERefreshTypeNotSet;
        }
    
    CBSLOGSTRING( "<<< CBSSERVER: CCbsSatRefresh::Refresh complete " );
	}

//  End of File  
