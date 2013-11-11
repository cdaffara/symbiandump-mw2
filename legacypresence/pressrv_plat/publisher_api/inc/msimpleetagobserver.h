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
* Description:    SIMPLE Engine publish etag value callback observer
*
*/




#ifndef M_simpleetagobserver_H
#define M_simpleetagobserver_H

#include <e32std.h>

/**
 *  MSimpleETagObserver
 *
 *  SIMPLE Engine publish ETag value callback observer
 *
 *  @lib simpleengine
 *  @since S60 3.2
 */

class MSimpleETagObserver
    {

public:

    /**
     * Callback for publish request completion
     *
     * @since Series60 3.2
     * @param aVal new ETag value. May be zero length when publication ends or fails.
     */
    virtual void NewETagL( const TDesC8& aVal ) = 0;

    };

#endif

// End of File
