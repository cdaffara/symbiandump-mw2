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

#include <badesca.h>
#include <s32mem.h>

#include <cmsvmimeheaders.h>
#include <cmsvattachment.h>

const TUid KUidMimeHeaders = {0x10204281};
const TInt KDesCArrayGranularity = 8;
const TInt KStreamBufferGranularity = 512;

_LIT8(KTxtWildcardBase64, "*base64*");
_LIT8(KTxtWildcardQP, "*quoted-printable*");
_LIT8(KTxtWildcard7Bit, "*7bit*");
_LIT8(KTxtWildcard8Bit, "*8bit*");
_LIT8(KTxtWildcardBinary, "*binary*");
_LIT8(KTxtWildcardUU, "*uu*");

/**
Standard 2-phase construction, creates the mime headers object.

Creates and initailises the mime headers with no data.

@return Pointer to the created mime headers object.
*/
EXPORT_C CMsvMimeHeaders* CMsvMimeHeaders::NewL()
	{
	CMsvMimeHeaders* self = NewLC();
	CleanupStack::Pop(self);
	return self;
	}

/**
Standard 2-phase construction, creates the mime headers object.

Creates and initailises the mime headers with no data. Leaves a pointer on the
cleanup stack.

@return Pointer to the created mime headers object.
*/
EXPORT_C CMsvMimeHeaders* CMsvMimeHeaders::NewLC()
	{
	CMsvMimeHeaders* self = new(ELeave) CMsvMimeHeaders();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}
	
CMsvMimeHeaders::CMsvMimeHeaders()
	{
	}

void CMsvMimeHeaders::ConstructL()
	{
	iContentTypeParams  = new(ELeave) CDesC8ArrayFlat(KDesCArrayGranularity);
	iContentDispositionParams  = new(ELeave) CDesC8ArrayFlat(KDesCArrayGranularity);	
	iXTypeParams  = new(ELeave) CDesC8ArrayFlat(KDesCArrayGranularity);
	}

/**
Standard destructor.
*/
EXPORT_C CMsvMimeHeaders::~CMsvMimeHeaders()
	{
	Reset();
	delete iContentTypeParams;
	delete iContentDispositionParams;
	delete iXTypeParams; 
	}

/**
Resets the mime headers.

Resets and removes all the mime headers and data that has been set.
*/
EXPORT_C void CMsvMimeHeaders::Reset()
	{
	delete iContentDescription;
	iContentDescription = NULL;
	delete iContentBase;
	iContentBase = NULL;
	delete iContentLocation;
	iContentLocation = NULL;
	delete iContentId;
	iContentId = NULL;
	delete iContentType;
	iContentType = NULL;
	delete iContentSubType;
	iContentSubType = NULL;
	delete iContentDisposition;
	iContentDisposition = NULL;
	iContentTypeParams->Reset();
	iContentDispositionParams->Reset();
	iXTypeParams->Reset();
	iMimeCharset = 0;
	delete iSuggestedFilename;
	iSuggestedFilename = NULL;
	delete iRelativePath;
	iRelativePath = NULL;
	iContentTransferEncoding = EEncodingTypeUnknown;
	}

/**
Returns the size of all the mime header data.

Returns the approximate size in bytes of all the mime header data
stored by this object. The returned value is a sum of the lengths
of all the string data and the size of an integer for each integer
type data.

@return The size in bytes of all the mime header data.
*/	
EXPORT_C TInt CMsvMimeHeaders::Size() const
	{
	TInt size = 0;
	size += ContentDescription().Length();
	size += ContentBase().Length();
	size += ContentLocation().Length();
	size += ContentId().Length();
	size += ContentType().Length();
	size += ContentSubType().Length();
	size += ContentDisposition().Length();
	
	TInt ii;
	for( ii=0; ii<ContentTypeParams().Count(); ++ii )
		size += ContentTypeParams()[ii].Length();

	for( ii=0; ii<ContentDispositionParams().Count(); ++ii )
		size += ContentDispositionParams()[ii].Length();

	for( ii=0; ii<XTypeParams().Count(); ++ii )
		size += XTypeParams()[ii].Length();
	
	size += sizeof(TUint); // for mime charset
	size += SuggestedFilename().Length();
	size += RelativePath().Length();
	size += sizeof(TMsvEncodingType);
	
	return size;
	}

