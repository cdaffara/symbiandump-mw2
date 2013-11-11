/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MPX client list observer interface definition
*
*/



#ifndef MMPXCLIENTLISTOBSERVER_H
#define MMPXCLIENTLISTOBSERVER_H


// INCLUDES
#include <e32base.h>


// FORWARD DECLARATIONS


// CLASS DECLARATION

/**
 *  MPX client list observer.
 *
 *  @lib mpxcommon.lib
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( MMPXClientlistObserver )
    {
public:
    enum TChangeType
        {
        EAdd,
        ERemove
        };
    /**
     * Handle client change
     *
     * @since 3.1
     * @param aPid Process id of the client
     * @param aChangeType EAdd - new client added, ERemove - a client is removed.
     */
    virtual void HandleClientChange(const TProcessId& aPid, 
                                    TChangeType aChangeType) = 0;
    };

#endif  // MMPXCLIENTLISTOBSERVER_H

// End of File
