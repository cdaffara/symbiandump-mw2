/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  STIF for mpx_albumart_utility_api
*
*/



// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "mpxalbumartutilitytest.h"
#include <mpxuser.h>

#include <mpxmediaaudiodefs.h>
#include <mpxmediamusicdefs.h>
#include <mpxmediadrmdefs.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Cmpxalbumartutilitytest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Cmpxalbumartutilitytest::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Cmpxalbumartutilitytest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Cmpxalbumartutilitytest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Example", Cmpxalbumartutilitytest::ExampleL ),
        ENTRY( "CMPXAlbumArtUtilityNewL", Cmpxalbumartutilitytest::CMPXAlbumArtUtilityNewL ),
        ENTRY( "CMPXAlbumArtUtilityDestructor", Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDestructor ),
        ENTRY( "CMPXAlbumArtUtilityExtractAlbumArtL", Cmpxalbumartutilitytest::CMPXAlbumArtUtilityExtractAlbumArtL ),
        ENTRY( "CMPXAlbumArtUtilityDecode", Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDecode ),
        ENTRY( "CMPXAlbumArtUtilityDecodeASourceJPG", Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDecodeASourceJPG ),
        ENTRY( "CMPXAlbumArtUtilityEncode", Cmpxalbumartutilitytest::CMPXAlbumArtUtilityEncode ),
        ENTRY( "CMPXAlbumArtUtilityScale", Cmpxalbumartutilitytest::CMPXAlbumArtUtilityScale ),
        ENTRY( "CMPXAlbumArtUtilityDecodeL", Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDecodeL ),
        ENTRY( "CMPXAlbumArtUtilityDecodeLASourceJPG", Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDecodeLASourceJPG ),
        ENTRY( "CMPXAlbumArtUtilityEncodeL", Cmpxalbumartutilitytest::CMPXAlbumArtUtilityEncodeL ),
        ENTRY( "CMPXAlbumArtUtilityScaleL", Cmpxalbumartutilitytest::CMPXAlbumArtUtilityScaleL ),
        ENTRY( "CMPXAlbumArtUtilityExtractL", Cmpxalbumartutilitytest::CMPXAlbumArtUtilityExtractL ),
        ENTRY( "CMPXAlbumArtUtilityBitmapL", Cmpxalbumartutilitytest::CMPXAlbumArtUtilityBitmapL ),
        ENTRY( "CMPXAlbumArtUtilityBitmapDataL", Cmpxalbumartutilitytest::CMPXAlbumArtUtilityBitmapDataL ),
        ENTRY( "CMPXAlbumArtUtilityCancelRequest", Cmpxalbumartutilitytest::CMPXAlbumArtUtilityCancelRequest ),
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// Cmpxalbumartutilitytest::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxalbumartutilitytest::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( Kmpxalbumartutilitytest, "mpxalbumartutilitytest" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, Kmpxalbumartutilitytest, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, Kmpxalbumartutilitytest, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Cmpxalbumartutilitytest::ExtractAlbumArtStarted
// Extraction of album art started
// -----------------------------------------------------------------------------
void Cmpxalbumartutilitytest::ExtractAlbumArtStarted()
{
//	FTRACE(FPrint(_L("Cmpxalbumartutilitytest::ExtractAlbumArtStarted")));
	iLog->Log(_L("Extraction of album art started"));
}

// -----------------------------------------------------------------------------
// Cmpxalbumartutilitytest::ExtractAlbumArtCompleted
// The extraction of album art has completed
// -----------------------------------------------------------------------------
void Cmpxalbumartutilitytest::ExtractAlbumArtCompleted(CFbsBitmap* aBitmap, TInt aErr)
{
//	FTRACE(FPrint(_L("Cmpxalbumartutilitytest::ExtractAlbumArtStarted")));
	iLog->Log(_L("The extraction of album art has completed"));
	iAlbumArtConverting = EFalse;
		
    delete iAlbumArt;
    iAlbumArt = NULL;
        
    if ( aErr == KErrNone && aBitmap )    
        {    
        iAlbumArt = aBitmap;
        if( iAlbumArt )
            {
            delete iBackgroundBitmap;
            iBackgroundBitmap = NULL;  
            TRAP_IGNORE( iBackgroundBitmap = new (ELeave) CFbsBitmap() );
            if ( iBackgroundBitmap )
                {
                TSize bitmapSize = iBackgroundBitmap->SizeInPixels();             	
                bitmapSize = iAlbumArt->SizeInPixels();
                iBackgroundBitmap->Duplicate(iAlbumArt->Handle());
                }
            }        
        }
    else
        {
        iArtExist = EFalse;  
        
        // Album art was NULL, meaning none was retrieved
        delete iBackgroundBitmap;
        iBackgroundBitmap = NULL;            
        TRAP_IGNORE( iBackgroundBitmap = new (ELeave) CFbsBitmap() );
        if ( iBackgroundBitmap )
            {
            iBackgroundBitmap->Duplicate((iDefaultAAImage->Bitmap())->Handle());            
            }
        }
}

