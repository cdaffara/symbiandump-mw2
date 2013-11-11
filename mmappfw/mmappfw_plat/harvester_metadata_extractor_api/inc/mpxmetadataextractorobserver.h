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
* Description:  Observer class for metadata extractor
*
*/


#ifndef MMPXMETADATAEXTRACTOROBSERVER_H
#define MMPXMETADATAEXTRACTOROBSERVER_H

class CMPXMediaArray;

/**
 *  Metadata Extractor observer
 *
 *  @lib mpxfilehandler.lib
 *  @since S60 v9.2
 */
class MMPXMetadataExtractorObserver
    {

public:

    /**
    * Callback when the CreateMediaL() is completed.
    * @param aMedia media with metadata populated. Ownership transferred.
    * @param aError error
    */
    virtual void HandleCreateMediaComplete( CMPXMedia* aMedia, TInt aError ) = 0;
    };

#endif // MMPXMETADATAEXTRACTOROBSERVER_H
