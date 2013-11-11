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
* Description: ChunkDataAPITester implementation for STIF Test Framework TestScripter.
*
*/
#include <MetaDataUtility.h>
#include "MetaDataUtilityTestAppFileDetails.h"
#include "ChunkDataReader.h"  
#include "ChunkDataAPITester.h"


const TInt KTimeToWaitInMillis = 3000;

// requests
const TInt KProcessMetaData     = 1;
const TInt KProcessNextChunk    = 2;


CChunkDataAPITester* CChunkDataAPITester::NewL()
    {
    CChunkDataAPITester* self = new CChunkDataAPITester();
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

CChunkDataAPITester::CChunkDataAPITester()
: CActive( EPriorityStandard ), 
  //iChunkDataComplete( EFalse ), 
  //iChunkDataError( KErrNone ),
  iRequest( KProcessNextChunk ), 
  iRequestChunkSize( 0 ),
  iFinalRequest( EFalse ),
  iPhase(0)
    {
    CActiveScheduler::Add( this );
    }

CChunkDataAPITester::~CChunkDataAPITester()
    {
    if ( iMetaDataUtility )
        {
        delete iMetaDataUtility;
        iMetaDataUtility = NULL;
        }
    
    if ( iChunkReader )
        {
        delete iChunkReader;
        iChunkReader = NULL;
        }
    
    Cancel();
    //iRTimer.Close();
    }

void CChunkDataAPITester::ConstructL()
    {
    iMetaDataUtility = CMetaDataUtility::NewL();
    iChunkReader = CChunkDataReader::NewL();
    
    /*
    TInt error = iRTimer.CreateLocal();
    if ( error != KErrNone )
        {
        User::Leave( error );
        }
    */
    }

CMetaDataUtility* CChunkDataAPITester::GetMetaDataUtility()
    {
    return iMetaDataUtility;
    }

/*
TInt CChunkDataAPITester::GetChunkDataError()
    {
    return iChunkDataError;
    }

TBool CChunkDataAPITester::IsChunkDataComplete()
    {
    return iChunkDataComplete;
    }
*/    

void CChunkDataAPITester::HandleChunkDataComplete( 
        TInt aError )
    {
    TRAP_IGNORE( iProcessor->ProcessMetaDataL( iMetaDataUtility, 
                                               aError ) );
    }

void CChunkDataAPITester::HandleChunkDataProcessed(
        TInt aError)
    {
    if ( aError == KErrNone )
        {
        TRAP_IGNORE( GetNextChunkL() );
        }
    else 
        {
        // No metadata found
        if ( aError == KErrNotFound )
            {
            TRAP_IGNORE( iProcessor->ProcessMetaDataL( iMetaDataUtility, 
                                                       KErrNone ) );            
            }
        }
    }

void CChunkDataAPITester::HandleChunkDataReadyToBeParsed()
    {
    iMetaDataUtility->ParseChunkData();
    }

void CChunkDataAPITester::GetNextChunkL()
    {
    iRequest = KProcessNextChunk;
    
/*****    
    switch ( phase )
        {
        case 0: iRequestChunkSize = 1; iFinalRequest = EFalse; break;
        case 1: iRequestChunkSize = 2; iFinalRequest = EFalse; break;
        case 2: iRequestChunkSize = 2; iFinalRequest = EFalse; break;
        case 3: iRequestChunkSize = 2000; iFinalRequest = EFalse; break;
        case 4: iRequestChunkSize = 12995; iFinalRequest = EFalse; break;
        default:
            User::Leave( KErrNotSupported );        
        }
        
   switch ( phase )
        {
        case 0: iRequestChunkSize = 1; iFinalRequest = EFalse; break;
        case 1: iRequestChunkSize = 2; iFinalRequest = EFalse; break;
        case 2: iRequestChunkSize = 2; iFinalRequest = EFalse; break;
        case 3: iRequestChunkSize = 2; iFinalRequest = EFalse; break;
        case 4: iRequestChunkSize = 2000; iFinalRequest = EFalse; break;
        case 5: iRequestChunkSize = 2000; iFinalRequest = ETrue; break;
        default:
            User::Leave( KErrNotSupported );        
        }                
        
*****/
        
    switch ( iPhase )
        {
    	case 0: iRequestChunkSize = 0; iFinalRequest = EFalse; break;
        case 1: iRequestChunkSize = 0; iFinalRequest = EFalse; break;
        case 2: iRequestChunkSize = 1; iFinalRequest = EFalse; break;
        case 3: iRequestChunkSize = 2; iFinalRequest = EFalse; break;
        case 4: iRequestChunkSize = 4; iFinalRequest = EFalse; break;
        case 5: iRequestChunkSize = 8; iFinalRequest = EFalse; break;
        case 6: iRequestChunkSize = 16; iFinalRequest = EFalse; break;
        case 7: iRequestChunkSize = 32; iFinalRequest = EFalse; break;
        case 8: iRequestChunkSize = 64; iFinalRequest = EFalse; break;
        case 9: iRequestChunkSize = 128; iFinalRequest = EFalse; break;
        
        case 10: iRequestChunkSize = 256; iFinalRequest = EFalse; break;
        case 11: iRequestChunkSize = 500; iFinalRequest = EFalse; break;
        case 12: iRequestChunkSize = 1024; iFinalRequest = EFalse; break;
        case 13: iRequestChunkSize = 2048; iFinalRequest = EFalse; break;
        case 14: iRequestChunkSize = 5000; iFinalRequest = EFalse; break;
        case 15: iRequestChunkSize = 10000; iFinalRequest = EFalse; break;
        case 16: iRequestChunkSize = 20000; iFinalRequest = EFalse; break;
        case 17: iRequestChunkSize = 30000; iFinalRequest = EFalse; break;
        case 18: iRequestChunkSize = 40000; iFinalRequest = EFalse; break;
        case 19: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        
        case 20: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 21: iRequestChunkSize = 40000; iFinalRequest = EFalse; break;
        case 22: iRequestChunkSize = 30000; iFinalRequest = EFalse; break;
        case 23: iRequestChunkSize = 20000; iFinalRequest = EFalse; break;
        case 24: iRequestChunkSize = 10000; iFinalRequest = EFalse; break;
        case 25: iRequestChunkSize = 5000; iFinalRequest = EFalse; break;
        case 26: iRequestChunkSize = 2048; iFinalRequest = EFalse; break;
        case 27: iRequestChunkSize = 1024; iFinalRequest = EFalse; break;
        case 28: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 29: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        
        case 30: iRequestChunkSize = 1000000; iFinalRequest = EFalse; break;
        case 31: iRequestChunkSize = 2000000; iFinalRequest = EFalse; break;
        case 32: iRequestChunkSize = 3000000; iFinalRequest = EFalse; break;
        case 33: iRequestChunkSize = 4000000; iFinalRequest = EFalse; break;
        case 34: iRequestChunkSize = 5000000; iFinalRequest = EFalse; break;
        case 35: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 36: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 37: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 38: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 39: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        
        case 40: iRequestChunkSize = 10000000; iFinalRequest = EFalse; break;
        case 41: iRequestChunkSize = 20000000; iFinalRequest = EFalse; break;
        case 42: iRequestChunkSize = 30000000; iFinalRequest = EFalse; break;
        case 43: iRequestChunkSize = 40000000; iFinalRequest = EFalse; break;
        case 44: iRequestChunkSize = 50000000; iFinalRequest = EFalse; break;
        case 45: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 46: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 47: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 48: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 49: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        
        case 50: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 51: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 52: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 53: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 54: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 55: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 56: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 57: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 58: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 59: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        
        case 60: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 61: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 62: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 63: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 64: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 65: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 66: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 67: iRequestChunkSize = 50000; iFinalRequest = EFalse; break;
        case 68: iRequestChunkSize = 28827648; iFinalRequest = EFalse; break;
        case 69: iRequestChunkSize = 15450739; iFinalRequest = ETrue; break;
        
        default:
        User::Leave( KErrNotSupported );        
        }        
         
    ++iPhase;
    
    iStatus = KRequestPending;
    SetActive();
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );
    }

