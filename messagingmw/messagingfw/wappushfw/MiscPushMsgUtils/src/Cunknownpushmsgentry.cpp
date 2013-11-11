// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#include <msvids.h>
#include <push/unknownmimedefs.h>
#include <bautils.h>		// Bafl
#include <barsc.h>		// RResourceFile
#include <barsread.h>		// TResourceReader
#include <wappushunknown.rsg>

#include <push/cunknownpushmsgentry.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include "PushEntryPanic.h"
#endif//SYMBIAN_ENABLE_SPLIT_HEADERS

#ifdef __MESSAGING_API_V2__
#include "CAsyncWaiter.h"

#include <cmsvattachment.h>
#include <mmsvattachmentmanager.h>
#endif // __MESSAGING_API_V2__

_LIT(KContentResourceFile, "\\system\\data\\wappushunknown.rsc");

GLDEF_C TPtrC16 LimitStringSize(const TPtrC16& aString, TInt aMaxSize);


/**
Allocates and constructs a new Unknown WAP Push message object. 

@return	CUnknownPushMsgEntry*	
New Unknown WAP Push message object.
*/
EXPORT_C CUnknownPushMsgEntry* CUnknownPushMsgEntry::NewL()
	{
	CUnknownPushMsgEntry* self = new (ELeave) CUnknownPushMsgEntry();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


/**
Allocates and constructs a new Unknown WAP Push message object, specifying the AppId 
as a string.
 
@param	aAppURI					
AppId value as a string. 

@return	CUnknownPushMsgEntry*	
New Unknown WAP Push message object.
*/
EXPORT_C CUnknownPushMsgEntry* CUnknownPushMsgEntry::NewL(const TPtrC8& aAppURI)
	{
	CUnknownPushMsgEntry* self = new (ELeave) CUnknownPushMsgEntry();
	CleanupStack::PushL(self);
	self->ConstructL(aAppURI);
	CleanupStack::Pop();
	return self;
	}


/**
Allocates and constructs a new Unknown WAP Push message object, 
specifying the AppId as a number. 

@param	aAppID					
AppId value as a number.

@return CUnknownPushMsgEntry*	
New Unknown WAP Push message object.
*/
EXPORT_C CUnknownPushMsgEntry* CUnknownPushMsgEntry::NewL(TInt& aAppID)
	{
	CUnknownPushMsgEntry* self = new (ELeave) CUnknownPushMsgEntry();
	CleanupStack::PushL(self);
	self->ConstructL(aAppID);
	CleanupStack::Pop();
	return self;
	}


/**
Destructor.
*/
EXPORT_C CUnknownPushMsgEntry::~CUnknownPushMsgEntry()
	{
	iRFs.Close();
	delete iMessageData;
	delete iContentType;
	delete iFilename;
	}


/**
Sets the Content Type of the message.

@param	aContentType	
The content type of the message
*/ 
EXPORT_C void CUnknownPushMsgEntry::SetContentTypeL(const TDesC& aContentType)
	{
	HBufC* tempBuf = aContentType.AllocL();

	delete iContentType;
	iContentType  = tempBuf;
	}


EXPORT_C const TDesC8& CUnknownPushMsgEntry::MessageData() const
/**
Gets the message data. 

If the buffer for the message data does not exist, a null descriptor is returned.

@return	
Message data
*/
	{
	if (iMessageData)
		return *iMessageData;
	else
		return KNullDesC8;
	}


/**
Sets the message data.
 
@param	aMessageData	
Message data
*/
EXPORT_C void CUnknownPushMsgEntry::SetMessageDataL(const TDesC8& aMessageData)
	{
	HBufC8* tempBuf = aMessageData.AllocL();

	delete iMessageData; 
	iMessageData = tempBuf;
	}


/**
Gets the content filename of the message. Content Filename is the name of the file where the 
Push message content is stored.

If filename does not exist then a default filename is returned, which is defined in wappushunknown.rss file.

Note the function will return the filename in memory which may not necessarily be the filename stored in the 
message store. RetrieveL should be called before calling this function to get the filename stored in the 
message store. 

@code
	// Create an unknown push message object.
	CUnknownPushMsgEntry* entry = CUnknownPushMsgEntry::NewL();
	CleanupStack::PushL(entry);

	// Retrieve the required message from the message store.
	entry->RetrieveL(session, messageId);

	// Get file name
	TFileName fileName = entry->ContentFileNameL();

	CleanupStack::PopAndDestroy(entry);
@endcode

@return	
The content filename of the message.

@see CUnknownPushMsgEntry::RetrieveL()
*/
EXPORT_C const TDesC& CUnknownPushMsgEntry::ContentFileNameL()
	{
	if (iFilename && iFilename->Length()>0 && iFilename->Length() <= KMaxFileName)
		{
		// filename has been set
		return *iFilename;
		}
	else
		{
		TFileName filename = iDefaultBaseFilename;
		// append the extension based on the content type
		filename.Append(ContentTypeExtension(ContentType()));
		SetContentFileNameL(filename);
		return *iFilename;
		}
	}


/**
Sets the content filename of the message. Content Filename is the name of the file where the 
Push message content is stored.

Note the function will only save the filename in memory. SaveL or UpdateL will have 
to be called to commit the changes to the message store.

@code
	// Create an unknown push message object.
	CUnknownPushMsgEntry* entry = CUnknownPushMsgEntry::NewL();
	CleanupStack::PushL(entry);

	// Retrieve the required message from the message store.
	entry->RetrieveL(session, messageId);

	// Set file name
	_LIT(KFilename, "image.jpg");
	entry->SetContentFileNameL(KFilename);

	// Commit the changes to the message store.
	entry->UpdateL(session);

	CleanupStack::PopAndDestroy(entry);
@endcode

@param	aFilename		
The content filename.

@leave	KErrBadName  aFilename is not a valid Symbian OS file name.
@leave	TDesC::AllocL()	
*/	
EXPORT_C void CUnknownPushMsgEntry::SetContentFileNameL(const TDesC& aFilename)
	{
	// Check for valid filname.
	if (!iRFs.IsValidName(aFilename))
		User::Leave(KErrBadName);
		
	HBufC* tempBuf = aFilename.AllocL();
	delete iFilename; 
	iFilename = tempBuf;
	}


/**
Constructor.
Simply connects to the File Server before calling base class ConstructL(). 
*/
void CUnknownPushMsgEntry::ConstructL()
	{
	User::LeaveIfError(iRFs.Connect());
	CPushMsgEntryBase::ConstructL();
	SetDefaultBaseFileNameL();
	}


/**
Constructor.
Simply connects to the File Server before calling base class ConstructL(). 

@param	
aAppURI set this value if AppId is in string form (must set aIsAnInt to EFalse).
*/
void CUnknownPushMsgEntry::ConstructL(const TPtrC8& aAppURI)
	{
	User::LeaveIfError(iRFs.Connect());
	CPushMsgEntryBase::ConstructL(aAppURI);
	SetDefaultBaseFileNameL();
	}


/**
Constructor.
Simply connects to the File Server before calling base class ConstructL(). 

@param	
aAppID set this value if AppId is a number (must set aIsAnInt to ETrue).
*/
void CUnknownPushMsgEntry::ConstructL(TInt& aAppID)
	{
	User::LeaveIfError(iRFs.Connect());
	CPushMsgEntryBase::ConstructL(aAppID);
	SetDefaultBaseFileNameL();
	}


/**
Constructor.
*/
CUnknownPushMsgEntry::CUnknownPushMsgEntry()
	{
	}


/**
Opens wappushunknown.rss resource file for reading.

@param	aFile			
A resource file.
*/	
void CUnknownPushMsgEntry::OpenResourceFileL(RResourceFile& aFile)
	{
	// open resource file
	TFileName fileName;
	Dll::FileName(fileName);
	TParse parse;
	parse.Set(KContentResourceFile, &fileName, 0);
	fileName=parse.FullName();

	BaflUtils::NearestLanguageFile(iRFs, fileName);
	aFile.OpenL(iRFs, fileName);
	}


/**
Sets default content base filename
*/	
void CUnknownPushMsgEntry::SetDefaultBaseFileNameL()
	{
	// generate default filename from the wappushunknown resource file. 
	RResourceFile resourceFile;
	OpenResourceFileL(resourceFile);
	CleanupClosePushL(resourceFile);

	HBufC8* buf = resourceFile.AllocReadLC(CONTENT);
	TResourceReader reader;
	reader.SetBuffer(buf);

	iDefaultBaseFilename.Copy(reader.ReadTPtrC());		
	CleanupStack::PopAndDestroy(2,&resourceFile); // buf, resourceFile
	}


/**
Stores the Unknown Push message in the message store.

Saves the message data in a new attachment file. This makes it more efficient for the UI
to read the data.

@param	aSession		
Message server session

@param	aParent	
ID of the parent for the new entry. It is the caller's responsibility to ensure 
that the parent ID is correct.

@return	
ID of the new message server entry.

@see	CPushMsgEntryBase::SaveL()
*/
EXPORT_C TMsvId CUnknownPushMsgEntry::SaveL(CMsvSession& aSession, TMsvId aParent)

	{
	CMsvEntry* entryPtr = aSession.GetEntryL(KMsvRootIndexEntryId);
	CleanupStack::PushL(entryPtr);

	// This tidies itself up
	TMsvId newEntry = CPushMsgEntryBase::SaveL(aSession, aParent);
	
#ifdef __MESSAGING_API_V2__
	TRAPD(
		saveError,
		TFileName fileName = ContentFileNameL();
		entryPtr->SetEntryL(newEntry);
		CMsvStore* store = entryPtr->EditStoreL();
		CleanupStack::PushL(store);
		SaveMessageBodyL(store->AttachmentManagerL(), fileName);
		store->CommitL();
		CleanupStack::PopAndDestroy(store);
		)
#else
	TRAPD(
		saveError,
		TFileName fileName = ContentFileNameL();
		entryPtr->SetEntryL(newEntry);
		SaveMessageBodyL(*entryPtr, fileName);
		)
#endif // __MESSAGING_API_V2__

	if (saveError != KErrNone)
		{
		aSession.RemoveEntry(iEntry.Id());
		User::Leave(saveError);
		}
	CleanupStack::PopAndDestroy(); //entryPtr
	return newEntry;
	}

#ifdef __MESSAGING_API_V2__
void CUnknownPushMsgEntry::SaveMessageBodyL(MMsvAttachmentManager& aAttachManager, const TDesC& aFileName)
#else	
/**
Stores the Push message body data in an attachment file in the message store.

@param	aEntry			
Reference to the message entry.

@param	aFileName		
The name of the file where the message body data is to be stored.
*/
void CUnknownPushMsgEntry::SaveMessageBodyL(const CMsvEntry& aEntry, TFileName& aFileName)
#endif // __MESSAGING_API_V2__
	{
#ifdef __MESSAGING_API_V2__
	// Create an empty file attachment store the data
	CAsyncWaiter* waiter = CAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);
	attachment->SetAttachmentNameL(aFileName);
	RFile file;
	aAttachManager.CreateAttachmentL(aFileName, file, attachment, waiter->iStatus);
	CleanupStack::Pop(attachment); // ownership passed
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	CleanupStack::PopAndDestroy(waiter);
	CleanupClosePushL(file);
	
	// write the data to the attachment file, if message data exists
	if( iMessageData )
		User::LeaveIfError(file.Write(*iMessageData));
	
	CleanupStack::PopAndDestroy(&file);
#else
	TFileName filePath;

	// create directory path for current entry, to put file into
	aEntry.GetFilePath(filePath);
	
	// check the path and file name is not too long... 
	TParse parse;
	if (parse.Set(aFileName, &filePath, NULL) != KErrNone)
		{
		// Try with the default file name as the file name the client set may be too long.
		// If this leaves then the entry path is probably too long.  Not much we
		// can do about that.
		TFileName defaultFileName;
		defaultFileName.Copy(iDefaultBaseFilename);
		defaultFileName.Append(ContentTypeExtension(ContentType()));
		User::LeaveIfError(parse.Set(defaultFileName, &filePath, NULL));
		};

	RFile file;
	User::LeaveIfError(file.Replace(iRFs, parse.FullName(), EFileWrite|EFileShareExclusive|EFileStream));
	CleanupClosePushL(file);

	// Got this far, so it worked - write data to it
	TInt fileErr = KErrNone;
	if (iMessageData)
		{
		fileErr = file.Write(*iMessageData);
		}

	if(fileErr!=KErrNone)
		{
		iRFs.Delete(parse.FullName());
		User::Leave(fileErr);
		}
	CleanupStack::PopAndDestroy(&file); // file	
#endif // __MESSAGING_API_V2__
	}


