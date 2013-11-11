/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Send DTMF string if necessary after creating a call 
*
*/

#include <etelmm.h>
#include <mmtsy_names.h>
#include "msatmultimodeapi.h"
#include "csetupcalldtmfsender.h"
#include "msatasynctosync.h"
#include "SatLog.h"

_LIT( KSatDtmfCharp, "p" );
_LIT( KSatDtmfCharw, "w" );

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CSetupCallDtmfSender::CSetupCallDtmfSender
// The class constructor.
// -----------------------------------------------------------------------------
//
CSetupCallDtmfSender::CSetupCallDtmfSender( MSatMultiModeApi& aPhone )
    : CActive( EPriorityStandard ), iPhone( aPhone ), iDtmfString( NULL ) 
    {
    LOG( SIMPLE,
        "SETUPCALL: CSetupCallDtmfSender::CSetupCallDtmfSender calling" )

    CActiveScheduler::Add( this );

    LOG( SIMPLE,
        "SETUPCALL: CSetupCallDtmfSender::CSetupCallDtmfSender exiting" )
    }

// -----------------------------------------------------------------------------
// CSetupCallDtmfSender::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSetupCallDtmfSender* CSetupCallDtmfSender::NewL( MSatMultiModeApi& aPhone )
    {
    LOG( SIMPLE, "SETUPCALL: CSetupCallDtmfSender::NewL calling" )

    CSetupCallDtmfSender* self =
        new ( ELeave ) CSetupCallDtmfSender( aPhone );
 
    LOG( SIMPLE, "SETUPCALL: CSetupCallDtmfSender::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSetupCallDtmfSender::~CSetupCallDtmfSender
// Destructor
// -----------------------------------------------------------------------------
//
CSetupCallDtmfSender::~CSetupCallDtmfSender()
    {
    LOG( SIMPLE, "SETUPCALL: \
            CSetupCallDtmfSender::~CSetupCallDtmfSender calling" )
    Cancel();
    
    delete iDtmfString;
    iDtmfString = NULL;

    LOG( SIMPLE, "SETUPCALL: \
            CSetupCallDtmfSender::~CSetupCallDtmfSender exiting" )
    }

// -----------------------------------------------------------------------------
// CSetupCallDtmfSender::SendDtmfString
// -----------------------------------------------------------------------------
//
void CSetupCallDtmfSender::SendDtmfString()
{
    LOG( SIMPLE, "SETUPCALL: CSetupCallDtmfSender::SendDtmfString calling" )
    if( !IsActive() && iDtmfString )
        {
        const TInt ret = iPhone.ContinueDTMFStringSending( ETrue );
        LOG2( SIMPLE, "SETUPCALL: CSetupCallDtmfSender::SendDtmfString\
            ContinueDTMFStringSending ret %d", ret )        
        iPhone.SendDTMFTones( iStatus, *iDtmfString );  
        
        SetActive();
        }
    LOG( SIMPLE, "SETUPCALL: CSetupCallDtmfSender::SendDtmfString exit" )
}

// -----------------------------------------------------------------------------
// CSetupCallDtmfSender::SeparateDtmfFromTelNumber
// Refers to 
// RPhCltEmergencyCall::RemoveDTMF
// RPhCltEmergencyCall::FindStartOfDtmfString
// -----------------------------------------------------------------------------
//
void CSetupCallDtmfSender::SeparateDtmfFromTelNumber(TDes& aString)
    {
    LOG( SIMPLE, 
        "SETUPCALL: CSetupCallDtmfSender::SeparateDtmfFromTelNumber calling")
        
    delete iDtmfString;
    iDtmfString = NULL;
    
    // String must contain 'p' or 'w'. Find start of dtmf string.
    TInt indexp; //char 'p' index
    TInt indexw; // char 'w' index
    TPtrC ptrDtmfCharp( KSatDtmfCharp );
    TPtrC ptrDtmfCharw( KSatDtmfCharw );

    indexp = aString.Locate( ptrDtmfCharp[ 0 ] );
    indexw = aString.Locate( ptrDtmfCharw[ 0 ] ); 

    TInt dtmfIndex = KErrNotFound;
    if ( indexp != KErrNotFound || indexw != KErrNotFound )
        {
        // dtmf char found
        if ( indexw == KErrNotFound )
            {
            // char p index
            dtmfIndex = indexp;    
            }   
        else if ( indexp == KErrNotFound )
            {
            // char w index
            dtmfIndex = indexw;    
            }                 
        else
            {
            // Both chars p and w found, smaller index selected
            indexp < indexw ? dtmfIndex = indexp : dtmfIndex = indexw;    
            }         
        }
    LOG2( SIMPLE, 
        "SETUPCALL: CSetupCallDtmfSender::SeparateDtmfFromTelNumber \
        dtmfIndex=%d", dtmfIndex )  
    
    // Separate purl tel number and dtmf string
    if ( dtmfIndex != KErrNotFound )
        {
        // Dtmf string to member data for furture sending
        TPtrC temp( aString.Right( aString.Length() - dtmfIndex ) );
        LOG2( SIMPLE, 
            "SETUPCALL: CSetupCallDtmfSender::SeparateDtmfFromTelNumber \
            DTMF=%S", &temp)  
        
        iDtmfString = temp.Alloc();
        LOG2( SIMPLE, 
            "SETUPCALL: CSetupCallDtmfSender::SeparateDtmfFromTelNumber \
            iDtmfString address=%d", iDtmfString )         
        
        // Tel number for dialing call
        aString.Delete( dtmfIndex, aString.Length() - dtmfIndex );
        LOG2( SIMPLE, 
            "SETUPCALL: CSetupCallDtmfSender::SeparateDtmfFromTelNumber \
            TelNumber=%S", &aString ) 
        }
    LOG( SIMPLE, 
        "SETUPCALL: CSetupCallDtmfSender::SeparateDtmfFromTelNumber exit" )
    }

// -----------------------------------------------------------------------------
// CSetupCallDtmfSender::RunL
// Handles the command.
// -----------------------------------------------------------------------------
//
void CSetupCallDtmfSender::RunL()
    {
    LOG2( SIMPLE, "SETUPCALL: CSetupCallDtmfSender::RunL calling iStatus=%i", 
        iStatus.Int() )
    
    delete iDtmfString;
    iDtmfString = NULL;
    
    LOG( SIMPLE, "SETUPCALL: CSetupCallDtmfSender::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// From class CActive.
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CSetupCallDtmfSender::DoCancel()
    {
    LOG( SIMPLE, "SETUPCALL: CSetupCallDtmfSender::DoCancel calling" )
    iPhone.CancelAsyncRequest( EMobilePhoneSendDTMFTones );
    LOG( SIMPLE, "SETUPCALL: CSetupCallDtmfSender::DoCancel exiting" )
    }

//  End of File
