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
* Description:  Observer class for extracting metadata
*
*/


#ifndef MMPXMETADATASCANOBSERVER_H
#define MMPXMETADATASCANOBSERVER_H

class CMPXMediaArray;

/**
 *  Metadata Scan observer
 *
 *  @lib mpxfilehandler.lib
 *  @since S60 v3.0
 */
class MMPXMetadataScanObserver
    {

public:

    /**
    * Handles a few files to be added to the collection
    * @param aMediaArray list of new medias to add
    */
    virtual void AddFilesToCollectionL( CMPXMediaArray& aMediaArray ) = 0;
    
    /**
    * Handles a few files to be updated to the collection
    * @param aMediaArray list of updated medias 
    */
    virtual void UpdatesFilesInCollectionL( CMPXMediaArray& aMediaArray ) = 0;
    };

#endif // MMPXMETADATASCANOBSERVER_H
