/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  VoIP specific function extension for
*                Service Provider Settings API
*
*/


#ifndef C_SPSETTINGSVOIPUTILS_H
#define C_SPSETTINGSVOIPUTILS_H

#include <e32base.h>

class CSPSettingsEngine;

/**
 *  VoIP specific extension of Service provider settings API class
 *
 *
 *  @lib serviceprovidersettings.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CSPSettingsVoIPUtils ): public CBase
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CSPSettingsVoIPUtils* NewL();

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CSPSettingsVoIPUtils* NewLC();

        /**
        * Destructors.
        */
        IMPORT_C virtual ~CSPSettingsVoIPUtils();

    public:

        /**
         * Checks is there any VoIP profiles available in service table.
         *
         * @since S60 v3.2
         * @return ETrue if there is at least one VoIP profile.
         */
        IMPORT_C TBool VoIPProfilesExistL() const;

        /**
         * Checks is VoIP supported.
         *
         * @since S60 v3.2
         * @return ETrue if Common Voip and Dynamic VoIP are ON
.        */
        IMPORT_C TBool IsVoIPSupported() const;

        /**
         * Checks the preferred telephony
         *
         * @since S60 v3.2
         * @return ETrue if Common Voip (Feature Manager), 
         *         Dynamic VoIP (Telephony settings) and 
         *         Preferred telephony (Rich call settings) are ON and 
         *         there is at least one SIP-VoIP profile stored
         */
        IMPORT_C TBool IsPreferredTelephonyVoIP() const;

       /**
       * Get the preferred service id
       *
       * @since S60 v3.2
       * @return KErrNone if preferred service found.
       */
      IMPORT_C TInt GetPreferredService( TUint& aServiceId ) const;
    
    private:

        /**
        * C++ default constructor.
        */
        CSPSettingsVoIPUtils();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Checks is there any VoIP profiles available in service table.
        */
        TBool IsVoIPSupportedL() const;

        /**
        * Checks the preferred telephony.
        */
        TBool IsPreferredTelephonyVoIPL() const;

      /**
       * Gets the preferred service id.
       */
       void DoGetPreferredServiceL( TUint& aServiceId ) const;

    private: // data

        /**
         * Settings engine
         * Own.
         */
        CSPSettingsEngine* iSettingsEngine;

    };

#endif // C_SPSETTINGSVOIPUTILS_H
