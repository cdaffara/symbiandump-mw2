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

/**
 @file
 @publishedPartner
 @released
*/

#ifndef __CUNKNOWNPUSHMSGENTRY_H__
#define __CUNKNOWNPUSHMSGENTRY_H__

#include <pushentry.h>

// Forward declarations
class RResourceFile;

#ifdef __MESSAGING_API_V2__
class MMsvAttachmentManager;
#endif // __MESSAGING_API_V2__

/** 
Handles storage of WAP Push Messages of unknown types in the message server.

If the framework receives a Push message but there are no Content Handlers for the specified
media type and are not BIO messages, then this class is used to store these Unknown 
Wap Push messages.

Apart from the data stored by the base class, the following data are stored:

1. Content Type of the message.

2. Message Body - this is the binary data contained in the message. No data conversion is performed. This
can be a very large amount of data.

3. Time that the message was sent

4. Content Filename of the message. Content Filename is the name of the file where the 
Push message content is stored.

@see	CPushMsgEntryBase

@publishedPartner
@released
*/
class CUnknownPushMsgEntry:public CPushMsgEntryBase
	{
public:
	IMPORT_C static	CUnknownPushMsgEntry* NewL();
	IMPORT_C static CUnknownPushMsgEntry* NewL(const TPtrC8& aAppURI);
	IMPORT_C static CUnknownPushMsgEntry* NewL(TInt& aAppID);
	IMPORT_C ~CUnknownPushMsgEntry();

	IMPORT_C TMsvId SaveL(CMsvSession& aSession, TMsvId aParent);
	IMPORT_C void UpdateL(CMsvSession& aSession);
	IMPORT_C void RetrieveL(CMsvSession& aSession, TMsvId aMsgId);

	inline const TDesC&	ContentType() const;
	IMPORT_C void SetContentTypeL(const TDesC& aContentType);
	IMPORT_C const TDesC8& MessageData() const;
	IMPORT_C void SetMessageDataL(const TDesC8& aMessageData);	
	IMPORT_C const TDesC& ContentFileNameL();	
	IMPORT_C void SetContentFileNameL(const TDesC& aFilename);

	inline void	SetTimeSent(const TTime& aTime);
	inline const TTime& TimeSent( ) const;
	TInt32 PushMsgType() const;
private:
	void ConstructL();
	void ConstructL(const TPtrC8& aAppURI);
	void ConstructL(TInt& aAppID);
	
	CUnknownPushMsgEntry();
	void SetPushMsgType();

#ifdef __MESSAGING_API_V2__
	void SaveMessageBodyL(MMsvAttachmentManager& aAttachManager, const TDesC& aFileName);
#else
	void SaveMessageBodyL(const CMsvEntry& aEntry, TFileName& aFileName);
#endif // __MESSAGING_API_V2__

	const TDesC& ContentTypeExtension(const TDesC& aContentType);

#ifdef __MESSAGING_API_V2__
	void DoRetrieveAttachmentL(MMsvAttachmentManager& aAttachManager);
#else
	void DoRetrieveAttachmentL(const CMsvEntry& aEntry);
#endif // __MESSAGING_API_V2__

	void OpenResourceFileL(RResourceFile& aFile);
	void SetDefaultBaseFileNameL();

	IMPORT_C void ExternalizeL(RMsvWriteStream& aStream);
	IMPORT_C void InternalizeL(RMsvReadStream& aStream);

private:
	/** Descriptor buffer contains the data of the message */
	HBufC8* iMessageData;
	/** A File Session */
	RFs     iRFs;

	/** Time that the message was sent */
	TTime iTimeSent;
	/** Descriptor buffer for the content type of the message */
	HBufC* iContentType;
	/** Descriptor buffer for the content filename of the message */
	HBufC* iFilename;
	/** Default base filename read from the wappushunknown resource file */
	TFileName iDefaultBaseFilename;
	};

#include <push/cunknownpushmsgentry.inl>

#endif // __CUNKNOWNPUSHMSGENTRY_H__
