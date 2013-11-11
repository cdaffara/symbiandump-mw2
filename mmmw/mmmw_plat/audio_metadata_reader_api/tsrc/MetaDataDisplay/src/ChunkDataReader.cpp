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
* Description: Chunk data reader  implementationfor STIF Test Framework TestScripter.
*
*/
#include "ChunkDataReader.h"    


CChunkDataReader* CChunkDataReader::NewL()
    {
    CChunkDataReader* self = new CChunkDataReader();
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

void CChunkDataReader::ConstructL()
    {
    //User::LeaveIfError( iFsSession.Connect() );
    }

CChunkDataReader::CChunkDataReader()
: iEndOfFileEncountered( EFalse )
    {    
    }

CChunkDataReader::~CChunkDataReader()
    {
    iDataFile.Close();
    iFsSession.Close();    
    }

void CChunkDataReader::OpenFile( const TDesC& aFileName )
    {    
    User::LeaveIfError(iFsSession.Connect());
    
    // Open a file as text, writable, and shared with other clients
    TInt err = iDataFile.Open( iFsSession, aFileName, EFileRead );
    
    if ( err != KErrNone )
        {
        User::Leave(err);
        }
    
    //iDataFile.Seek( ESeekStart, 0 );
    }

TBool CChunkDataReader::HasChunkData()
    {
    return !iEndOfFileEncountered;
    }

TDesC8& CChunkDataReader::NextChunkData( TInt readSize )
    {    
    iDataFile.Read( iDataBuf, readSize );

    if ( iDataBuf.Length() == 0 ) 
        {
        iEndOfFileEncountered = ETrue;
        }
    
    return iDataBuf;
    }


    
    

