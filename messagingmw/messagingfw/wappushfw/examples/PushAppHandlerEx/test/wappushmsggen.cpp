// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include "wappushmsggen.h"

_LIT8(KXmlVersion, "<?xml version=\"1.0\"?>\r\n");
_LIT8(KBadXmlVersion, "<?xml version=\"2.0\"?>\r\n");  // Unsupported XML version
_LIT8(KSiStart, "<!DOCTYPE si PUBLIC \"-//WAPFORUM//DTD SI 1.0//EN\"\r\n\"http://www.wapforum.org/DTD/si.dtd\">\r\n<si>\r\n<indication ");
_LIT8(KSiEnd, "</indication>\r\n</si>\r\n");
_LIT8(KSlStart, "<!DOCTYPE sl PUBLIC \"-//WAPFORUM//DTD SL 1.0//EN\"\r\n\"http://www.wapforum.org/DTD/sl.dtd\">\r\n<sl ");
_LIT8(KSlEnd, ">\r\n</sl>\r\n");

// Valid XML tags and data
_LIT8(KHRefField, "href=\"");
_LIT8(KSiIdField, "si-id=\"");
_LIT8(KSiCreatedField, "created=\"");
_LIT8(KSiExpiresField, "si-expires=\"");
_LIT8(KActionField, "action=\"");


//Invalid XML tags & data 
_LIT8(KBadHRefField, "hreg=\"");
_LIT8(KBadSiIdField, "si-eyed=\"");
_LIT8(KBadSiCreatedField, "crated=\"");
_LIT8(KBadSiExpiresField, "si-xpires=\"");
_LIT8(KBadActionField, "adtion=\"");


_LIT8(KCrLf, "\r\n");
_LIT8(KDoubleQuote,"\"");

/** amount used to lengthen the buffer when appending data to it that would otherwise overflow it
*/
const TInt KMsgBufferSegmentLength=256;

/** public constructor */
CWapPushMsgGen* CWapPushMsgGen::NewL()
	{
	CWapPushMsgGen* thisptr = new(ELeave) CWapPushMsgGen();
	return thisptr;
	}


/** destructor */
CWapPushMsgGen::~CWapPushMsgGen()
	{
	delete iMsgBuffer;
	delete iMsgFieldBuffer;
	}
	

/** appends a field to the end of the current message being generated
 @param aField	The type of field to be set
 @param aFieldValue The value of the field to be set. This should not 
 include CR/LF at the end for fields included in " "
*/
void CWapPushMsgGen::AppendFieldL(TMsgField aFieldType, const TDesC8& aFieldValue)
	{
	if (!iMsgFieldBuffer)
		iMsgFieldBuffer=HBufC8::NewL(KMsgBufferSegmentLength);

	switch (aFieldType)
		{
	case EHRef:
		AppendMsgFieldDesL(KHRefField);
		break;
	case EServiceIndicationID:
		AppendMsgFieldDesL(KSiIdField);
		break;
	case EServiceIndicationCreated:
		AppendMsgFieldDesL(KSiCreatedField);
		break;
	case EServiceIndicationExpires:
		AppendMsgFieldDesL(KSiExpiresField);
		break;
	case EServiceIndicationText:
		{
		_LIT8(KEndIndication,">\r\n");
		AppendMsgFieldDesL(KEndIndication);
		}
		break;
	case EAction:
		AppendMsgFieldDesL(KActionField);
		break;

	case EBadHRef:
		AppendMsgFieldDesL(KBadHRefField);
		break;
	case EBadServiceIndicationID:
		AppendMsgFieldDesL(KBadSiIdField);
		break;
	case EBadServiceIndicationCreated:
		AppendMsgFieldDesL(KBadSiCreatedField);
		break;
	case EBadServiceIndicationExpires:
		AppendMsgFieldDesL(KBadSiExpiresField);
		break;
	case EBadAction:
		AppendMsgFieldDesL(KBadActionField);
		break;
	case EXMLNoFieldStr:
	case EWBXMLNoFieldStr:
		//Present so that can create an empty body using the msg Gen
		break;
	default:
		Panic(EInvalidPushMessageField);
		}

	AppendMsgFieldDesL(aFieldValue);
	//if (iMsgType != EServiceIndicationText)
	if (aFieldType != EServiceIndicationText  && aFieldType != EXMLNoFieldStr  
						&& aFieldType != EWBXMLNoFieldStr)
		AppendMsgFieldDesL(KDoubleQuote);
	if (aFieldType!= EWBXMLNoFieldStr)
		AppendMsgFieldDesL(KCrLf);
	}


