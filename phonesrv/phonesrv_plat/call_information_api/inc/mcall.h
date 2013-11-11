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
* Description:  Class providing basic information about a call.
*
*/


#ifndef M_CALL_H
#define M_CALL_H

#include <ccpdefs.h>


/**
 *  Class presents a call a contains basic information of it.
 * 
 *
 *  @lib telephonyservice.lib
 *  @since S60 v5.1
 */
NONSHARABLE_CLASS( MCall )
    {

public:   
    

    /**
    * Call index getter
    *
    * @since S60 v5.1    
    * @return the call index
    */
    virtual TInt CallIndex() const = 0;    
    
    /**
    * Call type getter
    *
    * @since S60 v5.1
    * @return the call type
    */    
    virtual CCPCall::TCallType CallType() const = 0;
    
    
    /**
    * Call state getter
    *
    * @since S60 v5.1     
    * @return the call state
    */    
    virtual CCPCall::TCallState CallState() const = 0;
    
    /**
    * Getter for the service ID of the call.
    *
    * @since S60 v5.1    
    * @return the service ID of the call. Not valid for emergency call.
    */
    virtual TUint32 ServiceId() const = 0;
    
    /**
    * Call direction getter
    *
    * @since S60 v5.1     
    * @return the call direction
    */
    virtual CCPCall::TCallDirection CallDirection() const = 0;
    
    /**
     * Returns information whether call is an emergency call.
     * @since S60 v5.2
     * @param None.
     * @return TBool ETrue call is emergency call else EFalse.
     */
    virtual TBool IsEmergency() const = 0;

    };

#endif // M_CALL_H
