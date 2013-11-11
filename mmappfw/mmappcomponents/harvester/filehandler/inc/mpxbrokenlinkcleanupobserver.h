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
* Description:  Broken link cleanup observer
*
*/


#ifndef M_MMPXBROKENLINKOBSERVER_H
#define M_MMPXBROKENLINKOBSERVER_H

class CMPXHarvesterDB;

/**
 *  Broken link observer
 *
 *  @lib mpxfilehandler.lib
 *  @since S60 v3.0
 */
class MMPXBrokenLinkObserver
    {

public:

    /**
    * Virtual function to call back and delete n elements from the aFileArray
    * @param aFileArray file array to find the list of files
    * @param aColdIds list of collection ids of each item
    * @param aDbs list of dbs assocaited with each item
    * @param aCount number of items to delete from array[0] to array[aCount-1]
    */
    virtual void HandleBrokenLinkL( MDesCArray& aFileArray,
                                    RArray<TInt>& aColIds,
                                    RPointerArray<CMPXHarvesterDB>& aDbs,
                                    TInt aCount ) = 0;
    
    };


#endif // M_MMPXBROKENLINKOBSERVER_H
