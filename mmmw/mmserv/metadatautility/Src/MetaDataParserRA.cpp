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
* Description:  This class implements 
*
*/



// INCLUDE FILES
#include	"MetaDataParserRA.h"
#ifdef _DEBUG
#include	<e32svr.h>
#endif

#include <s32mem.h>
#include <hxmetadatautil.h>
#include "MetaDataSourceFile.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMetaDataParserRA::CMetaDataParserRA
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMetaDataParserRA::CMetaDataParserRA(
	CMetaDataSource& aSource )
	:	iSource(aSource)
	{
    }

// -----------------------------------------------------------------------------
// CMetaDataParserRA::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMetaDataParserRA::ConstructL()
    {
	if ( ValidateL() )
		{
//	    iExists = TRUE;
		iHxMetaDataUtility = CHXMetaDataUtility::NewL();
		}
	else
	    {
	    User::Leave(KErrNotSupported);    
	    }
	    
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserRA::ConstructL - Done"));
#endif
    }

// -----------------------------------------------------------------------------
// CMetaDataParserRA::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMetaDataParserRA* CMetaDataParserRA::NewL(
	CMetaDataSource& aSource )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserRA::NewL"));
#endif
	CMetaDataParserRA* self = new( ELeave ) CMetaDataParserRA(aSource);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CMetaDataParserRA::~CMetaDataParserRA()
	{
	delete iCharacterSet;
	if (iHxMetaDataUtility)
	    {
	    TRAPD(err, iHxMetaDataUtility->ResetL());
	    if (err != KErrNone)
	        {
	        #ifdef _DEBUG
	            RDebug::Print(_L("CMetaDataParserRA::~CMetaDataParserRA(), err=%d"), err);
	        #endif
	        }
	    delete iHxMetaDataUtility;
	    }
	}

// -----------------------------------------------------------------------------
// CMetaDataParserRA::ParseL
// -----------------------------------------------------------------------------
//
void CMetaDataParserRA::ParseL(
	const RArray<TMetaDataFieldId>& aWantedFields,
	CMetaDataFieldContainer& aContainer )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserRA::ParseL"));
#endif
	iContainer = &aContainer;
	TUint count = 0;
	HXMetaDataKeys::EHXMetaDataId id;
	TFileName fileName;
	
	// Determine if user entered a TDesC filename or a RFile:
	
	if (((CMetaDataSourceFile&)iSource).IsFileHandler())
	{
		RFile rFile;
		rFile.Duplicate( ((CMetaDataSourceFile&)iSource).FileHandler() );
		TRAPD(err, iHxMetaDataUtility->OpenFileL((RFile &)rFile));	// casting necessary--compile error	
#ifdef _DEBUG
		RDebug::Print(_L("CMetaDataParserRA::ParseL: Open File err = %d"), err);
#endif  
		rFile.Close();
		User::LeaveIfError(err);
	}
	else // filename
	{
        if (((CMetaDataSourceFile&)iSource).FileName().Length() > fileName.MaxLength())
        {
            #ifdef _DEBUG
                RDebug::Print(_L("CMetaDataParserRA::ParseL: Invalid File name"));
            #endif
            
            return;                           
        }
		fileName.Copy(((CMetaDataSourceFile&)iSource).FileName());
		iHxMetaDataUtility->OpenFileL(fileName);			
	}
		
	TInt status = iHxMetaDataUtility->GetMetaDataCount(count);
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserRA::ParseL: Meta Data Count = %d"), count);
#endif	

	if ( aWantedFields.Count() == 0 ) // Get all attributes from HxMetaDataUtil
	{		
		for (TUint i=0; i<count;i++)
		{
			HBufC *pDes = NULL;
			status = iHxMetaDataUtility->GetMetaDataAt(i, id, pDes);
			if (status == KErrNone)
			{
				switch (id)
				{
					case HXMetaDataKeys::EHXTitle:
					{
						iContainer->AppendL( EMetaDataSongTitle, *pDes );
						break;				
					}
					case HXMetaDataKeys::EHXAuthor:
					{
						iContainer->AppendL( EMetaDataArtist, *pDes );
						break;				
					}
					case HXMetaDataKeys::EHXGenre:
					{
						iContainer->AppendL( EMetaDataGenre, *pDes );
						break;				
					}
					case HXMetaDataKeys::EHXPerformer:
					{
						iContainer->AppendL( EMetaDataComposer, *pDes );
						break;				
					}
					case HXMetaDataKeys::EHXCopyright:
					{
						iContainer->AppendL( EMetaDataCopyright, *pDes );
						break;				
					}												
					default:
						break;
						
				} // End switch
			} // end if
			else
			{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserRA::ParseL: Error %d returned for count = %d"), status, count);
#endif	
				
			}

		} // end  For
	} // end if
	else   	// Only get attributes in the wanted field array
		{
			TInt count( aWantedFields.Count() );
			TInt err = KErrNone;
			for ( TInt i = 0; i < count; i++ )
			{
				switch ( aWantedFields[ i ] )
				{
					case EMetaDataSongTitle:
						err = GetTitleL(count);
						break;
					case EMetaDataArtist:
						err = GetArtistL(count);
						break;
					case EMetaDataGenre:
						err = GetGenreL(count);
						break;
					case EMetaDataComposer:					
						err = GetComposerL(count);
						break;									
					case EMetaDataCopyright:
						err = GetCopyrightL(count);
						break;
					default:
					break;
				} // end switch
				if (err != KErrNone)
					{
					// Handle error
					}
			} // end for
		} // end else
    }

