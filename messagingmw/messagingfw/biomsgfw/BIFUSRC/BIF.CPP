// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// BIF.CPP
//
#include "BIF.H"

#include <s32file.h>		// CFileStore
#include <barsc.h>
#include <barsread.h>

#include "cbifentry.h"
#include "BIFUPAN.H"

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include "bifbase.h"
#endif

//
// Defines and constants
enum TBioInfoFileVersion		// The order here is important, only ever APPEND to the list
	{
	EBifVersionOriginal=1,
	EBifVersionAddedAppCtrl,
	EBifVersionAddedFileExtension
	};

const TInt KIntDefaultArrayGranularity=4;

/** 2nd UID of BIF file stores. */
const TUid KUidBioInfoFile={0x10005233};		// unicode Uid, used as 2nd Uid of file store

/*
 *	CBioInfoFile
 */

CBioInfoFile::CBioInfoFile(RFs& aFs)
: iFs(aFs)
	{
	}

void CBioInfoFile::ConstructL()
	{
	iIconZoomLevelArray=new(ELeave) CArrayFixFlat<TIconZoomLevel>(KIntDefaultArrayGranularity);
	iIdHeaderArray=new(ELeave) CArrayPtrFlat<CIdHeader>(KIntDefaultArrayGranularity);
	}

CBioInfoFile::~CBioInfoFile()
	{
	delete iMessageParserName;
	delete iMessageAppCtrlName;
	delete iDescription;
	delete iFileExtension;
	delete iIconsFilename;
	
	if(iIdHeaderArray) 
		{
		iIdHeaderArray->ResetAndDestroy();
		delete iIdHeaderArray;
		}

	delete iIconZoomLevelArray;
	}

/*
 *	CIdHeader
 */

CBioInfoFile::CIdHeader::CIdHeader()
	{
	}

CBioInfoFile::CIdHeader::~CIdHeader()
	{
	if (iText.IsPtr())
		delete iText.AsPtr();
	}

void CBioInfoFile::CIdHeader::InternalizeL(RReadStream& aStream)
	{
	iType=(TBioMsgIdType)aStream.ReadInt16L();
	iConfidence=(CApaDataRecognizerType::TRecognitionConfidence)aStream.ReadInt32L();
	aStream>>iText;		// is a TSwizzle 
	iPort=(TUint16)aStream.ReadUint16L();
	iCharacterSet.iUid=(TInt32)aStream.ReadInt32L();
	iGeneralIdData=(TInt16)aStream.ReadInt16L();
	}

void CBioInfoFile::CIdHeader::ExternalizeL(RWriteStream& aStream) const
	{
	aStream.WriteInt16L(iType);
	aStream.WriteInt32L(iConfidence);
	aStream<<iText;		// is a TSwizzle 
	aStream.WriteUint16L(iPort);
	aStream.WriteInt32L(iCharacterSet.iUid);
	aStream.WriteInt16L(iGeneralIdData);
	}
	
/*
 *	TIconZoomLevel
 */

void CBioInfoFile::TIconZoomLevel::InternalizeL(RReadStream& aStream)
	{
	iZoomLevel=(TInt16)aStream.ReadInt16L();
	}

void CBioInfoFile::TIconZoomLevel::ExternalizeL(RWriteStream& aStream) const
	{
	aStream.WriteInt16L(iZoomLevel);
	}

/*
 *	CBioInfoFileReader
 */
 
/** Allocates and constructs a new BIF reader object, leaving the object on the 
cleanup stack.

It loads the specified BIF file.

@param	aFs 
Connected file handle

@param	aFileName 
BIF file name

@param	aMsgTypeUid 
Optional message type UID. If this is specified, and the file does not does not 
describe this type, then it leaves with KErrCorrupt.

@return	
New BIF reader object
*/
EXPORT_C CBioInfoFileReader* CBioInfoFileReader::NewLC(RFs& aFs,const TDesC& aFileName,TUid aMsgTypeUid)
	{
	CBioInfoFileReader* self=new(ELeave) CBioInfoFileReader(aFs);
	CleanupStack::PushL(self);
	self->ConstructL(aFileName,aMsgTypeUid);
	return self;
	}

