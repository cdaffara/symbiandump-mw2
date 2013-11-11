/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles APN operations
*
*/

// INCLUDE FILES
#include <cmmanager.h>
#include <cmmanagerext.h>
#include <cmconnectionmethod.h>
#include <cmconnectionmethodext.h>
#include <cmconnectionmethoddef.h>
#include <cmpluginpacketdatadef.h>
#include <cmdestination.h>
#include <cmmanagerdef.h>

#include    "MSatUtils.h"
#include    "CSatApnHandler.h"
#include    "SatLog.h"

const TUint32 KSatCMGranularity( 5 );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatApnHandler::CSatApnHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatApnHandler::CSatApnHandler( MSatUtils& aUtils ) :
    iUtils( aUtils )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatApnHandler::CSatApnHandler calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CSatApnHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatApnHandler* CSatApnHandler::NewL( MSatUtils& aUtils )
    {
    LOG( SIMPLE, "SATENGINE: CSatApnHandler::NewL calling" )

    CSatApnHandler* self = new( ELeave )CSatApnHandler( aUtils );
    LOG( SIMPLE, "SATENGINE: CSatApnHandler::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSatApnHandler::Destructor
// -----------------------------------------------------------------------------
//
CSatApnHandler::~CSatApnHandler()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatApnHandler::~CSatApnHandler calling" )
    LOG( SIMPLE,
        "SATENGINE: CSatApnHandler::~CSatApnHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatApnHandler::GetApnInfoL
// -----------------------------------------------------------------------------
//
void CSatApnHandler::GetApnInfoL(
    const RSat::TAccessName& aName,
    const RSat::TTextString& aUserLogin,
    const RSat::TTextString& aUserPwd,
    TUint32& aIapId,
    TUint32& aNwId,
    TBool& aApnCreated,
    const RPacketContext::TProtocolType& aPdpType,
    const TDesC8& aLocalAddress )
    {
    LOG( SIMPLE, "SATENGINE: CSatApnHandler::GetApnInfoL calling" )


    // First check the length of the APN
    if ( !aName.Length() )
        {
        LOG( SIMPLE, "SATENGINE: CSatApnHandler::GetApnInfoL \
        aName.Length() == 0" )
        User::Leave( KErrNotFound );
        }

    // Format APN
    HBufC* formatName = FormatAPN( aName );

    if ( !formatName )
        {
        LOG( SIMPLE, "SATENGINE: CSatApnHandler::GetApnInfoL wrong APN name \
            format" )
        // APN was in false format, leave
        User::Leave( KErrArgument );
        }
    CleanupStack::PushL( formatName );

    // Reference params
    TUint32 iapId( 0 );
    TUint32 networkId( 0 );

    // Find if the connection method already exists 
    // Initialize the flags
    TBool found = EFalse;
    aApnCreated = EFalse;

    // Create CMManager Session
    RCmManager cmManager;
    cmManager.OpenLC();

    // Get the Connection Method list from the open CMManager session
    RArray<TUint32> array = RArray<TUint32>( KSatCMGranularity );
    CleanupClosePushL( array );

    cmManager.ConnectionMethodL( array );
    
    // Go through the Connection Method list to find if there is matched one
    HBufC* apnCM;

    LOG2( SIMPLE, "SATENGINE: CSatApnHandler::GetApnInfoL, \
                   required pdp type is %d", aPdpType )

    for( TInt i = 0; ( i < array.Count() ) && !found; ++i )
        {
        RCmConnectionMethod cm = cmManager.ConnectionMethodL( array[i] );
        
        CleanupClosePushL( cm );
        // query the APN of the Connection Method
        apnCM = cm.GetStringAttributeL( CMManager::EPacketDataAPName );
        CleanupStack::PushL( apnCM );
        // query the pdpType of the Connection Method
        TInt pdpType = cm.GetIntAttributeL( CMManager::EPacketDataPDPType );

        
        if ( ( pdpType == aPdpType ) && ( *apnCM == *formatName ) )
            {
            // Found the Connection Method, query the IapId and NwId
            LOG( SIMPLE, "SATENGINE: CSatApnHandler::GetApnInfoL, \
                          Record found" )

            // Get IAP Id, Network Id
            networkId = cm.GetIntAttributeL( CMManager::ECmNetworkId );
            LOG2( SIMPLE, "SATENGINE: CSatApnHandler::GetApnInfoL, \
                           networkId %d", networkId ) 
            
            iapId = cm.GetIntAttributeL( CMManager::ECmIapId );
            LOG2( SIMPLE, "SATENGINE: CSatApnHandler::GetApnInfoL, \
                           iapId %d", iapId )
    
            found = ETrue;
            }
        CleanupStack::PopAndDestroy( apnCM );
        CleanupStack::PopAndDestroy( &cm );    
        }        
 
    CleanupStack::PopAndDestroy( &array );

    if ( !found )
        {
        // No Connection Method found, insert a new one
    
        LOG( SIMPLE, "SATENGINE: CSatApnHandler::GetApnInfoL, \
                      Record Not found, insert a new one" )
        
        TRAPD( insertError, InsertRecordL( *formatName, aUserLogin, 
               aUserPwd, iapId, networkId, aPdpType, aLocalAddress ) )

        if ( KErrNone == insertError )
            {
            // Insert OK
            LOG( NORMAL, "SATENGINE: CSatApnHandler::GetApnInfoL, \
                          new record inserted" )
            aApnCreated = ETrue;
            }
        }
    
    CleanupStack::PopAndDestroy( &cmManager );
    CleanupStack::PopAndDestroy( formatName );

    // Set return values
    aIapId = iapId;
    aNwId = networkId;

    LOG( SIMPLE, "SATENGINE: CSatApnHandler::GetApnInfoL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatApnHandler::DeleteApnL
// -----------------------------------------------------------------------------
//
void CSatApnHandler::DeleteApnL( TUint32 aApnId )
    {
    LOG( SIMPLE, "SATENGINE: CSatApnHandler::DeleteApnL calling" )

    if ( aApnId )
        { 
        // Create CMManager Session
        RCmManagerExt cmManager;
        cmManager.OpenLC();
        
        // Get the connection method
        RCmConnectionMethodExt cm;
        cm = cmManager.ConnectionMethodL( aApnId );
        CleanupClosePushL( cm );

        cm.DeleteL();
        LOG2( SIMPLE, "SATENGINE: CSatApnHandler::DeleteApnL, \
                       delete the connection method %d", aApnId )

        CleanupStack::PopAndDestroy( &cm );
        CleanupStack::PopAndDestroy( &cmManager );
        }

    LOG( SIMPLE, "SATENGINE: CSatApnHandler::DeleteApnL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatApnHandler::InsertRecordL
// -----------------------------------------------------------------------------
//
void CSatApnHandler::InsertRecordL(
    const TDesC& aReqApn,
    const RSat::TTextString& aUserLogin,
    const RSat::TTextString& aUserPwd,
    TUint32& aIapId,
    TUint32& aNwId,
    const RPacketContext::TProtocolType aPdpType,
    const TDesC8& aLocalAddress )
    {
    LOG( SIMPLE, "SATENGINE: CSatApnHandler::InsertRecordL calling" )

    LOG2( NORMAL, "SATENGINE: CSatApnHandler::InsertRecordL, apn is %S", 
        &aReqApn )
    
    // Create CMManager Session
    RCmManagerExt cmManager;
    cmManager.OpenLC();

    // Create a connection method without destination. So when we can find 
    // the connection method by going through the connection method list.
    RCmConnectionMethodExt cm;
    cm = cmManager.CreateConnectionMethodL( KUidPacketDataBearerType );
    CleanupClosePushL( cm );
    
    // This is shown in GS in Access Point list
    cm.SetStringAttributeL( CMManager::ECmName, iUtils.BipApnName() );    
    // GPRS connection name
    cm.SetStringAttributeL( CMManager::EPacketDataAPName, aReqApn );   
    // IPv4 or IPv6
    cm.SetIntAttributeL( CMManager::EPacketDataPDPType, aPdpType );

    // Set the local IP address (if any)
    if ( aLocalAddress.Length() )
        {
        LOG2( SIMPLE, "SATENGINE: CSatApnHandler::InsertRecordL, Local addr \
            length: %i", aLocalAddress.Length() )
        cm.SetString8AttributeL( CMManager::EPacketDataPDPAddress, 
            aLocalAddress );
        }

    cm.SetBoolAttributeL( CMManager::ECmWapIPSecurity, EFalse );
    cm.SetIntAttributeL( CMManager::ECmWapIPWSPOption, 
                         CMManager::ECmWapWspOptionConnectionOriented );
    cm.SetBoolAttributeL( CMManager::EPacketDataDisablePlainTextAuth, EFalse );
    cm.SetBoolAttributeL( CMManager::ECmIFPromptForAuth, EFalse );

    if ( aUserLogin != KNullDesC )
        {
        LOG2( NORMAL, "SATENGINE: CSatApnHandler::InsertRecordL, \
            username is %S", &aUserLogin ) 
        cm.SetStringAttributeL( CMManager::ECmIFAuthName, aUserLogin );

        if ( aUserPwd != KNullDesC )
            {
            LOG2( NORMAL, "SATENGINE: CSatApnHandler::InsertRecordL, \
                pwd is %S", &aUserPwd ) 
            cm.SetStringAttributeL( CMManager::ECmIFAuthPass, aUserPwd );
            }
        }
    
    // Update the access point ID for use later.
    cm.UpdateL();

    // Get IAP Id, Network Id
    aNwId = cm.GetIntAttributeL( CMManager::ECmNetworkId );
    LOG2( SIMPLE, "SATENGINE: CSatApnHandler::InsertRecordL, \
                   Nwid is %d", aNwId ) 
    
    aIapId = cm.GetIntAttributeL( CMManager::ECmIapId );
    LOG2( SIMPLE, "SATENGINE: CSatApnHandler::InsertRecordL, \
                   aIpId is %d", aIapId )

    CleanupStack::PopAndDestroy( &cm );
    CleanupStack::PopAndDestroy( &cmManager );

    LOG( SIMPLE, "SATENGINE: CSatApnHandler::InsertRecordL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatApnHandler::FormatAPN
// -----------------------------------------------------------------------------
//
HBufC* CSatApnHandler::FormatAPN( const RSat::TAccessName& 
    aReqApn ) const
    {
    LOG( SIMPLE, "SATENGINE: CSatApnHandler::FormatAPN calling" )

    // Length of the access point name
    const TInt accessNameLength( aReqApn.Length() );
    TInt index( 0 );

    // Format APN coming from SIM. Remove length infos and add dots
    // from the requested APN. The APN format is, for example,
    // '4Some9accesspnt2fi' and after the format the formatted APN
    // should be 'Some.accesspnt.fi'
    HBufC* formatApnName = HBufC::New( aReqApn.MaxLength() );

    if ( formatApnName )
        {
        TPtr formApn( formatApnName->Des() );

        // Indicates is the APN format correct. Can be detected only in loops.
        TBool correctFormat( ETrue );

        // Loop requested APN. The first byte is always the length
        for ( index = 0; index < accessNameLength && correctFormat; index++ )
            {
            // Next byte is the length of the next label
            const TInt length( aReqApn[index] );

            // Label length cannot be bigger than the length of the APN
            if ( ( index + length ) > accessNameLength )
                {
                LOG( SIMPLE, "SATENGINE: CSatApnHandler::FormatAPN \
                ( index + length ) > accessNameLength" )
                correctFormat = EFalse;
                }

            // Append the label
            for ( TInt j = 0; j < length && correctFormat; j++ )
                {
                index++; // move to next index
                TChar next = aReqApn[index];
                formApn.Append( next );
                }

            // Add dot to APN if not end of APN
            if ( accessNameLength > index + 1 )
                {
                LOG( SIMPLE, "SATENGINE: CSatApnHandler::FormatAPN \
                accessNameLength > index + 1" )
                formApn.Append( '.' );
                }
            }
        LOG2( SIMPLE, "SATENGINE: CSatApnHandler::FormatAPN index: %i", index )

        // Check did we read all characters
        if ( !correctFormat || ( index != accessNameLength ) )
            {
            // Format was not correct, return zero
            formApn.Zero();
            formApn.SetLength( 0 );
            }
        else
            {
            LOG( SIMPLE, "SATENGINE: CSatApnHandler::FormatAPN lower case" )
            // Put characters to lower case
            formApn.LowerCase();
            }

        LOG( SIMPLE, "SATENGINE: CSatApnHandler::FormatAPN exiting" )
        }

    return formatApnName;
    }

// -----------------------------------------------------------------------------
// CSatApnHandler::FindDefaultApL
// -----------------------------------------------------------------------------
//
TUint32 CSatApnHandler::FindDefaultApL(
        const RPacketContext::TProtocolType& aPdpType )
    {
    LOG( SIMPLE, "SATENGINE: CSatApnHandler::FindDefaultApL calling" );
    TUint32 defaultIap( 0 );
    TInt pdpType;
    TBool isFound( EFalse );

    // create a network destination
    RCmDestination des;
    RCmConnectionMethod cm;

    // Create CMManager Session
    RCmManager cmManager;
    cmManager.OpenL();
    CleanupClosePushL( cmManager );

    // Get the Connection Method list from the open CMManager session
    RArray<TUint32> array( KSatCMGranularity );
    CleanupClosePushL( array );

    // list all available destinations' id
    cmManager.AllDestinationsL( array );
    for( TInt i = 0; ( i < array.Count() ) && !isFound; ++i )
        {
        des = cmManager.DestinationL( array[i] );
        CleanupClosePushL( des );

        if ( CMManager::ESnapPurposeInternet ==
        des.MetadataL( CMManager::ESnapMetadataPurpose ) )
            {
            LOG( SIMPLE, "SATENGINE: CSatApnHandler::FindDefaultApL \
            the fixed destination is identified as 'Internet'" );
            for( TInt j = 0; ( j < des.ConnectionMethodCount() ) &&
            !isFound; ++j )
                {
                cm = des.ConnectionMethodL( j );
                CleanupClosePushL( cm );
                pdpType = cm.GetIntAttributeL(
                        CMManager::EPacketDataPDPType );
                LOG2( SIMPLE, "SATENGINE: CSatApnHandler::FindDefaultApL \
                        current protocol type is %d", pdpType )
                if ( pdpType == aPdpType )
                    {
                    defaultIap = cm.GetIntAttributeL( CMManager::ECmIapId );
                    isFound  = ETrue;
                    LOG2( SIMPLE, "SATENGINE: CSatApnHandler::FindDefaultApL \
                            default iap had been found %d", defaultIap )
                    }
                CleanupStack::PopAndDestroy( &cm );
                }
            }
        CleanupStack::PopAndDestroy( &des );
        }
    CleanupStack::PopAndDestroy( &array );
    CleanupStack::PopAndDestroy( &cmManager );

    if ( !isFound )
        {
        LOG( SIMPLE, "SATENGINE: CSatApnHandler: default AP not found" );
        User::Leave( KErrNotFound );
        }

    LOG( SIMPLE, "SATENGINE: CSatApnHandler::FindDefaultApL exit" )
    return defaultIap;
    }
//  End of File
