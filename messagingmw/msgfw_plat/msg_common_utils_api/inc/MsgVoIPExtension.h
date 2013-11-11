/*
* Copyright (c) 2005, 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Get count of VoIP profiles.
*
*/


#ifndef MSGVOIPEXTENSION_H
#define MSGVOIPEXTENSION_H

#include <e32base.h>

// FORWARD DECLARATIONS
//class CSPSettings;

// CLASS DECLARATION

/**
 * Internal class to get count of VoIP profiles and
 * the value of the settings Preferred Telephone.
 * Allows also to drop RCSE component from rom.
 *
 *  @since Series 60 3.0
 */
class CMsgVoIPExtension:
    public CBase
    {
    
    public:
        /**
         * Two-phased constructor.
         */
        IMPORT_C static CMsgVoIPExtension* NewL();

        /**
         * Destructor.
         */
        virtual ~CMsgVoIPExtension();

        /**
         * Check if any VoIP profiles saved in the phone.
         * @since Series60 3.0
         * @return ETrue if VoIP profiles exist, else EFalse.
         */
        IMPORT_C TBool VoIPProfilesExistL() const;

        /**
         * Check Preferred Telephony key value VoIP
         * @since Series60 3.0
         * @return ETrue if Send key creates VoIP call, else EFalse.
         */
        IMPORT_C TBool IsPreferredTelephonyVoIP() const;

        /**
         * Check if Dynamic VoIP setting is on
         * @since Series60 3.2
         * @return ETrue if VoIP is supported
         */
        IMPORT_C TBool IsVoIPSupported() const;

    private:
        /**
         * C++ default constructor.
         */
        CMsgVoIPExtension();

        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL();

    private:

        /**
         * A Service provider settings
         * Own.  
         */
       // CSPSettings*  iSPSettings;

    };
#endif      // MsgVoIPEXTENSION_H

// End of File
