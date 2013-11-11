/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Client side access to emergency number information
*
*/


#ifndef CPHCLTEMERGENCYCALL_H
#define CPHCLTEMERGENCYCALL_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class MPhCltEmergencyCallObserver;

// CONSTANTS
const TInt KPhCltEmergencyNumberSize = 100;

// DATA TYPES
typedef TBuf< KPhCltEmergencyNumberSize > TPhCltEmergencyNumber;

// CLASS DECLARATION
/**
*   Phone Client Emergency Call Api for emergency number query
*   and emergency dialing.
*
*   Example:
*
*   // Creates an instance of the class
*   CPhCltEmergencyCall* emergencyCall = CPhCltEmergencyCall::NewL( this );
*
*   CleanupStack::PushL( emergencyCall );
*   TPhCltEmergencyNumber emNumber;
*
*   // Number contains DTMF code
*   _LIT( KTestNumber, "112p123" );
*   emNumber.Copy( KTestNumber );
*   TBool isEmergency( EFalse );  
*   
*   // Check if this number is considered as an emergency number
*   TInt error = emergencyCall->IsEmergencyPhoneNumber( emNumber, isEmergency );
*
*   // Dial
*   if ( isEmergency && !error )
*       {
*       emergencyCall->DialEmergencyCallL( emNumber );
*       }
*
*   // Deletes emergencyCall
*   CleanupStack::PopAndDestroy( emergencyCall );     
*
*
*   @lib phoneclient.lib
*   @since SeriesS60_3.2
*/
class CPhCltEmergencyCall : public CActive
    {
    public:  // Constructors and destructor
 
        /**
        * Create an instance that implements the interface.
        *
        * @param aObserver It is the observer for the instance.
        * @return Returns the created instance.
        */
        IMPORT_C static CPhCltEmergencyCall* NewL( 
            MPhCltEmergencyCallObserver* aObserver );

    public: // New functions    
        
       /**
        * Returns information whether the number specified was an emergency number.
        * 
        * @param aNumber Number to be queried. aNumber might contain prefix (SS code)
        *                and/or postfix (DTMF part).  
        *                The aNumber parameter should be a buffer and the 
        *                clients are provided with a typedef called 
        *                TPhCltEmergencyNumber.
        *
        * @param aIsEmergencyNumber If ETrue, then the aNumber 
        *                           parameter included emergency number.
        * @return Returns error code of the success of the operation.
        */
        IMPORT_C virtual TInt IsEmergencyPhoneNumber(
            const TDesC& aNumber, 
            TBool& aIsEmergencyNumber ) = 0;

        /**
        * Method can be used to check if the number specified contains 
        * a valid emergency number in the end of the number.
        * 
        * @param aNumber Number to be queried, matched emergency number 
        *                is returned in this parameter.
        *                The aNumber parameter should be a buffer and the 
        *                clients are provided with a typedef called 
        *                TPhCltTelephoneNumber.
        *             
        * @param aIsEmergencyNumber If ETrue, then the aNumber parameter 
        *                           was emergency number. 
        * @return Returns error code of the success of the operation.
        */
        IMPORT_C virtual TInt FindEmergencyPhoneNumber(
            TDes& aNumber, 
            TBool& aIsEmergencyNumber ) = 0;

        /**
        * Initiates an emergency call.
        *
        * @param aNumber Emergency number to be dialed.
        *                The aNumber parameter should be a buffer and the 
        *                clients are provided with a typedef called 
        *                TPhCltEmergencyNumber. 
        *
        *                If the aNumber parameter supplies an "empty" buffer 
        *                the call is created either dialing the default emergency 
        *                phone number or use the SET UP EMERGENCY request  
        *                that does not contain a  dialling number.
        *
        *                Leaves with KErrArgument, if parameter aNumber was
        *                not an emergency number.
        *
        * @pre IsActive() returns EFalse, otherwise leaves with KErrInUse.
        */
        virtual void DialEmergencyCallL( const TDesC& aNumber ) = 0;
        
        /**
        * Returns information whether the number specified was an emergency 
        * number and also returns the plain emergency number.
        * 
        * @param aNumber Number to be queried.aNumber might contain prefix (SS code)
        *                and/or postfix (DTMF part).
        *                The aNumber parameter should be a buffer and the 
        *                clients are provided with a typedef called 
        *                TPhCltEmergencyNumber. 
        *
        * @param aMatchedEmergencyNumber Matched emergencynumber without prefix or 
        *                postfix is returned in this parameter. The aMatchedEmergencyNumber 
        *                parameter should be a buffer and the clients are
        *                provided with atypedef called TPhCltEmergencyNumber.
        * 
        * @param aIsEmergencyNumber If ETrue, then the aNumber 
        *                           parameter included emergency number.
        *
        * @return Returns error code of the success of the operation.
        */
        IMPORT_C virtual TInt IsEmergencyPhoneNumber(
            const TDesC& aNumber,
            TDes& aMatchedEmergencyNumber, 
            TBool& aIsEmergencyNumber ) = 0;                    

    protected:

        /**
        * C++ constructor.
        */
        CPhCltEmergencyCall( MPhCltEmergencyCallObserver* aObserver );


    protected: // Data
        
        // Observer to handle events.
        MPhCltEmergencyCallObserver* iObserver;

    };

#endif      // CPHCLTEMERGENCYCALL_H
            
// End of File
