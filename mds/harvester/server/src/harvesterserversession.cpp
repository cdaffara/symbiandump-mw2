/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Harvester server session*
*/

// INCLUDE FILES
#include "harvesterserversession.h"
#include "harvestercommon.h"
#include "harvesterlog.h"
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "harvesterserversessionTraces.h"
#endif

// ========================= MEMBER FUNCTIONS ==================================

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CHarvesterServerSession* CHarvesterServerSession::NewL( CHarvesterServer& aServer )
	{
    WRITELOG( "CHarvesterServerSession::NewL()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERSERVERSESSION_NEWL, "CHarvesterServerSession::NewL" );    
    
    CHarvesterServerSession* self = CHarvesterServerSession::NewLC( aServer );
    CleanupStack::Pop( self );
    return self;
	}

// ---------------------------------------------------------------------------
// NewLC
// ---------------------------------------------------------------------------
//
CHarvesterServerSession* CHarvesterServerSession::NewLC( CHarvesterServer& aServer )
	{
    WRITELOG( "CHarvesterServerSession::NewLC()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERSERVERSESSION_NEWLC, "CHarvesterServerSession::NewLC" );
    
    CHarvesterServerSession* self = new ( ELeave ) CHarvesterServerSession( aServer );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
	}

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CHarvesterServerSession::ConstructL()
	{
    WRITELOG( "CHarvesterServerSession::ConstructL()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERSERVERSESSION_CONSTRUCTL, "CHarvesterServerSession::ConstructL" );
    
	}

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
CHarvesterServerSession::CHarvesterServerSession( CHarvesterServer& aServer )
	: iServer( aServer )
	{
    WRITELOG( "CHarvesterServerSession::CHarvesterServerSession()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERSERVERSESSION_CHARVESTERSERVERSESSION, "CHarvesterServerSession::CHarvesterServerSession" );
    
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CHarvesterServerSession::~CHarvesterServerSession()
	{
    WRITELOG( "CHarvesterServerSession::~CHarvesterServerSession()" );
    OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERSERVERSESSION_CHARVESTERSERVERSESSION, "CHarvesterServerSession::~CHarvesterServerSession" );
    
	}

// ---------------------------------------------------------------------------
// ServiceL
// ---------------------------------------------------------------------------
//
void CHarvesterServerSession::ServiceL( const RMessage2& aMessage )
	{
    WRITELOG( "CHarvesterServerSession::ServiceL()" );	
    OstTrace0( TRACE_NORMAL, CHARVESTERSERVERSESSION_SERVICEL, "CHarvesterServerSession::ServiceL" );
    
    /* Command can't be negative */
    if ( aMessage.Function() < 0 )
        {
        User::Leave( aMessage.Function() );
        }
    
    switch ( aMessage.Function() )
   		{
		case EResumeHarvester:
			{
			WRITELOG( "CHarvesterServerSession::ServiceL() - EResumeHarvester command received" );
			iServer.Resume( aMessage );	
			}
		break;
		
		case EPauseHarvester:
			{
			WRITELOG( "CHarvesterServerSession::ServiceL() - EPauseHarvester command received" );
		    iServer.Pause( aMessage );			
			}
		break;
			
		case EHarvestFile:
			{
#ifdef _DEBUG
			WRITELOG( "CHarvesterServerSession::ServiceL() - EHarvestFile command received" );
			WRITELOG1( "CHarvesterServerSession::ServiceL() - EHarvestFile, param 0 size: %d", aMessage.GetDesLength(0) );
			WRITELOG1( "CHarvesterServerSession::ServiceL() - EHarvestFile, param 1 size: %d", aMessage.GetDesLength(1) );

			WRITELOG( "CHarvesterServerSession::ServiceL() - before harvest file" );
#endif
		    iServer.HarvestFile( aMessage );
			WRITELOG( "CHarvesterServerSession::ServiceL() - after harvest file" );
			}
		break;
			
        case ERegisterProcessOrigin:
            {
            WRITELOG( "CHarvesterServerSession::ServiceL() - ERegisterProcessOrigin command received" );
            iServer.RegisterProcessOrigin( aMessage );          
            }
        break;

        case EUnregisterProcessOrigin:
            {
            WRITELOG( "CHarvesterServerSession::ServiceL() - EUnregisterProcessOrigin command received" );
            iServer.UnregisterProcessOrigin( aMessage );          
            }
        break;
		
		case ERegisterHarvestComplete:
			{
			WRITELOG( "CHarvesterServerSession::ServiceL() - ERequestHarvestComplete command received" );
			TInt error = iServer.RegisterHarvestComplete( *this, aMessage );
			
			// complete message if registering failed
			if ( error != KErrNone && !aMessage.IsNull())
	        	{
	        	aMessage.Complete( error );
	        	}
			}
		break;
		
		case EUnregisterHarvestComplete:
			{
			WRITELOG( "CHarvesterServerSession::ServiceL() - EUnregisterHarvestComplete command received" );
			TInt error = iServer.UnregisterHarvestComplete( *this );
			
			// complete message if registering failed
			if ( !aMessage.IsNull() )
	        	{
	        	aMessage.Complete( error );
	        	}
			}
		break;
		
		case ERegisterHarvesterEvent:
			{
			WRITELOG( "CHarvesterServerSession::ServiceL() - ERegisterHarvesterEvent command received" );
			iServer.RegisterHarvesterEvent( aMessage );
			}
		break;
		
		case EUnregisterHarvesterEvent:
			{
			WRITELOG( "CHarvesterServerSession::ServiceL() - EUnregisterHarvesterEvent command received" );
			iServer.UnregisterHarvesterEvent( aMessage );
			}
		break;
		
	    case EHarvestFileWithUID:
	        {
#ifdef _DEBUG
	        WRITELOG( "CHarvesterServerSession::ServiceL() - EHarvestFileWithUID command received" );
	        WRITELOG1( "CHarvesterServerSession::ServiceL() - EHarvestFileWithUID, param 0 size: %d", aMessage.GetDesLength(0) );
	        WRITELOG1( "CHarvesterServerSession::ServiceL() - EHarvestFileWithUID, param 1 size: %d", aMessage.GetDesLength(1) );

	        WRITELOG( "CHarvesterServerSession::ServiceL() - before harvest file" );
#endif
	        iServer.HarvestFileWithUID( aMessage );
	        WRITELOG( "CHarvesterServerSession::ServiceL() - after harvest file" );
	        }
	    break;
		
        default:
			{
#ifdef _DEBUG
			WRITELOG1( "CHarvesterServerSession::ServiceL() - Invalid Call!!! Number: %d", aMessage.Function() );
#endif
			_LIT( KMessage, "Command not supported!" );
			iServer.PanicClient( aMessage, KErrNotSupported, KMessage );
			}
		break;
		}
	}

void CHarvesterServerSession::Disconnect(const RMessage2 &aMessage)
	{
	//just make sure the server is cleaned after client crash
	WRITELOG( "CHarvesterServerSession::Disconnect()" );
	OstTrace0( TRACE_NORMAL, CHARVESTERSERVERSESSION_DISCONNECT, "CHarvesterServerSession::Disconnect" );
	
	iServer.UnregisterHarvestComplete( *this );
	
	// Overridden CSession2 Disconnect, Message must completed
	// via CSession2::Disconnect
	CSession2::Disconnect( aMessage );
	}
