// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// RefTestAgentArchive.cpp
// 
//

#include <f32file.h>
#include "rtaArchive.h"
#include "drmfiles.h"
#include "drmfilesparser.h"
#include "drmrightsparser.h"
#include "PartOfFile.h"
#include "EntireFile.h"
#include "drmfilecontainer.h"
#include "drmfilecontent.h"
#include "reftestagentconstants.h"

using namespace ReferenceTestAgent;

EXPORT_C CRefTestAgentArchive* CRefTestAgentArchive::NewL(RFile& aFile)
	{
	CRefTestAgentArchive* self = new (ELeave) CRefTestAgentArchive();
	CleanupStack::PushL(self);
	self->ConstructL(aFile);
	CleanupStack::Pop(self);
	return self;
	}

CRefTestAgentArchive::CRefTestAgentArchive()
	{
	}

void CRefTestAgentArchive::ConstructL(RFile& aFile)
	{
	User::LeaveIfError(aFile.Size(iFileSize));
	
	User::LeaveIfError(iFileLock.CreateLocal(1, EOwnerProcess));
	iPartOfFile = CPartOfFile::NewL(aFile, iFileLock, 0, iFileSize);

	// read the header from the file, offsets to embedded rights and content XML descriptions
	iDrmRightsOffset = iPartOfFile->ReadIntL();
	iDrmFileOffset = iPartOfFile->ReadIntL();
	if(iDrmRightsOffset == 0 && iDrmFileOffset == 0)
		{
		// file has no rights and no content
		// must have either or both
		User::Leave(KErrCorrupt);
		}
	if(iDrmFileOffset != 0)
		{
		// read mime type from the field
		iMimeType = iPartOfFile->ReadDescriptor8L();
		}
	else
		{
		// no mime type if there is no content
		iMimeType = HBufC8::NewL(0);	
		}
	
	// get the offset of the start of the content objects
	iPartOfFile->Seek(ESeekCurrent, iContentOffset);
	}

CRefTestAgentArchive::~CRefTestAgentArchive()
	{
	delete iDrmFilesParser;
	delete iPartOfFile;	
	delete iContentXmlFile;
	delete iMimeType;
	iFileLock.Close();
	}

EXPORT_C const TDesC8& CRefTestAgentArchive::DefaultMimeType()
	{
	return *iMimeType;
	}
		
EXPORT_C CDrmFiles& CRefTestAgentArchive::DrmFilesL()
	{
	if(iDrmFileOffset == 0)
		{
		// No content description in this file
		User::Leave(KErrNotFound);
		}
	if(!iDrmFilesParser)
		{
		// go to the start of the XML content description file (the size field)
		User::LeaveIfError(iPartOfFile->Seek(ESeekStart, iDrmFileOffset));
		TInt size = iPartOfFile->ReadIntL();
		
		// Get the offset of the start of the actual XML file
		TInt offset = 0;
		iPartOfFile->Seek(ESeekCurrent, offset);
		
		// parse the XML file
		CPartOfFile* contentXmlFile = CPartOfFile::NewL(iPartOfFile->FileHandle(), iFileLock, offset, size); 
		CleanupStack::PushL(contentXmlFile);
		
		CDrmFilesParser *drmFilesParser = CDrmFilesParser::NewL(*contentXmlFile); 
		CleanupStack::PushL(drmFilesParser);
		drmFilesParser->DrmFilesL().SetEntireFileL(iPartOfFile->FileHandle(), iFileLock, 0, iFileSize);

		
		// associate embedded content files with content objects in the CDrmFile
		User::LeaveIfError(iPartOfFile->Seek(ESeekStart, iContentOffset));
		offset = iContentOffset;
		while(offset < iDrmFileOffset)
			{
			// find the each content Cid, size and offset
			// then setup the virtual file associated with it
			HBufC8* cid = iPartOfFile->ReadDescriptor8L();
			CleanupStack::PushL(cid);
			CDrmFileContent& content = drmFilesParser->DrmFilesL().FindL(cid->Des());
			CleanupStack::PopAndDestroy(cid);
			
			// Find the size and start offset of the content object
			size = iPartOfFile->ReadIntL();
			offset = 0;
			iPartOfFile->Seek(ESeekCurrent, offset);
			content.SetCommonSourceFileL(iPartOfFile->FileHandle(), iFileLock, offset, size);
			
			// Find the end of the object by seeking forward size bytes
			offset = size;
			User::LeaveIfError(iPartOfFile->Seek(ESeekCurrent, offset));
			}
		CleanupStack::Pop(drmFilesParser);
		CleanupStack::Pop(contentXmlFile);
		iDrmFilesParser = drmFilesParser;
		iContentXmlFile = contentXmlFile;
		}
	return iDrmFilesParser->DrmFilesL();
	}

