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
* Description:  Image saver observer interface
*
*/


#ifndef M_MPHSRVIMAGESAVEROBSERVER_H
#define M_MPHSRVIMAGESAVEROBSERVER_H

#include    <e32def.h>

/**
 *  Observer interface for .
 *
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( MPhSrvImageSaverObserver )
    {
public:
    /**
     * Callback that is called when image is saved.
     */
    virtual void ImageSaved( TInt aResult ) = 0;       
    };

#endif // M_MPHSRVIMAGESAVEROBSERVER_H

// End of File