/**
Updates an existing message server entry.

The functionality is similiar to SaveL(), except no new entry is created. Before calling 
this function, the existing entry must be loaded into the object using RetrieveL(). 

@param	aSession	
Message server session

@see	CUnknownPushMsgEntry::SaveL()
@see	CUnknownPushMsgEntry::RetrieveL()
@see	CPushMsgEntryBase::UpdateL()
*/
EXPORT_C void CUnknownPushMsgEntry::UpdateL(CMsvSession& aSession)
	{
	if (iHasServerEntry ==EFalse )
		User::Leave(KWPushNoMsgIndexEntry);
	CMsvEntry* msvEntry = aSession.GetEntryL(iEntry.Id());
	CleanupStack::PushL(msvEntry);

	//Can only overwrite a pushmessage of the same type 
	// Mtm & Push Type Uid   must be correct
	__ASSERT_ALWAYS( msvEntry->Entry().iMtm == KUidMtmWapPush, 
					User::Panic(KPushPanicMoniker, EPushEntryWrongMTMtype));
	__ASSERT_ALWAYS( msvEntry->Entry().iBioType == PushMsgType(), 
					User::Panic(KPushPanicMoniker, EPushEntryWrongMsgtype));

	// Remove existing contents of stream and store new data
	CMsvStore* store = msvEntry->EditStoreL();
	CleanupStack::PushL(store);

	TUid streamId;
	streamId.iUid = PushMsgType();
	RMsvWriteStream out;
	out.AssignLC(*store, streamId);
	ExternalizeL(out);

#ifdef __MESSAGING_API_V2__
	// remove all previous attachments
	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	CAsyncWaiter* waiter = CAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	TInt attachCount = manager.AttachmentCount();
	for( TInt ii=attachCount-1; ii>=0; --ii )
		{
		manager.RemoveAttachmentL(ii, waiter->iStatus);
		waiter->StartAndWait();
		User::LeaveIfError(waiter->Result());
		}
	CleanupStack::PopAndDestroy(waiter);
	
	// the message body data as an attachment
	TFileName dataFile = ContentFileNameL();
	SaveMessageBodyL(manager, dataFile);
	
	// update the entry
	iEntry.iMtm = KUidMtmWapPush;
	SetPushMsgType();
	msvEntry->ChangeL(iEntry);	
#else
	TFileName tempfile;
	tempfile.Copy(KTempUnknownFileName);
	tempfile.Append(ContentTypeExtension(ContentType()));
	SaveMessageBodyL(*msvEntry, tempfile); // NB this tidies up if it leaves

	// Ensure the defaults are set correctly. Assume others set by user
	iEntry.iMtm = KUidMtmWapPush;
	SetPushMsgType();

	TRAPD(error, msvEntry->ChangeL(iEntry) );

	TFileName filePath;
	msvEntry->GetFilePath(filePath);
	User::LeaveIfError(iRFs.SetSessionPath(filePath));

	if (error != KErrNone)
		{
		// Delete the temporary file and then leave
		iRFs.Delete(tempfile);
		User::Leave(error);
		}

	// delete the old data file, rename the temp file
	CDir* dirList = NULL;		
	User::LeaveIfError(iRFs.GetDir(filePath, KEntryAttNormal, ESortByName, dirList));

    if (dirList)
		{
		TFileName localFile;
		for (TInt i=0; i< dirList->Count(); i++)
			{
			localFile = (*dirList)[i].iName;
			if (localFile.CompareF(tempfile) != 0)
				{
				// Delete if it is not the tempfile
				User::LeaveIfError(iRFs.Delete(localFile));// Delete the old file
				}
			}
		}
	delete dirList;

	// Construct the new filename
	TFileName dataFile = ContentFileNameL();
	User::LeaveIfError(iRFs.Rename(tempfile, dataFile)); // Rename the temp file
#endif // __MESSAGING_API_V2__

	// Done the changes to the TMsvEntry, now commit changes to the stream & store
	out.CommitL();
	out.Close(); // make sure we close the file
	store->CommitL();
	CleanupStack::PopAndDestroy(3); //out, store, msventry
	iHasServerEntry = ETrue;
	}


