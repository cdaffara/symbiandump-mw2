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

#ifndef __CMSVMIMEHEADERS_H__
#define __CMSVMIMEHEADERS_H__

#include <e32base.h>

class CDesC8ArrayFlat;
class CMsvAttachment;
class CDesC8Array;

/**
String literal for Base64 encoding.
@publishedAll
@released
*/
_LIT8(KMsvBase64String, "Base64");

/**
String literal for Quoted-Printable encoding.
@publishedAll
@released
*/
_LIT8(KMsvQPString, "quoted-printable");

/**
String literal for 7 bit encoding.
@publishedAll
@released
*/
_LIT8(KMsv7BitString, "7bit");

/**
String literal for 8 bit encoding.
@publishedAll
@released
*/
_LIT8(KMsv8BitString, "8bit");

/**
String literal for binary encoding.
@publishedAll
@released
*/
_LIT8(KMsvBinaryString, "binary");

/**
String literal for UU encoding.
@publishedAll
@released
*/
_LIT8(KMsvUUString, "uuencode");

/**
Utility class to represent commonly used Mime Headers. This object represents a number
of common Mime Headers into a single class. It is intended that this class is used to
represent the Mime Headers for an attachment represented by the CMsvAttachment class when
using the Attachment API. The Mime header data can be associated with an attachment and 
stored and restored using the StoreL and RestoreL methods.
@publishedAll
@released
*/
class CMsvMimeHeaders : public CBase
	{
public:

/** Messaging encoding types. 
@publishedAll
@released
*/
enum TMsvEncodingType 
	{
	/** Unknown. */
	EEncodingTypeUnknown,
	/** None. */
	EEncodingTypeNone,
	/** 7-bit. */
	EEncodingType7Bit,
	/** 8-bit. */
	EEncodingType8Bit,		// ie lines still <1000 chars and terminated by CRLF
	/** Binary. */
	EEncodingTypeBinary,	// ie any length of data, not terminated by CRLF
	/** QP. */
	EEncodingTypeQP,
	/** Base64. */
	EEncodingTypeBASE64,
	/** UU. */
	EEncodingTypeUU
	};
	
public:
	IMPORT_C static CMsvMimeHeaders* NewL();
	IMPORT_C static CMsvMimeHeaders* NewLC();
	IMPORT_C ~CMsvMimeHeaders();
	
	IMPORT_C void StoreL(CMsvAttachment& aAttachment) const;
	IMPORT_C void RestoreL(CMsvAttachment& aAttachment);
	
	IMPORT_C void SetContentDescriptionL(const TDesC8& aContentDescription);
	IMPORT_C const TDesC8& ContentDescription() const;
	
	IMPORT_C void SetContentBaseL(const TDesC8& aContentBase);
	IMPORT_C const TDesC8& ContentBase() const;
	
	IMPORT_C void SetContentLocationL(const TDesC16& aContentLocation);
	IMPORT_C const TDesC16& ContentLocation() const;
	
	IMPORT_C void SetContentIdL(const TDesC8& aContentId);
	IMPORT_C const TDesC8& ContentId() const;
	
	IMPORT_C void SetContentTypeL(const TDesC8& aContentType);
	IMPORT_C const TDesC8& ContentType() const;
	
	IMPORT_C void SetContentSubTypeL(const TDesC8& aSubContentType);
	IMPORT_C const TDesC8& ContentSubType() const;
	
	IMPORT_C void SetContentDispositionL(const TDesC8& aContentDisposition);
	IMPORT_C const TDesC8& ContentDisposition() const;
	
	IMPORT_C CDesC8Array& ContentTypeParams();
	IMPORT_C CDesC8Array& ContentDispositionParams();
	IMPORT_C CDesC8Array& XTypeParams();
	
	IMPORT_C const CDesC8Array& ContentTypeParams() const;
	IMPORT_C const CDesC8Array& ContentDispositionParams() const;
	IMPORT_C const CDesC8Array& XTypeParams() const;
	
	IMPORT_C void SetMimeCharset(TUint aMimeCharset);
	IMPORT_C TUint MimeCharset() const;
	
	IMPORT_C void SetSuggestedFilenameL(const TDesC16& aSuggestedFilename);
	IMPORT_C const TDesC16& SuggestedFilename() const;
	
	IMPORT_C void SetRelativePathL(const TDesC8& aRelativePath);
	IMPORT_C const TDesC8& RelativePath() const;
	
	IMPORT_C void SetContentTransferEncoding(const TDesC8& aEncodingType);
	IMPORT_C TMsvEncodingType ContentTransferEncoding() const;
	IMPORT_C TMsvEncodingType ContentTransferEncoding(TPtrC8& aEncodingType) const;
	IMPORT_C const TPtrC8 GetContentTypeValue(const TDesC8& aContentTypeParameter) const;

	IMPORT_C void Reset();
	IMPORT_C TInt Size() const;
	IMPORT_C TBool ContainsMimeHeaders(const CMsvAttachment& aAttachment);
	
private:
	CMsvMimeHeaders();
	void ConstructL();
	
private:
	HBufC8* iContentDescription;
	HBufC8* iContentBase;
	HBufC16* iContentLocation;
	HBufC8* iContentId;
	HBufC8* iContentType;
	HBufC8* iContentSubType;
	HBufC8* iContentDisposition;
	CDesC8ArrayFlat* iContentTypeParams;            // zero or more "parameter"
	CDesC8ArrayFlat* iContentDispositionParams;     // zero or more "parameters"
	CDesC8ArrayFlat* iXTypeParams;                  // zero or more X-Type "parameters"
	TUint iMimeCharset;
	HBufC16* iSuggestedFilename;
	HBufC8* iRelativePath;
	TMsvEncodingType iContentTransferEncoding;
	};

#endif // __CMSVMIMEHEADERS_H__
