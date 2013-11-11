/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*
*/


// System includes
#include <centralrepository.h>
#include <s32mem.h>

// User includes
#include "cenrephandler.h"
#include "presetcrkeys.h"
#include <preset.h>
#include "cenrepbookkeeping.h"
#include "trace.h"


class TChannelIndexList
    {
public:
    TInt iChannelIndex;
    TInt iNameIndex;
    TInt iFrequencyIndex;
    TInt iUrlIndex;
    TInt iRestOfParametersIndex;
    };

// ============= STATIC FUNCTIONS ==============

static TInt CalculateChannelIndexes( const TInt aChannelIndex,  TChannelIndexList& aChannelIndexList)
    {
    FUNC_LOG;
    TInt err( KErrNone );
    if ( ( KMaxNumberOfPresetStations <= aChannelIndex ) ||
            ( 0 >  aChannelIndex ) )
        {
        err = KErrArgument;
        }
    else
        {
        aChannelIndexList.iChannelIndex = 
         KRadioPresetChannel1 + ( KNbrOfParametersPerChannelSequence * aChannelIndex );
        aChannelIndexList.iNameIndex =
            aChannelIndexList.iChannelIndex + KChannelNameOffset;        
        aChannelIndexList.iFrequencyIndex =
            aChannelIndexList.iChannelIndex + KFrequencyOffset;        
        aChannelIndexList.iUrlIndex =
            aChannelIndexList.iChannelIndex + KUrlOffset;        
        aChannelIndexList.iRestOfParametersIndex =
            aChannelIndexList.iChannelIndex + KRestOfParametersOffset;
        }
    return err;
    }

