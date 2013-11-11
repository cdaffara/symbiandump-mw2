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
* Description:  Maps extended error values to correct values.
*
*/



#ifndef TSATEXTERRORUTILS_H
#define TSATEXTERRORUTILS_H

#include <e32std.h>
// CLASS DECLARATION

/**
*  Extended GSM error utility class. Used to map extended error codes into
*  correct Network errors. These values are used in SetUpCall, SendSs,
*  SendUssd and SendSm TerminalResponses, when network returns error.
*
*  @lib SatEngine.lib
*  @since 3.0
*/
class TSatExtErrorUtils
    {
    
    public: // New functions

        /**
        * Maps extended error value to correct value.
        * @param aExtError Extended GSM error defined in exterror.h
        * @param aModifyMappedValue, should 8th bit changed into 1 or not. 
        *        default is ETrue
        * @return Hex number indicating the correct value
        */
        IMPORT_C static TUint8 MapError( 
            TInt  aExtError, 
            TBool aModifyMappedValue = ETrue );

        /**
        * Check is given value GSM extended error
        * @param aExtError Value to be checked
        * @return TBool indicating is the value extended GSM error
        */
        IMPORT_C static TBool IsExtendedError( TInt aExtError );

        /**
        * Check should given extended error to be handled as network error
        * @param aExtError Value to be checked
        * @return TBool indicating is the value network error
        */
        IMPORT_C static TBool IsNetworkError( TInt aExtError );
        
    private:
        
        /**
        * Checks specific cases of error codes and changes the code, 
        * if neccessary.
        * @param aExtErrorCode code to be checked
        * @param aNwError, this is where the new nw value is set
        */
        static void CheckSpecificCases( TInt aExtError, TUint8& aNwError );

    };

#endif      // TSATEXTERRORUTILS_H

// End of File
