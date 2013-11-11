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
* Description:    SIMPLE Engine publish callback observer
*
*/




#ifndef M_simplepublishobserver_H
#define M_simplepublishobserver_H

#include <e32std.h>

/**
 *  MSimplePublishObserver
 *
 *  SIMPLE Engine publish callback observer
 *
 *  @lib simpleengine
 *  @since S60 3.2
 */

class MSimplePublishObserver
    {

public:

    /**
     * Callback for publish request completion
     *
     * @since Series60 3.2
     * @param aOpid operation id
     * @param aStatus completion status
     */
    virtual void PublishReqCompleteL( TInt aOpid, TInt aStatus) = 0;

    /**
     * Callback for publish termination
     *
     * @since Series60 3.2
     * @param aOpid operation id
     */
    virtual void PublishTerminatedL( TInt aOpid ) = 0;

    };

#endif

// End of File
