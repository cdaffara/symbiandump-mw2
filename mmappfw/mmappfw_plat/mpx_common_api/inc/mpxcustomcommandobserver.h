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
* Description:  MPX custom command observer interface definition
*
*/



#ifndef C_MMPXCUSTOMCOMMANDOBSERVER_H
#define C_MMPXCUSTOMCOMMANDOBSERVER_H


// INCLUDES
#include <e32base.h>


// FORWARD DECLARATIONS


// CLASS DECLARATION

/**
 *  MPX custom command observer.
 *
 *  @lib mpxcommon.lib
 */
NONSHARABLE_CLASS( MMPXCustomCommandObserver )
    {
public:

    /**
     * Handle custom commands.
     *
     * @since S60 3.2.3
     * @param aUid Command category Uid. Different command set will have 
     *             different Uid to avoid clash.
     * @param aCommand Command to be handled.
     * @param aData data for the specific command
     */
    virtual void HandleCustomCommand( 
        const TUid& aUid,
        const TInt aCommand, 
        const TDesC8& aData = KNullDesC8 ) = 0;
    };

#endif  // C_MMPXCUSTOMCOMMANDOBSERVER_H

// End of File