/**
Stores the mime header data.

Associated and stores all the mime header data stored in this object 
as an attribute of an attachment object. The RestoreL method should be
used to restore the data from the attachment.

@param aAttachment The attachment object to store the mime header data to.
*/
EXPORT_C void CMsvMimeHeaders::StoreL(CMsvAttachment& aAttachment) const
	{
	CBufFlat* streamBuffer = CBufFlat::NewL(KStreamBufferGranularity);
	CleanupStack::PushL(streamBuffer);
	RBufWriteStream writeStream(*streamBuffer);
	writeStream.PushL();
	
	writeStream << ContentDescription();
	writeStream << ContentBase();
	writeStream << ContentLocation();
	writeStream << ContentId();
	writeStream << ContentType();
	writeStream << ContentSubType();
	writeStream << ContentDisposition();
	
	TInt count = ContentTypeParams().Count();
	writeStream.WriteInt32L(count);
	TInt ii;
	for( ii=0; ii<count; ++ii )
		writeStream << ContentTypeParams()[ii];

	count = ContentDispositionParams().Count();
	writeStream.WriteInt32L(count);
	for( ii=0; ii<count; ++ii )
		writeStream << ContentDispositionParams()[ii];
		
	count = XTypeParams().Count();
	writeStream.WriteInt32L(count);
	for( ii=0; ii<count; ++ii )
		writeStream << XTypeParams()[ii];
	
	writeStream.WriteUint32L(MimeCharset());
	writeStream << SuggestedFilename();
	writeStream << RelativePath();
	writeStream.WriteUint32L(iContentTransferEncoding);
	
	writeStream.CommitL();
	CleanupStack::PopAndDestroy(&writeStream);
	
	// set the headers as an attribute in the attachment
	aAttachment.SetDesC8AttributeL(KUidMimeHeaders, streamBuffer->Ptr(0));
	CleanupStack::PopAndDestroy(streamBuffer);
	}

/**
Restores the mime header data.

Restores all the mime header data from an existing attachment object. This
restores mime headers that was stored using the StoreL method. If the
attachment does not contain any mime headers, this object is reset.

@param aAttachment The attachment object to restore the mime header data from.

*/	
EXPORT_C void CMsvMimeHeaders::RestoreL(CMsvAttachment& aAttachment)
	{
	Reset();
	
	TPtrC8 bufferPtr;
	TInt err = aAttachment.GetDesC8Attribute(KUidMimeHeaders, bufferPtr);
	if( err == KErrNone )
		{
		RDesReadStream readStream(bufferPtr);
		readStream.PushL();
		
		iContentDescription = HBufC8::NewL(readStream, KMaxTInt);
		iContentBase = HBufC8::NewL(readStream, KMaxTInt);
		iContentLocation = HBufC16::NewL(readStream, KMaxTInt);
		iContentId = HBufC8::NewL(readStream, KMaxTInt);
		iContentType = HBufC8::NewL(readStream, KMaxTInt);
		iContentSubType = HBufC8::NewL(readStream, KMaxTInt);
		iContentDisposition = HBufC8::NewL(readStream, KMaxTInt);
		
		TInt count = readStream.ReadInt32L();
		TInt ii;
		HBufC8* desBuffer;
		for( ii=0; ii<count; ++ii )
			{
			desBuffer = HBufC8::NewLC(readStream, KMaxTInt);
			ContentTypeParams().AppendL(*desBuffer);
			CleanupStack::PopAndDestroy(desBuffer);
			}
		
		count = readStream.ReadInt32L();	
		for( ii=0; ii<count; ++ii )
			{
			desBuffer = HBufC8::NewLC(readStream, KMaxTInt);
			ContentDispositionParams().AppendL(*desBuffer);
			CleanupStack::PopAndDestroy(desBuffer);
			}
			
		count = readStream.ReadInt32L();	
		for( ii=0; ii<count; ++ii )
			{
			desBuffer = HBufC8::NewLC(readStream, KMaxTInt);
			XTypeParams().AppendL(*desBuffer);
			CleanupStack::PopAndDestroy(desBuffer);
			}
			
		iMimeCharset = readStream.ReadUint32L();
		iSuggestedFilename = HBufC16::NewL(readStream, KMaxTInt);
		iRelativePath = HBufC8::NewL(readStream, KMaxTInt);
		iContentTransferEncoding = static_cast<TMsvEncodingType>(readStream.ReadUint32L());	
			
		readStream.Close();
		CleanupStack::PopAndDestroy(&readStream);
		}
	// else do nothing if not found
	}

