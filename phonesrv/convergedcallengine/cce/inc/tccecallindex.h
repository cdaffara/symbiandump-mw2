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
* Description:  Keeps up the call index
*
*/


#ifndef TCCECALLINDEX_H
#define TCCECALLINDEX_H

#include <e32std.h>

class MCCECallArray;
class MCCECallInfo;

const TInt KInvalidCallIndex = 0;

/**
 *  Keeps track of the next call index for new call.
 *
 *  @lib CCE.lib
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( TCCECallIndex )
    {
public:

    TCCECallIndex( MCCECallArray& aCallArray );
    
    /**
     * Returns current available call index
     *
     * @since S60 5.0
     * @return Call index
     */
    TInt CurrentCallIndex() const;
    
private:
    
    TBool CheckAndIncrementCallIndex( 
        MCCECallInfo* aCallInfo, 
        TInt& aCallIndex ) const;
    
private: // Data
    
    /**
     * Call array.
     * Not own.
     */
    MCCECallArray& iCallArray;
    
    };

#endif // TCCECALLINDEX_H
