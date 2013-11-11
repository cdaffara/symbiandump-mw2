/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CCbsDbImpSettings class 
*                member functions.
*    
*                This class represents CBS settings stored in the database.
*                A functionality is provided here to access, modify, store and restore
*                settings.
*
*/


//  INCLUDE FILES
#include <centralrepository.h>
#include "CbsServerPrivateCRKeys.h"
#include "MCbsDbSettingsObserver.H"
#include "CbsServerPanic.h"
#include "CCbsDbImpSettings.H"
#include "CbsDbConstants.h"
#include "CbsStreamHelper.h"
#include "CbsUtils.h"
#include "CbsLogger.h"

// CONSTANTS
// Used if languages cannot be read from Shared Data.
const TInt8 KDefaultsLanguageSubscribedValue = '1';


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsDbImpSettings::CCbsDbImpSettings
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsDbImpSettings::CCbsDbImpSettings(  
    CCbsDbImp& aDatabase )
    : iDatabase( aDatabase )
    {
    }

// -----------------------------------------------------------------------------
// CCbsDbImp::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsDbImpSettings::ConstructL()
    {
    // Create an array for settings event observers.
    iObservers = new ( ELeave ) CArrayFixFlat< MCbsDbSettingsObserver* >( 
        KCbsDbObserverArraySize );

    TRAPD( result, LoadSettingsL() );
	if ( result != KErrNone )   
		{
		User::Leave( result );
		}
		
    __TEST_INVARIANT;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpSettings::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsDbImpSettings* CCbsDbImpSettings::NewL( CCbsDbImp& aDatabase )
    {
    CCbsDbImpSettings* self = 
        new ( ELeave ) CCbsDbImpSettings( aDatabase );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }
    
// Destructor
CCbsDbImpSettings::~CCbsDbImpSettings()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpSettings::~CCbsDbImpSettings()");
    delete iObservers;
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpSettings::~CCbsDbImpSettings()");
    }

// -----------------------------------------------------------------------------
// CCbsDbImpSettings::SetTopicDetectionStatusL
// Changes the topic detection status.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpSettings::SetTopicDetectionStatusL( 
    TBool aStatus )
    {

    // If the value does not change we do nothing but return
    if ( aStatus != iSettings.iTopicDetectionStatus ) 
        {
        // First we change the status value in cache.
        iSettings.iTopicDetectionStatus = aStatus;

        // Now we try to save the modified settings to the store
        if ( !SaveSettings() ) 
            {        
            // If an error occured, we leave the status unchanged.
            if ( iSettings.iTopicDetectionStatus ) 
                {
                iSettings.iTopicDetectionStatus = EFalse;
                }
            else 
                {
                iSettings.iTopicDetectionStatus = ETrue;
                }

            // and then leave again
            User::Leave( KErrWrite );
            }

        // Inform observers about the changed status
        TInt amountOfObservers( iObservers->Count() );
        for ( TInt i( 0 ); i < amountOfObservers; i++ )
            {
            iObservers->At( i )->TopicDetectionStatusChangedIndL();
            }                
        }    
    }

// -----------------------------------------------------------------------------
// CCbsDbImpSettings::SaveSettings
// Saves the settings to CenRep
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCbsDbImpSettings::SaveSettings()
    {
    __TEST_INVARIANT;
    TBool result( ETrue );

    // Try to save the settings.
    TRAPD( error, DoSaveSettingsL() );
    if ( error != KErrNone )
        {
        // Some kind of failure occurred.
        __TEST_INVARIANT;        
        result = EFalse;
        }

    __TEST_INVARIANT;
    return result;
    }    

