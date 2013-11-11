/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Sub-session for accessing icon data in SIM.
*
*/



// INCLUDE FILES
#include    <fbs.h>
#include    <centralrepository.h>
#include    <etelsat.h>
#include    <rsatservice.h>
#include    <rsatsession.h>
#include    <tsaticoninfo.h>

#include    "tsatinternaliconinfo.h"
#include    "SatSOpcodes.h"
#include    "SatLog.h"
#include    "SATPrivateCRKeys.h"
#include    "csatpluginmonitor.h"

// CONSTANTS
// Length of the one icon info,
// defined in chapter 4.6.1.1 ETSI TS 131 102 V4.10.0 (2003-09)
const TInt KInfoLength = 9;
const TInt KMaxIconSizeNotRead = -1;

// Icon description must follow ETSI TS 131 102 V4.10.0 specification
const TUint8 KBitsInByte( 8 );
const TUint8 KIconWidth( 0 );
const TUint8 KIconHeigth( 1 );
const TUint8 KImageCodingScheme( 2 );
const TUint8 KHighFileId( 3 );
const TUint8 KLowFileId( 4 );
const TUint8 KHighIconOffset( 5 );
const TUint8 KLowIconOffset( 6 );
const TUint8 KHighIconLength( 7 );
const TUint8 KLowIconLength( 8 );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// RSatService::RSatService
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C RSatService::RSatService() :
    RSubSessionBase(),
    iMaxIconDataSize( KMaxIconSizeNotRead )
    {
    }

// -----------------------------------------------------------------------------
// RSatService::OpenL
// Connects a sub-session to SatServer.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RSatService::OpenL( const RSatSession& aSession )
    {
    LOG( SIMPLE, "SATCLIENT: RSatService::OpenL calling" )
    #ifdef ENABLE_SAT_LOGGING
    RProcess test;
    LOG2( SIMPLE, 
    "SATCLIENT: RSatService::OpenL UID of calling process: 0x%x",
    test.SecureId().iId )
    test.Close();
    #endif

    const TIpcArgs args( TIpcArgs::ENothing );
    User::LeaveIfError(
        CreateSubSession( aSession, ESatSOpenIconSubSession, args ) );

    TInt err( KErrNone );
    CRepository* repository = NULL;

    // Which UID to monitor.
    TRAP( err, repository = CRepository::NewL( KCRUidSatServer ); );
    LOG2( SIMPLE, 
          "SATCLIENT: RSatService::OpenL: new CRepository err: %d", 
          err )
    
    if ( repository )
        {
        err = repository->StartTransaction(
            CRepository::EReadWriteTransaction );
        LOG2( SIMPLE, 
              "SATCLIENT: RSatService::OpenL StartTransaction err: %d", 
              err )
        if ( KErrNone == err )
            {
            err = repository->Get( KSatMaxIconDataSize, iMaxIconDataSize );
            LOG2( SIMPLE, 
                  "SATCLIENT: RSatService::OpenL: GetMaxIconDataSize err: %d",
                  err )
            }
        
        LOG2( SIMPLE, 
              "SATCLIENT: RSatService::OpenL Iconsize limit:%d",
              iMaxIconDataSize )

        delete repository;
        repository = NULL;
        }  
        
    // Monitor the idle mode text    
    iSatPluginMonitor = CSatPluginMonitor::NewL();       
    
    LOG( SIMPLE, "SATCLIENT: RSatService::OpenL exiting" )
    }

// -----------------------------------------------------------------------------
// RSatService::Close
// Closes the sub-session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RSatService::Close()
    {
    LOG( SIMPLE, "SATCLIENT: RSatService::Close calling" )
    #ifdef ENABLE_SAT_LOGGING
    RProcess test;
    LOG2( SIMPLE, 
    "SATCLIENT: RSatService::Close UID of calling process: 0x%x",
    test.SecureId().iId )
    test.Close();
    #endif

    delete iSatPluginMonitor;
    iSatPluginMonitor = NULL;
    CloseSubSession( ESatSCloseIconSubSession );

    LOG( SIMPLE, "SATCLIENT: RSatService::Close exiting" )
    }

