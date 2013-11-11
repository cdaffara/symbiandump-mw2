/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*
*/


#ifndef MMMTPENUMERATIONCALLBACK_H
#define MMMTPENUMERATIONCALLBACK_H

#include <e32std.h>

/**
* Defines the enumeration complete notification interface.
*/
class MMTPEnumerationCallback
    {
public:

    /**
    * Notifies that enumeration of the specified storage is completed.
    * @param aStorageId The MTP StorageID of the enumerated storage.
    * @param aError The enumeration completion error status.
    */
    virtual void NotifyEnumerationCompleteL( TUint32 aStorageId, TInt aError ) = 0;
    
    };

#endif // MMMTPENUMERATIONCALLBACK_H
