/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Reads metadata from rtp clip meta header
*
*/

#include "harvesterrtpmetadatareader.h"
#include "harvesterrtpplugin.h"
#include "harvesterlog.h"

_LIT( KTxtSpace, " ");

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CHarvesterRtpMetaDataReader* CHarvesterRtpMetaDataReader::NewL( 
    const TDesC8* aFileBuffer  )
    {
    CHarvesterRtpMetaDataReader* self =
        new( ELeave ) CHarvesterRtpMetaDataReader( aFileBuffer );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CHarvesterRtpMetaDataReader::~CHarvesterRtpMetaDataReader()
    {
    WRITELOG( "CHarvesterRtpMetaDataReader::~CHarvesterRtpMetaDataReader()" );
    delete iMetaData;
    iMetaData = NULL;
    }

// ---------------------------------------------------------------------------
// Read details from meta header
// ---------------------------------------------------------------------------
//
void CHarvesterRtpMetaDataReader::GetClipDetailsL( 
    CHarvesterRtpClipDetails& aDetails )
    {
    WRITELOG( "CHarvesterRtpMetaDataReader::GetClipDetailsL()" );
    
    // Attributes
    TPtrC8 bytes( iDataPtr.Mid( KAttributesPoint, KIntegerBytes ) );
    TUint data( 0 );
    User::LeaveIfError( GetValue( bytes, data ) );

    // Recording quality 
    aDetails.iQuality = 
        ( TUint8 )( ( data >> KQualityFieldShift ) & KMaxTUint8 );
    //Recording ongoing 
    aDetails.iRecOngoing = 
        ( data >> KOngoingFlagShift ) & ETrue;
    // Recording completed 
    aDetails.iRecCompleted = 
        ( data >> KCompletedFlagShift ) & ETrue;
    //Recording failed 
    aDetails.iRecFailed = 
        ( data >> KFailedFlagShift ) & ETrue;
    //Parental rate
    aDetails.iParental =
         ( TUint8 )( ( data >> KParentalFieldShift ) & KMaxTUint8 );
    // Post rule    
    TUint8 postRule = 
        ( TUint8 )( ( data >> KPostRuleFieldShift ) & KMaxTUint8 );
    
    if ( postRule == KContentRightsLockToDevice )
        {
        aDetails.iPostRule = ETrue;
        }
    else
        {
        aDetails.iPostRule = EFalse;
        }
        
    // Duration
    TUint duration( 0 ); 
    TPtrC8 durationBytes ( iDataPtr.Mid( KDurationPoint, KIntegerBytes ) );
    User::LeaveIfError( GetValue( durationBytes, duration ) );
    duration /= 1000; //Convert to seconds 
    aDetails.iDuration = static_cast<TReal32>( duration );
    
    // Title
    GetProgramTitleL( aDetails.iTitle );
    }

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
CHarvesterRtpMetaDataReader::CHarvesterRtpMetaDataReader(
    const TDesC8* aFileBuffer )
    : iMetaTotal( KErrNotFound ),
      iMetaData( NULL ),
      iDataPtr( 0, 0 ),
      iEsgDataPoint ( KErrNotFound ),
      iFileBuffer( aFileBuffer )
    {
    // None
    }

// ---------------------------------------------------------------------------
// 2nd phase constructor
// ---------------------------------------------------------------------------
//
void CHarvesterRtpMetaDataReader::ConstructL()
    {
    WRITELOG( "CHarvesterRtpMetaDataReader::ConstructL()" );
    TPtrC8 bytes;
    
    if ( iFileBuffer 
        && iFileBuffer->Length() > KIntegerBytes )
        {
        // Read whole meta area
        bytes.Set( iFileBuffer->Mid( KMetaLengthPoint, KIntegerBytes ) );
        }        

    TUint value ( 0 );
    User::LeaveIfError( GetValue( bytes, value ) );
    
    iMetaTotal = value;
    
    if ( iMetaTotal > KMaxMetaHeaderLength || iMetaTotal <= 0 )
        {
        //Length not reasonable, metadata corrupted
        WRITELOG( "CHarvesterRtpMetaDataReader - metadata corrupted, leave!" );
        User::Leave( KErrCorrupt );
        }

    iMetaData = HBufC8::NewL( iMetaTotal );
    *iMetaData = iFileBuffer->Mid( KMetaLengthPoint, iMetaTotal );

    iDataPtr.Set( iMetaData->Des() );
            
    // Check that all data exist in meta header
    if ( iDataPtr.Length() < iMetaTotal )
        {
        //Meta header corrupted
        WRITELOG( "CHarvesterRtpMetaDataReader - metaheader corrupted, leave!" );
        User::Leave( KErrCorrupt );
        }

    // ESG data point ( device info point + device info data )
    iEsgDataPoint = KDeviceInfoPoint + KStringLengthBytes +
                    iDataPtr[KDeviceInfoPoint];

    }

// -----------------------------------------------------------------------------
// Reads data with length info of meta header from the clip
// -----------------------------------------------------------------------------
//
void CHarvesterRtpMetaDataReader::ReadStringDataL( 
    const TInt aPosition, 
    TPtrC8& aData )
    {
    WRITELOG( "CHarvesterRtpMetaDataReader::ReadStringDataL()" );

    if ( aPosition < 0 
        || aPosition > iDataPtr.Length() )
        {
        WRITELOG( "CHarvesterRtpMetaDataReader - KErrArgument, leave!" );
        User::Leave( KErrArgument );
        }

    const TInt len( iDataPtr[aPosition] );
    
    if ( len < 0 
        || len > TInt( KMaxTUint8 ) )
        {
        WRITELOG( "CHarvesterRtpMetaDataReader - KErrCorrupt, leave!" );
        User::Leave( KErrCorrupt );
        }
        
    aData.Set( iDataPtr.Mid( aPosition + KStringLengthBytes, len ) );
    }

// -----------------------------------------------------------------------------
// Get value
// -----------------------------------------------------------------------------
//
TInt CHarvesterRtpMetaDataReader::GetValue( 
    const TDesC8& aBytes, 
    TUint& aValue )
    {
    WRITELOG( "CHarvesterRtpMetaDataReader::GetValue()" );
    if ( aBytes.Length() >= KIntegerBytes )
        {
        aValue = ( TUint )( aBytes[0] );
        aValue <<= 8;
        aValue |= ( TUint )( aBytes[1] );
        aValue <<= 8;
        aValue |= ( TUint )( aBytes[2] );
        aValue <<= 8;
        aValue |= ( TUint )( aBytes[3] );
        return KErrNone;
        }

    WRITELOG( "CHarvesterRtpMetaDataReader::GetValue() : KErrUnderFlow!" );
    return KErrUnderflow;
    }

// -----------------------------------------------------------------------------
// Get and format clip title
// -----------------------------------------------------------------------------
//
void CHarvesterRtpMetaDataReader::GetProgramTitleL( TDes& aTitle )
    {
    WRITELOG( "CHarvesterRtpMetaDataReader::GetProgramTitleL()" );
    
    User::LeaveIfError( iEsgDataPoint );
    aTitle.Zero();

    TUint32 totalLength( 0 );
    TPtrC8 service;
    TPtrC8 program;
    
    ReadStringDataL( iEsgDataPoint, service );
    
    totalLength += service.Length();
    totalLength += KSpaceLength;

    // Program name
    const TInt progPos( iEsgDataPoint + KStringLengthBytes + service.Length() );
    ReadStringDataL( progPos, program );
    
    totalLength += program.Length();

    HBufC8* title = HBufC8::NewLC( totalLength );
    title->Des().Append( service.Ptr(), service.Length() );
    title->Des().Append( KTxtSpace );
    title->Des().Append( program.Ptr(), program.Length() ); 
    
    if ( totalLength > KMaxProgramTitle ) 
        {
        //Make title fit in max title length
        TPtrC8 titlePtr( *title );
        aTitle.Copy( titlePtr.Mid( 0 , KMaxProgramTitle ) );
        }
    else
        {
        aTitle.Copy( *title );
        }
                
    CleanupStack::PopAndDestroy( title );
    }

