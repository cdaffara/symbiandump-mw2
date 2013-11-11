/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CPsetSAObserver handles all the queries to observer.
*
*
*/


#ifndef     PSETSAOBSERVER_H
#define     PSETSAOBSERVER_H

//  INCLUDES
#include <e32base.h>
#include <e32property.h> 
#include <psetcalldiverting.h>
#include <PSVariables.h>
#include <ctsydomaincrkeys.h>

// FORWARD DECLARATIONS
class MPsetNetworkInfoObserver;
class MPsetDivertObserver;
class CPSetSubscriberIdCheck;

//  CONSTANTS  
const TInt KCodeUnspecified(-1);

// CLASS DECLARATION

/**
*  Class is used to create params for NotifyDivertChange method.
*
*  @lib phonesettings.dll
*  @since 3.2
*/
class TCallDivertNotifySetting
    {
    public: // new    
    
        void Initialize()
            {
            iPreviousCfStatus = KCFIndicatorUnknown;
            iCfActivated = EFalse;
            iVmbxDivert = EFalse;
            iBasicServiceCode = KCodeUnspecified;
            }
            
    public: //member data
    
        // Current status of divert. Note: GetCurrentDivertStatus returns
        // current status.
        TUnconditionalCFStatus iPreviousCfStatus;
   
        // ETrue if activating divert and EFalse if deactivating.
        TBool iCfActivated;
        
        // True if voice mailbox divert, otherwise false.
        TBool iVmbxDivert;
        
        // BasicServiceCode related to divert operation, see nwDefs.h
        // for more information. 
        TInt iBasicServiceCode;
    };

/**
*  Class is used to exchange information to/from observer.
*
*  @lib phonesettings.dll
*  @since 1.0
*/
class CPsetSAObserver : public CActive
    {
    public: // constructor & destructor

        /**
        * First phase constructor.
        * 
        * @return Returns the CPsetSAObserver-object
        */
        IMPORT_C static CPsetSAObserver* NewL();

        /* Destructor */
        IMPORT_C ~CPsetSAObserver();

    public:

        //Current call status.
        enum TPSetCallStatus
            {
            EPSetNoCallsActive = 0,
            EPSetCallActive
            };

        /**
        * Queries if there is an active call.
        *
        * @return call status (active or not)
        */
        IMPORT_C TInt IsCallActive();


        //GPRS connection status.
        enum TPSetGPRSConnectionStatus
            {
            EPSetGPRSNotConnected = 0,
            EPSetGPRSConnectionActive
            };

        /**
        * Queries if there is an active gprs connection.
        *
        * @return gprs connection status (active or not)
        */
        IMPORT_C TInt IsGPRSConnected();

        /**
        * Sets Network observer object.
        *
        * @param aObserver Reference to network observer.
        */
        IMPORT_C void SetNetObserver( MPsetNetworkInfoObserver& aObserver );
               
        /**
        * Notifies of Divert changes. If the activator is not sure of the
        * settings, CPsetSAObserver tries to deduce correct state.
        *
        * @param aAlsStatus Currently used line.
        * @param aDivertNotify set of divert params, see TCallDivertNotifySetting above.
        * @param aActivatorUncertain Call divert requester does not know for certain
        *        all the values.
        */
        IMPORT_C void NotifyDivertChange( TSelectedLine aAlsStatus,
            TCallDivertNotifySetting& aDivertNotify, const TInt& aActivatorUncertain );
        
        /**
        * Returns whether there are active diverts and on what line.
        *
        * @param aStatus TCallForwardingStatus info.
        * @return KErrNone if successful else system wide error code if 
        * get fails.
        */
        IMPORT_C TInt GetCurrentDivertStatus( TUnconditionalCFStatus& aStatus );

    private: // from CActive

        void RunL();

        void DoCancel();

    private: //new
    
        /**
        * Handles divert indicator cases when ALS if OFF.
        */
        void HandleNoAlsIndicators( TCallDivertNotifySetting& aDivertNotify,
            const TInt& aActivatorUncertain );
          
        /**
        * Handles divert indicator cases when ALS on and
        * ALS line 1 active.
        */  
        void HandlePrimaryLineIndicators( TCallDivertNotifySetting& aDivertNotify );

        /**
        * Handles divert indicator cases when ALS on and
        * ALS line 2 active.
        */
        void HandleSecondaryLineIndicators( TCallDivertNotifySetting& aDivertNotify );
        
        /**
        * Set correct indicator value when divert activated 
        * to voicemail (no ALS).
        */
        void HandleVoiceMailBoxDivertActivation( 
            TUnconditionalCFStatus& aStatus,
            const TInt aBsc  );
        
        /**
        * Set correct indicator value when diverted to some
        * other number than voicemail (no ALS).
        */   
        void HandleDivertActivation( 
            TUnconditionalCFStatus& aStatus,
            const TInt aBsc  );
        
        /**
        * Set correct indicator value when deactivation
        * done (no ALS).
        */        
        void HandleDivertDeactivation( 
            TUnconditionalCFStatus& aStatus,
            const TInt aBsc  );
        
        /**
        * Set correct indicator value when Primary line 
        * activation done.
        */
        void HandlePrimaryLineDivertActivation( 
            TUnconditionalCFStatus& aStatus,
            const TInt aBsc  );
        
        /**
        * Set correct indicator value when Primary line 
        * deactivation done.
        */
        void HandlePrimaryLineDivertDeactivation( 
            TUnconditionalCFStatus& aStatus,
            const TInt aBsc  );
        
        /**
        * Set correct indicator value when Secondary line 
        * activation done.
        */
        void HandleSecondaryLineDivertActivation( 
            TUnconditionalCFStatus& aStatus,
            const TInt aBsc  );
        
        /**
        * Set correct indicator value when Secondary line 
        * deactivation done.
        */
        void HandleSecondaryLineDivertDeactivation( 
            TUnconditionalCFStatus& aStatus,
            const TInt aBsc  );
        
        /**
        * Set correct indicator value when Primary line 
        * activation done.
        */
        void HandlePrimaryLineDualDivertActivation( 
            TUnconditionalCFStatus& aStatus,
            const TInt aBsc  );
        
        /**
        * Set correct indicator value when Primary line 
        * deactivation done.
        */
        void HandlePrimaryLineDualDivertDeactivation( 
            TUnconditionalCFStatus& aStatus,
            const TInt aBsc  );
                
        /**
        * Set correct indicator value when Secondary line 
        * activation done.
        */
        void HandleSecondaryLineDualDivertActivation( 
            TUnconditionalCFStatus& aStatus,
            const TInt aBsc  );
        
        /**
        * Set correct indicator value when Secondary line 
        * deactivation done.
        */
        void HandleSecondaryLineDualDivertDeactivation( 
            TUnconditionalCFStatus& aStatus,
            const TInt aBsc  );
        
        /**
        * Starts to listen to call alerting or call ringing events.
        */
        void NotifyCallStartedL();
        
        /**
        * Checks KTelephonyLVFlagDivertDualAffect status.
        */
        TBool IsDivertDualAffectFeatureEnabledL();

    private: // constructors

        void ConstructL();

        CPsetSAObserver();

    private: //member data

        //Observer for network related SS.
        MPsetNetworkInfoObserver*   iNetObserver;
        
        // The Notify interface to Publish And Subscribe.
        RProperty                   iNotifyProperty;
        
        CRepository* iRepository;
    }; 
#endif // PSETSAOBSERVER_H
// end of file