// ============= MEMBER FUNCTIONS ==============

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CCenrepHandler::CCenrepHandler()
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCenrepHandler::ConstructL()
    {
    FUNC_LOG;
    iCentralRepository = CRepository::NewL( KCRUidPresetUtility );
    iBookKeeping = new (ELeave) CCenrepBookKeeping;
    ReadBookKeepingL();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CCenrepHandler* CCenrepHandler::NewL()
    {
    FUNC_LOG;
    CCenrepHandler* self = new (ELeave) CCenrepHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CCenrepHandler::~CCenrepHandler()
    {
    FUNC_LOG;
    delete iCentralRepository;
    delete iBookKeeping;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CCenrepHandler::MaxPresetCount() const
    {
    FUNC_LOG;
    return KMaxNumberOfPresetStations;
    }

// -----------------------------------------------------------------------------
// Get the current number of presets
// -----------------------------------------------------------------------------
//
TInt CCenrepHandler::PresetCount() const
    {
    FUNC_LOG;
    return iBookKeeping->PresetCount();
    }

// -----------------------------------------------------------------------------
// Gets the first preset
// -----------------------------------------------------------------------------
//
TInt CCenrepHandler::FirstPreset() const
    {
    FUNC_LOG;
    return iBookKeeping->FirstIndex();
    }

// -----------------------------------------------------------------------------
// Get the next preset index that has been set, given the current index
// -----------------------------------------------------------------------------
//
TInt CCenrepHandler::NextPreset( TInt aFromIndex ) const
    {
    FUNC_LOG;
    return iBookKeeping->NextIndex( aFromIndex );
    }

// -----------------------------------------------------------------------------
// Delete the current preset
// -----------------------------------------------------------------------------
//
TInt CCenrepHandler::DeletePreset( TInt aIndex )
    {
    FUNC_LOG;
    TInt err( KErrNone );
    if ( -1 == aIndex )
        {
        // Delete all
        TChannelIndexList channelIndexList;
        for (  TInt j( FirstPreset() ); 0 <= j;  j = FirstPreset() )
            {
            INFO_1("Deleting index: %i", j );
            if ( KErrNone == CalculateChannelIndexes( j, channelIndexList ) )
                {                
                iCentralRepository->Delete( channelIndexList.iNameIndex );    
                iCentralRepository->Delete( channelIndexList.iFrequencyIndex );   
                iCentralRepository->Delete( channelIndexList.iUrlIndex );
                iCentralRepository->Delete( channelIndexList.iRestOfParametersIndex );
                iBookKeeping->RemoveIndex( j );        
                }
            }
        }
    else
        {
        TChannelIndexList channelIndexList;
        err = CalculateChannelIndexes( aIndex, channelIndexList );
        if ( KErrNone == err )
            {
            iCentralRepository->Delete( channelIndexList.iNameIndex );    
            iCentralRepository->Delete( channelIndexList.iFrequencyIndex );   
            iCentralRepository->Delete( channelIndexList.iUrlIndex );
            iCentralRepository->Delete( channelIndexList.iRestOfParametersIndex );
            iBookKeeping->RemoveIndex( aIndex );        
            }
        }
    if ( KErrNone == err )
        {
        SaveBookKeeping();
        }
    return err;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CCenrepHandler::SavePresetL( const TPreset& aPreset, TInt aIndex )
    {
    FUNC_LOG;
    if ( 0 > aIndex )
        {
        aIndex = iBookKeeping->FindUnusedIndex();
        }
    if ( 0 > aIndex )
        {
        User::Leave( KErrDiskFull ); // Not really a good code for this
        }
    TChannelIndexList channelIndexList;
    User::LeaveIfError( CalculateChannelIndexes( aIndex, channelIndexList) );

    // First save name to its own key in the key sequence.
    User::LeaveIfError( iCentralRepository->Set( channelIndexList.iNameIndex, aPreset.Name() ) );

    // Frequency  to its own key in the key sequence.
    User::LeaveIfError( iCentralRepository->Set( channelIndexList.iFrequencyIndex, (TInt)aPreset.Frequency() ) );

    // Url to its own key in the key sequence.
    User::LeaveIfError( iCentralRepository->Set( channelIndexList.iUrlIndex, aPreset.Url() ) );

    // Then externalize the rest of the data to a common key.
    TInt err( KErrNone );
    TInt length( aPreset.MyExternalizedDataSize() );
    TUint i ( 1 );
    INFO_1( "aPreset.MyExternalizedDataSize()=%i", length );
    for ( TBool cont = ( 0 < length ) ? ETrue : EFalse; (cont); cont = ( KErrOverflow == err ) ?  ETrue : EFalse ) 
        {
        HBufC8* buf = HBufC8::NewLC( i * length  );
        TPtr8 bufPtr = buf->Des();
        RDesWriteStream outStream( bufPtr );
        CleanupClosePushL( outStream );
        TRAP( err, aPreset.ExternalizeL( outStream ) );
        if ( KErrNone == err )
            {
            outStream.CommitL();
            User::LeaveIfError( iCentralRepository->Set( channelIndexList.iRestOfParametersIndex, *buf ) );
            }
        else
            {
            INFO_2( "SavePresetL bigger buffer (%i) needed than required by aPreset.MyExternalizedDataSize()=%i", i*length, aPreset.MyExternalizedDataSize() );
            ++i;
            }
        CleanupStack::PopAndDestroy( &outStream );
        CleanupStack::PopAndDestroy( buf );
        }
    User::LeaveIfError( err );

    iBookKeeping->AddIndex( aIndex );
    SaveBookKeeping();
    return aIndex;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCenrepHandler::ReadPresetL( TInt aIndex, TPreset& aPreset )
    {
    FUNC_LOG;

    // Read name from its own key in the key sequence
    TChannelIndexList channelIndexList;
    User::LeaveIfError( CalculateChannelIndexes( aIndex, channelIndexList) );
    TPresetName tempChannelName;
    User::LeaveIfError( iCentralRepository->Get(channelIndexList.iNameIndex, tempChannelName ) );
    aPreset.SetName( tempChannelName );

    // Read frequency from its own key in the key sequence
    TInt frequency( 0 );
    User::LeaveIfError( iCentralRepository->Get( channelIndexList.iFrequencyIndex, frequency ) );
    aPreset.SetFrequency( frequency );

    // Read url from its own key in the key sequence
    TRadioUrl tempUrlName;
    TInt err = iCentralRepository->Get( channelIndexList.iUrlIndex, tempUrlName );
    if ( KErrNone == err )
        {        
        aPreset.SetUrl( tempUrlName );
        }
    else if ( KErrNotFound == err )
        {
        // This is an optional parameter.
        }
    else
        {
        User::Leave( err );
        }
                      

    // Then use the below code to internalize rest of parameters from the common key
    TInt actualLength( aPreset.MyExternalizedDataSize() );
    INFO_1( "ReadPresetL aPreset.MyExternalizedDataSize()=%i", actualLength );
    for ( TBool cont = ( 0 < actualLength ) ? ETrue : EFalse; (cont); cont = ( KErrOverflow == err ) ?  ETrue : EFalse ) 
        {
        HBufC8* buf = HBufC8::NewLC( actualLength );
        TPtr8 bufPtr = buf->Des();
        err = iCentralRepository->Get( channelIndexList.iRestOfParametersIndex, bufPtr, actualLength );
        if ( ( KErrNone == err ) && ( 0 < buf->Length() ) )
            {
            RDesReadStream inStream;
            inStream.Open( *buf );
            CleanupClosePushL( inStream );       
            aPreset.InternalizeL( inStream );        
            CleanupStack::PopAndDestroy( &inStream );
            }
        else if ( KErrNotFound == err )
            {
            // This is not filled by CT tool even though name, frequency and URL might be 
            // filled for the current channel. Don't leave.
            INFO_1( "ReadPresetL KErrNotFound, key=%i", channelIndexList.iRestOfParametersIndex );
            }
        else if ( KErrOverflow == err )
            {
            INFO_2( "ReadPresetL KErrOverflow, buf->Length()=%i actualLength=%i", buf->Length(), actualLength );
            // Trying again with actual with buffer that can store the whole key
            }
        else
            {
            User::Leave( err );
            }
        CleanupStack::PopAndDestroy(buf);
        } // for (;;) 
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCenrepHandler::ReadBookKeepingL()
    {
    FUNC_LOG;
    TPtr8 bufPtr = iBookKeeping->BookKeepingData();

    TInt err( iCentralRepository->Get( KRadioBookKeepingLocation, bufPtr ) );
    if ( KErrNotFound == err )
        {
        // Bookkeeping does not exist. Search possible preconfigured channels.
        ReadPreDefinedPresetsL();
        err = KErrNone;
        }
    User::LeaveIfError( err );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCenrepHandler::SaveBookKeeping()
    {
    FUNC_LOG;
    TInt err( iCentralRepository->Set( KRadioBookKeepingLocation, iBookKeeping->BookKeepingData() ) );
    if ( KErrNone !=  err )
        {
        INFO_1("Saving of BookKeeping failed with error: %i ", err)
        // It is better to destroy data that is out of sync
        iCentralRepository->Delete( KRadioBookKeepingLocation );
        // Go on with local bookkeeping data
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCenrepHandler::ReadPreDefinedPresetsL()
    {
    FUNC_LOG;
    TInt frequency ( 0 );
    TInt err( KErrNone );
    TChannelIndexList channelIndexList;
    iBookKeeping->Clear();
    for(TInt index = 0; index < KMaxNumberOfPresetStations; index++)
        {
        User::LeaveIfError( CalculateChannelIndexes( index, channelIndexList) );
        err = iCentralRepository->Get( channelIndexList.iFrequencyIndex, frequency );
        if ( KErrNone ==  err )
            {
            iBookKeeping->AddIndex( index );
            }
        } 
    SaveBookKeeping();
    }
