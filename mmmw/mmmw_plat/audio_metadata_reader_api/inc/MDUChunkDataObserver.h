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
* Description:  Observer class for Metadata Utility
*
*/


#ifndef MMDUCHUNKDATAOBSERVER_H
#define MMDUCHUNKDATAOBSERVER_H

/**
 *  Observer to MetaDataUtility for processing data asynchronously
 *
 *  @lib MetaDataUtility.lib
 */
NONSHARABLE_CLASS(MMDUChunkDataObserver)
    {
public:

    /** 
     *  Notify that processing of the previous received chunk has completed.
     *  User can call CMetaDataUtility::ProcessChunkData() to send a new chunk.
     *  No Additional chunks should be sent, when there is an error.
     *  This will be called when Final Chunk is sent and no metadata is found.
     *  @param aError a return error code
     *      KErrNone
     *      KErrNoMemory
     *      KErrCorrupt
     *      KErrNotFound
     *  @return void 
     */ 
    virtual void HandleChunkDataProcessed(TInt aError) = 0;
    

    /**
    *  Notify that collecting of metadata has completed.
    *  No Additional chunks should be sent through ProcessChunkData(), 
    *  All metadata have been collected, and ready to be parsed.
    *  User need to call CMetaDataUtility::ParseChunkData() to parse the data
    *  @return void
    */
    virtual void HandleChunkDataReadyToBeParsed() = 0;


    /**
    *  Notify that parsing of metadata has completed.
    *  No Additional chunks should be sent through ProcessChunkData(), 
    *  all metadata processing is complete.
    *  This will only be called if the user has called ParseChunkData().
    *  User can retrive metadata by calling CMetaDataUtility::MetaDataCount(), 
    *  CMetaDataUtility::MetaDataFieldsL().
    *  @param aError a return error code
    *      KErrNone
    *      KErrNoMemory
    *      KErrCorrupt
    *  @return void
    */
    virtual void HandleChunkDataComplete(TInt aError) = 0;


    };


#endif // MMDUCHUNKDATAOBSERVER_H
