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
* Description:  Handles APN operations
*
*/



#ifndef CSATAPNHANDLER_H
#define CSATAPNHANDLER_H

//  INCLUDES
#include <e32base.h>
#include "MSatApnHandler.h"

class MSatUtils;

/**
*  Handles the Access point operations with CMManager. Searches, inserts and
*  deletes APNs, if neccessary
*
*  @lib SatEngine
*  @since S60 3.0
*/
class CSatApnHandler : public CBase,
                       public MSatApnHandler
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aUtils Reference to SAT Utils
        */
        static CSatApnHandler* NewL( MSatUtils& aUtils );

        /**
        * Destructor.
        */
        virtual ~CSatApnHandler();

    public: // Functions from MSatApnHandler

        /**
        * Locates APN from CMManager, creates new APN if not found, and
        * returns IAP id and Network ID in referenced parameters
        * @param aName Accesspoint to locate / create
        * @param aUserLogin, User name in case new APN is needed
        * @param aUserPwd, Password in case, new APN is needed
        * @param aIapId, after completion, holds the IAP id of the APN
        * @param aNwId, after completion, holds the Network id of the APN
        * @param aApnCreated indicates is new APN created
        * @param aPdpType, indicates whether the address type is IPv4  or IPv6
        * @param aLocalAddress, represents the Local Address provided by NAA
        */
        void GetApnInfoL(
            const RSat::TAccessName& aName,
            const RSat::TTextString& aUserLogin,
            const RSat::TTextString& aUserPwd,
            TUint32& aIapId,
            TUint32& aNwId,
            TBool& aApnCreated,
            const RPacketContext::TProtocolType& aPdpType,
            const TDesC8& aLocalAddress );

        /**
        * Deletes the connection method with given APN ID. Note that aApnId is 
        * the same id that GetApnInfoL returns in aNwId.
        * @param aApnId APN to delete
        */
        void DeleteApnL( TUint32 aApnId );

        /**
         * Find the default APN Id
         * @param aPdpType, indicates the address type is IPv4  or IPv6
         * @return a default iap id
         */
        TUint32 FindDefaultApL(
                const RPacketContext::TProtocolType& aPdpType );
    private:

        /**
        * C++ default constructor.
        * @param aUtils Reference to SAT Utils
        */
        CSatApnHandler( MSatUtils& aUtils );

        /**
        * create a new connection method  in case, access point does not exist
        * @param aReqApn Formatted required access point name.
        * @param aUserLogin User name
        * @param aUserPwd Password
        * @param aIapId, after completion, holds the IAP id of the new APN
        * @param aNwId, after completion, holds the Network id of the new APN
        * @param aPdpType, indicates whether the address type is IPv4  or IPv6
        * @param aLocalAddress, represents the Local Address provided by NAA
        */
        void InsertRecordL(
            const TDesC& aReqApn,
            const RSat::TTextString& aUserLogin,
            const RSat::TTextString& aUserPwd,
            TUint32& aIapId,
            TUint32& aNwId,
            const RPacketContext::TProtocolType aPdpType,
            const TDesC8& aLocalAddress );

        /**
        * Formats the APN from SIM into correct format.
        * @param aReqApn Accesspoint to format
        * @return Returns formatted APN in an HBufC
        *         Invoker is responsible to release
        *         the memory! 
        */
        HBufC* FormatAPN( const RSat::TAccessName& aReqApn ) const;

    private:  // Data

        // Reference to command container
        MSatUtils& iUtils;

    };

#endif      // CSATAPNHANDLER_H

// End of File
