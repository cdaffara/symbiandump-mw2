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
* Description: 
*      Customer Service Profile wrapper layer for SsSettings. 
*      PsetCustomerServiceProfile is a class that access CSP through
*      SSsettings. It is in Phone Settings to provide easier access 
*      to CSP values. Whole class should only be used if CSP is supported.                                                         
*
*
*/


#ifndef     PSETCSP_H
#define     PSETCSP_H

// INCLUDES
#include <e32base.h>
#include <rcustomerserviceprofilecache.h> 

// CLASS DECLARATION
/**
*  CPsetCustomerServiceProfile class is a wrapper for Ss Settings.
*  @lib phonesettings.lib
*  @since 1.0
*/
class CPsetCustomerServiceProfile : public CBase
    {

    public: // constructor & destructor

        /** 
        * Symbian OS 2-phase Constructor. 
        * @return CPsetCustomerServiceProfile object.
        */
        IMPORT_C static CPsetCustomerServiceProfile* NewL();    

        /**
        * Destructor 
        */
        IMPORT_C ~CPsetCustomerServiceProfile();

    public: // New functions

        /**
        * Checks if Call Waiting is to be supported 
        * @param aSupport Feature supported.
        * @return Error code or KErrNone.
        */
        IMPORT_C TInt IsCWSupported( TBool& aSupport );

        /**
        * Checks if Call Barring is to be supported .
        * @param aSupport Feature supported.
        * @return Error code or KErrNone.
        */
        IMPORT_C TInt IsCBSupported( TBool& aSupport );

        /**
        * Checks if Call Forwarding is to be supported.
        * @param aSupport Feature supported.
        * @return Error code or KErrNone.
        */
        IMPORT_C TInt IsCFSupported( TBool& aSupport );

        /**
        * Checks if Alternate Line is to be supported.
        * @param aSupport Feature supported.
        * @return Error code or KErrNone.
        */
        IMPORT_C TInt IsALSSupported( TBool& aSupport );

        /**
        * Opens Customer Service Profile from SIM.
        * @return Error code or KErrNone.
        */
        IMPORT_C TInt OpenCSProfileL();

        /**
        * Checks if Manual Network Selection is to be supported.
        * @since 1.2
        * @param aSupport Feature supported.
        * @return Error code or KErrNone.
        */
        IMPORT_C TInt IsNetworkSelectionSupported( TBool& aSupport );


    public: // New functions

        /**
        * Checks if Call Waiting is to be supported in parameter. 
        * @since 2.8
        * @param aContainer The parameter to be checked.
        * @return ETrue if supported, otherwise EFalse.
        */
        TBool CheckIfCWSupported(
            const RMobilePhone::TCspCallCompletion aContainer ) const;

        /**
        * Checks if Call Barring is to be supported in parameter.
        * @since 2.8
        * @param aContainer The parameter to be checked.
        * @return ETrue if supported, otherwise EFalse.
        */
        TBool CheckIfCBSupported(
            const RMobilePhone::TCspCallRestriction aContainer ) const;

        /**
        * Checks if Call Forwarding is to be supported in parameter.
        * @since 2.8
        * @param aContainer The parameter to be checked.
        * @return ETrue if supported, otherwise EFalse.
        */
        TBool CheckIfCFSupported(
            const RMobilePhone::TCspCallOffering aContainer ) const;

        /**
        * Checks if Alternate Line is to be supported in parameter.
        * @since 2.8
        * @param aContainer The parameter to be checked.
        * @return ETrue if supported, otherwise EFalse.
        */
        TBool CheckIfAlsSupported(
            const RMobilePhone::TCspCPHSTeleservices aContainer ) const;

        /**
        * Checks if Manual Network Selection is supported in parameter.
        * @since 2.8
        * @param aContainer The parameter to be checked.
        * @return ETrue if supported, otherwise EFalse.
        */
        TBool CheckIfNetworkSelectionSupported(
            const RMobilePhone::TCspValueAdded aContainer ) const;

    private:

        /**
        * Checks if the first parameter supports the feature mentioned in the
        * second parameter.
        * @since 2.8
        * @param aContainer The parameter to be checked.
        * @param aFeature The feature to be checked.
        * @return ETrue if supported, otherwise EFalse.
        */
        TBool IsSupported(
            const TInt aContainer,
            const TInt aFeature ) const;


    private: // Member data

        // SsSettings' csp cache.
        RCustomerServiceProfileCache* iCsp;
    };
#endif //PSETCSP_H

// End of File