/**
Sets the Content-Description mime header.

Over-writes any existing data.

@param aContentDescription Descriptor conatining the Content-Description mime header.
*/	
EXPORT_C void CMsvMimeHeaders::SetContentDescriptionL(const TDesC8& aContentDescription)
	{
	delete iContentDescription;
	iContentDescription = aContentDescription.AllocL();
	}
	
/**
Gets the Content-Description mime header.

Returns a zero length descriptor if mime header not set.

@return Descriptor containing the Content-Description mime header. Zero length
		if the mime header not set.
*/
EXPORT_C const TDesC8& CMsvMimeHeaders::ContentDescription() const
	{
	if( iContentDescription==NULL )
		return KNullDesC8();
	
	return *iContentDescription;
	}

/**
Sets the Content-Base mime header.

Over-writes any existing data.

@param aContentBase Descriptor conatining the Content-Base mime header.
*/	
EXPORT_C void CMsvMimeHeaders::SetContentBaseL(const TDesC8& aContentBase)
	{
	delete iContentBase;
	iContentBase = aContentBase.AllocL();
	}

/**
Gets the Content-Base mime header.

Returns a zero length descriptor if mime header not set.

@return Descriptor containing the Content-Base mime header. Zero length
		if the mime header not set.
*/	
EXPORT_C const TDesC8& CMsvMimeHeaders::ContentBase() const
	{
	if( iContentBase==NULL)
		return KNullDesC8();
	
	return *iContentBase;
	}

/**
Sets the Content-Location mime header.

Over-writes any existing data.

@param aContentLocation Descriptor conatining the Content-Base mime header.
*/
EXPORT_C void CMsvMimeHeaders::SetContentLocationL(const TDesC16& aContentLocation)
	{
	delete iContentLocation;
	iContentLocation = aContentLocation.AllocL();
	}

/**
Gets the Content-Location mime header.

Returns a zero length descriptor if mime header not set.

@return Descriptor containing the Content-Location mime header. Zero length
		if the mime header not set.
*/	
EXPORT_C const TDesC16& CMsvMimeHeaders::ContentLocation() const
	{
	if( iContentLocation==NULL )
		return KNullDesC();
	
	return *iContentLocation;
	}

/**
Sets the Content-Id mime header.

Over-writes any existing data.

@param aContentId Descriptor conatining the Content-Id mime header.
*/
EXPORT_C void CMsvMimeHeaders::SetContentIdL(const TDesC8& aContentId)
	{
	delete iContentId;
	iContentId = aContentId.AllocL();
	}

/**
Gets the Content-Id mime header.

Returns a zero length descriptor if mime header not set.

@return Descriptor containing the Content-Id mime header. Zero length
		if the mime header not set.
*/		
EXPORT_C const TDesC8& CMsvMimeHeaders::ContentId() const
	{
	if( iContentId==NULL )
		return KNullDesC8();
	
	return *iContentId;
	}

/**
Sets the Content-Type mime header.

Over-writes any existing data. This mime header contains the beginning part of the
mime-type, eg. the 'text' part of 'text/plain'.

@param aContentType Descriptor conatining the Content-Type mime header.
*/
EXPORT_C void CMsvMimeHeaders::SetContentTypeL(const TDesC8& aContentType)
	{
	delete iContentType;
	iContentType = aContentType.AllocL();
	}

/**
Gets the Content-Type mime header.

Returns a zero length descriptor if mime header not set. This mime header contains
the beginning part of the mime-type, eg. the 'text' part of 'text/plain'.

@return Descriptor containing the Content-Type mime header. Zero length
		if the mime header not set.
*/	
EXPORT_C const TDesC8& CMsvMimeHeaders::ContentType() const
	{
	if( iContentType==NULL )
		return KNullDesC8();
	
	return *iContentType;
	}

/**
Sets the Content-SubType mime header.

Over-writes any existing data. This mime header contains the end part of the
mime-type, eg. the 'plain' part of 'text/plain'.

@param aContentSubType Descriptor conatining the Content-SubType mime header.
*/	
EXPORT_C void CMsvMimeHeaders::SetContentSubTypeL(const TDesC8& aContentSubType)
	{
	delete iContentSubType;
	iContentSubType = aContentSubType.AllocL();
	}

/**
Gets the Content-SubType mime header.

Returns a zero length descriptor if mime header not set. This mime header contains
the end part of the mime-type, eg. the 'plain' part of 'text/plain'.

@return Descriptor containing the Content-SubType mime header. Zero length
		if the mime header not set.
*/	
EXPORT_C const TDesC8& CMsvMimeHeaders::ContentSubType() const
	{
	if( iContentSubType==NULL )
		return KNullDesC8();
	
	return *iContentSubType;
	}