// -----------------------------------------------------------------------------
// CCbsDbImpSettings::DoSaveSettingsL
// Saves the settings to CenRep
// The method leaves in case the writing to the does not succeed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpSettings::DoSaveSettingsL()
    {
    
    TInt reception( iSettings.iReceptionStatus );
    TInt topicDetection( iSettings.iTopicDetectionStatus );
    
    TBuf< ECbsCount > languages; 
   
    for ( TInt i( 0 ); i < ECbsCount; i++ )
        {
        languages.AppendNum( iSettings.iLanguageStatus.iLanguages[i] );	
        }
   
    // Connecting and initialization
    CRepository* repository = CRepository::NewL( KCRUidCellBroadcast );
 
    // Get the values from Central Repository
    repository->Set( KCbsReception, reception );
    repository->Set( KCbsTopicDetection, topicDetection );
    repository->Set( KCbsLanguages, languages );
    
    // Closing the connection
    delete repository;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpSettings::GetTopicDetectionStatus
// Returns the current value of the topic detection status.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpSettings::GetTopicDetectionStatus( 
    TBool& aStatus ) const
    {
    // We just give the asked status value from cache to aStatus
    aStatus = iSettings.iTopicDetectionStatus;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpSettings::SetReceptionStatusL
// Changes the reception status.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpSettings::SetReceptionStatusL( 
    TBool aStatus )    
    {

    // If the value does not change we do nothing but return
    if ( aStatus != iSettings.iReceptionStatus ) 
        {
        // First we change the status value in iSettings
        iSettings.iReceptionStatus = aStatus;

        // Now we try to save the modified settings to the store
        if ( !SaveSettings() ) 
            {
            // If an error occured, we leave the status unchanged.
            if ( iSettings.iReceptionStatus ) 
                {
                iSettings.iReceptionStatus = EFalse;
                }
            else 
                {
                iSettings.iReceptionStatus = ETrue;
                }

            // and then leave.
            User::Leave( KErrWrite );
            }

        // Inform observers about the changed status
        TInt amountOfObservers( iObservers->Count() );
        for ( TInt i( 0 ); i < amountOfObservers; i++ )
            {
            iObservers->At( i )->ReceptionStatusChangedIndL();
            }                
        }    
    }