/**
Retrieves Unknown Push message from the message store into the object. 

@param	aSession	
Message server session

@param	aMsgId		
ID of the message entry to load

@see	CPushMsgEntryBase::RetrieveL()
*/
EXPORT_C void CUnknownPushMsgEntry::RetrieveL(CMsvSession& aSession, TMsvId aMsgId)
	{
	__ASSERT_ALWAYS(aMsgId!= KMsvNullIndexEntryId, 
					User::Panic(KPushPanicMoniker, EPushEntryNullMsgId));

	// Switch to our entry & then get the associated message store.
	CMsvEntry* msvEntry = aSession.GetEntryL(aMsgId);
	CleanupStack::PushL(msvEntry);
	// Set our TMsvEntry member variable
	iEntry = msvEntry->Entry();

	//Create store & load in base class data 
	CMsvStore* store = msvEntry->ReadStoreL();
	CleanupStack::PushL(store);
	RestoreL(*store);

#ifdef __MESSAGING_API_V2__
	DoRetrieveAttachmentL(store->AttachmentManagerL());
#else
	DoRetrieveAttachmentL(*msvEntry);
#endif // __MESSAGING_API_V2__

	CleanupStack::PopAndDestroy(2); //store, msvEntry
	// Obviously has a server entry
	iHasServerEntry = ETrue;
	}

