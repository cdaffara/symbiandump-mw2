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
* Description:   Encapsulates the ECom plugged sending service data.
*
*/



#ifndef CSENDINGSERVICEINFO_H
#define CSENDINGSERVICEINFO_H

// INCLUDES
#include <e32base.h>
//#include <SendUi.h>     // TSendingCapabilities

// FORWARD DECLARATIONS
class TSendingCapabilities;
class CSendingServiceInfoImpl;

// CLASS DECLARATION

/**
 * Encapsulates the ECom plugged sending service data.
 *
 * @lib Sendui
 * @since Series 60 3.0
 */ 
class CSendingServiceInfo : public CBase
    {
    public:
    
    enum TServiceFeatures
        {
        EServiceInValid = 1,
        EServiceHidden = 2,
        EServiceCanSendDirectly = 4,
        ENotUsed3 = 8
        };
        
    public:  // Constructors and destructor
    
        /**
        * Two-phased constructor.
        * @return Pointer to object of CSendingServiceInfo.
        */
        IMPORT_C static CSendingServiceInfo* NewL();
        
        /**
        * Two-phased constructor.
        * @return Pointer to object of CSendingServiceInfo.
        */
        IMPORT_C static CSendingServiceInfo* NewLC();

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CSendingServiceInfo();

    public: // New functions

        /**
        * Creates a copy of CSendingServiceInfo object.
        * @since Series 60 3.0
        * @return A pointer to the new copy of the CSendingServiceInfo object.
        */
        IMPORT_C CSendingServiceInfo* CopyLC() const;

        /**
        * Set service human readable name.
        * Service name is used in editors "To"-field.
        * @since Series 60 3.0
        * @param aServiceName Service human readable name.
        * @return None.
        */
        IMPORT_C void SetServiceNameL( const TDesC& aServiceName );

        /**
        * Returns service human readable name.
        * Service name is used in editors "To"-field.
        * @since Series 60 3.0
        * @return Service human readable name.
        */
        IMPORT_C const TPtrC ServiceName() const;

        /**
        * Set service human readable name for the "Send"-menu.
        * @since Series 60 3.0
        * @param aServiceMenuName Service human readable name for the
        *        "Send"-menu.
        * @return None.
        */
        IMPORT_C void SetServiceMenuNameL( const TDesC& aServiceMenuName );

        /**
        * Returns service human readable name for the "Send"-menu.
        * @since Series 60 3.0
        * @return Service human readable name for the "Send"-menu.
        */
        IMPORT_C const TPtrC ServiceMenuName() const;

        /**
        * Set service address.
        * @since Series 60 3.0
        * @param aServiceAddress Service address.
        * @return None.
        */
        IMPORT_C void SetServiceAddressL( const TDesC& aServiceAddress );

        /**
        * Returns service address. 
        * @since Series 60 3.0
        * @return Service address.
        */
        IMPORT_C const TPtrC ServiceAddress() const;

        /**
        * Set sending service id.
        * @since Series 60 3.0
        * @param aServiceId Service id.
        * @return None.
        */
        IMPORT_C void SetServiceId( TUid aServiceId );

        /**
        * Returns sending service id.
        * @since Series 60 3.0
        * @return Service id.
        */
        IMPORT_C TUid ServiceId() const;

        /**
        * Set sending service provider id.
        * @since Series 60 3.0
        * @param aServiceProviderId Service provider id.
        * @return None.
        */
        IMPORT_C void SetServiceProviderId( TUid aServiceProviderId );

        /**
        * Returns sending service provider id.
        * @since Series 60 3.0
        * @return Service provider id.
        */
        IMPORT_C TUid ServiceProviderId() const;

        
          /**
        * Returns sending service technology type id.
        * @since Series 60 3.2
        * @return Service id.
        */
        IMPORT_C void SetTechnologyTypeId( TUid aTechnologyType );
        /**
        * Returns sending service technology type id.
        * @since Series 60 3.2
        * @return Service id.
        */
        IMPORT_C TUid TechnologyTypeId() const;


        /**
        * Set service sending capabilities.
        * @since Series 60 3.0
        * @param aServiceCapabilities Service sending capabilities.
        * @return None.
        */
        IMPORT_C void SetServiceCapabilities(
            TSendingCapabilities aServiceCapabilities );

        /**
        * Returns service sending capabilities.
        * @since Series 60 3.0
        * @return Service sending capabilities.
        */
        IMPORT_C TSendingCapabilities ServiceCapabilities() const;

       /**
        * Set sending service features
        * @since Series 60 3.2
        * @param aServiceCapabilities Sending service features.
        * @return None.
        */
        IMPORT_C void SetServiceFeatures(
            TInt aServiceFeatures );

        /**
        * Returns service features.
        * @since Series 60 3.2
        * @return Sending service features.
        */
        IMPORT_C TInt ServiceFeatures() const;
        

    private:


        /**
        * C++ default constructor.
        */
        CSendingServiceInfo();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:  // Data

        CSendingServiceInfoImpl*            iServiceInfoImpl;
    };

#endif      // CSENDINGSERVICEINFO_H

// End of File
