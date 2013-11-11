/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class provides implementation body for the API specified
*                by CMetaDataUtility.
*
*/



// INCLUDE FILES
#include	<MetaDataFieldContainer.h>
#include	<Oma2Agent.h>
#include	"MetaDataUtilityBody.h"
#include	"MetaDataParserFactory.h"
#include	"MetaDataParser.h"
#include	"MetaDataSourceFile.h"
#include	"MetaDataSourceDescriptor.h"
#include    "MetaDataBufferAgg.h"
#ifdef _DEBUG
#include	<e32svr.h>
#endif


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::CMetaDataUtilityBody
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMetaDataUtilityBody::CMetaDataUtilityBody()
	:	iContainer(NULL),
		iParser(NULL),
		iSource(NULL),
		iID3Version(ENonID3),
		iChunkMode(EFalse),
        iBufferAgg(NULL)
    {
    }

// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMetaDataUtilityBody::ConstructL()
    {
	iContainer = CMetaDataFieldContainer::NewL();
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataUtilityBody::ConstructL - Done!"));
#endif
    }

// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMetaDataUtilityBody* CMetaDataUtilityBody::NewL()
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataUtilityBody::NewL"));
#endif
	CMetaDataUtilityBody* self = new( ELeave ) CMetaDataUtilityBody;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CMetaDataUtilityBody::~CMetaDataUtilityBody()
	{
	delete iSource;	    
	delete iParser;
	delete iContainer;
	
	CloseChunkData();
	}

// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::OpenFileL
// -----------------------------------------------------------------------------
//
void CMetaDataUtilityBody::OpenFileL(
	const TDesC& aFileName,
	const RArray<TMetaDataFieldId>& aWantedFields )
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataUtilityBody::OpenFileL (TDesc)"));
#endif
	if ( iParser )
		{
		User::Leave(KErrAlreadyExists);
		}
	iParser = FindParserFileL(aFileName);
	if ( iParser )
		{
		iParser->CommonParseL((CMetaDataSourceFile*)iSource, aWantedFields, *iContainer);
        if( iParser->ID32Offset() )
            {						
            CMetaDataParser* id32parser = 
                    CMetaDataParserFactory::CreateID3ParserL(*iSource, iParser->ID32Offset() );
            if( id32parser )
                {
                CleanupStack::PushL( id32parser );
                id32parser->ParseL(aWantedFields, *iContainer);
                CleanupStack::PopAndDestroy( id32parser );
                }
            }
		}
	}

// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::OpenFileL
// -----------------------------------------------------------------------------
//
void CMetaDataUtilityBody::OpenFileL(
	RFile& aFile,
	const RArray<TMetaDataFieldId>& aWantedFields )
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataUtilityBody::OpenFileL(RFile)"));
#endif
	if ( iParser )
		{
		User::Leave(KErrAlreadyExists);
		}
	iParser = FindParserFileL(aFile);
	if ( iParser )
		{
		iParser->CommonParseL((CMetaDataSourceFile*)iSource, aWantedFields, *iContainer);
        if( iParser->ID32Offset() )
            {
            CMetaDataParser* id32parser = 
                    CMetaDataParserFactory::CreateID3ParserL(*iSource, iParser->ID32Offset() );
            if( id32parser )
                {
                CleanupStack::PushL( id32parser );
                id32parser->ParseL(aWantedFields, *iContainer);
                CleanupStack::PopAndDestroy( id32parser );
                }   
            }
		}
	}

// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::OpenDesL
// -----------------------------------------------------------------------------
//
void CMetaDataUtilityBody::OpenDesL(
	const TDesC8& aDes,
	const RArray<TMetaDataFieldId>& aWantedFields )
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataUtilityBody::OpenDesL"));
#endif
	if ( iParser )
		{
		User::Leave(KErrAlreadyExists);
		}
	iParser = FindParserDesL(aDes);
	if ( iParser )
		{
		iParser->CommonParseDesL(aWantedFields, *iContainer);
        if( iParser->ID32Offset() )
            {
            CMetaDataParser* id32parser = 
                    CMetaDataParserFactory::CreateID3ParserL(*iSource, iParser->ID32Offset() );
            if( id32parser )
                {
                CleanupStack::PushL( id32parser );
                id32parser->ParseL(aWantedFields, *iContainer);
                CleanupStack::PopAndDestroy( id32parser );
                }        
            }
		}
	}

// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::MetaDataCount
// -----------------------------------------------------------------------------
//
TInt CMetaDataUtilityBody::MetaDataCount() const
   	{
	return iContainer->Count();
    }

// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::MetaDataFieldsL
// -----------------------------------------------------------------------------
//
const CMetaDataFieldContainer& CMetaDataUtilityBody::MetaDataFieldsL()
   	{
	return *iContainer;
	}

// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::DoParseChunkDataL
//
// This is a callback from the "Buffer Aggregator" to start parsing after all
// meta-data has been aggregated.
// -----------------------------------------------------------------------------
//
void CMetaDataUtilityBody::DoParseChunkDataL(TDesC8& aChunk, TDesC8& aMimeType)
    {
    RArray<TMetaDataFieldId> wantedFields;
    CleanupClosePushL( wantedFields );
    OpenDesL( aChunk, wantedFields, aMimeType );
    CleanupStack::PopAndDestroy();
    }

// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::InitChunkData
// -----------------------------------------------------------------------------
//	
TInt CMetaDataUtilityBody::InitChunkData(
        TDesC8& aMimeType, 
        MMDUChunkDataObserver& aObserver )
    {
            
    if ( &aObserver == NULL ) 
        {
        return KErrArgument;
        }

    if ( aMimeType.Compare(KMp3MimeType) )
        {
        if ( aMimeType.Compare(KMpegMimeType) )      
            {
            return KErrArgument;
            }   
        }
        
    TRAPD( err, iBufferAgg = CMetaDataBufferAgg::NewL( this, aMimeType, aObserver ) );
    if ( err )
        {
        return err;
        }

    iChunkMode = ETrue;    
        
    return KErrNone;  
    }

// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::ProcessChunkData
// -----------------------------------------------------------------------------
//  
TInt CMetaDataUtilityBody::ProcessChunkData( 
        TDesC8& aChunk,
        TBool aFinalChunk )
    {

    TInt error = iBufferAgg->RequestProcessChunkData( aChunk, aFinalChunk );    
    iBufferAgg->ReadyToGo();
    
    return error;
    }

// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::ParseChunkData
// -----------------------------------------------------------------------------
//  
TInt CMetaDataUtilityBody::ParseChunkData()
    {

    TInt error = iBufferAgg->RequestParseChunkData();    
    iBufferAgg->ReadyToGo();
    
    return error;
    }

// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::IsChunkDataMode
// -----------------------------------------------------------------------------
//	
TBool CMetaDataUtilityBody::IsChunkDataMode()
    {
    return iChunkMode;
    }

// -----------------------------------------------------------------------------
// CMetaDataUtility::CloseChunkData
// -----------------------------------------------------------------------------
//	
void CMetaDataUtilityBody::CloseChunkData()
    {
    iChunkMode = EFalse;
    
    if ( iBufferAgg )
        {
        delete iBufferAgg;
        iBufferAgg = NULL;
        }
    }


// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::FindParserFileL
// -----------------------------------------------------------------------------
//
CMetaDataParser* CMetaDataUtilityBody::FindParserFileL(
	RFile& aFile )
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataUtilityBody::FindParserFileL"));
#endif
	iSource = CMetaDataSourceFile::NewL(aFile);	
	if (iSource) // MetaDataParserRA needs this information
	{
		((CMetaDataSourceFile*)iSource)->SetFileHandler(aFile);
		((CMetaDataSourceFile*)iSource)->SetIsFileHandler(ETrue); 
	}
	return DoFindParserFileL();
	}

// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::FindParserFileL
// -----------------------------------------------------------------------------
//
CMetaDataParser* CMetaDataUtilityBody::FindParserFileL(
	const TDesC& aFileName )
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataUtilityBody::FindParserFileL"));
#endif
	iSource = CMetaDataSourceFile::NewL(aFileName);
	return DoFindParserFileL();
	}
	
// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::DoFindParserFileL
// -----------------------------------------------------------------------------
//
CMetaDataParser* CMetaDataUtilityBody::DoFindParserFileL()
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataUtilityBody::DoFindParserFileL"));
#endif
	TBool fileProtected = ((CMetaDataSourceFile*)iSource)->FileProtected();
	TBool keyExist = ETrue;
	if(fileProtected)
		{
		keyExist = ((CMetaDataSourceFile*)iSource)->KeyExist();
		}
	TInt drmType = ((CMetaDataSourceFile*)iSource)->DrmType();
	
	CMetaDataParser* parser = NULL;
	// DRMv1 file with no rights, read from DCF header.
	if ( (drmType == ContentAccess::EOma1Dcf && !keyExist) || 
	        (drmType == ContentAccess::EOma1DcfBased) )
		{
			parser = CMetaDataParserFactory::CreateDcf1FileParserL(*iSource);
			if(parser)
				{
				return parser;
				}
			else 
				{
				return NULL;			
				}
		}

	if ( drmType == ContentAccess::EOma2Dcf )
		{
			parser = CMetaDataParserFactory::CreateDcf2FileParserL(*iSource);
			if(parser)
				{
				return parser;
				}
			else 
				{
				return NULL;			
				}
		}
	
	TInt start = 0;
	((CMetaDataSourceFile*)iSource)->Seek(ESeekStart, start);
	parser = CMetaDataParserFactory::CreateID3ParserL(*iSource);
	if(parser)
		{
		iID3Version = (TInt)(parser->ID3Version());
		}
				
	 if (parser == NULL)
	 	{
	 	TRAPD(err, parser = CMetaDataParserFactory::CreateMP4ParserFileL(*iSource));
	    if(!err && parser )
		   {
			return parser;
			}
	 	}
  
	if (parser == NULL)
		{
		TInt start = 0;
		((CMetaDataSourceFile*)iSource)->Seek(ESeekStart, start);
		TRAPD(err, parser = CMetaDataParserFactory::CreateWMAParserFileL(*iSource));
	    if(!err && parser )
		   	{
			return parser;
			}		
		}
		
#ifndef __METADATA_UTILITY_DISABLE_RA_SUPPORT    // NOT define		
	if (parser == NULL)
		{
		TInt start = 0;
		((CMetaDataSourceFile*)iSource)->Seek(ESeekStart, start);
		parser = CMetaDataParserFactory::CreateRAParserL(*iSource);	
		}
#endif		
				
	return parser;
	}

// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::FindParserDesL
// -----------------------------------------------------------------------------
//
CMetaDataParser* CMetaDataUtilityBody::FindParserDesL(
	const TDesC8& aDes )
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataUtilityBody::FindParserDesL"));
#endif
	CMetaDataParser* parser = NULL;
	TRAPD(err, parser = CMetaDataParserFactory::CreateMP4ParserDesL(aDes));
	if ( !err && parser )
		{
		return parser;
		}
	iSource = CMetaDataSourceDescriptor::NewL(aDes);
	parser = CMetaDataParserFactory::CreateID3ParserL(*iSource);
	if ( !err && parser )
		{
		return parser;
		}
#ifndef __METADATA_UTILITY_DISABLE_RA_SUPPORT    // NOT define						
	parser = CMetaDataParserFactory::CreateRAParserL(*iSource);			
#endif			
	return parser;
	}

// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::ResetL
// -----------------------------------------------------------------------------
//
void CMetaDataUtilityBody::ResetL()
   	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataUtilityBody::ResetL"));
#endif
	delete iSource;
	iSource = NULL;
	delete iParser;
	iParser = NULL;
	delete iContainer;
	iContainer = NULL;
	iContainer = CMetaDataFieldContainer::NewL();
	}


// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::ID3Version
// -----------------------------------------------------------------------------
//	
TID3Version CMetaDataUtilityBody::ID3Version()
{
	return (TID3Version)iID3Version;
}

// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::OpenFileL with MIME Type info
// -----------------------------------------------------------------------------
//
void CMetaDataUtilityBody::OpenFileL(
	const TDesC& aFileName,
	const RArray<TMetaDataFieldId>& aWantedFields,
	const TDesC8& aMimeType )
	{
	#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataUtilityBody::OpenFileL with MimeType"));
	#endif

	if ( iParser )
		{
		User::Leave(KErrAlreadyExists);
		}
		if(KErrNone == IsSupportedMimeType(aMimeType))	
	{
		iParser = FindParserFileL(aFileName, aMimeType);
	}
	else
	{
		User::Leave(KErrNotSupported);	
	}
	
	if ( iParser )
	{
		iParser->CommonParseL((CMetaDataSourceFile*)iSource, aWantedFields, *iContainer);
		if( iParser->ID32Offset() )
		    {
		    CMetaDataParser* id32parser = 
		            CMetaDataParserFactory::CreateID3ParserL(*iSource, iParser->ID32Offset() );
            if( id32parser )
                {
                CleanupStack::PushL( id32parser );
                id32parser->ParseL(aWantedFields, *iContainer);
                CleanupStack::PopAndDestroy( id32parser );
                }    
		    }
	}	
	}
// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::OpenFileL with MIME Type info
// -----------------------------------------------------------------------------
//
void CMetaDataUtilityBody::OpenFileL(
	RFile& aFile,
	const RArray<TMetaDataFieldId>& aWantedFields,
	const TDesC8& aMimeType )
	{
	#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataUtilityBody::OpenFileL with MimeType"));
	#endif
	if ( iParser )
	{
		User::Leave(KErrAlreadyExists);
	}
		
	if(KErrNone == IsSupportedMimeType(aMimeType))	
	{
		iParser = FindParserFileL(aFile, aMimeType);
	}
	else
	{
		User::Leave(KErrNotSupported);		
	}
	
	if ( iParser )
	{
		iParser->CommonParseL((CMetaDataSourceFile*)iSource, aWantedFields, *iContainer);
        if( iParser->ID32Offset() )
            {
            CMetaDataParser* id32parser = 
                    CMetaDataParserFactory::CreateID3ParserL(*iSource, iParser->ID32Offset() );
            if( id32parser )
                {
                CleanupStack::PushL( id32parser );
                id32parser->ParseL(aWantedFields, *iContainer);
                CleanupStack::PopAndDestroy( id32parser );
                }
            }
	}
	}

// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::DoFindParserFileL with MIME Type info
// -----------------------------------------------------------------------------
//
CMetaDataParser* CMetaDataUtilityBody::DoFindParserFileL(const TDesC8& aMimeType)
	{
	#ifdef _DEBUG
		RDebug::Print(_L("CMetaDataUtilityBody::DoFindParserFileL with MimeType"));
	#endif

	TBool fileProtected = ((CMetaDataSourceFile*)iSource)->FileProtected();
	TInt drmType = ((CMetaDataSourceFile*)iSource)->DrmType();
	TBool keyExist = ETrue;
	if(fileProtected)
		{
		keyExist = ((CMetaDataSourceFile*)iSource)->KeyExist();
		}
	CMetaDataParser* parser = NULL;
	// DRMv1 file with no rights, read from DCF header.
	if ( (drmType == ContentAccess::EOma1Dcf && !keyExist) || 
         (drmType == ContentAccess::EOma1DcfBased) )
		{
			parser = CMetaDataParserFactory::CreateDcf1FileParserL(*iSource);
			if(parser)
				{
				return parser;
				}
			else 
				{
				return NULL;			
				}
		}

	if ( drmType == ContentAccess::EOma2Dcf )
		{
			parser = CMetaDataParserFactory::CreateDcf2FileParserL(*iSource);
			if(parser)
				{
				return parser;
				}
			else 
				{
				return NULL;			
				}
		}
	
	TInt start = 0;
	((CMetaDataSourceFile*)iSource)->Seek(ESeekStart, start);
	   
	if(aMimeType.Compare(KMp3MimeType) == 0 
	   || aMimeType.Compare(KMpegMimeType) == 0 
	   || aMimeType.Compare(KAacMimeType) == 0 )
		{
		TRAPD(err, parser = CMetaDataParserFactory::CreateID3ParserL(*iSource));
		if(!err && parser)
		  {
		  iID3Version = (TInt)(parser->ID3Version());
		  return parser;
		  }
		}
	else if (aMimeType.Compare(KMp4AudioMimeType) == 0 
				|| aMimeType.Compare(K3gpAudioMimeType) == 0 
			 	|| aMimeType.Compare(K3g2AudioMimeType) == 0 )
		{
		TInt start = 0;
	    ((CMetaDataSourceFile*)iSource)->Seek(ESeekStart, start);
		TRAPD(err, parser = CMetaDataParserFactory::CreateMP4ParserFileL(*iSource));
	   	if(!err && parser )
		   	{
			return parser;
			}	
		}
	else if (aMimeType.Compare(KWmaMimeType) == 0)
		{
			TInt start = 0;
			((CMetaDataSourceFile*)iSource)->Seek(ESeekStart, start);
			TRAPD(err, parser = CMetaDataParserFactory::CreateWMAParserFileL(*iSource));
			if(!err && parser)
			{
			return parser;
			}	
		}
#ifndef __METADATA_UTILITY_DISABLE_RA_SUPPORT    // NOT define		
	else if (aMimeType.Compare(KRApnMimeType) == 0
				|| aMimeType.Compare(KRArnMimeType) == 0	
				|| aMimeType.Compare(KRAxMimeType) == 0 )
		{
			TInt start = 0;
			((CMetaDataSourceFile*)iSource)->Seek(ESeekStart, start);
			TRAPD(err, parser = CMetaDataParserFactory::CreateRAParserL(*iSource));
			if(!err && parser)
			{
			return parser;
			}	
		}
#endif		
	return parser;
	}
// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::FindParserFileL with MIME Type info
// -----------------------------------------------------------------------------
//
CMetaDataParser* CMetaDataUtilityBody::FindParserFileL( 
	RFile& aFile, const TDesC8& aMimeType )
	{
	#ifdef _DEBUG
		RDebug::Print(_L("CMetaDataUtilityBody::FindParserFileL with MimeType"));
	#endif
		iSource = CMetaDataSourceFile::NewL(aFile);	
		return DoFindParserFileL(aMimeType);
	}

// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::FindParserFileL with MIME Type info
// -----------------------------------------------------------------------------
//
CMetaDataParser* CMetaDataUtilityBody::FindParserFileL(
	const TDesC& aFileName, const TDesC8& aMimeType )
	{
	#ifdef _DEBUG
		RDebug::Print(_L("CMetaDataUtilityBody::FindParserFileL with MimeType"));
	#endif

		iSource = CMetaDataSourceFile::NewL(aFileName);
		return DoFindParserFileL(aMimeType);
	}

// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::FindParserDesL with MIME Type info
// -----------------------------------------------------------------------------
//
CMetaDataParser* CMetaDataUtilityBody::FindParserDesL(
	const TDesC8& aDes,const TDesC8& aMimeType )
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataUtilityBody::FindParserDesL with MimeType"));
#endif
	CMetaDataParser* parser = NULL;
	if (aMimeType.Compare(KMp4AudioMimeType) == 0 
					|| aMimeType.Compare(K3gpAudioMimeType) == 0 
				 	|| aMimeType.Compare(K3g2AudioMimeType) == 0 )	
	{
		TRAPD(err, parser = CMetaDataParserFactory::CreateMP4ParserDesL(aDes));
		if ( !err && parser )
		{
		return parser;
		}
	}
	else if(aMimeType.Compare(KMp3MimeType) == 0 
	   || aMimeType.Compare(KMpegMimeType) == 0 
	   || aMimeType.Compare(KAacMimeType) == 0 )
	{
		iSource = CMetaDataSourceDescriptor::NewL(aDes);
		parser = CMetaDataParserFactory::CreateID3ParserL(*iSource);	
	}
	return parser;
	}
	
// -----------------------------------------------------------------------------
// CMetaDataUtilityBody::OpenDesL with Mimetype info
// -----------------------------------------------------------------------------
//
void CMetaDataUtilityBody::OpenDesL(
	const TDesC8& aDes,
	const RArray<TMetaDataFieldId>& aWantedFields,
	const TDesC8& aMimeType )
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataUtilityBody::OpenDesL with MimeType"));
#endif
	if ( iParser )
		{
		User::Leave(KErrAlreadyExists);
		}
	if(KErrNone == IsSupportedMimeType(aMimeType))	
		{	
		iParser = FindParserDesL(aDes, aMimeType);
		}
	else
		{
		User::Leave(KErrNotSupported);	
		}
	if ( iParser )
		{
		iParser->CommonParseDesL(aWantedFields, *iContainer);
        if( iParser->ID32Offset() )
            {
            CMetaDataParser* id32parser = 
                    CMetaDataParserFactory::CreateID3ParserL(*iSource, iParser->ID32Offset() );
            if( id32parser )
                {
                CleanupStack::PushL( id32parser );
                id32parser->ParseL(aWantedFields, *iContainer);
                CleanupStack::PopAndDestroy( id32parser );
                } 
            }
		}
	}	
	
// -----------------------------------------------------------------------------
// TInt CMetaDataUtilityBody::IsSupportedMimeType
// -----------------------------------------------------------------------------
//
TInt CMetaDataUtilityBody::IsSupportedMimeType( const TDesC8& aMimeType)
{
#ifndef __METADATA_UTILITY_DISABLE_RA_SUPPORT   // NOT define
	if (aMimeType.Compare(KMp4AudioMimeType) == 0 
					|| aMimeType.Compare(K3gpAudioMimeType) == 0 
				 	|| aMimeType.Compare(K3g2AudioMimeType) == 0 
				 	|| aMimeType.Compare(KMpegMimeType) == 0 
				 	|| aMimeType.Compare(KMp3MimeType) == 0 
				 	|| aMimeType.Compare(KAacMimeType) == 0 
				 	|| aMimeType.Compare(KWmaMimeType) == 0 
				 	|| aMimeType.Compare(KRApnMimeType) == 0
				 	|| aMimeType.Compare(KRArnMimeType) == 0
				 	|| aMimeType.Compare(KRAxMimeType) == 0
				 	)	
#else
	if (aMimeType.Compare(KMp4AudioMimeType) == 0 
					|| aMimeType.Compare(K3gpAudioMimeType) == 0 
				 	|| aMimeType.Compare(K3g2AudioMimeType) == 0 
				 	|| aMimeType.Compare(KMpegMimeType) == 0 
				 	|| aMimeType.Compare(KMp3MimeType) == 0 
				 	|| aMimeType.Compare(KAacMimeType) == 0 
				 	|| aMimeType.Compare(KWmaMimeType) == 0 
				 	)	
#endif				 	
	{
		return KErrNone;
	}
	else
	{
		return KErrNotSupported;
	}		
}
//  End of File