#ifdef __MESSAGING_API_V2__
void CUnknownPushMsgEntry::DoRetrieveAttachmentL(MMsvAttachmentManager& aAttachManager)
#else
/**
This function performs the loading of the data from the attachment file. 
It should be noted that if there is a lot of data then the creation of a copy of the 
data will squeeze the free memory.

@param	aEntry			
Current message entry to load

@leave	KErrNotFound The attachment file cannot be located in the message store.
@leave	KErrBadName Will leave if filename is not a valid Symbian OS file name.
@leave	TDesC::AllocL()
@leave	RFile::Open()	
*/
void CUnknownPushMsgEntry::DoRetrieveAttachmentL(const CMsvEntry& aEntry)
#endif // __MESSAGING_API_V2__
	{
	TFileName fileName = ContentFileNameL();

#ifdef __MESSAGING_API_V2__
	// get the attachment file that matches the filename
	RFile file;
	for( TInt ii=0; ii<aAttachManager.AttachmentCount(); ++ii )
		{
		CMsvAttachment* attachment = aAttachManager.GetAttachmentInfoL(ii);
		CleanupStack::PushL(attachment);
		if( attachment->AttachmentName().CompareF(fileName) == 0 )
			{
			// found the attachment, get the file and break from the loop
			CleanupStack::PopAndDestroy(attachment); // dont need anymore
			file = aAttachManager.GetAttachmentFileL(ii);
			break;
			}
		// if we get here, the attachment was not found, destroying the attachment
		// will be handled by the leave
		User::Leave(KErrNotFound);
		}
#else
	// get directory for file from current entry
	TFileName filePath;
	aEntry.GetFilePath(filePath);

	// create the full file name with path
	TParse parse;
	parse.Set(fileName, &filePath, NULL);

	// create the file
	RFile file;
	User::LeaveIfError(file.Open(iRFs, parse.FullName(), EFileShareExclusive|EFileStream));
#endif // __MESSAGING_API_V2__

	CleanupClosePushL(file);

	// find the size of the file
	TInt fileSize = 0;
	User::LeaveIfError(file.Size(fileSize));
	
	// read into descriptor resizing as we go
	HBufC8* data = HBufC8::NewL(fileSize);
	TBuf8<KMaxFileName> fileChunk;
	TInt fileError = KErrNone;
	
	while(fileError ==KErrNone)
		{
		fileError = file.Read(fileChunk, fileChunk.MaxLength());
		if(fileChunk.Length()==0)
			break;
		data->Des().Append(fileChunk);
		}
		
	// if it worked assign data to our message buffer
	delete iMessageData;
	iMessageData = data;

	// Close the file and the file server session
	CleanupStack::PopAndDestroy();//file
	if(fileError!=KErrNone)
		User::Leave(fileError);
	}


