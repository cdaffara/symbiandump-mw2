/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides DOS emergency number policy API.
*
*/



#ifndef CDOSEMERGENCYNUMBERPOLICY_H
#define CDOSEMERGENCYNUMBERPOLICY_H

// INCLUDES
#include    <cemergencynumberpolicy.h> 
#include    <rmmcustomapi.h>
#include    <cenrepnotifyhandler.h>

// FORWARD DECLARATIONS
//None.


// CLASS DECLARATION

/**
*  CDosEmergencyNumberPolicy
*  Description: Defines DOS emergency number policy.
*
*  @since Series60_2.6
*/

NONSHARABLE_CLASS( CDosEmergencyNumberPolicy ) : 
    public CEmergencyNumberPolicy,
    private MCenRepNotifyHandlerCallback
    {
    public:  // Constructors and destructor
        
        /**
        * Destructor.
        */
        ~CDosEmergencyNumberPolicy();

    public: // New

        /**
        * Symbian 2nd phase constructor.
        * @param aETelServer a reference of RTelServer.
        */
        void ConstructL( RTelServer& aETelServer );

        /**
        * Emergency number check.
        * @since Series60_2.6
        * @param aTelNumber a telephone number to be checked.
        * @param aMode a check mode, Normal/Advanced.
        * @param aIsEmergencyNumber  return True if the number is emergency.
        * @return error code.
        */
        TInt IsEmergencyNumber( 
                                TPhCltTelephoneNumber& aTelNumber, 
                                TPhCltEmergencyNumberQueryMode aMode,
                                TBool& aIsEmergencyNumber );

    private: // New

        /** 
        * Reads product specific numbers.
        *
        * @return None.
        */
        void ReadProductSpecificNumbers();

        /** 
        * Is number in current list with normal mode.
        *
        * @param aNumber This is number to be checked. 
        * @return Returns ETrue if in list, otherwice EFalse.
        */
        TBool IsInListByNormalMode ( const TDesC& aNumber );

        /** 
        * Is number in current list with advanced mode.
        *
        * @param aNumber This is number to be checked. 
        * @return Returns ETrue if in list, otherwice EFalse.
        */
        TBool IsInListByAdvancedMode ( TDes& aNumber );
        
        /** 
        * Parses number in corrent format to be checked as  
        * emergency number.
        *
        * @since 3.0
        * @param aTelNumber This is number to be parsed. 
        * @return Returns None.
        */
        void ParseNumber( TPhCltTelephoneNumber& aTelNumber );
        
    private: // From MCenRepNotifyHandlerCallback

        void HandleNotifyString( TUint32 aId, const TDesC16& aNewValue );
        
    private:// Data

        // The RTelServer instance.
        /*****************************************************
        *   Series 60 Customer / ETel
        *   Series 60  ETel API
        *****************************************************/
        RTelServer                                   iETelServer;

        // The RMobilePhone instance.
        /*****************************************************
        *   Series 60 Customer / ETel
        *   Series 60  ETel API
        *****************************************************/
        RMobilePhone                                 iMobilePhone;

        // The CustomAPI instance.
        /*****************************************************
        *   Series 60 Customer / ETel
        *   Series 60  ETel API
        *****************************************************/
        RMmCustomAPI                                 iCustomPhone;

        // The information if advanced Emergency check is supported.
        TBool                                        iAdvancedCheck;


        // The product specific emergency numbers.
        TBuf< KEnPolicySDEmergencyNumberListLength > iProductSpecificList;

        /**
         * CenRep, needed by listener
         * 
         * Own
         */
        CRepository* iCenRep;
        
        /**
         * CenRep listener
         * 
         * Own
         */
        CCenRepNotifyHandler* iListener;
        
    };

#endif  // CDOSEMERGENCYNUMBERPOLICY_H

// End of File
