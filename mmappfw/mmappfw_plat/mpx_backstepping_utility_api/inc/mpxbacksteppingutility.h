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
* Description:  MPX back stepping utility interface definition
*
*/

 
#ifndef MMPXBACKSTEPPINGUTILITY_H
#define MMPXBACKSTEPPINGUTILITY_H


// INCLUDES
#include <e32base.h>

const TInt KMPXBackSteppingNotConsumed = 0;
const TInt KMPXBackSteppingConsumed = 1;


// CLASS DECLARATION

/**
 *  Music Player common utility interface
 *
 *  @lib mpxbacksteppingutility.lib
 */
NONSHARABLE_CLASS( MMPXBackSteppingUtility )
    {
public: 

    /**
     * Retrieve a back stepping utility of this thread.  All clients in this thread
     * shares the same utility (singleton)
     * Call Close() when this object is not needed anymore.
     *
     * @since S60 3.2
     * @return Pointer to backstepping utility. Ownership not transferred.
     */
    IMPORT_C static MMPXBackSteppingUtility* UtilityL();

    /**
     * This method must be called when this object can be freed. This object 
     * will be destroyed when no one is using it.
     *
     * @since S60 3.2
     */
    virtual void Close() = 0;

    /**
     * Forwards activation event.
     *
     * @since S60 3.2
     * @param aState state of application like view ids
     * @param aEnter indicate if it is entry/exit activation
     * @return result code retutned by BS Service interface
     * @leave leaves from HandleResultL are propagated
     */
    virtual TInt ForwardActivationEventL(
        const TDesC8& aState,
        const TBool aEnter ) = 0;
    
    /**
     * Handles back command.
     *
     * @since S60 3.2
     * @param aState state of application like view ids
     * @return result code retutned by BS Service interface
     * @leave leaves from HandleResultL are propagated
     */
    virtual TInt HandleBackCommandL( const TDesC8& aState ) = 0;

    /**
     * Initializes Backstepping Service.
     *
     * @since S60 3.2
     * @param aUid client application UID
     * @leave KErrNotSupported or KErrArgument 
     *        (if arguments passed to BS Service are incorrect 
     *        or could not initialize BS)
     *        KErrNotFound (if there was no return value)
     */
    virtual void InitializeL( const TUid aUid ) = 0;
    };

#endif  // MMPXBACKSTEPPINGUTILITY_H

// End of File