// ---------------------------------------------------------------------------
// Cmpxalbumartutilitytest::CMPXAlbumArtUtilityNewL
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxalbumartutilitytest::CMPXAlbumArtUtilityNewL( CStifItemParser& /*aItem*/ )
    {
    TInt err=KErrNone;
    iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityNewL"));
    TRAP( err , iAlbumArtUtility = CMPXAlbumArtUtility::NewL());
   
   	if ( err == KErrNone )
   		{
   		iLog->Log(_L("Cmpxalbumartutilitytest::NewL returned no err"));
   		}
   	else 
   		{
   		iLog->Log(_L("Cmpxalbumartutilitytest::NewL returned: %d"), err);
   		}
    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDestructor
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDestructor( CStifItemParser& /*aItem*/ )
    {
    TInt err = KErrNone;
    delete iAlbumArtUtility;
    iAlbumArtUtility = NULL;
	iLog->Log(_L("Cmpxalbumartutilitytest::Destructor returned CMPXAlbumArtUtility::~ end err=%d"), err);
	return err;
    }

// ---------------------------------------------------------------------------
// Cmpxalbumartutilitytest::CMPXAlbumArtUtilityExtractAlbumArtL
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxalbumartutilitytest::CMPXAlbumArtUtilityExtractAlbumArtL( CStifItemParser& aItem )
    {
    TInt err=KErrNone;
    TPtrC string;
   	if ( KErrNone == aItem.GetNextString(string) )
	   	{
	    TBuf<120> playlistBuf;
	    playlistBuf.Append(Kmpxalbumartutilitytest_testPath);
	    playlistBuf.Append(string);
	    RArray<TInt> suppIds;
	    CleanupClosePushL(suppIds);
	    suppIds.AppendL(KMPXMediaIdMusic);
	    suppIds.AppendL(KMPXMediaIdGeneral);
	    suppIds.AppendL(KMPXMediaIdAudio);
	    suppIds.AppendL(KMPXMediaIdDrm);
	    CMPXMedia* media=CMPXMedia::NewL(suppIds.Array());
	    CleanupStack::PopAndDestroy(&suppIds);        
	    CleanupStack::PushL(media);
	    media->SetTObjectValueL<TMPXGeneralType>(TMPXAttribute(KMPXMediaIdGeneral,EMPXMediaGeneralType), EMPXGroup); // default to group type
	    media->SetTextValueL(KMPXMediaGeneralUri,playlistBuf);
	    media->SetTextValueL(KMPXMediaMusicAlbumArtFileName,playlistBuf);
	    iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityExtractAlbumArtL"));
	  //  iAlbumArtUtility->ExtractAlbumArtL(*media,*this,TSize(100,100));
	    CleanupStack::PopAndDestroy(media);
	    
	   	iLog->Log(_L("Cmpxalbumartutilitytest::ExtractAlbumArtL returned: %d"), err);
	    }
   	return err;
    }


// ---------------------------------------------------------------------------
// Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDecode
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDecode( CStifItemParser& aItem )
    {
    TInt err=KErrNone;
	TPtrC string;
    if( aItem.GetNextString( string ) == KErrNone )
       {
  	    TBuf<120> KTestStr;
  	    KTestStr.Append(Kmpxalbumartutilitytest_testPath);
  	    KTestStr.Append(string);
	    TRequestStatus iStatus(0);
	    TBufC<50> path(KTestStr);
	    CFbsBitmap* iDestBMP( NULL );
	    TRAP( err , iDestBMP = new(ELeave) CFbsBitmap());
	    iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDecode"));
	    TRAP( err , iAlbumArtUtility = CMPXAlbumArtUtility::NewL());
	   
	    iAlbumArtUtility->Decode(iStatus,path,*iDestBMP,TSize(1,1));
	    User::WaitForRequest(iStatus);
	    
	    delete iDestBMP;
	    iDestBMP = NULL;
	    delete iAlbumArtUtility;
	    iAlbumArtUtility = NULL;
	    if ( err == KErrNone )
       		{
       		iLog->Log(_L("Cmpxalbumartutilitytest::Decode returned no err"));
       		}
       	else 
       		{
       		iLog->Log(_L("Cmpxalbumartutilitytest::Decode returned: %d"), err);
       		}
       }
    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDecodeASourceJPG
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDecodeASourceJPG( CStifItemParser& aItem )
    {
    TInt err=KErrNone;   
    TPtrC string;
    if( aItem.GetNextString( string ) == KErrNone )
       {
  	    TBuf<50> KTestStr;
  	    KTestStr.Append(Kmpxalbumartutilitytest_testPath);
  	    KTestStr.Append(string);
	    TRequestStatus iStatus(0);
	    TBufC<50> descriptor16(KTestStr);
	    TPtrC8 sourcepath = MPXUser::Ptr(descriptor16);
	
	    CFbsBitmap* iDestBMP( NULL );
	    TRAP(err,iDestBMP = new(ELeave) CFbsBitmap());
	    iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDecodeASourceJPG"));
	    iAlbumArtUtility->Decode(iStatus,sourcepath,*iDestBMP,TSize(1,1));
	    User::WaitForRequest(iStatus);
	    if ( err == KErrNone )
       		{
       		iLog->Log(_L("Cmpxalbumartutilitytest::Decode returned no err"));
       		}
       	else 
       		{
       		iLog->Log(_L("Cmpxalbumartutilitytest::Decode returned: %d"), err);
       		}
       }
    return err;
    }


// ---------------------------------------------------------------------------
// Cmpxalbumartutilitytest::CMPXAlbumArtUtilityEncode
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxalbumartutilitytest::CMPXAlbumArtUtilityEncode( CStifItemParser& aItem )
    {
    TInt err=KErrNone;
    TPtrC string;
    if( aItem.GetNextString( string ) == KErrNone )
       {
  	    TBuf<50> KTestStr;
  	    KTestStr.Append(Kmpxalbumartutilitytest_testPath);
  	    KTestStr.Append(string);
	    /*TRequestStatus iStatus(0);
	    CFbsBitmap* iSourceBMP;
	    TRAP(err , iSourceBMP = new(ELeave) CFbsBitmap());	  
	    iSourceBMP->Load(KTestStr);
	   // HBufC8* iDestJPG(NULL);
	    HBufC8* destJPG = HBufC8::NewLC( 100 );
	    destJPG->Des().Append( KDestJPG );
	    iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityEncode"));
	  //  iAlbumArtUtility->Encode(iStatus,*iSourceBMP,destJPG);	 
	    
	    User::WaitForRequest(iStatus);
	    delete iSourceBMP;*/
	   	iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityEncode returned: %d"), err);
       }
   	return err;
    
    }

// ---------------------------------------------------------------------------
// Cmpxalbumartutilitytest::CMPXAlbumArtUtilityScale
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxalbumartutilitytest::CMPXAlbumArtUtilityScale( CStifItemParser& /*aItem*/ )
    {
    TInt err=KErrNone;
    TRequestStatus iStatus(0); 
    CFbsBitmap* iSourceBMP( NULL );
    TRAP(err,iSourceBMP=new(ELeave) CFbsBitmap());
    CFbsBitmap* iDestBMP( NULL );
    TRAP(err,iDestBMP=new(ELeave) CFbsBitmap());
    
    iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityScale"));
    TRAP(err,iAlbumArtUtility=CMPXAlbumArtUtility::NewL());
    iAlbumArtUtility->Scale(iStatus,*iSourceBMP,*iDestBMP,TSize(1,1));
    
    User::WaitForRequest(iStatus);
    delete iAlbumArtUtility;
    iAlbumArtUtility=NULL;
    
   	iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityScale returned: %d"), err);
    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDecodeL
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//8
TInt Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDecodeL( CStifItemParser& aItem )
    {
    TInt err=KErrNone;
    TPtrC string;
	if ( KErrNone == aItem.GetNextString(string) )
		{
		TBuf<120> KUri;
		KUri.Append(Kmpxalbumartutilitytest_testPath);
		KUri.Append(string);
	    iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDecodeL"));
	    TRAP( err , iAlbumArtUtility->DecodeL(KUri,TSize(1,1)));
	   	iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDecodeL returned: %d"), err);
		}
    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDecodeLASourceJPG
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDecodeLASourceJPG( CStifItemParser& aItem )
    {
    TInt err=KErrNone;
    TPtrC string;
	if ( KErrNone == aItem.GetNextString(string) )
		{
		TBuf8<120> KPath;
		KPath.Append(Kmpxalbumartutilitytest_testPath);
		KPath.Append(string);
	    iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDecodeLASourceJPG"));
	    TRAP(err , iAlbumArtUtility->DecodeL(KPath,TSize(1,1)));
	   	iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityDecodeLASourceJPG returned: %d"), err);
		}
    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxalbumartutilitytest::CMPXAlbumArtUtilityEncodeL
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxalbumartutilitytest::CMPXAlbumArtUtilityEncodeL( CStifItemParser& aItem )
    {
    TInt err=KErrNone;
    TPtrC string;
    if( aItem.GetNextString( string ) == KErrNone )
       {
  	    TBuf<50> KTestStr;
  	    KTestStr.Append(Kmpxalbumartutilitytest_testPath);
  	    KTestStr.Append(string);
	    CFbsBitmap* iSourceBMP( NULL );
	    TRAP(err , iSourceBMP = new(ELeave) CFbsBitmap());	  
	    iSourceBMP->Load(KTestStr);   
	    iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityEncodeL"));
	  //  TRAP(err,iAlbumArtUtility->EncodeL(*iSourceBMP));
	   	iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityEncodeL returned: %d"), err);
	   	delete iSourceBMP;
       }        	
   	return err;
    }

// ---------------------------------------------------------------------------
// Cmpxalbumartutilitytest::CMPXAlbumArtUtilityScaleL
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxalbumartutilitytest::CMPXAlbumArtUtilityScaleL( CStifItemParser& aItem )
    {
    TInt err=KErrNone;
    TPtrC string;
	if ( KErrNone == aItem.GetNextString(string) )
		{
		TBuf<120> KPath;
		KPath.Append(Kmpxalbumartutilitytest_testPath);
		KPath.Append(string);
	    iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityScaleL"));
	    CFbsBitmap* iSourceBMP = iAlbumArtUtility->DecodeL(KPath,TSize(1,1));//=new(ELeave) CFbsBitmap();
	    
	    TRAP( err , iAlbumArtUtility->ScaleL(*iSourceBMP,TSize(10,10)));
	   
	   	iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityScaleL returned: %d"), err);
		}
    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxalbumartutilitytest::CMPXAlbumArtUtilityExtractL
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//12
TInt Cmpxalbumartutilitytest::CMPXAlbumArtUtilityExtractL( CStifItemParser& aItem )
    {
    TInt err = KErrNone;
    TPtrC string;
	if ( KErrNone == aItem.GetNextString(string) )
		{
		TBuf<120> KPath;
		KPath.Append(Kmpxalbumartutilitytest_testPath);
		KPath.Append(string);
	    iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityExtractL"));
	    iAlbumArtUtility->ExtractL(KPath);
	   	iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityExtractL returned: %d"), err);
		}
    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxalbumartutilitytest::CMPXAlbumArtUtilityBitmapL
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//13
TInt Cmpxalbumartutilitytest::CMPXAlbumArtUtilityBitmapL( CStifItemParser& aItem )
    {
    TInt err=KErrNone;
    TPtrC string;
	if ( KErrNone == aItem.GetNextString(string) )
		{
		TBuf8<120> KPath;
		KPath.Append(Kmpxalbumartutilitytest_testPath);
		KPath.Append(string);  	 
	    iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityBitmapL"));
	    
	   // iAlbumArtUtility->BitmapL(KPath);
	    
	   	iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityBitmapL returned: %d"), err);
		}
    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxalbumartutilitytest::CMPXAlbumArtUtilityBitmapDataL
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxalbumartutilitytest::CMPXAlbumArtUtilityBitmapDataL( CStifItemParser& aItem )
    {
    TInt err = KErrNone;
    TPtrC string;
   	if ( KErrNone == aItem.GetNextString(string) )
	{
	 TBuf<120> KPath;
	 KPath.Append(Kmpxalbumartutilitytest_testPath);
	 KPath.Append(string);
     iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityBitmapDataL"));
     CFbsBitmap* iSourceBMP = iAlbumArtUtility->DecodeL(KPath,TSize(1,1));
    
     TRAP( err , iAlbumArtUtility->BitmapDataL(*iSourceBMP));
   
   	 iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityBitmapDataL returned: %d"), err);
	}
    return err;
    }


// ---------------------------------------------------------------------------
// Cmpxalbumartutilitytest::CMPXAlbumArtUtilityCancelRequest
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxalbumartutilitytest::CMPXAlbumArtUtilityCancelRequest( CStifItemParser& /*aItem*/ )
    {
    TInt err=KErrNone;
    iLog->Log(_L("Cmpxalbumartutilitytest::CMPXAlbumArtUtilityCancelRequest"));
    iAlbumArtUtility->CancelRequest();
   	iLog->Log(_L("Cmpxalbumartutilitytest::CancelRequest returned: %d"), err);
    return err;
    }

//  [End of File] - Do not remove