/** Allocates and constructs a new BIF reader object.

It loads the specified BIF file.

@param	aFs 
Connected file handle

@param	aFileName 
BIF file name

@param	aMsgTypeUid 
Optional message type UID. If this is specified, and the file does not does not 
describe this type, then it leaves with KErrCorrupt.

@return
New BIF reader object
*/
EXPORT_C CBioInfoFileReader* CBioInfoFileReader::NewL(RFs& aFs,const TDesC& aFileName,TUid aMsgTypeUid)
	{
	CBioInfoFileReader* self=CBioInfoFileReader::NewLC(aFs,aFileName,aMsgTypeUid);
	CleanupStack::Pop(self);
	return self;
	}

CBioInfoFileReader::CBioInfoFileReader(RFs& aFs)
: CBioInfoFile(aFs)
	{
	}

void CBioInfoFileReader::ConstructL(const TDesC& aFileName,TUid aMsgTypeUid)
	{
	CBioInfoFile::ConstructL();		

	iFileName = aFileName.AllocL();
	TEntry entry;
	User::LeaveIfError(iFs.Entry(aFileName, entry));
	
	iEntry = CBifEntry::NewL(entry);
	
	// Load the data
	if( IsResFileL() )
		{
		LoadResourceL(aMsgTypeUid);
		}
	else if( IsDatFile(entry.iType) )
		{
		LoadDataL(aMsgTypeUid);
		}
	else
		{
		User::Leave(KErrCorrupt);
		}
	}

TBool CBioInfoFileReader::IsDatFile(const TUidType& aUidType) const
	{
	if( aUidType[0] == KDirectFileStoreLayoutUid && aUidType[1] == KUidBioInfoFile)
		{
		return ETrue;
		}
	else
		{
		return EFalse;
		}
	}

void CBioInfoFileReader::LoadDataL(TUid aMsgTypeUid)
	{
	// Open file and validate its Uids (if Uid supplied as argument)
	CFileStore* store = CFileStore::OpenLC(iFs, *iFileName, EFileRead|EFileShareReadersOnly);

	const TUidType& type=store->Type();
	if (aMsgTypeUid!=KNullUid && type[2]!=aMsgTypeUid)
		{
		User::Leave(KErrCorrupt); // it's the wrong file!!!!!
		}

	// Read from file into our data members
	RStoreReadStream inStream;
	inStream.OpenLC(*store,store->Root());
	InternalizeL(inStream);
	
	CleanupStack::PopAndDestroy(2, store); // inStream, store
	}

TBool CBioInfoFileReader::IsResFileL() const
	{
	// Check the extension to see if it's a resource file
	TParse parse;
	User::LeaveIfError(parse.Set(*iFileName, NULL, NULL));

	// If first alpha character of extension is 'r' assume we're dealing with a resource file
	TPtrC ext(parse.Ext());
	return ext.Length() > 1 && TCharF(ext[1]) == TCharF('r');
	}


void CBioInfoFileReader::LoadResourceL(TUid aMsgTypeUid)
	{
	// Open the resource file
	RResourceFile file;
	file.OpenL(iFs, *iFileName);
	CleanupClosePushL(file);

	// Read the resource
	HBufC8* resBuf = file.AllocReadLC(1);

	// interpret the resource buffer
	TResourceReader reader;
	reader.SetBuffer(resBuf);

	// Read message type uid, and check it's valid
	iMessageTypeUid = TUid::Uid(reader.ReadInt32());
	if (aMsgTypeUid != KNullUid && iMessageTypeUid != aMsgTypeUid)
		{
		User::Leave(KErrCorrupt);
		}
	
	// Read message parser name
	delete iMessageParserName;
	iMessageParserName = NULL;
	iMessageParserName = reader.ReadHBufCL();
	// Read message app uid
	iMessageAppUid = TUid::Uid(reader.ReadInt32());

	// Read message appctrl uid
	delete iMessageAppCtrlName;
	iMessageAppCtrlName = NULL;
	iMessageAppCtrlName = reader.ReadHBufCL();

	// Read description
	delete iDescription;
	iDescription = NULL;
	iDescription = reader.ReadHBufCL();

	// Read file extension
	delete iFileExtension;
	iFileExtension = NULL;
	iFileExtension = reader.ReadHBufCL();

	// Read general data[1..3]
	iGeneralData1 = (TInt16)reader.ReadInt16();
	iGeneralData2 = (TInt16)reader.ReadInt16();
	iGeneralData3 = (TInt16)reader.ReadInt16();

	// Read in icon stuff
	delete iIconsFilename;
	iIconsFilename = NULL;
	iIconsFilename = reader.ReadHBufCL();	

	// Read in zoom levels
	TInt icons = reader.ReadInt16();
	while(icons--)
		{
		TIconZoomLevel zoom;
		zoom.iZoomLevel = (TInt16)reader.ReadInt16();
		iIconZoomLevelArray->AppendL(zoom);
		}

	// Read in IDs
	TInt ids = reader.ReadInt16();	
	while( ids-- > 0 )
		{
		CIdHeader* header = new(ELeave)CIdHeader;
		CleanupStack::PushL(header);

		header->iType = (TBioMsgIdType)reader.ReadInt16();
		header->iConfidence = (CApaDataRecognizerType::TRecognitionConfidence)reader.ReadInt32();
		header->iCharacterSet = TUid::Uid(reader.ReadInt32());

		// Don't ask me why it's like this!
		TPtrC text(reader.ReadTPtrC());
		header->iText = HBufC::NewL(text.Length());
		(*header->iText) = text;
		
		header->iPort = (TInt16)reader.ReadInt16();
		header->iGeneralIdData = (TInt16)reader.ReadInt16();

		iIdHeaderArray->AppendL(header);
		CleanupStack::Pop(header);
		}
			
	CleanupStack::PopAndDestroy(2, &file);	// resBuf, file
	}