TBool CChunkDataAPITester::TestIsChunkDataModeAPI()
    {
    return iMetaDataUtility->IsChunkDataMode();
    }

TInt CChunkDataAPITester::TestInitChunkDataAPI( const TDesC8& aMimeType )
    {
    return iMetaDataUtility->InitChunkData( aMimeType, *this );
    }

// returns an error if the ChunkReader fails
TInt CChunkDataAPITester::TestProcessChunkDataAPI( const TDesC& aFileName )
    {
    TInt error = KErrNone;
    
    iChunkReader->OpenFile( aFileName );    
    GetNextChunkL();
  
    return error;
    }

void CChunkDataAPITester::TestCloseChunkDataAPI()
    {
    iMetaDataUtility->CloseChunkData();
    }

void CChunkDataAPITester::WaitToProcessMetaData( 
        MMetaDataProcessor* aProcessor)
    {
    iProcessor = aProcessor;
    }
    
void CChunkDataAPITester::DoCancel()
    {
    //iRTimer.Cancel();
    }

void CChunkDataAPITester::RunL()
    {    
    // Process and display the metadata whether or not we received the 
    // complete event, i.e. whether or not we have metadata
    if ( KErrNone == iStatus.Int() )        
        {
        switch ( iRequest )
            {
            case KProcessMetaData:
                {

                }
                break;
            case KProcessNextChunk:
                {
                TDesC8& chunkData = iChunkReader->NextChunkData( iRequestChunkSize );
                TInt error = iMetaDataUtility->ProcessChunkData( chunkData, 
                                                                 iFinalRequest );
                if ( error != KErrNone )
                    {
                    User::Leave( error );
                    }
                
                
                // Delete testing  FLO
                //delete iMetaDataUtility;

                }
                break;
            default:
                {
                User::Leave( KErrNotSupported );
                }
            }
        }
    }
    
