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
* Description:  This file contains the header file of the CCbsDbImpSettings class
*    
*                This class represents the settings stored in the database.
*
*/



#ifndef     CCBSDBIMPSETTINGS_H
#define     CCBSDBIMPSETTINGS_H

// INCLUDES
#include "CbsCommon.h"
#include "CCbsDbImp.H"

//  FORWARD DECLARATIONS
class MCbsDbSettingsObserver;

//  CLASS DECLARATION 

/**
*   CCbsDbImpSettings represents server persistent settings.
*   These settings include receive mode, topic detection mode
*   and user selected languages.
*
*   Receive mode controls whether any CB messages are received
*   or not. If topic detection is enabled and a message belonging
*   to a previously unknown topic (i.e., a topic not on topic list)
*   is received, the topic of this message is added onto the topic
*   list.
*
*   User selected languages determine which message are received.
*   The language of an incoming message has to be on the array
*   of these preferred languages. Selection "All" allows reception
*   of all messages, regardless of language. 
*
*   Settings are modifiable only through UI client requests.
*   MCN client requests have no effect on these settings,
*   that is, the settings in the file store. MCN client requests
*   are taken into account when a call to CCbsRecEtel::ApplyStateChangesL()
*   is made.
*   
*/
class CCbsDbImpSettings : public CBase

    {        
    public:        // New functions
        /**
        *   Creates a new instance of the class. 
        *
        *   @param  aDatabase               Reference to the main DB instance.
        *   @return                         Returns a pointer to the created 
        *                                   instance of the class.
        */
        static CCbsDbImpSettings* NewL( CCbsDbImp& aDatabase );

        /**
        *   Destructor.
        */
        ~CCbsDbImpSettings();

        /**
        *   Changes the topic detection status.
        *
        *   @param aStatus                  New topic detection status.
        */
        void SetTopicDetectionStatusL( TBool aStatus );

        /**
        *   Returns the current value of the topic detection status.
        *
        *   @param aStatus                  The method will store to 
        *                                   the variable the current topic 
        *                                   detection status.
        */
        void GetTopicDetectionStatus( TBool& aStatus ) const;

        /**
        *   Changes the reception status.
        *
        *   @param aStatus                  New reception status.
        */
        void SetReceptionStatusL( TBool aStatus );

        /**
        *   Returns the current value of the topic reception status.
        *
        *   @param aStatus                  The method will store to 
        *                                   the variable 
        *                                   the current reception status.
        */                  
        void GetReceptionStatus( TBool& aStatus ) const;
    
        /**
        *   Changes the preferred languages.
        *
        *   @param aLanguages               New preferred languages.
        */
        void SetLanguagesL( const TCbsDbLanguages& aLanguages );

        /**
        *   Returns the preferred languages.
        *
        *   @param aLanguages               The method will store to the variable
        *                                   the current preferred languages.
        */
        void GetLanguages( TCbsDbLanguages& aLanguages ) const;

        /**
        *   Adds an observer to the settings. 
        *
        *   Observers are notified when an event occurs on the settings.
        *
        *   Panics if aObserver is null.
        *
        *   @param aObserver                Pointer to the observer that is 
        *                                   requested to be added.
        */
        void AddObserverL( MCbsDbSettingsObserver* aObserver );
    
        /**
        *   Removes database observer. 
        *
        *   The method will panic, if there is no such observer added
        *   or the given observer is null.
        *
        *   @param aObserver                Pointer to the observer that is 
        *                                   requested to be removed.
        */
        void RemoveObserver( const MCbsDbSettingsObserver* aObserver );

    private:

        // A structure that contains the settings.
        struct TCbsDbImpSettings
            {
            // Reception status. ETrue, if the reception is enabled.
            TBool iReceptionStatus;

            // Topic detection status. ETrue, if the topic detection is enabled.
            TBool iTopicDetectionStatus;

            // Language settings.
            TCbsDbLanguages iLanguageStatus;
            };

        /**
        *   Constructor.
        *
        *   @param aDatabase                Reference to the main database object.
        */
        CCbsDbImpSettings( CCbsDbImp& aDatabase );

        /**
        *   Creates the instances.
        *
        */
        void ConstructL();
  
        /**
        *   Saves the settings to the shared data. 
        *
        *   @return Boolean value indicating whether the saving succeeded.
        */
        TBool SaveSettings();

        /**
        *   Saves the settings to the shared data.
        *
        *   The method leaves in case the writing to the stream does not 
        *   succeed.
        */
        void DoSaveSettingsL();

        /**
        *   Loads the settings, i.e. values for reception status, 
        *   topic detection and language subscriptions.
        *
        *   These values are retrieved from Central Repository, associated with
        *   the UID of CbsServer.
        *
        */
        void LoadSettingsL();

        /**
        *   Determines whether the language settings are equal.
        *
        *   @param  aLanguage1              First language set.
        *   @param  aLanguage2              Second language set.
        *   @return                         ETrue, if the languages are equal.
        */
        TBool IsLanguagesEqual( const TCbsDbLanguages& aLanguage1, 
            const TCbsDbLanguages& aLanguage2 ) const;

        /**
        *   Sets default language settings.
        *
        *   @param  aLanguage               Place where to put the default 
        *                                   settings.
        */
        void SetDefaultLanguageSettings( TCbsDbLanguages& aLanguage ) const;

        /**
        *   Reads settings from shared data.
        *
        */
        //void ReadSettingsL( );

    private:    

        // The main database object.
        CCbsDbImp& iDatabase;

        // Cached CB settings.
        TCbsDbImpSettings iSettings;

        // Owns:   Array containing registered settings observers.
        CArrayFixFlat<MCbsDbSettingsObserver*>* iObservers;

        __DECLARE_TEST;
    };

#endif      //  CCBSDBIMPSETTINGS_H   
            
// End of File
