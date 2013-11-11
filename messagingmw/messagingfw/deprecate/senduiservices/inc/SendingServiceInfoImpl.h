/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Implementation of CMessageAddress interface.
*                Encapsulates the ECom plugged sending service data.
*
*/



#ifndef CSENDINGSERVICEINFOIMPL_H
#define CSENDINGSERVICEINFOIMPL_H

// INCLUDES
#include <e32base.h>
#include <sendui.h>     // TSendingCapabilities

class RReadStream;
class RWriteStream;
// CLASS DECLARATION

/**
 * Encapsulates the ECom plugged sending service data.
 *
 * @lib Sendui
 * @since Series 60 3.0
 */ 
class CSendingServiceInfoImpl : public CBase
    {
    public:  // Constructors and destructor
    
        /**
        * Two-phased constructor.
        * @return Pointer to object of CSendingServiceInfoImpl.
        */
        static CSendingServiceInfoImpl* NewL();

        /**
        * Two-phased constructor.
        * @return Pointer to object of CSendingServiceInfoImpl.
        */
        static CSendingServiceInfoImpl* NewLC();        

        /**
        * Destructor.
        */
        virtual ~CSendingServiceInfoImpl();

    public: // New functions

        /**
        * Creates a copy of CSendingServiceInfoImpl object.
        * @since Series 60 3.0
        * @return A pointer to the new copy of the CSendingServiceInfoImpl object.
        */
        CSendingServiceInfoImpl* CopyL() const;

        /**
        * Set service human readable name.
        * Service name is used in editors "To"-field.
        * @since Series 60 3.0
        * @param aServiceName Service human readable name.
        * @return None.
        */
        void SetServiceNameL( const TDesC& aServiceName );

        /**
        * Returns service human readable name.
        * Service name is used in editors "To"-field.
        * @since Series 60 3.0
        * @return Service human readable name.
        */
        inline const TPtrC ServiceName() const;

        /**
        * Set service human readable name for the "Send"-menu.
        * @since Series 60 3.0
        * @param aServiceMenuName Service human readable name for the
        *        "Send"-menu.
        * @return None.
        */
        void SetServiceMenuNameL( const TDesC& aServiceMenuName );

        /**
        * Returns service human readable name for the "Send"-menu.
        * @since Series 60 3.0
        * @return Service human readable name for the "Send"-menu.
        */
        inline const TPtrC ServiceMenuName() const;

        /**
        * Set service address.
        * @since Series 60 3.0
        * @param aServiceAddress Service address.
        * @return None.
        */
        void SetServiceAddressL( const TDesC& aServiceAddress );

        /**
        * Returns service address. 
        * @since Series 60 3.0
        * @return Service address.
        */
        inline const TPtrC ServiceAddress() const;

        /**
        * Set sending service id.
        * @since Series 60 3.0
        * @param aServiceId Service id.
        * @return None.
        */
        inline void SetServiceId( TUid aServiceId );

        /**
        * Returns sending service id.
        * @since Series 60 3.0
        * @return Service id.
        */
        inline TUid ServiceId() const;

        /**
        * Set sending service provider id.
        * @since Series 60 3.0
        * @param aServiceProviderId Service provider id.
        * @return None.
        */
        inline void SetServiceProviderId( TUid aServiceProviderId );

        /**
        * Returns sending service provider id.
        * @since Series 60 3.0
        * @return Service provider id.
        */
        inline TUid ServiceProviderId() const;

        /**
        * Set service sending capabilities.
        * @since Series 60 3.0
        * @param aServiceCapabilities Service sending capabilities.
        * @return None.
        */
        inline void SetServiceCapabilities(
            TSendingCapabilities aServiceCapabilities );

        /**
        * Returns service sending capabilities.
        * @since Series 60 3.0
        * @return Service sending capabilities.
        */
        inline TSendingCapabilities ServiceCapabilities() const;
        
        /**
        * Returns sending service's technology type id.
        * @since Series 60 3.1
        * @return Technology type.
        */
        inline TUid TechnologyTypeId() const;
        
       /**
        * Set sending service id.
        * @since Series 60 3.0
        * @param aServiceId Service id.
        * @return None.
        */
        inline void SetTechnologyTypeId( TUid aTechnologyTypeId );
        

        void ExternalizeL( RWriteStream& aStream );
    
        void InternalizeL( RReadStream& aStream );

        /**
        * Set service sending capabilities.
        * @since Series 60 3.0
        * @param aServiceCapabilities Service sending capabilities.
        * @return None.
        */
        inline void SetServiceFeatures( TInt aServiceFeatures );

        /**
        * Returns service sending capabilities.
        * @since Series 60 3.0
        * @return Service sending capabilities.
        */
        inline TInt ServiceFeatures() const;
        
    private:


        /**
        * C++ default constructor.
        */
        CSendingServiceInfoImpl();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:  // Data

        HBufC*                      iServiceName;
        HBufC*                      iServiceMenuName;
        HBufC*                      iServiceAddress;
        TUid                        iServiceId;
        TUid                        iServiceProviderId;
        TUid                        iTechnologyTypeId;
        TSendingCapabilities        iServiceCapabilities;
        TInt                        iServiceFeatures;
    };

#include "CSendingServiceInfo.inl"

#endif      // CSENDINGSERVICEINFOIMPL_H

// End of File
