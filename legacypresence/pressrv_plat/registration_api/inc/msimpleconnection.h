/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    SIMPLE engine connection
*
*/




#ifndef M_simpleconnection_H
#define M_simpleconnection_H

#include <e32std.h>

/**
 *  MSimpleConnection
 *
 *  SIMPLE engine connection
 *
 *  @lib simpleengine
 *  @since S60 v3.2
 */

class MSimpleConnection
    {

public:

enum TSimpleState
    {
    /** Connection initializing */
    EIdle=1,
    /** Connection active */
    EActive,
    /** Connection suspended */
    ESuspend,
    /** Connection is inactive */
    EInactive,
    /** Connection is permanently unavailable */
    EUnavailable
    };

    /**
     * Destroy this entity
     *
     * @since S60 3.2
     */
    virtual void Close() = 0;

    /**
		 * Login using default SIP Profile settings and SIMPLE Engine default values
		 * instead of given Presence Settings     
     * This method is meant mainly for testing purposes.
     * Leaves KErrAlreadyExists if this MSimpleConnection
     *        entity is already logged in.
     * @since S60 3.2
     * @return operation id
     */
    virtual TInt LoginL() = 0;

    /**
     * Login using specified Presence Settings
     * Leaves KErrAlreadyExists if this MSimpleConnection
     *        entity is already logged in.
     * Leaves KErrNotFound if specified settings
     *        are not found.
     * @since S60 3.2
     * @param aId Presence Settings id
     * @return operation id
     */
    virtual TInt LoginL( TInt aId ) = 0;

    /**
     * Getter for the connection staus.
     *
     * @since S60 3.2
     * @return connection staus
     */
    virtual TSimpleState ConnectionStatus() = 0;
    
    /**
     * Current SIP identity accessor
     *
     * @since S60 3.2     
     * @return SIP identity
     */
    virtual TPtrC8 CurrentSIPIdentityL() = 0;    

    };

#endif

// End of File
