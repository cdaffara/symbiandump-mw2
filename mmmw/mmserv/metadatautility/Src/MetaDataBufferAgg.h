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
* Description:  Active object for MetadataUtility
*
*/


#ifndef METADATABUFFERAGG_H
#define METADATABUFFERAGG_H

// INCLUDES
#include <e32base.h>

class CMetaDataUtilityBody;
class MMDUChunkDataObserver;


NONSHARABLE_CLASS(CMetaDataBufferAgg): public CActive
{


public:
    /**
     * Two-phased constructor.
     * @since 9.1
     * @param aMimeType The MIME type
     * @param aObserver should not be NULL
     * @return A pointer to a new instance of this class
     */    
    static CMetaDataBufferAgg* NewL( CMetaDataUtilityBody* aBody,
                                     TDesC8& aMimeType, 
                                     MMDUChunkDataObserver& aObserver);
    
    /**
     * Destructor.
     */
    virtual ~CMetaDataBufferAgg();
    
    /** 
    * Process the chunk data.
    * @since 5.1
    *      Max size of buffer passed in is N bytes for performance optimization
    *      Buffer overflow (out of memory) results in callback with error.
    *      invalid chunk data results in callback with error
    *      parsing APIs are invoked before callback is received results in 0 count or empty container
    *      parsing APIs are invoked after callback is received results in positive count and full container
    * @param aChunk the data chunk to be processed
    * @return error value
    *       KErrNone
    *       KErrNotReady
    */
    TInt RequestProcessChunkData( TDesC8& aChunk, TBool aFinalChunk );    
    
    /** 
    * Parse the chunk data.
    * @since 5.1
    *      parsing APIs are invoked before callback is received results in 0 count or empty container
    *      parsing APIs are invoked after callback is received results in positive count and full container
    * @return error value
    *       KErrNone
    *       KErrNotReady
    */
    TInt RequestParseChunkData();    
    
    void ReadyToGo();
    
    /**
     * Do Cancel
     * @since 9.1
     */    
    void DoCancel();
    
    /**
     * Run for Active Object
     * @since 9.1
     */        
	void RunL();

private:
    
private:
    enum TAggregateState 
        { 
        EFindIdentifier,
        ECalculateSize,
        ECollectData,
        EParseData,
        EParseComplete
        };

 
    void ConstructL(const TDesC8& aMimeType );
    CMetaDataBufferAgg( CMetaDataUtilityBody* aBody, 
                        MMDUChunkDataObserver& aObserver );
    
    void FindMetaDataIdentifierL( TDesC8* aBuf );
    void CalculateMetaDataSizeL( TDesC8* aBuf );
    void ComputeSize( TDesC8* aBuf, TInt aOffset );
    void CollectMetaDataL( TDesC8* aBuf );
    void ProcessPreviousChunkL( TDesC8* aBuf );
    void DoCallback( TInt aError );
    void CopyBufferL(RBuf8* aDes, TUint8* aPtr, TInt aSize );
    void AppendBufferL(RBuf8* aDes, TUint8* aPtr, TInt aSize );

    
private: // data    
    CMetaDataUtilityBody* iBody;
    MMDUChunkDataObserver& iObserver;
    TAggregateState iState;
    RBuf8 iMimeType;
    TDesC8* iUserChunk;
    RBuf8 iPrevBuf;         // to save previous data
    RBuf8 iMetaDataBuf;     
    TBool iFinalChunk;
    TInt iMetaDataSize;

};

#endif	// METADATABUFFERAGG_H
