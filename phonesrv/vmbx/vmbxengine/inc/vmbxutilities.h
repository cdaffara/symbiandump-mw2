/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Utility methods for universal usage
*  Interface   : Private, VmbxUtilities
*
*/


#ifndef VMBXUTILITIES_H
#define VMBXUTILITIES_H

// INCLUDES
#include <voicemailboxdefs.h>

// FORWARD DECLARATIONS


// CLASS DECLARATION

/**
*  Implementation of the auxiliary methods for the VMBX engine classes
*
*  @lib vmbxengine.lib
*/
NONSHARABLE_CLASS(  VmbxUtilities )
    {

public:  // New functions

    /**
     * Verifies that the given string is a valid phone number.
     * Rules:
     * - '+' is only allowed as the first character
     * - The phone number part must be between 3-40 digits
     *
     * @param in aNumber Vmbx number
     * @return ETrue if valid number
     */
    static TBool IsValidPhoneNumber( const TDesC& aNumber );

    /**
    /**
     * Returns active ALS
     *
     * @return Active ALS line
     */
    static TVmbxAlsLineType AlsLine();

    /**
     * Returns state of the video support
     *
     * @return ETrue if Video supported
     */
    static TBool VideoSupported();

    /**
     * Returns state of the VoIP Support
     *
     * @return ETrue if VoIP Supported
     */
    static TBool VoIPSupported();

    /**
     * Verifies that the given number is not an emergency number.
     *
     * @param in aNumber Mailbox number
     * @return ETrue if emergency number
     */
    static TBool IsEmergencyNumber( const TDesC& aNumber );

private:

    /**
     * Verifies that the given string is a valid Tel number.
     *
     * @param in aLexer string to be validate
     * @return ETrue if tel number validate
     */
    static TBool ValidateTelNum( TLex& aLexer );

    /**
     * Verifies that the given string is a valid DTMF number.
     *
     * @param in aLexer string to be validate
     * @return ETrue if DTMf validate
     */
    static TBool ValdateDtmfPart( TLex& aLexer );

    };

#endif    // VMBXUTILITIES_H
