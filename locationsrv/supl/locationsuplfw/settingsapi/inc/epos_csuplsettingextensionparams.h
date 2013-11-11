/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SUPL Settings extension parameter class
*
*/


#ifndef __EPOS_CSUPLSETTINGEXTENSIONPARAMS_H__
#define __EPOS_CSUPLSETTINGEXTENSIONPARAMS_H__

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS

// CONSTANTS

// CLASS DECLARATION

/**
 * This class is used to specify/retrieve the desired extended server properties.  The required memory for
 * the parameters will be allocated during construction of this class and this class owns
 * the memory allocated.  And same will be released when this class is destroyed.  The 
 * server property includes 
 *  - AGPS server ID.
 *  - Internet Access point 1.
 *  - Internet Access point 2.
 *  - Server address type.
 */
class CServerExtensionParams : public CBase 
	{
    public:  // Constructors and destructor

            
        /**
        * Two-phased constructor.
        *
        * @since S60 5.1
        * @returns A new instance of this class.
        */
        IMPORT_C static CServerExtensionParams* NewL();

        /**
        * Two-phased constructor.
        *
        * @since S60 5.1 
        * @returns A new instance of this class.
        */
        IMPORT_C static CServerExtensionParams* NewLC();

        /**
        * Destructor.
        *
        * @since S60 5.1
        *
        */
        IMPORT_C ~CServerExtensionParams();

    private:

        /**
        * C++ default constructor.
        *
        * @since S60 5.1
        *
        */
	    CServerExtensionParams();

        /**
        * By default EPOC constructor is private.
        *
        * @since S60 5.1
        *
        */
        void ConstructL();
public:

    /**
     * Sets server parameters.  
     * 
     * @param [IN] aServerId Server identifier.Max length can be KMaxServerIdLen. 
     * @param [IN] aIap1 The Internet Access Point Name refers to the access point which is 
     * used to access the HSLP over the internet.  This can have maximum size of 100 characters.
     * @param [IN] aIap2 The Internet Access Point Name refers to the access point which is 
     * used to access the HSLP over the internet.  This can have maximum size of 100 characters.More than one Iap are used for fallback
     * in case of failures.
     * @param [IN] aServerAddr Server address type.Max. length can be KMaxServerAddLen.
     * @param [IN] aServerAddType Inidcates server address type.Max. length can be KMaxServerAddTypeLen.
     *
     * @return following error codes 
     *      - KErrNone if successful
     *      - KErrArgument if any of the argument is not in the specified range
     */
	IMPORT_C TInt Set(
                const TDesC& aServerId,
                const TDesC& aIap1,
                const TDesC& aIap2,
                const TDesC& aServerAddr,
                const TDesC& aServerAddType
                );


   /**
     * Returns server parameters.  
     * 
     * @param [OUT] aServerId Server identifier 
     * @param [OUT] aIap1 The Internet Access Point Name refers to the access point which is 
     * used to access the HSLP over the internet.
     * @param [OUT] aIap2 The Internet Access Point Name refers to the access point which is 
     * used to access the HSLP over the internet.More than one Iap are used for fallback
     * in case of failures.
     * @param [OUT] aServerAddr Inidcates server address.
     * @param [OUT] aServerAddType Inidcates server address type.If not set before,default value is "FQDN".
     *
     * @return following error codes 
     *      - KErrNone if successful
     *      - KErrNotFound if all parameters has reset value
     *	    - KErrOverflow if the input descriptors are too small to hold the values 
     *	    - KErrArgument if client has not allocated buffer for any parameter i.e. if its NULL.
     */
	IMPORT_C TInt Get(
                TPtr aServerId,
                TPtr aIap1,
                TPtr aIap2,
                TPtr aServerAddr,
                TPtr aServerAddType
                ) const;

private:
    HBufC* iServerId;
    HBufC* iIap1;
    HBufC* iIap2;
    HBufC* iServerAddr;
    HBufC* iServerAddType;
    
    
    //To indicate if the values were set using the Set method 
    TBool iIsValueSet;
	};

#endif /* __EPOS_CSUPLSETTINGEXTENSIONPARAMS_H__ */