/** 
Destructor. 
*/
EXPORT_C CBioInfoFileReader::~CBioInfoFileReader()
	{
	delete iFileName;
	delete iEntry;
	}


void CBioInfoFileReader::InternalizeL(RReadStream& aStream)
	{
	//
	// Get version number
	const TInt32 version(aStream.ReadInt32L());	
	__ASSERT_DEBUG(version==EBifVersionOriginal || 
			       version==EBifVersionAddedAppCtrl ||
				   version==EBifVersionAddedFileExtension,
				   Panic(EBifUnknownVersion));

	// Get data for BIF version EBifVersionOriginal
	iMessageTypeUid.iUid=(TInt32)aStream.ReadInt32L();

	delete iMessageParserName;
	iMessageParserName = NULL;
	iMessageParserName = HBufC::NewL(aStream, KMaxFileName);
	
	iMessageAppUid.iUid=(TInt32)aStream.ReadInt32L();
	
	delete iDescription;
	iDescription = NULL;
	iDescription = HBufC::NewL(aStream, KMaxFileName);

	iGeneralData1=(TInt16)aStream.ReadInt16L();
	iGeneralData2=(TInt16)aStream.ReadInt16L();
	iGeneralData3=(TInt16)aStream.ReadInt16L();

	delete iIconsFilename;
	iIconsFilename = NULL;
	iIconsFilename = HBufC::NewL(aStream, KMaxFileName);

	aStream>>*iIconZoomLevelArray;		
	
	const TInt32 numHeaders=(TInt32)aStream.ReadInt32L();
	for (TInt32 cc=0; cc<numHeaders; ++cc)
		{
		CIdHeader* header=new(ELeave) CIdHeader;
		CleanupStack::PushL(header);
		aStream>>*header;
		iIdHeaderArray->AppendL(header);
		CleanupStack::Pop(header);
		}
	
	if(version==EBifVersionOriginal)
		return;

	// Get data for BIF version EBifVersionAddedAppCtrl
	delete iMessageAppCtrlName;
	iMessageAppCtrlName = NULL;
	iMessageAppCtrlName = HBufC::NewL(aStream, KMaxFileName);
	if(version==EBifVersionAddedAppCtrl)
		return;

	delete iFileExtension;
	iFileExtension = NULL;
	iFileExtension = HBufC::NewL(aStream, KMaxFileName);
	if(version==EBifVersionAddedFileExtension)
		return;

	// Put code here for reading data from newer versions of BIF
	}

/** Gets the BIF's message type UID setting.

@return 
Message type UID 
*/
EXPORT_C TUid CBioInfoFileReader::MessageTypeUid() const
	{
	return iMessageTypeUid;
	}
	
/** Gets the BIF's message parser name.

@return 
BIF's message parser name
*/
EXPORT_C const TPtrC CBioInfoFileReader::MessageParserName() const
	{
	return (iMessageParserName!=NULL) ? iMessageParserName->Des() : TPtrC();  
	}


/** Gets the BIF's application UID setting.

@return
Application UID setting
*/
EXPORT_C TUid CBioInfoFileReader::MessageAppUid() const
	{
	return iMessageAppUid;
	}

/** Deprecated method.

Use CBioInfoFileReader::MessageAppCtrlName

@see CBioInfoFileReader::MessageAppCtrlName

@return
Invalid return.

@deprecated
*/
EXPORT_C TUid CBioInfoFileReader::MessageAppCtrlUid() const
	{
	User::Invariant();
	return TUid();
	}
	
