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
// CMsvAttachment.h
//
/**
 * @file 
 * @publishedAll
 * @released
 */

#ifndef __CMSVATTACHMENT_H__
#define __CMSVATTACHMENT_H__

#include <e32base.h>
#include <msvstd.h>

/**
Attachment Id for uniquely identifying attachments. 

This is unique on a per message entry basis.

@publishedAll
@released
*/
typedef TUint TMsvAttachmentId;

class CMsvAttributeManager;

/**
Represents a single attachment and information about the attachment.

This object is used to represent all types of attachments (file, linked file or message 
entry). Note that some APIs may not be valid for particular attachment types. This is
described under each method. It is the responsibility of the client to ensure that the
data about the attachment is set such as its mime-type or size.

@publishedAll
@released
*/
class CMsvAttachment : public CBase
	{
public:
	/**
	Enum to define the different types of attachments supported by messaging.
	*/
	enum TMsvAttachmentType
		{
		/**
		File attachment. This is a file attachment that is copied into the message store.
		*/
		EMsvFile = 0,
		/**
		Linked file attachment. This is an attachment that is linked to using its file path,
		this is not copied into the message store.
		*/
		EMsvLinkedFile,
		/**
		Message entry attachment. This is a message entry that is registered as an attachment.
		*/
		EMsvMessageEntry
		};
	
public:
	IMPORT_C static CMsvAttachment* NewL(TMsvAttachmentType aType);
	IMPORT_C static CMsvAttachment* NewL(TMsvAttachmentType aType, TInt aSize, HBufC8* aMimeType, HBufC* aAttachmentName);
	
	IMPORT_C static CMsvAttachment* NewL(const CMsvAttachment& aAttachment);
	
	IMPORT_C ~CMsvAttachment();
	
	IMPORT_C TMsvAttachmentId Id() const;
	IMPORT_C void SetId(TMsvAttachmentId aId);
	
	IMPORT_C TMsvAttachmentType Type() const;
	
	IMPORT_C TMsvId EntryAttachmentId() const;
	void SetEntryAttachmentId(TMsvId aEntryId);
	
	IMPORT_C const TDesC& AttachmentName() const;
	IMPORT_C void SetAttachmentNameL(const TDesC& aAttachmentName);
	
	IMPORT_C const TDesC& FilePath() const;
	void SetFilePathL(const TDesC& aFilePath);
	
	IMPORT_C TInt Size() const;
	IMPORT_C void SetSize(TInt aSize);
	
	IMPORT_C const TDesC8& MimeType() const;
	IMPORT_C void SetMimeTypeL(const TDesC8& aMimeType);
		
	IMPORT_C TBool Complete() const;
	IMPORT_C void SetComplete(TBool aComplete);
	
	IMPORT_C void SetDesC8AttributeL(TUid aAttributeId, const TDesC8& aAttribute);
	IMPORT_C TInt GetDesC8Attribute(TUid aAttributeId, TPtrC8& aAttribute) const;
	IMPORT_C void RemoveDesC8Attribute(TUid aAttributeId);
	
	IMPORT_C void SetIntAttributeL(TUid aAttributeId, TInt aAttribute);
	IMPORT_C TInt GetIntAttribute(TUid aAttributeId, TInt& aAttribute) const;
	IMPORT_C void RemoveIntAttribute(TUid aAttributeId);
	
	void InternalizeL(RReadStream& aStream);
	void ExternalizeL(RWriteStream& aStream) const;
	
	TBool IsPathRequired() const;
	
private:
	CMsvAttachment(TMsvAttachmentType aType);
	CMsvAttachment(TMsvAttachmentType aType, TInt aSize, HBufC8* aMimeType, HBufC* aAttachmentName);
	void ConstructL();
	void ConstructL(const CMsvAttachment& aAttachment);
	
private:
	/** The unique attachment Id. This is unique per message entry and is auto-assigned. */
	TMsvAttachmentId iId;
	
	/** The attachment type. */
	TMsvAttachmentType iType;
	
	/** Size of the attachment in bytes. */
	TInt iSize;
	
	/** Descriptor indicating the mime type of the attachment. */
	HBufC8* iMimeType;
			
	/** The full file path and file name of attachment files and linked files. */
	HBufC* iFilePath;
		
	/** The message entry Id of the entry attachment. */
	TMsvId iEntryId;
	
	/** Name of the attachment to identify to the attachment. */
	HBufC* iAttachmentName;
	
	/** Attachment entry completeness flag. */
	TBool iComplete;
	
	/** Attribute manager for handling the attributes for this attachment. */
	CMsvAttributeManager* iAttributeManager;
	
	/** Flag to indicate whether a path needs to be added. */
	TBool iNeedsPath;
	};

#endif // __CMSVATTACHMENT_H__
