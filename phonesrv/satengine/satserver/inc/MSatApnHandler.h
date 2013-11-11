/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface for APN handler
*
*/


#ifndef MSATAPNHANDLER_H
#define MSATAPNHANDLER_H

//  INCLUDES
#include <e32std.h>
#include <etelsat.h>
#include <etelpckt.h>
// CLASS DECLARATION

/**
*  Interface for APN operations with CMManager.
*
*  @lib SatEngine
*  @since S60 3.0
*/
class MSatApnHandler
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        MSatApnHandler() {};

        /**
        * Destructor.
        */
        virtual ~MSatApnHandler() {};

    public: // New functions

        /**
        * Locates APN from CMManager Session, creates new APN if not  
        * found, and returns IAP id and Network ID in referenced parameters
        * @param aName Accesspoint to locate / create
        * @param aUserLogin, User name in case new APN is needed
        * @param aUserPwd, Password in case, new APN is needed
        * @param aIapId, after completion, holds the IAP id of the APN
        * @param aNwId, after completion, holds the Network id of the APN
        * @param aApnCreated indicates is new APN created
        * @param aPdpType, indicates whether the address type is IPv4  or IPv6
        * @param aLocalAddress, represents the Local Address provided by NAA
        */
        virtual void GetApnInfoL( 
            const RSat::TAccessName& aName,
            const RSat::TTextString& aUserLogin,
            const RSat::TTextString& aUserPwd,
            TUint32& aIapId,
            TUint32& aNwId,
            TBool& aApnCreated, 
            const RPacketContext::TProtocolType& aPdpType,
            const TDesC8& aLocalAddress ) = 0;

        /**
        * Deletes APN from connection method list with given APN ID. Note  
        * that aApnId is the same id that GetApnInfoL returns in aNwId.
        * @param aApnId APN to delete
        */
        virtual void DeleteApnL( TUint32 aApnId ) = 0;

        /**
         * Find the default APN Id
         * @param aPdpType, indicates the address type is IPv4  or IPv6
         * @return a default iap id
         */
        virtual TUint32 FindDefaultApL(
                const RPacketContext::TProtocolType& aPdpType ) = 0;
    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatApnHandler( const MSatApnHandler& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatApnHandler& operator=( const MSatApnHandler& );

    };

#endif      // MSATAPNHANDLER_H

// End of File
