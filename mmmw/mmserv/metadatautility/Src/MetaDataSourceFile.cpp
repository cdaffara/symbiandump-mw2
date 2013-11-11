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
* Description:  This class implements the interface specified in CMetaDataSource
*				 when audio source is from a file.
*
*/



// INCLUDE FILES
#include <Oma2Agent.h>
#include <caf/caferr.h>
#include <drmrightsinfo.h>
#include <drmutilitytypes.h>

#include	"MetaDataSourceFile.h"
#ifdef _DEBUG
#include	<e32svr.h>
#endif

   

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMetaDataSourceFile::CMetaDataSourceFile
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMetaDataSourceFile::CMetaDataSourceFile()
    {
    }

// -----------------------------------------------------------------------------
// CMetaDataSourceFile::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMetaDataSourceFile::ConstructL(
	const TDesC& aFileName )
    {
    iFileName.Copy(aFileName);
    TRAPD(err, iContent = CContent::NewL(aFileName, EContentShareReadWrite ));
    if(err)
    	{
    	iContent = CContent::NewL(aFileName, EContentShareReadOnly );
    	}
    iData = iContent->OpenContentL(EPeek);
    GetSourceAttributesL(); 
    
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataSourceFile::ConstructL - Done"));
#endif
	}

// -----------------------------------------------------------------------------
// CMetaDataSourceFile::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMetaDataSourceFile::ConstructL(
	RFile& aFile )
    {
    //iFileName.Copy(aFileName);
    iContent = CContent::NewL(aFile);
    iData = iContent->OpenContentL(EPeek);
    GetSourceAttributesL();
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataSourceFile::ConstructL - Done"));
#endif
	}

// -----------------------------------------------------------------------------
// CMetaDataSourceFile::GetSourceAttributesL
// -----------------------------------------------------------------------------
//
void CMetaDataSourceFile::GetSourceAttributesL()
{
#ifdef _DEBUG
    RDebug::Print(_L("CMetaDataSourceFile::GetSourceAttributesL"));
#endif

    TInt value = 0;
	TInt err = iContent->GetAttribute(EIsProtected, value);
	if(err == KErrNone && value == 1)
		{
		iProtected = ETrue;
    	}
    value = 0; err = 0;
    err = iData->GetAttribute(ECanPlay, value);
    if(err == KErrNone && value == 1)
    	{
    	iCanPlay = ETrue;
    	}
    	
    iDRMType = -1; err = 0;	
    err = iData->GetAttribute(ContentAccess::EFileType, iDRMType);
    if(err != KErrNone)
    	{
    	iDRMType = -1;
    	}


   // Retrieve Content ID
    TPtr cid( NULL, 0 );
    HBufC* uniqueId( HBufC::NewLC( ContentAccess::KMaxCafUniqueId ) );
    cid.Set( uniqueId->Des() );
    err = iData->GetStringAttribute(ContentAccess::EContentID, cid);

   // Verify key exist or not, if iCanPlay is false and cid is existed
   iKeyExist = ETrue;  // Assume key is exist
   if (err == KErrNone && iCanPlay == EFalse)
       {
   	    DRM::CDrmRightsInfo* ri( DRM::CDrmRightsInfo::NewLC() );
        DRM::TDrmRightsInfo info( DRM::EURightsInfoValid );    
        TRAP( err, ri->CheckRightsL( cid, ContentAccess::EPeek, info )) ; 
              
        if (err != KErrNone || info == DRM::EURightsInfoMissing)
            {
            #ifdef _DEBUG
     	      RDebug::Print(_L("CMetaDataSourceFile::GetSourceAttributesL-CheckRightsL-err=%d, info=%d"), err, info);
            #endif
           
            iKeyExist = EFalse;
            }
        CleanupStack::PopAndDestroy( ri );
       }
    CleanupStack::PopAndDestroy( uniqueId );    

}


// -----------------------------------------------------------------------------
// CMetaDataSourceFile::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMetaDataSourceFile* CMetaDataSourceFile::NewL(
	const TDesC& aFileName )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataSourceFile::NewL"));
#endif
	CMetaDataSourceFile* self = new( ELeave ) CMetaDataSourceFile;
    CleanupStack::PushL( self );
    self->ConstructL( aFileName );
    CleanupStack::Pop();
    return self;
    }
    
    
// -----------------------------------------------------------------------------
// CMetaDataSourceFile::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMetaDataSourceFile* CMetaDataSourceFile::NewL(
	RFile& aFile )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataSourceFile::NewL"));
#endif
	CMetaDataSourceFile* self = new( ELeave ) CMetaDataSourceFile;
    CleanupStack::PushL( self );
    self->ConstructL( aFile );
    CleanupStack::Pop();
    return self;
    }