// -----------------------------------------------------------------------------
// CMetaDataParserRA::ValidateL
// -----------------------------------------------------------------------------
//
TBool CMetaDataParserRA::ValidateL()
	{
	TInt size;
	User::LeaveIfError( iSource.Size( size ) );
	if ( size <= 0 )
		{
		return EFalse;
		}
		
		// Anything else? Call GetMetaDataCount?
		
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CMetaDataParserRA::OpenFileL
// 
// -----------------------------------------------------------------------------
//
void CMetaDataParserRA::OpenFileL(RFile& aFile)
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserRA::OpenFileL"));
#endif

	if (iHxMetaDataUtility)
		{
		iHxMetaDataUtility->OpenFileL((TDesC &)aFile);	
		}
    }

// -----------------------------------------------------------------------------
//
void CMetaDataParserRA::OpenDesL(const TDesC8& aDes)
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserRA::OpenDesL"));
#endif

	if (iHxMetaDataUtility)
		{
		iHxMetaDataUtility->OpenDesL(aDes);	
		}
    }

// -----------------------------------------------------------------------------
// CMetaDataParserRA::GetTitleL
// -----------------------------------------------------------------------------
//
TInt CMetaDataParserRA::GetTitleL(TInt aCount)
	{
	TInt status = KErrNotFound;
	HXMetaDataKeys::EHXMetaDataId id;
	iData = NULL;
	status = iHxMetaDataUtility->GetMetaDataAt(aCount, id, iData);
	if (id == HXMetaDataKeys::EHXTitle)
	{
		iContainer->AppendL( EMetaDataSongTitle, *iData );
		return KErrNone;			
	}		
	return status;
	}

// -----------------------------------------------------------------------------
// CMetaDataParserRA::GetArtistL
// -----------------------------------------------------------------------------
//
TInt CMetaDataParserRA::GetArtistL(TInt aCount)
	{
	TInt status = KErrNotFound;
	HXMetaDataKeys::EHXMetaDataId id;
	iData = NULL;
	status = iHxMetaDataUtility->GetMetaDataAt(aCount, id, iData);
	if (id == HXMetaDataKeys::EHXAuthor)
	{
		iContainer->AppendL( EMetaDataArtist, *iData );
		return KErrNone;			
	}		
	return status;
	}
	
// -----------------------------------------------------------------------------
// CMetaDataParserRA::GetGenreL
// -----------------------------------------------------------------------------
//
TInt CMetaDataParserRA::GetGenreL(TInt aCount)
	{
	TInt status = KErrNotFound;
	HXMetaDataKeys::EHXMetaDataId id;
	iData = NULL;
	status = iHxMetaDataUtility->GetMetaDataAt(aCount, id, iData);
	if (id == HXMetaDataKeys::EHXGenre)
	{
		iContainer->AppendL( EMetaDataGenre, *iData );
		return KErrNone;			
	}		
	return status;
	}	

// -----------------------------------------------------------------------------
// CMetaDataParserRA::GeComposerL
// -----------------------------------------------------------------------------
//
TInt CMetaDataParserRA::GetComposerL(TInt aCount)
	{
	TInt status = KErrNotFound;
	HXMetaDataKeys::EHXMetaDataId id;
	iData = NULL;
	status = iHxMetaDataUtility->GetMetaDataAt(aCount, id, iData);
	if (id == HXMetaDataKeys::EHXPerformer)
	{
		iContainer->AppendL( EMetaDataComposer, *iData );
		return KErrNone;			
	}		
	return status;
	}	

// -----------------------------------------------------------------------------
// CMetaDataParserRA::GetCopyrightL
// -----------------------------------------------------------------------------
//
TInt CMetaDataParserRA::GetCopyrightL(TInt aCount)
	{
	TInt status = KErrNotFound;
	HXMetaDataKeys::EHXMetaDataId id;
	iData = NULL;
	status = iHxMetaDataUtility->GetMetaDataAt(aCount, id, iData);
	if (id == HXMetaDataKeys::EHXCopyright)
	{
		iContainer->AppendL( EMetaDataCopyright, *iData );
		return KErrNone;			
	}		
	return status;
	}	

//  End of File