EXPORT_C TInt CRefTestAgentArchive::CreateArchive(const TDesC& aContentXmlFileName, const TDesC& aRightsXmlFileName, TDes& aOutputFileName, TDes8& aOutputMimeType)
	{
	RFs fs;
	TInt err;
	
	err = fs.Connect();
	if(err == KErrNone)
		{
		// remove extension from output file name
		TInt pos = aOutputFileName.LocateF(TChar(KExtDelimiter));
		if(pos != KErrNotFound)
			{
			aOutputFileName.SetLength(pos);
			}
		
		// Add the appropriate extension
		if(aContentXmlFileName.Length() == 0)
			{
			aOutputFileName.Append(KRtaExtensionRights());
			aOutputMimeType.Copy(KXmlRORights());
			}
		else if(aRightsXmlFileName.Length() == 0)
			{
			aOutputFileName.Append(KRtaExtensionContent());
			aOutputMimeType.Copy(KRtaMimeContent());
			}
		else
			{
			aOutputFileName.Append(KRtaExtensionContentRights());
			aOutputMimeType.Copy(KRtaMimeContentRights());
			}
		
		TRAP(err, DoCreateArchiveL(aContentXmlFileName, aRightsXmlFileName, aOutputFileName));
		if(err != KErrNone)
			{
			// If the archive creation failed, delete the resulting output file
			fs.Delete(aOutputFileName);
			}
		fs.Close();
		}
	return err;
	}