/** closes the current message and returns the whole generated message
 @return returns a dynamic descriptor that is on the cleanup stack
*/	
HBufC8* CWapPushMsgGen::CreateMsgLC()
	{
	TInt size = (*iMsgBuffer).Length() + (*iMsgFieldBuffer).Length();
	switch(iMsgType)
		{
	case EServiceIndication:
		size+= KSiEnd().Length();
		break;
	case EServiceLoad:
		size+=KSlEnd().Length();
		break;
	case ECorruptMsgTest:
	case ECorruptXmlVersionTest:
	case EEmptyMsgBody:
		// No action - don't add anything extra to the body
		break;
	default:
		Panic(EInvalidPushMessageType);
		};

	HBufC8* finalMsg=HBufC8::NewLC(size);
	TPtr8 finalMsgPtr = finalMsg->Des();
	finalMsgPtr.Append(*iMsgBuffer);
	finalMsgPtr.Append(*iMsgFieldBuffer);

	switch(iMsgType)
		{
	case EServiceIndication:
		finalMsgPtr.Append(KSiEnd);
		break;
	case EServiceLoad:
		finalMsgPtr.Append(KSlEnd);
		break;
	case ECorruptMsgTest:
	case ECorruptXmlVersionTest:
	case EEmptyMsgBody:
		// Do nothing  - leave body as is
		break;
	default:
		Panic(EInvalidPushMessageType);
		}
	return finalMsg;
}


/** starts a new message 
 This starts a new message by clearing the current one. It then appends the
 header for the type of message being generated

  @param aMsgType an enum of type TMsgType specifing the type of message to be generated
*/	
void CWapPushMsgGen::StartNewMsgL(TMsgType aMsgType)
	{
	delete iMsgBuffer;
	iMsgBuffer = NULL;
	delete iMsgFieldBuffer;
	iMsgFieldBuffer = NULL;
	iMsgType = aMsgType;
	iMsgBuffer = HBufC8::NewL(KMsgBufferSegmentLength);

	
	switch (iMsgType)
		{
	case EServiceIndication:
		AppendMsgDesL(KXmlVersion);
		AppendMsgDesL(KSiStart);
		break;
	case EServiceLoad:
		AppendMsgDesL(KXmlVersion);
		AppendMsgDesL(KSlStart);
		break;
	case ECorruptMsgTest:
		AppendMsgDesL(KXmlVersion);
		break;
	case ECorruptXmlVersionTest:
		AppendMsgDesL(KBadXmlVersion);
		break;
	case EEmptyMsgBody:
		// do nothing - all we want is a Null Descriptor
		break;
	default:
		Panic(EInvalidPushMessageType);
		}
	}


/** used to append a descriptor to the current message buffer
  @param aDes, the descriptor to be added to the message buffer
*/
void CWapPushMsgGen::AppendMsgDesL(const TDesC8& aDes)
	{
	TInt maxLength = iMsgBuffer->Des().MaxLength();
	if (maxLength < aDes.Length() + (*iMsgBuffer).Length())
		{
		TInt newMaxLength = maxLength;
		newMaxLength += aDes.Length() > KMsgBufferSegmentLength? aDes.Length() : KMsgBufferSegmentLength;
		iMsgBuffer = iMsgBuffer->ReAllocL(newMaxLength);
		}
	iMsgBuffer->Des().Append(aDes);
	}


/**  used to append a descriptor to the current message field buffer 
  @param aDes, the descriptor to be added to the message buffer
*/
void CWapPushMsgGen::AppendMsgFieldDesL(const TDesC8& aDes)
	{
	TInt maxLength = iMsgFieldBuffer->Des().MaxLength();
	if (maxLength < aDes.Length() + (*iMsgFieldBuffer).Length())
		{
		TInt newMaxLength = maxLength;
		newMaxLength += aDes.Length() > KMsgBufferSegmentLength? aDes.Length() : KMsgBufferSegmentLength;
		iMsgFieldBuffer = iMsgFieldBuffer->ReAllocL(newMaxLength);
		}
	iMsgFieldBuffer->Des().Append(aDes);
	}

/** Panic function
	
*/
void CWapPushMsgGen::Panic(TWapPushMsgGenPanic aPanicCode)
	{
	_LIT(KWapPushMsgGenName,"PushMsgGen");
	User::Panic(KWapPushMsgGenName, aPanicCode);
	}