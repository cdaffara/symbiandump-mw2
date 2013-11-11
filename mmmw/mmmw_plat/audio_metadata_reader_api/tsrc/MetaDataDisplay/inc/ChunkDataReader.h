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
* Description: ChunkDataReader header for STIF Test Framework TestScripter.
*
*/

#ifndef CHUNKDATAREADER_H_
#define CHUNKDATAREADER_H_

#include <e32base.h>
#include <f32file.h>

#define MAX_BUF_SIZE        50000
#define DEFAULT_READ_SIZE   1024

class CChunkDataReader
    {
    public:        
        static CChunkDataReader* NewL();
        ~CChunkDataReader(); 
        
        void OpenFile( const TDesC& aFileName );
        TBool HasChunkData();
        TDesC8& NextChunkData( TInt readSize = DEFAULT_READ_SIZE );        
    private:        
        void ConstructL();
        CChunkDataReader(); 
    private:        
        TBool                   iEndOfFileEncountered;
        RFile                   iDataFile;
        RFs                     iFsSession;
        TBuf8<MAX_BUF_SIZE>     iDataBuf;
    };

#endif /*CHUNKDATAREADER_H_*/