/** 
Gets the Content type extension based on the content type supplied

@param	aContentType	
The content type 
*/
const TDesC& CUnknownPushMsgEntry::ContentTypeExtension(const TDesC& aContentType)
	{
	if (aContentType.CompareF(KTextBasic)==0 || aContentType.Compare(KTextPlain)==0)
		return KUnknownTextExt;
	else if (aContentType.CompareF(KUnknownRTextExt)==0 )
		return KUnknownRTextExt;
	else if (aContentType.CompareF(KTextXML) ==0 || aContentType.CompareF(KAppXML)==0) 
		return KUnknownXmlExt;
	else if (aContentType.CompareF(KTextHTML) ==0)
		return KUnknownHtmlExt;
	else if (aContentType.CompareF(KTextWML)==0)
		return KUnknownWmlExt;
	else if (aContentType.CompareF(KImageGIF)==0)
		return KUnknownGifExt;
	else if (aContentType.CompareF(KImageJPEG)==0)
		return KUnknownJpegExt;
	else if (aContentType.CompareF(KImageTIFF)==0)
		return KUnknownTiffExt;
	else if (aContentType.CompareF(KImageWBMP) ==0)
		return KUnknownWbmpExt;
	else if (aContentType.CompareF(KAppSIS)==0)
		return KUnknownSISExt;
	else if (aContentType.CompareF(KAppWMLC)==0 || aContentType.CompareF(KAppWBXML)==0)
		return KUnknownWmlcExt;
	else if (aContentType.CompareF(KTextMSWord)==0 ||
				aContentType.CompareF(KTextMSWord95)==0 ||
				aContentType.CompareF(KTextMSWord97)==0 ||
				aContentType.CompareF(KTextMSWord00)==0 ||
				aContentType.CompareF(KTextMSWord2000)==0 )
		return KUnknownDocExt;
	else
		return KStdUnknownExt;
	}


