/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface to access call information
*
*/


#ifndef MCCECALLARRAY_H
#define MCCECALLARRAY_H

#include <e32std.h>

class MCCECallInfo;

/**
 *  Interface to access calls information
 *
 *  @lib CCE.lib
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( MCCECallArray )
    {
public:
    
    /**
     * Get emergency call info.
     * 
     * @since S60 5.0
     * @return Emergency call info 
     */
    virtual MCCECallInfo* EmergencyCallInfo() const = 0;

    /**
     * Get call info with specific id.
     *
     * @since S60 v3.2
     * @param aCallId Specifies which call is returned
     * @return Found call, NULL if none is found.
     */
    virtual MCCECallInfo* CallInfo( TInt aIndex ) const = 0;
       
    /**
     * Gives the maximum call id.
     * 
     * @since S60 5.0
     * @return Maximum call id.
     */
    virtual TInt MaxNumberOfCalls() const = 0;

    };


#endif // MCCECALLARRAY_H
