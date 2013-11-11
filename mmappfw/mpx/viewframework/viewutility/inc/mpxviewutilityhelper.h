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
* Description:  MPX view utility helper
*
*/



#ifndef CMPXVIEWUTILITYHELPER_H
#define CMPXVIEWUTILITYHELPER_H


// INCLUDES
#include <e32base.h>

// CLASS DECLARATION

/**
 *  MPX view utility helper.
 *
 *  @lib mpxviewutility.lib
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CMPXViewUtilityHelper )
    {
public:

    /**
     * Extracts number from data.
     *
     * @param aData Plugin's opaque data.
     * @param aNextPos Next position to be searched from. On return, this value
     *                 will be updated to next position. KErrNotFound if
     *                 the end of data is reached.
     * @param aResult Signed integer if found.
     * @return KErrNone if found. KErrNotFound if not found.
     *         KErrGeneral if aNextPos is negative or aNextPos is greater than
     *         the length of aData.
     */
    static TInt ExtractNum(
        const TDesC8& aData,
        TInt& aNextPos,
        TInt& aResult );

    /**
     * Extracts number from data.
     *
     * @param aData Plugin's opaque data.
     * @param aNextPos Next position to be searched from. On return, this value
     *                 will be updated to next position. KErrNotFound if
     *                 the end of data is reached.
     * @param aResult Signed integer if found.
     * @return KErrNone if found. KErrNotFound if not found.
     *         KErrGeneral if aNextPos is negative or aNextPos is greater than
     *         the length of aData.
     */
    static TInt ExtractNum(
        const TDesC& aData,
        TInt& aNextPos,
        TInt& aResult );
    };

#endif  // CMPXVIEWUTILITYHELPER_H

// End of File