// -----------------------------------------------------------------------------
// RSatService::NotifySetupIdleModeTextChange
// Get notified of idle mode text change.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RSatService::NotifySetupIdleModeTextChange( 
    TRequestStatus& aStatus )
    {
    TInt err( KErrNotReady );
    LOG( SIMPLE, 
         "SATCLIENT: RSatService::NotifySetupIdleModeTextChange calling" )
         
    // New the SATMonitor to monitor the P&S key. 
    // IsActive checked also here to avoid a Codescanner warning.
    if ( iSatPluginMonitor && !iSatPluginMonitor->IsActive() )
        {
        err = iSatPluginMonitor->Start( aStatus );          
        }
    LOG( SIMPLE, 
         "SATCLIENT: RSatService::NotifySetupIdleModeTextChange exiting" )    
         
    return err;
    }

// -----------------------------------------------------------------------------
// RSatService::NotifySetupIdleModeTextChangeCancel
// Cancel notified of idle mode text change.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RSatService::NotifySetupIdleModeTextChangeCancel()
    {
    LOG( SIMPLE, 
         "SATCLIENT: RSatService::NotifySetupIdleModeTextChangeCancel calling" )  
           
    if ( iSatPluginMonitor )
        {
        iSatPluginMonitor->Cancel();
        }

    LOG( SIMPLE, 
     "SATCLIENT: RSatService::NotifySetupIdleModeTextChangeCancel exiting" )  
    }
    
// -----------------------------------------------------------------------------
// RSatService::GetSetupIdleModeTextL
// Get text after bootup
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool RSatService::GetSetupIdleModeTextL( HBufC*& aText, 
                                          TSatIconQualifier& aIconQualifier,
                                          TUint8& aRecordNumber )  
    {
    LOG( SIMPLE, "SATCLIENT: RSatService::GetSetupIdleModeTextL calling" )
    #ifdef ENABLE_SAT_LOGGING
    RProcess test;
    LOG2( SIMPLE,  "SATCLIENT: RSatService::GetSetupIdleModeTextL \
          UID of calling process:  0x%x",
          test.SecureId().iId )
    test.Close();
    #endif
  
    TBool ret ( ETrue ); 
    TSatIconQualifier satIconQualifier( ESatIconNoIcon );
    TUint8 satRecordNumber( NULL );    
    TInt iconQualifier( KSatIdleIconQInit );
    TInt recordNumber( KErrNotFound );
    
    // 1. Read icon qualifier, icon id and text
    if ( iSatPluginMonitor )
        {
        LOG( SIMPLE, "SATCLIENT: RSatService::GetSetupIdleModeTextL read key" )
        TInt result = 
             iSatPluginMonitor->GetIdleModeIconQualifier( iconQualifier );
             
        // Read icon id.
        if ( KSatIdleIconQNoIcon != iconQualifier )
            {
            result = iSatPluginMonitor->GetIdleModeIconId( recordNumber );
            }
        // Read the text on default.
        result = iSatPluginMonitor->GetIdleModeText( aText );

        }
    
    // 2. Convert icon qualifier.
    // Form enum TSatIdleModeTextIconQualifierValue defined in 
    // satinternalpskeys.h to enum TSatIconQualifier defined in rsatservice.h
    // NOTE: the value are different.
    switch ( iconQualifier )
        {
        case KSatIdleIconQInit: // go through
        case KSatIdleIconQNoIcon: 
            {
            satIconQualifier = RSatService::ESatIconNoIcon;
            break;
            }
        case KSatIdleIconQSelfExplanatory:
            {
            satIconQualifier = RSatService::ESatIconSelfExplanatory;
            break;
            }        
        case KSatIdleIconQNotSelfExplanatory:
            {
            satIconQualifier = RSatService::ESatIconNotSelfExplanatory;
            break;
            }
        default:
            {
            satIconQualifier = RSatService::ESatIconNoIcon;
            break;            
            }  
        }
    
    // 3. Convert icon id
    if ( 0 <= recordNumber )
        {
        // The PS key is published in TInt, and the value in etelsat.h is TUint8
        // the revode number have to be cast to TUint8
        satRecordNumber = static_cast<TUint8>( recordNumber );
        LOG2( SIMPLE, "SATCLIENT: RSatService::GetSetupIdleModeTextL\
             convert icon id: %i", satRecordNumber )           
        }
    else
        {
        satIconQualifier = RSatService::ESatIconNoIcon;
        satRecordNumber = NULL;        
        }

    // 4. Inspect icon qualifier, icon id and text.
    if ( RSatService::ESatIconNoIcon ==  satIconQualifier)
        {
        LOG( SIMPLE, 
             "SATCLIENT: RSatService::GetSetupIdleModeTextL ESatIconNoIcon" )         
        aIconQualifier = ESatIconNoIcon;
        aRecordNumber = NULL;
        }
    else if ( RSatService::ESatIconNotSelfExplanatory == satIconQualifier && 
              !aText )
        {
        LOG( SIMPLE, 
             "SATCLIENT: RSatService::GetSetupIdleModeTextL \
             ESatIconNotSelfExplanatory but no text" )  
        User::LeaveIfError( KErrNotFound );
        }
    else
        {
        aIconQualifier = satIconQualifier;
        aRecordNumber = satRecordNumber;        
        }        
        
    LOG( SIMPLE, "SATCLIENT: RSatService::GetSetupIdleModeTextL exiting" )     
    return ret;   
    }