void CRefTestAgentArchive::DoCreateArchiveL(const TDesC& aContentXmlFileName, const TDesC& aRightsXmlFileName, const TDesC& aOutputFileName)
	{
	TBuf8 <1024> buffer;

	TInt rightsOffset = 0;
	TInt contentOffset = 0;
	
	// create output file
	CEntireFile* outputFile = CEntireFile::NewL(aOutputFileName, ETrue);
	CleanupStack::PushL(outputFile);

	if(aContentXmlFileName.Length() == 0)
		{
		// write header for "rights only" file
		WriteHeaderL(*outputFile, 0, 0);
		}
	else 
		{
		// Open content xml file
		CEntireFile* contentXmlFile = CEntireFile::NewL(aContentXmlFileName);
		CleanupStack::PushL(contentXmlFile);

		// Parse content xml file
		CDrmFilesParser *drmFilesParser = CDrmFilesParser::NewL(*contentXmlFile); 
		CleanupStack::PushL(drmFilesParser);
		
		// Write header 
		WriteHeaderL(*outputFile, 0, 0);
		
		TBuf8 <KMaxDataTypeLength> mimeType;
		mimeType.Zero();

		if(drmFilesParser->DrmFilesL().ContentCount() == 1)
			{
			// Write mime type of "default" content if there is exactly one content object
			mimeType.Copy(drmFilesParser->DrmFilesL().Content(0).MimeType());
			}
		
		// Write default mime type
		outputFile->WriteDescriptor8L(mimeType);
		
		// Write content files
		for(TInt i = 0; i < drmFilesParser->DrmFilesL().ContentCount(); i++)
			{
			// Write Cid
			outputFile->WriteDescriptor8L(drmFilesParser->DrmFilesL().Content(i).Cid());
			
			// Write file size
			TInt size = 0;
			User::LeaveIfError(drmFilesParser->DrmFilesL().Content(i).Size(size));
			outputFile->WriteIntL(size);
			
			// Copy data from source file to output file
			TInt length = 1024;
			while(length > 0)
				{
				User::LeaveIfError(drmFilesParser->DrmFilesL().Content(i).Read(buffer, buffer.MaxLength()));
				length = buffer.Length();
				User::LeaveIfError(outputFile->Write(buffer, length));	
				}
			}
	
	
		// Find offset of xml content description
		User::LeaveIfError(outputFile->Seek(ESeekCurrent, contentOffset));				
		
		// Write content xml description
		TInt size = 0;
		User::LeaveIfError(contentXmlFile->Size(size));
		outputFile->WriteIntL(size);
		
		TInt length = 1024;
		TInt pos = 0;
		User::LeaveIfError(contentXmlFile->Seek(ESeekStart,pos));
		while(length > 0)
				{
				User::LeaveIfError(contentXmlFile->Read(buffer, buffer.MaxLength()));
				length = buffer.Length();
				User::LeaveIfError(outputFile->Write(buffer, length));	
				}
		CleanupStack::PopAndDestroy(2, contentXmlFile); // drmFile, contentXmlFile
		}
	
	if(aRightsXmlFileName.Length() > 0)
		{
		CEntireFile* rightsXmlFile = CEntireFile::NewL(aRightsXmlFileName);
		CleanupStack::PushL(rightsXmlFile);
		
		// Parse rights to ensure they are valid
		 CDrmRightsParser *drmRightsParser = CDrmRightsParser::NewL(*rightsXmlFile);
		 delete drmRightsParser;
		 drmRightsParser = NULL;

		// Find offset of xml rights description
		User::LeaveIfError(outputFile->Seek(ESeekCurrent, rightsOffset));				
		
		// Write rights xml description size
		TInt size = 0;
		User::LeaveIfError(rightsXmlFile->Size(size));
		outputFile->WriteIntL(size);
		
		// rewind the rights XML file pointer after parsing earlier
		TInt pos = 0;
		User::LeaveIfError(rightsXmlFile->Seek(ESeekStart,pos));
		TInt length = 1024;
		while(length > 0)
				{
				User::LeaveIfError(rightsXmlFile->Read(buffer, buffer.MaxLength()));
				length = buffer.Length();
				User::LeaveIfError(outputFile->Write(buffer, length));	
				}

		CleanupStack::PopAndDestroy(rightsXmlFile);
		}
	
	// update header with Xml offsets	
	TInt start = 0;
	User::LeaveIfError(outputFile->Seek(ESeekStart, start));
	WriteHeaderL(*outputFile, rightsOffset, contentOffset);

	CleanupStack::PopAndDestroy(outputFile); 
	}

void CRefTestAgentArchive::WriteHeaderL(CVirtualFile& aVirtualFile, TInt aRightsOffset, TInt aContentOffset)
	{
	// write Rights Xml file offset
	aVirtualFile.WriteIntL(aRightsOffset);
	
	// write content Xml file offset
	aVirtualFile.WriteIntL(aContentOffset);
	}

EXPORT_C void CRefTestAgentArchive::GetDefaultMimeTypeFromHeaderL(const TDesC8& aBuffer, TDes8& aDefaultMimeType)
	{
	TInt contentOffset = 0;
	TPckg <TInt> contentOffsetPckg(contentOffset);

	TInt length = 0;
	TPckg <TInt> lengthPckg(length);

	if(aBuffer.Length() >= lengthPckg.MaxLength() * 3)
		{
		contentOffsetPckg.Copy(aBuffer.Mid(contentOffsetPckg.MaxLength(), contentOffsetPckg.MaxLength()));
	
		// file only has a default content mime type if it has content
		if(contentOffset > 0)
			{
			// File has three integers at the start of it
			// we want to read the third one which is the length of the default mime type
			lengthPckg.Copy(aBuffer.Mid(lengthPckg.MaxLength() * 2, lengthPckg.MaxLength()));
		
			if(length > 0 && (aBuffer.Length() >= (length + lengthPckg.MaxLength() * 3)))
				{
				aDefaultMimeType.Copy(aBuffer.Mid(lengthPckg.MaxLength() * 3, length));
				}
			}
		}
	}

