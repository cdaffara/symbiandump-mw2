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
* Description:  Collection engine observer
*
*/

 

#ifndef MMPXCOLLECTIONENGINEOBSERVER_H
#define MMPXCOLLECTIONENGINEOBSERVER_H

#include <badesca.h>
#include <mpxcollectionenginecommonobserver.h>

/**
*  Interface for receiving data asynchronously 
*/
NONSHARABLE_CLASS(MMPXCollectionEngineObserver) : public MMPXCollectionEngineCommonObserver
    {
public:
    /**
    *  Handles the collection entries being opened. Typically called
    *  when client has Open()'d a folder
    *  Or Handles the item being opened. Typically called
    *  when client has Open()'d an item. Client typically responds by
    *  'playing' the item via the playlist
    *
    *  @param aMedia media to hold collection entries
    *  @param aIndex focused entry
    *  @param aComplete ETrue no more entries. EFalse more entries expected
    *  @param aError error code   
    */
    virtual void HandleOpen(CMPXMedia* aMedia,
                            TInt aIndex,TBool aComplete,TInt aError) = 0;
    
    /**
    * Handles a generic Async Op completion ( nothing to return )
    * @param aError, error to complete
    */
    virtual void HandleOpComplete( TInt aError ) = 0;
    };    
    
#endif // MMPXCOLLECTIONENGINEOBSERVER_H