// -----------------------------------------------------------------------------
// RSatService::GetIconInfoL
// Sends aRecordNumber to SatServer and waits for the icon informations.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RSatService::GetIconInfoL(
    TUint8 aRecordNumber,
    RIconEf& aIconEf ) const
    {
    LOG( SIMPLE, "SATCLIENT: RSatService::GetIconInfoL calling" )
    #ifdef ENABLE_SAT_LOGGING
    RProcess test;
    LOG2( SIMPLE, 
    "SATCLIENT: RSatService::GetIconInfoL UID of calling process: 0x%x",
    test.SecureId().iId )
    test.Close();
    #endif

    // Inform client that icons are not supported at all
    if ( 0 >= iMaxIconDataSize )
        {
        LOG( SIMPLE, "SATCLIENT:M RSatService::GetIconInfoL KErrNotSupported" )
        User::Leave( KErrNotSupported );
        }

    TPckgC<TUint8> recordNumberPckg( aRecordNumber );
    RSat::TIcon iconEf;
    TPckg<RSat::TIcon> iconEfPckg( iconEf );

    const TIpcArgs args( &recordNumberPckg, &iconEfPckg );

    // Get the icon info
    const TInt error( SendReceive( ESatSGetIconInfo, args ) );
    
    LOG2( SIMPLE, "SATCLIENT: RSatService::GetIconInfo with error %i", error )
    User::LeaveIfError( error );

    // Check that data is valid.
    CheckInfoValidityL( iconEf );

    if ( 0 < iconEf.Length() )
        {
        LOG( SIMPLE, 
             "SATCLIENT: RSatService::GetIconInfoL iconEf.Length() > 0" )
        const TInt infoCount( iconEf[0] );
        LOG2( SIMPLE, 
        "SATCLIENT: RSatService::GetIconInfo infoCount: %i", infoCount )
        for ( TInt i = 0; i < infoCount; i++ )
            {
            // Icon info is 9 bytes long and first byte is number of
            // icon info descriptors.
            const TInt KStart( i * KInfoLength + 1 );

            // Extract the info and append to aIconEf.
            const TPtrC8 info( iconEf.Mid( KStart, KInfoLength ) );
            
            TSatIconInfo satIconInfo;
            SetIconInfoL( satIconInfo, info );
            aIconEf.AppendL( satIconInfo );
            }

        }

    LOG( SIMPLE, "SATCLIENT: RSatService::GetIconInfoL exiting" )
    }