// Destructor
CMetaDataSourceFile::~CMetaDataSourceFile()
	{
	delete iData;
	delete iContent;
	}

// -----------------------------------------------------------------------------
// CMetaDataSourceFile::ReadL
// -----------------------------------------------------------------------------
//
void CMetaDataSourceFile::ReadL(
	TDes8& aDes )
   	{
	User::LeaveIfError( iData->Read(aDes) );
	}

// -----------------------------------------------------------------------------
// CMetaDataSourceFile::ReadL
// -----------------------------------------------------------------------------
//
void CMetaDataSourceFile::ReadL(
	TDes8& aDes,
	TInt aLength )
   	{
	User::LeaveIfError( iData->Read(aDes, aLength) );
    }

// -----------------------------------------------------------------------------
// CMetaDataSourceFile::ReadL
// -----------------------------------------------------------------------------
//
void CMetaDataSourceFile::ReadL(
	TInt aPos,
	TDes8& aDes )
    {
    User::LeaveIfError( iData->Seek(ESeekStart, aPos) );
	User::LeaveIfError( iData->Read(aDes) );
    }

// -----------------------------------------------------------------------------
// CMetaDataSourceFile::ReadL
// -----------------------------------------------------------------------------
//
void CMetaDataSourceFile::ReadL(
	TInt aPos,
	TDes8& aDes,
	TInt aLength )
    {
    User::LeaveIfError( iData->Seek(ESeekStart, aPos) );
	User::LeaveIfError( iData->Read(aDes, aLength) );
    }

// -----------------------------------------------------------------------------
// CMetaDataSourceFile::Size
// -----------------------------------------------------------------------------
//
TInt CMetaDataSourceFile::Size(
	TInt& aSize ) const
    {
    TRAPD(err, iData->DataSizeL(aSize) );
    if(err)
    	{
    	return (KErrNotFound);
    	}
    else
    	{
    	return err;
    	}
    }


TInt CMetaDataSourceFile::Seek(TSeek aMode,TInt& aPos) const
{
	return iData->Seek(aMode, aPos);
}


TBool CMetaDataSourceFile::FileProtected() const
{
    return iProtected;
}

TBool CMetaDataSourceFile::CanPlay() const
{
    return iCanPlay;
}

TInt CMetaDataSourceFile::DrmType() const
{
    return iDRMType;
}

TBool CMetaDataSourceFile::KeyExist() const
{
    return iKeyExist;
}

TInt CMetaDataSourceFile::GetStringAttribute(TInt aAttribute, TDes& aValue) const
{
	if(aAttribute == ERecordingYear)
	{
		TInt year = 0;
		TInt err = iData->GetAttribute(aAttribute, year);
		if(err != KErrNone)
		{
			return err;	
		}
		if(year >= 0)
		{
			aValue.AppendNum(year); 
			return KErrNone;
		}
		return KErrCorrupt;
	}
	return iData->GetStringAttribute(aAttribute, aValue);
}


    
TInt CMetaDataSourceFile::GetAttributeL(TInt aAttribute, HBufC8** albumArt)
{
	TBuf<256> coverUriValue;
	TInt err = KErrNone;
	err = iContent->GetStringAttribute( aAttribute, coverUriValue );	
	if(err != KErrNone)
		{
		return err;
		}
	
	CData* albumArtPart = iContent->OpenContentL( EView, coverUriValue );
	if(albumArtPart == NULL)
		{
		return KErrNotFound;
		}
	CleanupStack::PushL(albumArtPart);
	
	TInt albumArtDataSize = 0;
	albumArtPart->DataSizeL(albumArtDataSize);
    (*albumArt) = HBufC8::NewL(albumArtDataSize);
    TPtr8 albumArtData = (*albumArt)->Des();
    err = albumArtPart->Read(albumArtData);
    CleanupStack::PopAndDestroy(); // albumArtPart
	return err;
}


ContentAccess::CData* CMetaDataSourceFile::DataPtr()
{
	return iData;
}

TDesC8& CMetaDataSourceFile::FileName()
{
	return iFileName;
}

RFile CMetaDataSourceFile::FileHandler()
{
	return iFileHandler;
}

void CMetaDataSourceFile::SetFileHandler(RFile aRFile)
{
	iFileHandler = aRFile;
}

TBool CMetaDataSourceFile::IsFileHandler()
{
	return iIsFileHandler;
}

void CMetaDataSourceFile::SetIsFileHandler(TBool aIsFileHandler)
{
	iIsFileHandler = aIsFileHandler;
}


//  End of File
