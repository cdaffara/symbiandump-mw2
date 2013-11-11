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
//

#include <cmsvattachment.h>
#include "CMsvAttributeManager.h"

const TInt KMsvAttachmentVersion = 2; // version number used for streaming

/**
Standard 2-phase construction, creates the attachment object.

The object should then be initialised with information about the attachment.

@param aType The attachment type.
@return Pointer to the created attachment object.
*/
EXPORT_C CMsvAttachment* CMsvAttachment::NewL(TMsvAttachmentType aType)
	{
	CMsvAttachment* self = new(ELeave) CMsvAttachment(aType);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
Standard 2-phase construction, creates the attachment object. 

The object should then be initialised with information about the attachment. This allows
the caller to pass in commonly used attributes to initialise them on construction to
save the caller from having to call the relavant Set... methods. The attributes that can
be initialised are Mime-Type, attachment name and size. Callers should pass in NULL for
any attributes that are not required to be set.

@param aType The attachment type.
@param aSize The size of the attachment. For file attachments this should be the file
	   		 size and for message entry attachments this should be the message size.
@param aMimeType The Mime-Type of the attachment.
@param aAttachmentName The attachment name to identify the attachment.
@return Pointer to the created attachment object.
*/
EXPORT_C CMsvAttachment* CMsvAttachment::NewL(TMsvAttachmentType aType, TInt aSize, HBufC8* aMimeType, HBufC* aAttachmentName)
	{
	CMsvAttachment* self = new(ELeave) CMsvAttachment(aType, aSize, aMimeType, aAttachmentName);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CMsvAttachment* CMsvAttachment::NewL(const CMsvAttachment& aAttachment)
	{
	CMsvAttachment* self = new(ELeave) CMsvAttachment(aAttachment.iType);
	CleanupStack::PushL(self);
	self->ConstructL(aAttachment);
	CleanupStack::Pop(self);
	return self;
	}
	
CMsvAttachment::CMsvAttachment(TMsvAttachmentType aType)
	: iType(aType)
	{
	}	

CMsvAttachment::CMsvAttachment(TMsvAttachmentType aType, TInt aSize, HBufC8* aMimeType, HBufC* aAttachmentName)
	: iType(aType), iSize(aSize), iMimeType(aMimeType), iAttachmentName(aAttachmentName)
	{
	}

void CMsvAttachment::ConstructL()
	{
	iAttributeManager = CMsvAttributeManager::NewL(*this);
	}

void CMsvAttachment::ConstructL(const CMsvAttachment& aAttachment)
	{
	ConstructL();
	
	// Creates a deep copy of the attachment object passed in
	iId = aAttachment.iId;
	iSize = aAttachment.iSize;
	if( aAttachment.iMimeType != NULL )
		iMimeType = aAttachment.iMimeType->AllocL();
	if( aAttachment.iFilePath != NULL )
		iFilePath = aAttachment.iFilePath->AllocL();
	iEntryId = aAttachment.iEntryId;
	if( aAttachment.iAttachmentName != NULL )
		iAttachmentName = aAttachment.iAttachmentName->AllocL();
	iComplete = aAttachment.iComplete;
	
	// Create a copy of the attribute manager
	iAttributeManager->CloneL(*aAttachment.iAttributeManager);
	}

/**
Standard destructor.
*/
EXPORT_C CMsvAttachment::~CMsvAttachment()
	{
	delete iAttributeManager;
	delete iMimeType;
	delete iFilePath;
	delete iAttachmentName;
	}

/**
Returns the attachment Id.

The attachment Id is unique on a per message entry basis.

@return The attachment Id.
*/
EXPORT_C TMsvAttachmentId CMsvAttachment::Id() const
	{
	return iId;
	}
	
/**
Sets the attachment Id.

The attachment Id is automatically set when passed to the Attachment Manager. This
method allows the Id to be over-ridden or set by different implementations of
attachment managers.
@param aId The attachment Id to set.
*/
EXPORT_C void CMsvAttachment::SetId(TMsvAttachmentId aId)
	{
	iId = aId;
	}

/**
Returns the attachment type.

The attachment types can be a file, a linked file or a message entry attachment.

@return The attachment type.
@see TMsvAttachmentType
*/
EXPORT_C CMsvAttachment::TMsvAttachmentType CMsvAttachment::Type() const
	{
	return iType;
	}

/**
Returns the message entry Id for message entry attachment. This is only valid if the attachment
type is EMessageEntry.
@return The message entry Id for the entry attachment.
*/
EXPORT_C TMsvId CMsvAttachment::EntryAttachmentId() const
	{
	__ASSERT_DEBUG(iType==EMsvMessageEntry, User::Invariant());
	
	return iEntryId;
	}
	
void CMsvAttachment::SetEntryAttachmentId(TMsvId aEntryId)
	{
	iEntryId = aEntryId;
	}

/**
Returns the attachment name.

The attachment name is a readable text attribute to identify the attachment.

@return Descriptor containing attachment name. Zero-length descriptor if attachment 
		name is not set. 
*/	
EXPORT_C const TDesC& CMsvAttachment::AttachmentName() const
	{
	if( iAttachmentName==NULL )
		return KNullDesC();
	
	return *iAttachmentName;
	}
	
/*
Sets the attachment name.

The attachment name is a readable text attribute to identify the attachment.

For example, this might be set as the filename for file based attachemnts.
@param aAttachmentName Descriptor containing the attachment name.
*/
EXPORT_C void CMsvAttachment::SetAttachmentNameL(const TDesC& aAttachmentName)
	{
	delete iAttachmentName;
	iAttachmentName = NULL;
	iAttachmentName = aAttachmentName.AllocL();
	}

/**
Returns the full path specification for file attachments.

This is only valid for file attachments where the attachment type is EMsvFile
or EMsvLinkedFile.

@return Descriptor containing full file path of the file attachment.
*/	
EXPORT_C const TDesC& CMsvAttachment::FilePath() const
	{
	__ASSERT_ALWAYS(iType!=EMsvMessageEntry, User::Invariant());
	
	if (iFilePath!=NULL)
		return *iFilePath;
	else
		return KNullDesC;
	}
	
void CMsvAttachment::SetFilePathL(const TDesC& aFilePath)
	{
	__ASSERT_DEBUG(iType!=EMsvMessageEntry, User::Invariant());
	
	HBufC* newFilePath = aFilePath.AllocL();
	delete iFilePath;
	iFilePath = NULL;
	iFilePath = newFilePath;
	iNeedsPath = EFalse;
	}
	
/**
Size of the attachment.

Return the size of the attachment in bytes.

@return The size of the attachment.
*/
EXPORT_C TInt CMsvAttachment::Size() const
	{
	return iSize;
	}

/**
Sets the attachment size.

When initialising or updating the attachment, this method should be used to set
the size in bytes.

@param aSize The size of the attachment in bytes.
*/
EXPORT_C void CMsvAttachment::SetSize(TInt aSize)
	{
	iSize = aSize;
	}

/**
The mime-type of the attachment.

@return Descriptor containing mime-type. Zero-length descriptor if mime-type is not set.
*/
EXPORT_C const TDesC8& CMsvAttachment::MimeType() const
	{	
	if(iMimeType==NULL)
		return KNullDesC8();
	
	return *iMimeType;
	}

/**
Sets the mime-type of the attachment.

@param aMimeType Descriptor containing the mime-type.	
*/
EXPORT_C void CMsvAttachment::SetMimeTypeL(const TDesC8& aMimeType)
	{
	delete iMimeType;
	iMimeType = NULL;
	iMimeType = aMimeType.AllocL();
	}

void CMsvAttachment::ExternalizeL(RWriteStream& aStream) const
	{
	// Stream out attachment....
	
	// 1. Start with version - TInt
	aStream.WriteInt32L(KMsvAttachmentVersion);
	
	// 2. attachment id - TUint
	aStream.WriteUint32L(iId);
	
	// 3. attachment type - TInt (enum)
	aStream.WriteInt32L(iType);
	
	// 4. attachment size - TInt
	aStream.WriteInt32L(iSize);
	
	// 5. mime-type - HBufC8
	aStream << MimeType();
			
	// 6. message entry id - TInt (TMsvId)
	aStream.WriteInt32L(iEntryId);
	
	// 7. file path - HBufC (if not message entry)
	if(	iType != EMsvMessageEntry )
		{
		// Only stream out the filename for file attachments
		if( iType == EMsvFile )
			{
			TParsePtrC filePath(FilePath());
			aStream << filePath.NameAndExt();
			}
		else
			{
			// must be linked file, stream whole path
			aStream << FilePath();
			}
		}
	
	// 8. attachment name - HBufC
	aStream << AttachmentName();
	
	// 9. complete flag - TInt (TBool)
	aStream.WriteInt32L(iComplete);
	
	// 10. the attributes (CMsvAttributeManager)
	aStream << *iAttributeManager;
	}

void CMsvAttachment::InternalizeL(RReadStream& aStream)
	{
	// Stream in attachment info and populate data members...
	// 1. Start with version - TInt
	TInt version = aStream.ReadUint32L();
	if( version>KMsvAttachmentVersion )
		{
		User::Leave(KErrNotSupported);
		}
	else
		{
		// 2. attachment id - TUint
		iId = aStream.ReadUint32L();
		
		// 3. attachment type - TInt (enum)
		iType = static_cast<TMsvAttachmentType>(aStream.ReadInt32L());
		
		// 4. attachment size - TInt
		iSize = aStream.ReadInt32L();
		
		// 5. mime-type - HBufC8
		iMimeType = HBufC8::NewL(aStream, KMaxTInt);
				
		// 6. message entry id - TInt (TMsvId)
		iEntryId = aStream.ReadInt32L();
		
		// Versioning is required for file path, v1 simply stores the
		// whole file path but v2 only stores the file name for EMsvFile
		// types and requires that the attachment manager sets the path
		// dynamically.		
		if(	iType != EMsvMessageEntry )
			{
			// 7. file path - HBufC (if not message entry)
			iFilePath = HBufC::NewL(aStream, KMaxTInt);
			if( version==KMsvAttachmentVersion && iType==EMsvFile )
				{
				// Flag that the path still needs to be dynamically set
				iNeedsPath = ETrue;
				}
			}
		
		// 8. attachment name - HBufC
		iAttachmentName = HBufC::NewL(aStream, KMaxTInt);
		
		// 9. complete flag - TInt (TBool)
		iComplete = aStream.ReadInt32L();
		
		// 10. the attributes (CMsvAttributeManager)
		aStream >> *iAttributeManager;
		}
	}

/**
Indicates whether attachment is complete or not.

This allows support for incomplete or pending attachments.

@return ETrue if the attachment is complete, EFalse otherwise.
*/
EXPORT_C TBool CMsvAttachment::Complete() const
	{
	return iComplete;
	}

/**
Sets whether the attachment is complete or not.

This allows support for incomplete or pending attachments.

@param aComplete ETrue if the attachment is complete, EFalse otherwise.
*/
EXPORT_C void CMsvAttachment::SetComplete(TBool aComplete)
	{
	iComplete = aComplete;
	}

/**
Sets an 8-bit descriptor attribute for the attachment.

A UID identifier uniquely identifies the attribute. If an attibute already exists with the same
identifier, the old attribute is over-written.

This can be used to store an attribute as an 8-bit descriptor or binary data.
@param aAttributeId The unique identifier for the attribute.
@param aAttribute The attribute data to store. The descriptor is copied internally.
*/
EXPORT_C void CMsvAttachment::SetDesC8AttributeL(TUid aAttributeId, const TDesC8& aAttribute)
	{
	iAttributeManager->SetDesC8AttributeL(aAttributeId, aAttribute);
	}

/**
Gets an 8-bit descriptor attribute.

Gets the attribute uniquely identified by the UID identifer set using the SetDesC8AttributeL
method.
@param aAttributeId The unique identifier for the attribute.
@param aAttribute If attribute is found, this will be set to point to the attribute data.
@return KErrNone is successful, KErrNotFound if the attribute cannot be found.
*/	
EXPORT_C TInt CMsvAttachment::GetDesC8Attribute(TUid aAttributeId, TPtrC8& aAttribute) const
	{
	return iAttributeManager->GetDesC8Attribute(aAttributeId, aAttribute);
	}

/**
Removes an 8-bit descriptor attribute.

Removes the attribute identified by its UID identifer. This methods has no effect if the
attribute does not exist.
@param aAttributeId The unique identifier for the attribute.
*/
EXPORT_C void CMsvAttachment::RemoveDesC8Attribute(TUid aAttributeId)
	{
	iAttributeManager->RemoveDesC8Attribute(aAttributeId);
	}

/**
Sets an integer attribute for the attachment.

A UID identifier uniquely identifies the attribute. If an attibute already exists with the same
identifier, the old attribute is over-written.
@param aAttributeId The unique identifier for the attribute.
@param aAttribute The attribute data to store.
*/
EXPORT_C void CMsvAttachment::SetIntAttributeL(TUid aAttributeId, TInt aAttribute)
	{
	iAttributeManager->SetIntAttributeL(aAttributeId, aAttribute);
	}

/**
Gets an integer attribute.

Gets the attribute uniquely identified by the UID identifer set using the SetIntAttributeL method.
@param aAttributeId The unique identifier for the attribute.
@param aAttribute If attribute is found, this will be set to the attribute data.
@return KErrNone is successful, KErrNotFound if the attribute cannot be found.
*/	
EXPORT_C TInt CMsvAttachment::GetIntAttribute(TUid aAttributeId, TInt& aAttribute) const
	{
	return iAttributeManager->GetIntAttribute(aAttributeId, aAttribute);
	}

/**
Removes an integer attribute.

Removes the attribute identified by its UID identifer. This methods has no effect if the
attribute does not exist.
@param aAttributeId The unique identifier for the attribute.
*/	
EXPORT_C void CMsvAttachment::RemoveIntAttribute(TUid aAttributeId)
	{
	iAttributeManager->RemoveIntAttribute(aAttributeId);
	}
	
TBool CMsvAttachment::IsPathRequired() const
	{
	return iNeedsPath;
	}

