/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Notifies when Sim store is changed
*  Interface   : Internal, MVmbxSimStoreObserver
*
*/


#ifndef M_VMBXSIMSTOREOBSERVER_H
#define M_VMBXSIMSTOREOBSERVER_H

// INCLUDES

// CLASS DECLARATION

/**
 *  Notifier for sim store file entry changes
 *
 *  @lib vmbxengine.lib
 *
 */
NONSHARABLE_CLASS ( MVmbxSimStoreObserver )
    {
public:

    /**
    * Does the user wants when the sim store file entry has been changed.
    * 
    **/
    virtual void HandleSimStoreChanged() = 0;

    };

#endif // M_VMBXSIMSTOREOBSERVER_H
