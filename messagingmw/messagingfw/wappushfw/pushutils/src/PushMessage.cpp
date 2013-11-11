// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
    
// System Include
#include <push/pushmessage.h>
#include "WapDecoder.h"

// User Include
#include "chttpresponse.h"

/**
Allocates and constructs a new WAP Push message object.

CPushMessage takes ownership of aHeader and aBody. aHeader and aBody must be put on
the cleanup stack before calling CPushMessage::NewL(), and the arguments must be poped
off the cleanupstack after CPushMessage has been constructed. For example:
@code
 	CleanupStack::PushL(header);
 	CleanupStack::PushL(body);
 	CPushMessagemessage = CPushMessage::NewL(header, body);
 	CleanupStack::Pop(2);	//header, body
	CleanupStack::PushL(message);
 	message->FunctionL();
@endcode

If the arguments are data members, than they must be set to null after CPushMessage 
has been constructed. For example:
@code
 	iMessage = CPushMessage::NewL(iHeader, iBody);
 	iHeader = NULL;
 	iBody = NULL;
@endcode

CPushMessage::NewLC can not be implemented as the arguments must be poped of the cleanupstack 
or set to NULL before pushing CPushMessage object on the cleanupstack. 

@param aHeader 
Pointer to message header. Ownership passes to CPushMessage.

@param aBody 
Pointer to message body. Ownership passes to CPushMessage.

@return 
New WAP Push message object.
*/
EXPORT_C CPushMessage* CPushMessage::NewL(HBufC8* aHeader, HBufC8* aBody)
	{
	__ASSERT_DEBUG(aHeader!=NULL,User::Invariant());
	CPushMessage* self =  new (ELeave) CPushMessage(aHeader);
	CleanupStack::PushL(self);
	self->ConstructL(aHeader, aBody, NULL);
	CleanupStack::Pop();
	return self;
	}


/** 
Allocates and constructs a new WAP Push message object.

It takes ownership of aHeader, aBody and aServerAddress. These must be put 
on the cleanup stack before calling NewL(), and popped off after the call 
returns.

@param aHeader 
Pointer to message header. Ownership passes to CPushMessage.

@param aBody 
Pointer to message body. Ownership passes to CPushMessage.

@param aPushID 
Message push ID.

@param aRemoteAddress 
Pointer to remote server address. Ownership passes to CPushMessage.

@return 
New WAP Push message object.
*/
EXPORT_C CPushMessage* CPushMessage::NewL(HBufC8* aHeader, HBufC8* aBody, 
										  TUint8 aPushID, HBufC8* aRemoteAddress)
	{
	__ASSERT_DEBUG(aHeader!=NULL,User::Invariant());
	CPushMessage* self =  new (ELeave) CPushMessage(aHeader, aPushID);
	CleanupStack::PushL(self);
	self->ConstructL(aHeader, aBody, aRemoteAddress);
	CleanupStack::Pop();
	return self;
	}


/**
Constructor.

@param aHeader 
Pointer to the push message header.

@param aPushID 
Push Message ID
*/
CPushMessage::CPushMessage(HBufC8* aHeader, TUint8 aPushID) : iPushID(aPushID)
	{
	iPushHeader.Set(*aHeader);
	}


/**
Constructor.
Ownership of aHeader is passed to the CHTTPResponse member.
 
@param aHeader 
Pointer to 8 bit descriptor with the header data.

@param aBody 
Pointer to 8 bit body data.

@param aRemoteAddress 
Pointer to 8 bit remote server address.
*/
void CPushMessage::ConstructL(HBufC8* aHeader, HBufC8* aBody, HBufC8* aRemoteAddress)
	{
	iHttpResponse = CHTTPResponse::NewL();
	iHttpResponse->AddResponse(aHeader);// Passes over ownership to iResponse
	iPushBody = aBody;// It is now safe to take ownership of aBody. CPushMessage::NewL can't fail after this point.
	iServerAddress = aRemoteAddress;
	}


/** 
Destructor. 
*/
EXPORT_C CPushMessage::~CPushMessage()
	{
	delete iHttpResponse;
	delete iPushBody;
	delete iServerAddress;
	delete iContentType;
	}


