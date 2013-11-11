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
* Description: ChunkDataAPITester header for STIF Test Framework TestScripter.
*
*/
#ifndef CCHUNKDATAAPITESTER_H_
#define CCHUNKDATAAPITESTER_H_

#include <e32base.h>
#include <MDUChunkDataObserver.h>


class CMetaDataUtility;
class CChunkDataReader;


class MMetaDataProcessor
    {
    public:
        virtual void ProcessMetaDataL( CMetaDataUtility *metaDataUtility, 
                        TInt error ) = 0;
    };

class CChunkDataAPITester: public CActive, public MMDUChunkDataObserver
    {
    public:
        static CChunkDataAPITester* NewL();
        ~CChunkDataAPITester();
                      
        CMetaDataUtility* GetMetaDataUtility();

        /*
        TInt GetChunkDataError();
        TBool IsChunkDataComplete();
        */
        
        TBool TestIsChunkDataModeAPI();        
        TInt TestInitChunkDataAPI( const TDesC8& aMimeType );
        TInt TestProcessChunkDataAPI( const TDesC& aFileName );
        void TestCloseChunkDataAPI(); 

        // This uses an active object and timer to wait 3 seconds after
        // the last process chunk call before processing the metadata.
        void WaitToProcessMetaData( MMetaDataProcessor* aProcessor );
        void DoCancel();
        void RunL();
       
    public: 
        // callback from MMetaDataUtilityObserver
        void HandleChunkDataComplete( TInt aError );  
        void HandleChunkDataProcessed( TInt aError );
        void HandleChunkDataReadyToBeParsed();
        
        void GetNextChunkL();
       
    private:
        CChunkDataAPITester();
        void ConstructL();
    private:
        CMetaDataUtility*                   iMetaDataUtility;
        CChunkDataReader*                   iChunkReader;
        MMetaDataProcessor*                 iProcessor;
        
        TInt                                iRequest;
        TInt                                iRequestChunkSize;
        TBool                               iFinalRequest;
        TInt                                iPhase;              
    };


#endif /*CCHUNKDATAAPITESTER_H_*/