// -----------------------------------------------------------------------------
// RSatService::GetIconL
// Sends icon information to SatServer and waits the icon bitmap handle.
// SatServer is notified when the bitmap is duplicated, so that server knows
// that it can free the bitmap. If the server does not get the notification
// of bitmap duplication then server will free the bitmap when next
// icon is fetched.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CFbsBitmap* RSatService::GetIconL(
    const TSatIconInfo& aIconInfo ) const
    {
    LOG( SIMPLE, "SATCLIENT: RSatService::GetIconL calling" )
    #ifdef ENABLE_SAT_LOGGING
    RProcess test;
    LOG2( SIMPLE, 
    "SATCLIENT: RSatService::GetIconL UID of calling process: 0x%x",
    test.SecureId().iId )
    test.Close();
    #endif

    CFbsBitmap* bitmap = NULL;

    // Icon data size is checked
    // If under or same as the maximum size, icon is fetched
    // Otherwise null is returned.
    if ( ( aIconInfo.DataLength() <= iMaxIconDataSize ) )
        {
        LOG( SIMPLE, "SATCLIENT: RSatService::GetIconL icon fetch started" )
        TInt bitmapHandle( 0 );
        TPckg<TInt> bitmapHandlePckg( bitmapHandle );

        TSatInternalIconInfo interIconInfo;
        TSatInternalIconInfoPckg interIconPckg( interIconInfo );
        FillInternalIconInfo( aIconInfo, interIconInfo );


        TIpcArgs args( &interIconPckg, &bitmapHandlePckg );

        // Fetch the handle of the icon.
        User::LeaveIfError( SendReceive( ESatSGetIconInstance, args ) );

        // Duplicate the bitmap and notify server that bitmap is duplicated.
        // After the duplication the bitmap is owned by the client.
        args.Set( 0, TIpcArgs::ENothing );
        args.Set( 1, TIpcArgs::ENothing );
        bitmap = new( ELeave )CFbsBitmap();
        CleanupStack::PushL( bitmap );
        User::LeaveIfError( bitmap->Duplicate( bitmapHandle ) );
        User::LeaveIfError( Send( ESatSIconInstanceGot, args ) );
        CleanupStack::Pop( bitmap );
        LOG( SIMPLE, "SATCLIENT: RSatService::GetIconL icon fetch exit" )
        }

    LOG( SIMPLE, "SATCLIENT: RSatService::GetIconL exiting" )
    return bitmap;
    }

// -----------------------------------------------------------------------------
// RSatService::SetIdleModeTextResponse
// Sends SetUpIdleModeText response to Server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RSatService::SetIdleModeTextResponse(
    const TSATIdleResult& aResult ) const
    {
    LOG( SIMPLE, "SATCLIENT: RSatService::SetIdleModeTextResponse calling" )
    
    #ifdef ENABLE_SAT_LOGGING
    RProcess test;
    LOG2( SIMPLE, "SATCLIENT: RSatService::SetIdleModeTextResponse \
    UID of calling process: 0x%x",
    test.SecureId().iId )
    test.Close();
    #endif
    
    TInt responseValue( aResult );

    // Build IPC package
    const TIpcArgs args( responseValue );

    // Send the SetUpIdleMode response
    TInt err = SendReceive( ESatSIdleModeResponse, args );
    LOG2( SIMPLE,
        "SATCLIENT: RSatService::SetIdleModeTextResponse error while sending \
        response to SAT Engine: %d", err )    
    LOG( SIMPLE, "SATCLIENT: RSatService::SetIdleModeTextResponse exiting" )
    }

