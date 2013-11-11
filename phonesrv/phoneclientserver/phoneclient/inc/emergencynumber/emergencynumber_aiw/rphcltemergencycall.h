/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Client side access to emergency call
*
*/


#ifndef RPhCltEmergencyCall_H
#define RPhCltEmergencyCall_H

//  INCLUDES
#include <e32std.h>
#include "phclttypes.h" 
#include "rphcltserver.h" 

/**
* Emergency number query mode
*/
enum TPhCltEmergencyNumberQueryMode
    {
    EPhCltEmergencyNumberQueryModeNormal = 0,  // Normal mode
    EPhCltEmergencyNumberQueryModeAdvanced,    // advanced mode
    EPhCltEmergencyNumberQueryModeNormalWriteback   // Same as "normal", but writes back emergency number
    };

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Client side access to emergency call
*
*  @lib phoneclient.lib
*  @since Series60_1.0
*/
class RPhCltEmergencyCall : public RSubSessionBase
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        RPhCltEmergencyCall();


    public:  // New functions

        /**
        * Opens the subsession.
        *
        * @param aServer It is the session of which subsession this is.
        * @return Returns error code.
        */
        TInt Open( RPhCltServer& aServer );

        /**
         * Closes the subsession.
         */
        void Close();


    public:

       /**
        * Returns information whether the number specified was an emergency number.
        * 
        * @param aNumber Number to be queried. aNumber might contain prefix (SS code)
        *                and/or postfix (DTMF part).  
        *                The aNumber parameter should be a buffer and the 
        *                clients are provided with a typedef called 
        *                TPhCltEmergencyNumber, which is equivalent to a 
        *                TBuf< KPhCltEmergencyNumberSize > where 
        *                KPhCltEmergencyNumberSize is defined as 100. 
        *
        * @param aIsEmergencyNumber If ETrue, then the aNumber 
        *                           parameter included emergency number.
        * @return Returns error code of the success of the operation.
        */
        TInt IsEmergencyPhoneNumber(
            const TDesC& aNumber, 
            TBool& aIsEmergencyNumber );

        /**
        * Method can be used to check if the number specified contains 
        * a valid emergency number in the end of the number.
        * 
        * @param aNumber Number to be queried, matched emergency number 
        *                is returned in this parameter.
        *                The aNumber parameter should be a buffer and the 
        *                clients are provided with a typedef called 
        *                TPhCltTelephoneNumberwhich is equivalent to a 
        *                TBuf< KPhCltTelephoneNumberLength > where 
        *                KPhCltTelephoneNumberLength is defined as 100. 
        *             
        * @param aIsEmergencyNumber If ETrue, then the aNumber parameter 
        *                           was emergency number. 
        * @return Returns error code of the success of the operation.
        */
        TInt FindEmergencyPhoneNumber(
            TDes& aNumber, 
            TBool& aIsEmergencyNumber );

        /**
        * Initiates an emergency call.
        *
        * @param aReqStatus On return, KErrNone if the emergency call successfully reaches 
        *                   the connected state. If the call set-up fails, the function 
        *                   member will complete with an error.
        *
        * @param aNumber Emergency number to be dialed (optional).
        *                The aNumber parameter should be a buffer and the 
        *                clients are provided with a typedef called 
        *                TPhCltEmergencyNumber,which is equivalent to a 
        *                TBuf< KPhCltEmergencyNumberSize > where 
        *                KPhCltEmergencyNumberSize is defined as 100. 
        *
        *                If the aNumber parameter supplies an "empty" buffer 
        *                the call is created either dialing the default emergency 
        *                phone number or use the SET UP EMERGENCY request  
        *                that does not contain a dialling number.               
        *
        * @pre IsActive() returns EFalse, otherwise leaves with KErrInUse.
        */
        void DialEmergencyCall( TRequestStatus& aReqStatus, TDesC8& aNumber );
        
        /**
        * Cancels an emergency call.
        *
        */
        void CancelDialEmergencyCall();
        
        /**
        * Returns information whether the number specified was an emergency 
        * number and also returns the plain emergency number.
        * 
        * @param aNumber Number to be queried.aNumber might contain prefix (SS code)
        *                and/or postfix (DTMF part).
        *                The aNumber parameter should be a buffer and the 
        *                clients are provided with a typedef called 
        *                TPhCltEmergencyNumberwhich is equivalent to a 
        *                TBuf< KPhCltEmergencyNumberSize > where 
        *                KPhCltEmergencyNumberSize is defined as 100. 
        *
        * @param aMatchedEmergencyNumber Matched emergencynumber without prefix or 
        *                postfix is returned in this parameter. The aMatchedEmergencyNumber 
        *                parameter should be a buffer and the clients are
        *                provided with atypedef called TPhCltEmergencyNumber,
        *                which is equivalent to a TBuf< KPhCltEmergencyNumberSize > 
        *                where KPhCltEmergencyNumberSize is defined as 100.
        * 
        * @param aIsEmergencyNumber If ETrue, then the aNumber 
        *                           parameter included emergency number.
        *
        * @return Returns error code of the success of the operation.
        */
        TInt IsEmergencyPhoneNumber(
            const TDesC& aNumber,
            TDes& aMatchedEmergencyNumber, 
            TBool& aIsEmergencyNumber );                    
        
       private:            
           
        /**
         * Extract telephone number from string. If EFalse is returned
         * then aString does not contain valid emergency number
         * 
         * @param aString string from which those characters are removed.
         *
         * @return ETrue - aString can be emergency number and aString
         *                 contains a telephone number
         *         EFalse - aString cannot be emergency number and aString
         *                  does not contain a number that can be emergency number
         */
        TBool ExtractTelephoneNumber( TDes& aString );
        
        /**
        * Finds start of dtmf string in string.
        *
        * @param   aString      Telephone number
        *
        * @return  Index where dtmf string starts or KErrNotFound if no
        * dtmf found.
        *
        */
        TInt FindStartOfDtmfString( const TDesC& aString );
        
        /**
        * Removes CLIR ss code from beginning of the string if CLIR is
        * found from string.
        *
        * @param    aString Telephone number
        */
        void RemoveCLIR( TDes& aString );
        
        /**
         * Removes DTMF string from string.
         * 
         * @param aString Telephone number.
         */
        void RemoveDTMF( TDes& aString );
        
        /**
        * Executes checks to given phone number to find out if it is a valid 
        * emergency number. 
        *
        * @param    aString             Telephone number
        * @param    aIsEmergencyNumber  If ETrue, then the aNumber parameter 
        *                               was a valid emergency number.
        *
        * @return Returns error code of the success of the operation.
        *
        */
        TInt ValidateNumber( TDes& aString, TBool& aIsEmergencyNumber, 
                            const TBool aWriteEmergencyNumBack );
                            
                                 

    };

#endif // RPhCltEmergencyCall_H

// End of File
