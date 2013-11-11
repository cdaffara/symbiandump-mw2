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
* Description:  Provides system state information to SAT Server
*
*/



#ifndef CSATSYSTEMSTATE_H
#define CSATSYSTEMSTATE_H

//  INCLUDES
#include <e32base.h>
#include <e32property.h>
#include <msvapi.h>
#include "MSatSystemState.h"
#include "csatnetworkregstatusmonitor.h"

// PREDEFINED CLASS
class MSatMultiModeApi;

// CONSTANTS
const TInt KMaxSCANumberSize = 251;

/**
*  Accessors for system state information.
*
*  @lib SatSystemState
*  @since Series 60 3.0
*/
class CSatSystemState : public CBase,
                        public MSatSystemState,
                        public MMsvSessionObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aPhone Reference to mobile phone
        */
        static MSatSystemState* NewL( MSatMultiModeApi& aPhone );

        /**
        * Destructor.
        */
        virtual ~CSatSystemState();

    public: // Functions from base classes

        /**
        * From MSatSystemState Gets the network registrtaion status
        * @return Mobile phone's registration status
        */
        RMobilePhone::TMobilePhoneRegistrationStatus
            GetNetworkRegistrationStatus();

        /**
        * From MSatSystemState Checks is call active.
        * @return TBool indicating wether call is active or not
        */
        TBool IsCallActive();

        /**
        * From MSatSystemState Checks is call active.
        * @return TBool indicating wether call is coming or not
        */
        TBool IsCallIncoming();

        /**
        * From MSatSystemState Gets SCA number from CommsDB and
        * returns it.
        * @return SCANumber from CommsDB.
        */
        const TDesC& SCANumber();

        /**
        * From MSatSystemState Checks is phone in silent mode.
        * @return TBool indicating wether phone is in silent mode
        */
        TBool IsSilentMode();

        /**
        * From MSatSystemState Checks is phone in idle state.
        * @return TBool indicating wether phone is in idle mode
        */
        TBool IsPhoneInIdleStateL();

        /**
        * From MSatSystemState Checks is screen saver actived from idle.
        * @return TBool indicating whether screen saver is actived from idle.
        */
        TBool IsScreenSaverActivedFromIdle();
        
        /**
        * From MSatSystemState Sets time between language selection and reboot.
        * @param aTime Time before reboot.
        * @return KErrNone if time is set successfully.
        */
        TInt WriteLanguageSelectionTime(
            TInt aTime );

        /**
        * From MSatSystemState Puts proactive Idle Mode Text command data to
        * shared data.
        * @param aString Idle mode text string
        * @param aIconId Idle mode text icon ID
        * @param aIconQualifier Idle mode text icon qualifier
        * @return KErrNone if setting data is OK
        */
        TInt WriteSetIdleModeText(
            const TDesC& aString,
            TInt aIconId,
            RSat::TIconQualifier aIconQualifier );

        /**
        * From MSatSystemState Checks is backup / restore process ongoing.
        * @return TBool indicating is process ongoing or not.
        */
        TBool IsBackupProcessOngoing();

        /**
        * From MSatSystemState Checks is ConfirmSatOperations on.
        * @return TBool indicating is confirmation on or off.
        */
        TBool IsConfirmSatOperationsOn();

        /**
        * From MSatSystemState Gets the display language from
        * CentRep and returns it.
        * @return ID indicating the selected language.
        */
        TInt DisplayTextLanguage();

        /**
        * Checks is Bluetooth SIM Access Profile active
        * @return ETrue if BT SAP is active
        */
        TBool IsBtSapActive();

        /**
        * From MMsvSessionObserver Indicates an event has occurred from a
        * Message Server session.
        * @param aEvent  Indicates the event type
        * @param aArg1  Event type-specific argument value
        * @param aArg2  Event type-specific argument value
        * @param aArg3  Event type-specific argument value
        */
        virtual void HandleSessionEventL(
            TMsvSessionEvent /*aEvent*/,
            TAny* /*aArg1*/,
            TAny* /*aArg2*/,
            TAny* /*aArg3*/) {};
            
        /**
        * From MSatSystemState Checks are phone warnings and game tones on.
        * @return TBool indicating wether phone are warnings and game tone on.
        */
        TBool IsWarningAndGameTonesOn();

    private:

        /**
        * C++ default constructor.
        * @param aPhone Reference to mobile phone
        */
        CSatSystemState( MSatMultiModeApi& aPhone );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( MSatMultiModeApi& aPhone );

        /**
        * Reads current Short Message Service Center Address.
        * @param aAddress The reference for SCA number.
        */
        void ReadCurrentSMSCL( TDes& aAddress );

        /**
        * Specifies an entry in the Message Server index.
        * @param aSession The current message server session.
        * @param aMtm The SMS messaging type module UID.
        * @return The entry in the Message Server index.
        */
        TMsvId ServiceForMTML(
            CMsvSession& aSession,
            const TUid& aMtm ) const;

    private:    // Data

        // Network registration status monitor
        CSatNetworkRegStatusMonitor* iNetworkRegStatusMonitor;

        // Descriptor for SCA number
        TBuf<KMaxSCANumberSize> iScaAddress;

        // Reference for MSatMultiModeApi
        MSatMultiModeApi& iPhone;

    };

#endif      // CSATSYSTEMSTATE_H

// End of File
