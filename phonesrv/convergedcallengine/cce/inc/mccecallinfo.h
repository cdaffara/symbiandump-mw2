/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCE call info interface
*
*/



#ifndef MCCECALLINFO_H
#define MCCECALLINFO_H

#include <cccpcallparameters.h>
#include "mccecallobserver.h"

/**
 *  Interface for calls info
 *  
 *
 *  @lib cce.dll
 *  @since S60 v3.2
 */
class MCCECallInfo
    {
    
public:
      
    /**
     * Implementation id info
     *
     * @since S60 v3.2
     * @return Implementation id of call
     */
    virtual const TUid ImplementationUid() const = 0;
    
    /**
     * State of the call.
     * 
     * @since S60 5.0
     * @return State of the call.
     */
    virtual CCPCall::TCallState State() const = 0;
    
    /**
     * Direction of the call.
     * 
     * @since S60 5.0
     * @return Direction of the call.
     */
    virtual CCPCall::TCallDirection Direction() const = 0;
    
    /**
     * Type of the call.
     * 
     * @since S60 5.0
     * @return Type of the call.
     */
    virtual CCPCall::TCallType CallType() const = 0;
    
    /**
     * Service id of the call.
     * 
     * @since S60 5.0
     * @return Service id.
     */
    virtual TUint32 ServiceId() const = 0;
    
    /**
     * Call index of the call.
     * 
     * @since S60 5.0
     * @return Call index.
     */
    virtual TInt CallIndex() const = 0;
    
    /**
     * Returns if call is an emergency call or not.
     * @since Series 60 5.2
     * @param None.
     * @return TBool ETrue call is emergency call else EFalse.
     */
    virtual TBool IsEmergency() const = 0;    
    };

#endif      // MCCECALLINFO_H

// End of File