/**
Gets the value of the Uid for the Push Message type of the Push Entry.

@return	TInt32
Uid as a long integer.
*/
TInt32 CUnknownPushMsgEntry::PushMsgType() const
	{
	return KUidWapPushMsgUnknown.iUid;
	}


/**
Sets the Uid of the Push Message Entry to the value for an Unknown Push message. 
*/
void CUnknownPushMsgEntry::SetPushMsgType()
	{
	iEntry.iBioType = KUidWapPushMsgUnknown.iUid;	
	}


/** 
Externalises Push message data to a write stream.

Apart from the data stored by the base class, other data stored are Content Type, 
Time Sent and Content Filename.

@param	aStream	
The stream to which the data should be externalised.

@see	CPushMsgEntryBase::ExternalizeL()
*/
EXPORT_C void CUnknownPushMsgEntry::ExternalizeL(RMsvWriteStream& aStream)
	{	
	CPushMsgEntryBase::ExternalizeL(aStream);

	aStream<<LimitStringSize(ContentType(), KLongestStringAllowed);
	aStream << iTimeSent.Int64();
	
	aStream<<LimitStringSize(ContentFileNameL(), KLongestStringAllowed);
	}


/** 
Internalises Push message data from a read stream.

Apart from the data read by the base class, other data read are Content Type, Time Sent 
and Content Filename.

@param	aStream	
The stream from which the data should be internalised.

@leave	KErrOverflow		
If the length of the data read from the stream is greater than KLongestStringAllowed.

@leave	KErrNotSupported
An older version of the stream was read that does support Content Filename.

@leave	HBufC::NewL

@see	CPushMsgEntryBase::InternalizeL()
*/
EXPORT_C void CUnknownPushMsgEntry::InternalizeL(RMsvReadStream& aStream)
	{
	CPushMsgEntryBase::InternalizeL(aStream);

	delete iContentType;
	iContentType = NULL;
	iContentType = HBufC::NewL(aStream, KLongestStringAllowed);

	TInt64 timeSent;
	aStream >> timeSent;
	iTimeSent = timeSent;

	delete iFilename;
	iFilename = NULL;
	TRAPD(error, iFilename = HBufC::NewL(aStream, KLongestStringAllowed));
	if (error != KErrNone && error != KErrEof)
		{
		User::Leave(error);
		}
	else if(error == KErrEof)
		{
		User::Leave(KErrNotSupported);
		}
	}


#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason/* aReason*/)	
	{
	return (KErrNone);
	}
#endif