/**
Sets the Content-Disposition mime header.

Over-writes any existing data.

@param aContentId Descriptor conatining the Content-Disposition mime header.
*/
EXPORT_C void CMsvMimeHeaders::SetContentDispositionL(const TDesC8& aContentDisposition)
	{
	delete iContentDisposition;
	iContentDisposition = aContentDisposition.AllocL();
	}

/**
Gets the Content-Disposition mime header.

Returns a zero length descriptor if mime header not set.

@return Descriptor containing the Content-Disposition mime header. Zero length
		if the mime header not set.
*/	
EXPORT_C const TDesC8& CMsvMimeHeaders::ContentDisposition() const
	{
	if( iContentDisposition==NULL )
		return KNullDesC8();
	
	return *iContentDisposition;
	}

/**
Returns a modifiable descriptor array containing the Content-Type parameters.

@return A modifiable descriptor array.
*/
EXPORT_C CDesC8Array& CMsvMimeHeaders::ContentTypeParams()
	{
	return *iContentTypeParams;
	}

/**
Returns a modifiable descriptor array containing the Content-Disposition parameters.

@return A modifiable descriptor array.
*/		
EXPORT_C CDesC8Array& CMsvMimeHeaders::ContentDispositionParams()
	{
	return *iContentDispositionParams;
	}

/**
Returns a modifiable descriptor array containing the X-Type parameters.

@return A modifiable descriptor array.
*/			
EXPORT_C const CDesC8Array& CMsvMimeHeaders::XTypeParams() const
	{
	return *iXTypeParams;
	}
	
/**
Returns a non-modifiable descriptor array containing the Content-Type parameters.

@return A non-modifiable descriptor array.
*/
EXPORT_C const CDesC8Array& CMsvMimeHeaders::ContentTypeParams() const
	{
	return *iContentTypeParams;
	}
		
/**
Returns a non-modifiable descriptor array containing the Content-Disposition parameters.

@return A non-modifiable descriptor array.
*/
EXPORT_C const CDesC8Array& CMsvMimeHeaders::ContentDispositionParams() const
	{
	return *iContentDispositionParams;
	}
	
/**
Returns a non-modifiable descriptor array containing the X-Type parameters.

@return A non-modifiable descriptor array.
*/
EXPORT_C CDesC8Array& CMsvMimeHeaders::XTypeParams()
	{
	return *iXTypeParams;
	}
	
/**
Sets the mime charset header.

Over-writes any existing data.

@param Integer representing the mime charset.
*/
EXPORT_C void CMsvMimeHeaders::SetMimeCharset(TUint aMimeCharset)
	{
	iMimeCharset = aMimeCharset;
	}
	
/**
Gets the mime charset header.

@return An integer representing the mime charset.
*/
EXPORT_C TUint CMsvMimeHeaders::MimeCharset() const
	{
	return iMimeCharset;
	}

/**
Sets the suggested filename mime header.

Over-writes any existing data.

@param aSuggestedFilename Descriptor containing the suggested filename mime header data.
*/
EXPORT_C void CMsvMimeHeaders::SetSuggestedFilenameL(const TDesC16& aSuggestedFilename)
	{
	delete iSuggestedFilename;
	iSuggestedFilename = aSuggestedFilename.AllocL();
	}

/**
Gets the suggested filename mime header.

Returns a zero length descriptor if header is not set.

@return Descriptor containing the suggested filename mime header. Zero length
		if the header not set.
*/
EXPORT_C const TDesC16& CMsvMimeHeaders::SuggestedFilename() const
	{
	if( iSuggestedFilename==NULL )
		return KNullDesC();
	
	return *iSuggestedFilename;
	}

/**
Sets the relative path header.

Over-writes any existing data.

@param aSuggestedFilename Descriptor containing the suggested filename header data.
*/
EXPORT_C void CMsvMimeHeaders::SetRelativePathL(const TDesC8& aRelativePath)
	{
	delete iRelativePath;
	iRelativePath = aRelativePath.AllocL();
	}
	
/**
Gets the relative path mime header.

Returns a zero length descriptor if header is not set.

@return Descriptor containing the relative path mime header. Zero length
		if the header not set.
*/
EXPORT_C const TDesC8& CMsvMimeHeaders::RelativePath() const
	{
	if( iRelativePath==NULL )
		return KNullDesC8();
	
	return *iRelativePath;
	}
	
