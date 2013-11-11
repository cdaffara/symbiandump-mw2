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
* Description:  Accessor for system states.
*
*/



#ifndef MSATSYSTEMSTATE_H
#define MSATSYSTEMSTATE_H

//  INCLUDES
#include <e32std.h>
#include <etelmm.h>
#include <etelsat.h>
#include <PSVariables.h>

// CLASS DECLARATION

/**
*  Accessors for system state.
*
*  @lib SatSystemState.lib
*  @since Series 60 3.0
*/
class MSatSystemState
    {
    public:  // Constructors and destructor

        /*
        * Destructor.
        */
        virtual ~MSatSystemState() {};

    public: // New functions

        /**
        * Getter for NetworkRegistrationStatus
        * @return Network registration status.
        */
        virtual RMobilePhone::TMobilePhoneRegistrationStatus
            GetNetworkRegistrationStatus() = 0;

        /**
        * Checks is call active.
        * @return ETrue if call is currently active.
        */
        virtual TBool IsCallActive() = 0;

        /**
        * Checks is call incoming.
        * @return ETrue if call is incoming.
        */
        virtual TBool IsCallIncoming() = 0;

        /**
        * Gets the Service Center Address number.
        * @return SCA number.
        */
        virtual const TDesC& SCANumber() = 0;

        /**
        * Checks whether the phone is in silent mode.
        * @return Silent mode activated state.
        */
        virtual TBool IsSilentMode() = 0;

        /**
        * Checks whether the phone is in idle state.
        * @return ETrue if phone is in idle mode.
        */
        virtual TBool IsPhoneInIdleStateL() = 0;
        
        /**
        * Checks whether the screen saver is actived from idle.
        * @return ETrue if screen saver is actived from idle.
        */        
        virtual TBool IsScreenSaverActivedFromIdle() = 0;

        /**
        * Sets time between language selection and reboot.
        * @param aTime Time before reboot.
        * @return KErrNone if time is set successfully.
        */
        virtual TInt WriteLanguageSelectionTime(
            TInt aTime ) = 0;

        /**
        * Puts proactive Idle Mode Text command data to shared data
        * @param aString Idle mode text string
        * @param aIconId Idle mode text icon ID
        * @param aIconQualifier Idle mode text icon qualifier
        * @return KErrNone if setting data is OK
        */
        virtual TInt WriteSetIdleModeText(
            const TDesC& aString,
            TInt aIconId,
            RSat::TIconQualifier aIconQualifier ) = 0;

        /**
        * Checks is backup / restore process ongoing.
        * @return TBool indicating is process ongoing or not.
        */
        virtual TBool IsBackupProcessOngoing() = 0;

        /**
        * Checks is ConfirmSatOperations on.
        * @return TBool indicating is confirmation on or off.
        */
        virtual TBool IsConfirmSatOperationsOn() = 0;

        /**
        * Gets the display language from CentRep and returns it.
        * @return ID indicating the selected language.
        */
        virtual TInt DisplayTextLanguage() = 0;

        /**
        * Checks is Bluetooth SIM Access Profile active
        * @return ETrue if BT SAP is active
        */
        virtual TBool IsBtSapActive() = 0;
        
        /**
        * Checks are phone warnings and game tones on.
        * @return TBool indicating wether phone are warnings and game tone on.
        */
        virtual TBool IsWarningAndGameTonesOn() = 0;

    protected:

        /**
        * Constructor.
        */
        MSatSystemState() {};

        // Prohibit copy constructor if not deriving from CBase.
        MSatSystemState( const MSatSystemState& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatSystemState& operator=( const MSatSystemState& );

    };

#endif      // MSATSYSTEMSTATE_H

// End of File
