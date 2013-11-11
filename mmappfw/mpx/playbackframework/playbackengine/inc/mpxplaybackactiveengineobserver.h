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
* Description:  Active playback engine observer.
*
*/



#ifndef MMPXPLAYBACKACTIVEENGINEOBSERVER_H
#define MMPXPLAYBACKACTIVEENGINEOBSERVER_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CMPXPlaybackEngine;

/**
*  Interface for handling active engine
*/
NONSHARABLE_CLASS(MMPXPlaybackActiveEngineObserver)
    {
public:
    /**
    *   Handle active engine
    *
    *   @param aEngine pointer to active engine
    *   @param aActive ETure activated, EFalse deactivated
    */
    virtual void HandleActiveEngineL(const CMPXPlaybackEngine* aEngine, 
                                     TBool aActive)=0;
    };
          
#endif // MMPXPLAYBACKACTIVEENGINEOBSERVER_H
            
// End of File