/**
Sets the Content-Transfer Encoding mime header.

Over-writes any existing data.

@param aEncodingType Descriptor conatining the Content-Transfer Encoding mime header.
*/
EXPORT_C void CMsvMimeHeaders::SetContentTransferEncoding(const TDesC8& aEncodingType)
	{
	iContentTransferEncoding = EEncodingTypeUnknown;

	if (KErrNone==aEncodingType.MatchF(KTxtWildcardBase64))
		iContentTransferEncoding = EEncodingTypeBASE64;
	else if (KErrNone==aEncodingType.MatchF(KTxtWildcardQP))
		iContentTransferEncoding = EEncodingTypeQP;
	else if (KErrNone==aEncodingType.MatchF(KTxtWildcard7Bit))
		iContentTransferEncoding = EEncodingType7Bit;
	else if (KErrNone==aEncodingType.MatchF(KTxtWildcard8Bit))
		iContentTransferEncoding = EEncodingType8Bit;
	else if (KErrNone==aEncodingType.MatchF(KTxtWildcardBinary))
		iContentTransferEncoding = EEncodingTypeBinary;
	else if (KErrNone==aEncodingType.MatchF(KTxtWildcardUU))	// can't be right - there's no MIME standard text to describe UU is there??
		iContentTransferEncoding = EEncodingTypeUU;
	}

/**
Gets the Content-Transfer Encoding mime header.

@return An enumeration of the Content-Transfer Encoding header. Returns EEncodingTypeUnknown if not set.
@see TMsvEncodingType
*/	
EXPORT_C CMsvMimeHeaders::TMsvEncodingType CMsvMimeHeaders::ContentTransferEncoding() const
	{
	return iContentTransferEncoding;
	}

/**
Gets the Content-Transfer Encoding mime header.

Also provides a descriptor that contains a textual representation of the content-transfer
encoding header.

@param aEncodingType This desriptor is set to a textual representation of the mime header.
					 Returns a zero-length 
@return An enumeration of the Content-Transfer Encoding header. Returns EEncodingTypeUnknown if not set.
@see TMsvEncodingType
*/		
EXPORT_C CMsvMimeHeaders::TMsvEncodingType CMsvMimeHeaders::ContentTransferEncoding(TPtrC8& aEncodingType) const
	{
	switch (iContentTransferEncoding)
		{
	case EEncodingTypeBASE64:
		aEncodingType.Set(KMsvBase64String);
		break;
	case EEncodingTypeQP:
		aEncodingType.Set(KMsvQPString);
		break;
	case EEncodingType7Bit:
		aEncodingType.Set(KMsv7BitString);
		break;
	case EEncodingType8Bit:
		aEncodingType.Set(KMsv8BitString);
		break;
	case EEncodingTypeBinary:
		aEncodingType.Set(KMsvBinaryString);
		break;
	case EEncodingTypeUU:
		aEncodingType.Set(KMsvUUString);
		break;
	case EEncodingTypeNone:
	case EEncodingTypeUnknown:
	default:
		aEncodingType.Set(KNullDesC8());
		break;
		}
	return iContentTransferEncoding;
	}

/**
Gets the Content-Type mime header parameter value.

This method finds the specifed parameter and extracts the value for it.

@param aContentTypeParameter The Content-Type parameter to get the value for.
@return The Content-Type parameter value requested. Returns a zero-length if
		the paramter cannot be found.
*/
EXPORT_C const TPtrC8 CMsvMimeHeaders::GetContentTypeValue(const TDesC8& aContentTypeParameter) const
	{
	__ASSERT_DEBUG (!(iContentTypeParams->Count()&1), User::Invariant()); // Should not have odd number of elements

	TInt result;
	if (KErrNone==iContentTypeParams->Find(aContentTypeParameter,result,ECmpFolded8))
		{
		++result;
		if ((result&1) && result <= iContentTypeParams->Count())	// check result+1 is odd & entry exists
			{	
			return ContentTypeParams()[result];
			}
		}
	return KNullDesC8();	// couldn't find match so return an empty descriptor
	}

/**
Enquire whether a CMsvAttachment has MIME headers without restoring all the headers.

@param aAttachment The attachment.
@return True if there is an MIME headers 
*/
EXPORT_C TBool CMsvMimeHeaders::ContainsMimeHeaders(const CMsvAttachment& aAttachment)
    {
    TPtrC8 bufferPtr;
    return (aAttachment.GetDesC8Attribute(KUidMimeHeaders, bufferPtr) == KErrNone);    
    }