// -----------------------------------------------------------------------------
// CCbsDbImpSettings::GetReceptionStatus
// Returns the current value of the topic reception status.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpSettings::GetReceptionStatus( 
    TBool& aStatus ) const
    {
    // We just give the asked status value from cache to aStatus
    aStatus = iSettings.iReceptionStatus;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpSettings::SetLanguagesL
// Changes the preferred languages.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CCbsDbImpSettings::SetLanguagesL( 
    const TCbsDbLanguages& aLanguages )
    {
    __TEST_INVARIANT;

    // Compare language settings.
    if ( !IsLanguagesEqual( aLanguages, iSettings.iLanguageStatus ) ) 
        {
        // We take a backup of the language status before changing the value.
        TCbsDbLanguages oldLanguageStatus = iSettings.iLanguageStatus;
        
        // We change the language status value
        iSettings.iLanguageStatus = aLanguages;

        // Now we try to save the modified settings struct to the store
        if ( !SaveSettings() ) 
            {
            // If an error occured, we leave the settings unchanged.
            iSettings.iLanguageStatus = oldLanguageStatus;
            User::Leave( KErrWrite );
            }

        // Inform observers about the changed status
        TInt amountOfObservers( iObservers->Count() );
        for ( TInt i( 0 ); i < amountOfObservers; i++ )
            {
            iObservers->At( i )->LanguagesChangedIndL();
            }    
        }
    
    __TEST_INVARIANT;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpSettings::GetLanguages
// Returns the preferred languages.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CCbsDbImpSettings::GetLanguages( 
    TCbsDbLanguages& aLanguages ) const
    {
    // We just give the asked status value from cache to aLanguages
    aLanguages = iSettings.iLanguageStatus;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpSettings::AddObserverL
// Adds an observer to the settings.
// Observers are notified when an event occurs on the settings.    
// Panics on debug mode if aObserver is null.  
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CCbsDbImpSettings::AddObserverL( 
    MCbsDbSettingsObserver* aObserver )
    {
    __TEST_INVARIANT;
    __ASSERT_DEBUG( aObserver!=0, CbsServerPanic( ECbsObserverNull ) );
    iObservers->AppendL( aObserver );
    __TEST_INVARIANT;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpSettings::RemoveObserver
// Removes database observer.
// The method will panic, if there is no such observer added
// or the given observer is null.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CCbsDbImpSettings::RemoveObserver( 
    const MCbsDbSettingsObserver* aObserver )
    {
    __ASSERT_DEBUG( aObserver != 0, CbsServerPanic( ECbsObserverNull ) );

    // Find the observer to remove
    TInt amountOfObservers( iObservers->Count() );
    TBool found( EFalse );

    for ( TInt index( 0 ); ( index < amountOfObservers ) && !found; index++ )
        {
        if ( aObserver == iObservers->At( index ) )
            {
            iObservers->Delete( index );
            iObservers->Compress();  
            found = ETrue;            
            }
        }

    // Panic if the observer was not found
    if ( !found )
        {
        CbsServerPanic( ECbsObserverNotFound );
        }    
    }

// -----------------------------------------------------------------------------
// CCbsDbImpSettings::LoadSettingsL
// Loads the settings, i.e. values for reception status, topic detection
// and language subscriptions.
// These values are retrieved from Central Repository, associated with
// the UID of CbsServer.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CCbsDbImpSettings::LoadSettingsL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpSettings::LoadSettingsL()");
    
    // Connecting and initialization
    CRepository* repository = CRepository::NewL( KCRUidCellBroadcast );
    
    TBuf< ECbsCount > languages;
    languages.Zero();

    TInt reception( 0 );
    TInt topicDetection( 0 );
    
    // Get the values from Central Repository
    repository->Get( KCbsReception, reception );
    repository->Get( KCbsTopicDetection, topicDetection );
    repository->Get( KCbsLanguages, languages );
    
    // Closing the connection
    delete repository;
    
    CBSLOGSTRING("CBSSERVER: CCbsDbImpSettings::LoadSettingsL(): Repository reading finished.");

    iSettings.iReceptionStatus = reception == 0 ? EFalse : ETrue;
    iSettings.iTopicDetectionStatus = topicDetection == 0 ? EFalse : ETrue;

    if ( languages.Length() != ECbsCount )
        {
        User::Leave( KErrCorrupt );
        }
    
    for ( TInt i( 0 ); i < ECbsCount; i++ )
        {
        iSettings.iLanguageStatus.iLanguages[ i ] = 
            ( languages[ i ] == KDefaultsLanguageSubscribedValue );
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpSettings::LoadSettingsL()");
    }

// -----------------------------------------------------------------------------
// CCbsDbImpSettings::IsLanguagesEqual
// Determines whether the language settings are equal. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TBool CCbsDbImpSettings::IsLanguagesEqual( 
    const TCbsDbLanguages& aLanguage1, 
    const TCbsDbLanguages& aLanguage2 ) const
    {
    // Go through each language settings.
    for ( TInt index( 0 ); index < ECbsCount; index++ )
        {
        if ( aLanguage1.iLanguages[ index ] != aLanguage2.iLanguages[ index ] )
            {
            return EFalse;
            }
        }

    // Equal.
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpSettings::SetDefaultLanguageSettings
// Sets default language settings.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CCbsDbImpSettings::SetDefaultLanguageSettings( 
    TCbsDbLanguages& aLanguage ) const
    {
    // First, set all languages to false.
    for ( TInt index( 0 ); index < ECbsAll; index++ )
        {
        aLanguage.iLanguages[ index ] = EFalse;
        }

    // And then set the "All"-choice to true.
    aLanguage.iLanguages[ ECbsAll ] = ETrue;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpSettings::__DbgTestInvariant
// Checks that the object is in a valid state, and panics if it is not.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CCbsDbImpSettings::__DbgTestInvariant() const
    {
#if defined(_DEBUG)
    if ( iSettings.iReceptionStatus < 0 || iSettings.iReceptionStatus > 1 ||
        iSettings.iTopicDetectionStatus < 0 || 
        iSettings.iTopicDetectionStatus > 1 ||
        iObservers == NULL )
        {
        User::Invariant();
        }
#endif
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================
//  End of File  