// -----------------------------------------------------------------------------
// RSatService::CheckDataValidityL
// Checks that data is valid.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RSatService::CheckInfoValidityL( const TDesC8& aIconEf ) const
    {
    LOG( SIMPLE, "SATCLIENT: RSatService::CheckInfoValidityL calling" )

    TInt err( KErrCorrupt );
    TInt realLength( aIconEf.Length() );
    TInt requiredLength( aIconEf[0] * KInfoLength + 1 );
    
    LOG2( SIMPLE, 
          "SATCLIENT: RSatService::CheckInfoValidityL realLength=%d",
          realLength )
    LOG2( SIMPLE, 
          "SATCLIENT: RSatService::CheckInfoValidityL requiredLength=%d",
          requiredLength )   

    // The length can be more than required, but the lenght has to be
    // at least required length.            
    if ( 0 < realLength && realLength >= requiredLength )
        {
        err = KErrNone;
        }
    User::LeaveIfError( err );

    LOG( SIMPLE, "SATCLIENT: RSatService::CheckInfoValidityL exiting" )
    }
  
// -----------------------------------------------------------------------------
// RSatService::SetIconInfo
// Set aIconSatData to TSatIconInfo.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RSatService::SetIconInfoL( TSatIconInfo& aContainer, 
                               const TDesC8& aIconSatData ) const
    {
    LOG( SIMPLE, "SATCLIENT: RSatService::SetIconInfo calling" )
    TInt err( KErrCorrupt );
    
    if ( KLowIconLength <= aIconSatData.Size() )
        {
        LOG( SIMPLE, "SATCLIENT: RSatService::SetIconInfo valid descriptor" )  
        
        TUint16 integer( 0 );
        TSize size( 0, 0 );

        // 1. icon size
        size.SetSize( aIconSatData[KIconWidth], aIconSatData[KIconHeigth] );
        aContainer.SetIconSize( size );  
         
        // 2. code scheme     
        aContainer.SetCodingScheme( static_cast<TSatIconInfo::TCodingScheme>( 
                                    aIconSatData[KImageCodingScheme] ) );

        // 3. file identifier
        // High byte
        integer = static_cast<TUint16>( 
                  aIconSatData[KHighFileId] << KBitsInByte );
        // Low byte
        integer |= aIconSatData[KLowFileId];
        aContainer.SetFileIdentifier( integer );

        // 4. offset
        // High byte
        integer = static_cast<TUint16>( 
                  aIconSatData[KHighIconOffset] << KBitsInByte );
        // Low byte
        integer |= aIconSatData[KLowIconOffset];
        aContainer.SetOffset( integer );

        // 5. length 
        // High byte
        integer = static_cast<TUint16>( 
                  aIconSatData[KHighIconLength] << KBitsInByte );
        // Low byte
        integer |= aIconSatData[KLowIconLength];
        aContainer.SetDataLength( integer );  
        err = KErrNone;
        }
    User::LeaveIfError( err );   
    LOG( SIMPLE, "SATCLIENT: RSatService::SetIconInfo exiting" )
    }
  
// -----------------------------------------------------------------------------
// RSatService::llInternalIconInfo
// Convert TSatIconinfo to TSatInternalIconInfo.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
void RSatService::FillInternalIconInfo( const TSatIconInfo& aIconInfo, 
                  TSatInternalIconInfo& aInternalIconInfo ) const
    {
    LOG( SIMPLE, "SATCLIENT: RSatService::FillInternalIconInfo calling" )

    // Fill the fields    
    aInternalIconInfo.iWidth = aIconInfo.IconSize().iWidth;
    aInternalIconInfo.iHeight = aIconInfo.IconSize().iHeight;
    if ( TSatIconInfo::EBasic == aIconInfo.CodingScheme() )
        {
        aInternalIconInfo.iCodingScheme = TSatInternalIconInfo::EBasic;
        }
    else
        {
        aInternalIconInfo.iCodingScheme = TSatInternalIconInfo::EColor;
        }       
    aInternalIconInfo.iFileIdentifier = aIconInfo.FileIdentifier();
    aInternalIconInfo.iOffset = aIconInfo.Offset();
    aInternalIconInfo.iLength = aIconInfo.DataLength();
    
    LOG( SIMPLE, "SATCLIENT: RSatService::FillInternalIconInfo exit" )    
    }

//  End of File