/** 
Gets the X-Wap-Application-Id field in the WSP header.

As the App Id can be either an integer or a URI string, the function handles 
both of these data types.

@param aAppURI 
On return, App Id URI (if present).

@param aAppID 
On return, App Id integer (if present).

@param aIsAnInt 
On return, ETrue if the App ID is an integer, EFalse if it's a URI.

@return 
True if the field was found, false if not.

@deprecated
Use the new CPushMessage::GetAppIdL function.
*/
EXPORT_C TBool CPushMessage::GetAppID(TPtrC8& aAppURI, TInt& aAppID, TBool& aIsAnInt)
	{
	TInt index = iHttpResponse->LocateField(EHttpXWapApplicationID);
	
	if (index == KErrNotFound)  //Field isn't present in the header
		return EFalse;

	// Use our friendship to take a peek at the first byte 
	// of the next field - is it a text character?
	TInt nextByte = iHttpResponse->iResponse->Des()[index];

	if (nextByte > 0x1F && nextByte < 0x80)  // call FindField as it's a string
		{
		iHttpResponse->FindField(EHttpXWapApplicationID, aAppURI);
		aIsAnInt = EFalse;
		}
	else
		{
		if (nextByte >=0x80) // Bit 7 set - encoded short integer value <=127
			aAppID = (nextByte & 0x7F); // AND away Bit7 to get the integer value
		else
			{
			TPtrC8 sourceBuffer =  iHttpResponse->iResponse->Des().Mid(index);
			TWapBinCodex::ExtractMultiOctetInteger(sourceBuffer, aAppID);
			}
		aIsAnInt = ETrue;
		}

	return ETrue;
	}
	
/** 
Gets the X-Wap-Application-Id field in the WSP header.

The App Id can be in either an integer or a URI string format - this function
handles both of these formats.

@param aAppURI 
An output argument with the App ID, URI string format. Only valid if aIsInt is
true and the function returns a value of true.

@param aAppID 
An output argument with the App ID, integer format. Only valid if aIsInt is true
and the function returns a value of true.

@param	aIsInt 
An output argument indicating if the App ID is in integer format. Only valid if
the function returns a value of true.

@return 
A value of true if the X-Wap-Application-Id field was found, false if not.

@leave	KErrCorrupt
The App ID could not be parsed as the binary data was corrupted.
*/
EXPORT_C TBool CPushMessage::GetAppIdL(TPtrC8& aAppURI, TInt& aAppID, TBool& aIsInt)
	{
	TInt index = iHttpResponse->LocateField(EHttpXWapApplicationID);
	
	if (index == KErrNotFound)  // X-Wap-Application-Id field isn't present
		return EFalse;

	User::LeaveIfError(index);

	// Use our friendship to take a peek at the first byte 
	// of the next field - is it a text character?
	TInt nextByte = iHttpResponse->iResponse->Des()[index];

	if (nextByte > 0x1F && nextByte < 0x80)  
		{
		// The AppId is given in Uri-value format - a string. Use FindField
		// to get the AppId.
		iHttpResponse->FindField(EHttpXWapApplicationID, aAppURI);
		aIsInt = EFalse;
		}
	else
		{
		// The AppId is in either Short-integer or Long-integer format...
		if (nextByte >=0x80)
			{
			// The AppId is in Short-integer format - 
			//		Short-integer = OCTET 
			//		; encoded as 7-nit data - 1xxx xxxx
			//
			// Using AND function to mask 7th bit and get the AppId.
			aAppID = (nextByte & 0x7F); 
			}
		else
			{
			// The AppId is in Long-integer format.
			TPtrC8 sourceBuffer =  iHttpResponse->iResponse->Des().Mid(index);
			User::LeaveIfError(TWapBinCodex::ParseMultiOctetInteger(sourceBuffer, aAppID));
			}
		aIsInt = ETrue;
		}

	return ETrue;
	}


