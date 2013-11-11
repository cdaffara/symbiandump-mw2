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
* Description:  Client-Server framework 
*
*/

/*! \file epos_suplterminalparams.h
    \brief Supl Server Properties class
*/


#ifndef __EPOS_CSUPLTERMINALPARAMS_H__
#define __EPOS_CSUPLTERMINALPARAMS_H__

//  INCLUDES
#include <e32base.h>

/**
 * This class is used to specify/retrieve the desired server properties.  The required memory for
 * the parameters will be allocated during construction of this class and this class owns
 * the memory allocated.  And same will be released when this class is destroyed.  The 
 * server property includes 
 *  - Supported protocol version.
 *  - Emergency support.
 *  - TLS enabled / disabled.
 *  - PSK used / not used ( GBA or any other key generation mechanism ).
 *  - IAP / destination.
 *  - SLP address.
 *  - SLP enabled / disabled.
 *  - Manufacturer ( Optional ).
 *  - Remove SLP, when SIM change ( yes / no ).
 *  - Usage in home network ( yes / no ).
 *  - Timestamp ( when last used ).
 *  - Timestamp ( when tried last ).
 *  - Network & Operator code ( when tried last ).
 *  - Network & Operator code ( when last used successfully).
 *  - Editable ( yes / no).
 *
 *  From the abover properties, only following fieds can be changed by SUPL settings client:
 *  - SLP address
 *  - IAP / destination.
 *  - SLP enabled / disabled.
 *  - Remove SLP, when SIM change ( yes / no ).
 *  - Usage in home network ( yes / no ).
 *  - Editable ( yes / no).
 * 
 */
class CSuplTerminalParams : public CBase 
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @since S60 9.1TB
        * @returns A new instance of this class.
        */
        IMPORT_C static CSuplTerminalParams* NewL();

        /**
        * Two-phased constructor.
        *
        * @since S60 9.1TB 
        * @returns A new instance of this class.
        */
        IMPORT_C static CSuplTerminalParams* NewLC();

        /**
        * Destructor.
        *
        * @since S60 9.1TB
        *
        */
        IMPORT_C ~CSuplTerminalParams();

    private:

        /**
        * C++ default constructor.
        *
        * @since S60 9.1TB
        *
        */
	    CSuplTerminalParams();

        /**
        * By default EPOC constructor is private.
        *
        * @since S60 9.1TB
        *
        */
        void ConstructL();
public:

    /**
     * Sets server parameters.  
     * 
     * @param [IN] aServerAddress Server address in string format 
     * @param [IN] aIapName The Internet Access Point Name refers to the access point which is 
     * used to access the HSLP over the internet.  This can have maximum size of 100 characters.
     * @param [IN] aServerEnabled Inidcates whether server enabled or not
     * @param [IN] aSimChangeRemove Inidcates whether this server entry needs to be deleted when SIM changes 
     * @param [IN] aUsageInHomeNw Inidcates whether server can be used in the home network or not 
     * @param [IN] aEditable Inidcates whether this server entry can be edited or not 
     *
     * @return following error codes 
     *      - KErrNone if successful
     *      - KErrArgument if any of the argument is not in the specified range or values specified are not reset value
     */
	IMPORT_C TInt Set(
                const TDesC& aServerAddress,
                const TDesC& aIapName,
                const TBool aServerEnabled = ETrue,
                const TBool aSimChangeRemove = EFalse,
                const TBool aUsageInHomeNw = ETrue,
                const TBool aEditable = ETrue,
                const TInt64 aSlpId = 0
                );


    /**
     * Returns server parameters. 
     *
     * @param [OUT] aServerAddress Server address in string format 
     * @param [OUT] aIapName The Internet Access Point Name refers to the access point which is 
     * used to access the HSLP over the internet.  This can have maximum size of 100 characters.
     * @param [OUT] aServerEnabled Inidcates whether server enabled or not
     * @param [OUT] aSimChangeRemove Inidcates whether this server entry needs to be deleted when SIM changes 
     * @param [OUT] aUsageInHomeNw Inidcates whether server can be used in the home network or not 
     * @param [OUT] aEditable Inidcates whether this server entry can be edited or not 
     *
     * @return following error codes
     *      - KErrNone if successful
     *      - KErrNotFound if all of the parameters has reset value 
     */
	IMPORT_C TInt Get(
                TInt64& aSlpId,                    
                TPtr aServerAddress,
                TPtr aIapName,
                TBool& aServerEnabled,
                TBool& aSimChangeRemove,
                TBool& aUsageInHomeNw,
                TBool& aEditable
                ) const;

private:
    TInt64 iSlpId;
    HBufC* iServerAddress;
    HBufC* iIap;
    TBool iServerEnabled;
    TBool iSimChangeRemove;
    TBool iUsageInHomeNw;
    TBool iEditable;
    TBool iIsValueSet;
	};

#endif /* __EPOS_CSUPLTERMINALPARAMS_H__ */