/** Gets the BIF's control name.

@return
Control name
*/
EXPORT_C const TPtrC CBioInfoFileReader::MessageAppCtrlName() const
	{
	return (iMessageAppCtrlName != NULL) ? iMessageAppCtrlName->Des() : TPtrC();  
	}
 

/** Gets the BIF's general data 1 setting.

@return 
General data 1 setting
*/
EXPORT_C TInt16 CBioInfoFileReader::GeneralData1() const
	{
	return iGeneralData1;
	}

/** Gets the BIF's general data 2 setting.

@return 
General data 2 setting
*/
EXPORT_C TInt16 CBioInfoFileReader::GeneralData2() const
	{
	return iGeneralData2;
	}

/** Gets the BIF's general data 3 setting.

@return
General data 3 setting
*/
EXPORT_C TInt16 CBioInfoFileReader::GeneralData3() const
	{
	return iGeneralData3;
	}

/** Gets the BIF's description setting.

@return
BIF's description setting
*/
EXPORT_C const TPtrC CBioInfoFileReader::Description() const
	{
	return (iDescription!=NULL) ? iDescription->Des() : TPtrC();  
	}

//
// Data getter
EXPORT_C const TPtrC CBioInfoFileReader::FileExtension() const
/** Gets the BIF's file extension setting.

@return BIF's file extension setting */
	{
	return (iFileExtension!=NULL) ? iFileExtension->Des() : TPtrC();  
	}

/** Gets the BIF's icons filename setting.

@return
BIF's icons filename setting
*/
EXPORT_C const TPtrC CBioInfoFileReader::IconsFilename() const
	{
	return (iIconsFilename!=NULL) ? iIconsFilename->Des() : TPtrC();  
	}

/** Gets the BIF's icon zoom levels setting.

The array of icon zoom level settings is left of the cleanup stack and ownership
is returned to the caller.

@return
Icon zoom levels setting
*/
EXPORT_C const CArrayFixFlat<TInt16>* CBioInfoFileReader::ZoomLevelsLC() const
	{
	CArrayFixFlat<TInt16>* array=new(ELeave) CArrayFixFlat<TInt16>(KIntDefaultArrayGranularity);
	CleanupStack::PushL(array);

	const TInt count=iIconZoomLevelArray->Count();

	for(TInt i=0;i<count;i++)
		{
		array->AppendL((*iIconZoomLevelArray)[i].iZoomLevel);
		}

	return array;
	}

/** Gets the BIF's number of icon zoom levels.

@return
Number of icon zoom levels
*/
EXPORT_C TInt CBioInfoFileReader::ZoomLevelsCount() const			
	{
	return iIconZoomLevelArray->Count();
	}

/** Gets the BIF's ID array.

The of IDs is left of the cleanup stack and ownership is returned to the caller.

@return
ID array
*/
EXPORT_C const CArrayFixFlat<TBioMsgId>* CBioInfoFileReader::IdsLC() const
	{
	CArrayFixFlat<TBioMsgId>* array=new(ELeave) CArrayFixFlat<TBioMsgId>(KIntDefaultArrayGranularity);
	CleanupStack::PushL(array);

	TBioMsgId id;
	const TInt count=iIdHeaderArray->Count();

	for(TInt i=0;i<count;++i)
		{
		// Get header for the Id
		CIdHeader* header=iIdHeaderArray->At(i);
	
		// Read in swizzle(s) of Id
		if (!header->iText.IsPtr())
			{
			CFileStore* store = CFileStore::OpenLC(iFs,*iFileName,EFileRead|EFileShareReadersOnly);		
			RStoreReadStream inStream;
			TStreamId streamId=header->iText.AsId();
			inStream.OpenLC(*store,streamId);

			TBioMsgIdText idText;
			inStream>>idText;
			header->iText = idText.AllocL();

			CleanupStack::PopAndDestroy(2, store);		// inStream, store
			}

		// Fill object, ready for appending to array
		id.iType=header->iType;
		id.iConfidence=header->iConfidence;
		id.iText=*(header->iText);
		id.iPort=header->iPort;
		id.iCharacterSet=header->iCharacterSet;
		id.iGeneralIdData=header->iGeneralIdData;
				
		// Append object to array
		array->AppendL(id);
		}
	return array;
	}

/** BIF entry.

@return
BIF entry

@internalTechnology
@released
*/
EXPORT_C const CBifEntry& CBioInfoFileReader::BifEntry() const
	{
	return *iEntry;
	}