/** 
Gets the Content-Type header.

If the content type is not found or not recognised, a default of "application/octet-stream" 
is returned.

@param aContentType 
On return, Content-Type header value. An empty descriptor (zero-length) indicates an 
allocation error occured. 
*/
EXPORT_C void CPushMessage::GetContentType(TPtrC& aContentType) const
	{
	if (iPushHeader.Length())
		{
		// Get content type from HTTPResponse
		TPtrC8 type;
		iHttpResponse->ContentType(type);

		// Copy content-type to local unicode buffer
		delete iContentType;
		iContentType = NULL;
		iContentType = HBufC::New(type.Length());
		if(iContentType)
			{
			TPtr typeBuffer(iContentType->Des());
			typeBuffer.Copy(type);
			aContentType.Set(*iContentType);
			}
		else // Buffer could not be created so set to null descriptor
			aContentType.Set(KNullDesC());
		}
	}


/** 
Gets a specified header field.

@param aField 
Header field identifier.

@param aFieldValue 
On return, the header field text.

@return 
True if the field was found, false if not.
*/
EXPORT_C TBool CPushMessage::GetHeaderField(THttpHeaderField aField, TPtrC8& aFieldValue) const
	{
	return iHttpResponse->FindField(aField, aFieldValue);
	}


/** 
Gets a specified header field as an 8-bit octet data.

@param aField 
Header field identifier.

@param aFieldValue 
On return, the header field value in binary form.

@return 
True if the field was found, false if not.
*/
EXPORT_C TBool CPushMessage::GetBinaryHeaderField(THttpHeaderField aField, TPtrC8& aFieldValue) const 
	{
	return iHttpResponse->FindBinaryDescField(aField, aFieldValue);
	}


/** 
Gets a specified header field as a date/time value.

@param aField 
Header field identifier.

@param aTime 
On return, the header field value in date/time form.

@return 
True if the field was found, false if not. 
*/
EXPORT_C TBool CPushMessage::GetHeaderField(THttpHeaderField aField, TTime& aTime) const
	{
	return iHttpResponse->FindField(aField, aTime);
	}


/** 
Gets the message body.

@param aMessageBodyPtr 
On return, the message body. KNullDesC8 if no body was found. 

@return 
True if a message body was found, false if it was empty. 
*/
EXPORT_C TBool CPushMessage::GetMessageBody(TPtrC8& aMessageBodyPtr) const
	{
	if (iPushBody == NULL)
		{
		aMessageBodyPtr.Set(KNullDesC8); 
		return EFalse; 
		}
	else
		{
		aMessageBodyPtr.Set(*iPushBody);
		return ETrue;
		}
	}


/** 
Gets the complete message header.

@param aMessageHeaderPtr 
On return, the complete message header. KNullDesC8 if no header was found.

@return 
True if a message header was found, false if it was empty. 
*/
EXPORT_C TBool CPushMessage::GetHeader(TPtrC8& aMessageHeaderPtr) const
	{
	aMessageHeaderPtr.Set(iPushHeader);
	return ETrue;
	}


/** 
Gets the remote server address.

@param aServerAddress 
On return, the remote server address (if present).

@return 
True if the remote server address was found, false if not. 
*/
EXPORT_C TBool CPushMessage::GetServerAddress(TPtrC8& aServerAddress) const
	{
	if (iServerAddress)
		{
		aServerAddress.Set(*iServerAddress);
		return ETrue;
		}
	return EFalse;
	}


/** 
Gets the Push ID.

@param aPushID 
On return, the ID value.

@return 
Indicates if the ID has been returned: EFalse if the ID was not set. 
*/
EXPORT_C TBool CPushMessage::GetPushId(TUint8& aPushID) const
	{
	if (iPushID)
		{
		aPushID=iPushID;
		return ETrue;
		}
	return EFalse;
	}


/**
Sets the PDU source allowed flag.

@param aAllowed ETrue - push PDU source address is present in whitelist.
				EFalse - push PDU is from an unknown origin. 
*/
EXPORT_C void CPushMessage::SetMessageAllowed(TBool aAllowed)
	{
	iAllowed = aAllowed;
	}


/**
Gets the PDU source allowed flag.

@return ETrue - push PDU source address is present in whitelist.
		EFalse - push PDU is from an unknown origin.
*/
EXPORT_C TBool CPushMessage::MessageAllowed() const
	{
	return iAllowed;
	}
	














