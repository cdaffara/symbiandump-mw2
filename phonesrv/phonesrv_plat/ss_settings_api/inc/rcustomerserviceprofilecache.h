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
* Description:  Customer service profile cache
*
*/


#ifndef RCUSTOMERSERVICEPROFILECACHE_H
#define RCUSTOMERSERVICEPROFILECACHE_H

//  INCLUDES
#include <etelmm.h>
#include <e32property.h>           // RProperty.


// FORWARD DECLARATIONS
class RProperty;

// CLASS DECLARATION

/**
* Customer service profile cache
* @since    1.0
* @lib      SsSettings.lib
*/
class RCustomerServiceProfileCache
    {
    public: // Constructor and destructor
        IMPORT_C RCustomerServiceProfileCache();
        IMPORT_C ~RCustomerServiceProfileCache();

    public: // New functions
        //Settings categories
        enum TSsGroups
            {
            ESsCspGroup1, // contains Call Offering, Call Restriction,
                          // Call Completion and Teleservices
            ESsCspGroup2  // contains Value Added Services
            };

        /**
        * Opens connections. Must be done before fetching any items.
        * @return Error code.
        */
        IMPORT_C TInt Open();

        /**
        * Closes connections.
        */
        IMPORT_C void Close();

        /**
        * Get call completion flags from CSP.
        * @param aParams Call completion parameters.
        * @return Error code.
        */
        IMPORT_C TInt CspCallCompletion( 
            RMobilePhone::TCspCallCompletion& aParams );

        /**
        * Get call offering flags from CSP.
        * @param aParams Call offering parameters.
        * @return Error code.
        */
        IMPORT_C TInt CspCallOffering( 
            RMobilePhone::TCspCallOffering& aParams );

        /**
        * Get call restriction flags from CSP.
        * @param aParams Call restriction parameters.
        * @return Error code.
        */
        IMPORT_C TInt CspCallRestriction( 
            RMobilePhone::TCspCallRestriction& aParams );

        /**
        * Get CPHS tele services flags from CSP.
        * @param aParams Tele services parameters.
        * @return Error code.
        */
        IMPORT_C TInt CspCPHSTeleservices( 
            RMobilePhone::TCspCPHSTeleservices& aParams );

        /**
        * Handles refresh.
        * @return Error code.
        */
        IMPORT_C TInt HandleRefresh();

        /**
        * Get value added services flags from CSP.
        * @since 1.2.
        * @param aParams Value added services parameters.
        * @return Error code.
        */
        IMPORT_C TInt CspCPHSValueAddedServices(
            RMobilePhone::TCspValueAdded& aParams );
        
        /**
        * Get tele services flags from CSP.
        * @since 2.8.
        * @param aParams Tele services parameters.
        * @return Error code.
        */
        IMPORT_C TInt CspTeleServices(
            RMobilePhone::TCspTeleservices& aParams );

        /**
        * Get the changed CSP information after Refresh.
        * @since 2.8.
        * @param aChangedCsps The changed CSP fields, 
        *                     type RMobilePhone::TMobilePhoneCspFileV1Pckg.
        * @param aNewValues The new values of CSP fields, 
        *                   type RMobilePhone::TMobilePhoneCspFileV1Pckg.
        * @return Error code.  KErrNone if successful.
        */
        IMPORT_C TInt ChangedCspTable( 
            TDes8& aChangedCsps,
            TDes8& aNewValues );

    private:

        // Opens connections
        void DoOpenL();

        // Gets the values
        TInt DoGetValues( TInt& aValue, TSsGroups aGroup );

        // Reset the iOldCspFile.
        void DoResetOldCspFile();

        // Define and set the Publish and subscribe value.
        TInt DefineAndSetValue(
            TUid aCategory,
            TUint aKey,
            TInt aValue );

        // Prohibit copy constructor if not deriving from CBase.
        RCustomerServiceProfileCache(
            const RCustomerServiceProfileCache& );
        // Prohibit assigment operator if not deriving from CBase.
        RCustomerServiceProfileCache& operator= (
            const RCustomerServiceProfileCache& );

    private:

        // Member data structure.
        class TCspCacheData
            {
            public:
                TCspCacheData();
                
            // ETel connection.
            RTelServer iEtel;
            
            // Provides client access to mobile phone functionality 
            // provided by TSY.
            RMobilePhone iPhone;
            
            // Defines contents of the CSP (Customer Service Profile) 
            // on the SIM.
            RMobilePhone::TMobilePhoneCspFileV1 iCspFile;
            
            // Packaged contents of the CSP (Customer Service Profile) 
            // on the SIM.
            RMobilePhone::TMobilePhoneCspFileV1Pckg iCspFilePckg;
            
            // RProperty.
            RProperty       iProperty;
            };

        //Pointer to the above.
        TCspCacheData* iData;

        // Defines the old content of the CSP (Customer Service Profile) on 
        // the SIM before refresh.
        RMobilePhone::TMobilePhoneCspFileV1* iOldCspFile;
    };

#endif      // RCUSTOMERSERVICEPROFILECACHE_H  

// End of File
