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

/**
 @file
 @test
 @internalComponent
*/
#include <s32mem.h>
#include <f32file.h>
#include <s32file.h>

#include "rtacontenthandler.h"
#include "RTAsession.h"
#include "RTAserver.h"
#include "rightsobject.h"
#include "rtaArchive.h"
#include "drmfilecontent.h"
#include "drmfilecontainer.h"
#include "drmfiles.h"

using namespace ReferenceTestAgent;


CRtaContentHandler* CRtaContentHandler::NewL(const CSession2& aSession)
	{
	return new (ELeave) CRtaContentHandler(aSession);
	}

CRtaContentHandler::CRtaContentHandler(const CSession2& aSession) : CRtaMessageHandler(aSession)
	{
	}
	
CRtaContentHandler::~CRtaContentHandler()
	{
	delete iArchive;
	iFile.Close();
	}

void CRtaContentHandler::ServiceL(const RMessage2& aMessage)
//
// Handle a client request.
	{
	switch (aMessage.Function())
		{	
	// RRtaContent functions
	case EOpenFile:
	case ENoEnforceOpenFile:
		OpenFileL(aMessage);
		break;
	case EOpenFileHandle:
	case ENoEnforceOpenFileHandle:
		OpenFileHandleL(aMessage);
		break;
	case EOpenContainer:
	case ENoEnforceOpenContainer:
		OpenContainerL(aMessage);
		break;
	case ECloseContainer:
	case ENoEnforceCloseContainer:
		CloseContainerL(aMessage);
		break;
	case ESearch:
	case ENoEnforceSearch:
		SearchL(aMessage);
		break;
	case EGetEmbeddedObjects1:
	case ENoEnforceGetEmbeddedObjects1:
		GetEmbeddedObjects1L(aMessage);		
		break;
	case EGetEmbeddedObjects2:
	case ENoEnforceGetEmbeddedObjects2:
		GetEmbeddedObjects2L(aMessage);		
		break;
	case EGetAttribute:
		GetAttributeL(aMessage);
		break;
	case EGetAttributeSet:
		GetAttributeSetL(aMessage);
		break;
	case EGetStringAttribute:
		GetStringAttributeL(aMessage);
		break;
	case EGetStringAttributeSet:
		GetStringAttributeSetL(aMessage);
		break;
	default:
		CRtaSession::PanicClient(aMessage,EPanicIllegalFunction);
		break;
		}
	}


void CRtaContentHandler::OpenFileL(const RMessage2& aMessage)
	{
	TUint mode;
	TPckg <TUint> fileModePckg(mode);
	
	HBufC* fileName = ReadDesC16LC(aMessage, 0);
	aMessage.ReadL(1, fileModePckg);
	
	User::LeaveIfError(Server().CheckPermittedFileName(*fileName));
	User::LeaveIfError(iFile.Open(Server().Fs(), *fileName, mode));
	
	// create a DRM archive based upon the file handle 
	iArchive = CRefTestAgentArchive::NewL(iFile);
	CleanupStack::PopAndDestroy(fileName); 
	}
	
void CRtaContentHandler::OpenFileHandleL(const RMessage2& aMessage)
	{
	iFile.AdoptFromClient(aMessage, 0, 1);
	
	// create a DRM archive based upon the file handle 
	iArchive = CRefTestAgentArchive::NewL(iFile);
	}


void CRtaContentHandler::OpenContainerL(const RMessage2& aMessage)
	{
	HBufC* uniqueId = ReadDesC16LC(aMessage, 0);

	User::LeaveIfError(iArchive->DrmFilesL().OpenContainer(*uniqueId));
	
	CleanupStack::PopAndDestroy(uniqueId);
	}

void CRtaContentHandler::CloseContainerL(const RMessage2& /*aMessage*/)
	{
	User::LeaveIfError(iArchive->DrmFilesL().CloseContainer());
	}

void CRtaContentHandler::GetEmbeddedObjects1L(const RMessage2& aMessage)
	{
	RStreamablePtrArray<CEmbeddedObject> array;
	CleanupClosePushL(array);
	
	iArchive->DrmFilesL().CurrentContainer().GetEmbeddedObjectsL(array);

	// Write the object out to a buffer, send to client
	CBufFlat* buf = CBufFlat::NewL(50);
	CleanupStack::PushL(buf);
	
	// create write stream
	RBufWriteStream writeStream(*buf);
	CleanupClosePushL(writeStream);

	// write the directory to the stream
	array.ExternalizeL(writeStream);
	CleanupStack::PopAndDestroy(&writeStream);
	
	TPtr8 bufPtr = buf->Ptr(0);
	
	WriteL(aMessage, 0, bufPtr);
	CleanupStack::PopAndDestroy(2, &array); // buf, array
	}

void CRtaContentHandler::GetEmbeddedObjects2L(const RMessage2& aMessage)
	{
	RStreamablePtrArray<CEmbeddedObject> array;
	CleanupClosePushL(array);
	
	TEmbeddedType type;
	TPckg<TEmbeddedType> typePckg(type);
	
	aMessage.Read(1, typePckg);
	
	iArchive->DrmFilesL().CurrentContainer().GetEmbeddedObjectsL(array, type);

	// Write the object out to a buffer, send to client
	CBufFlat* buf = CBufFlat::NewL(50);
	CleanupStack::PushL(buf);
	
	// create write stream
	RBufWriteStream writeStream(*buf);
	CleanupClosePushL(writeStream);

	// write the directory to the stream
	array.ExternalizeL(writeStream);
	CleanupStack::PopAndDestroy(&writeStream);
	
	TPtr8 bufPtr = buf->Ptr(0);
	
	WriteL(aMessage, 0, bufPtr);
	CleanupStack::PopAndDestroy(2, &array); // buf, array
	}

void CRtaContentHandler::SearchL(const RMessage2& aMessage)
	{
	RStreamablePtrArray<CEmbeddedObject> array;
	CleanupClosePushL(array);
	
	HBufC8* mimeType = ReadDesC8LC(aMessage, 1);
	TBool recursive;
	TPckg<TBool> recursivePckg(recursive);
	
	aMessage.Read(2, recursivePckg);
	
	User::LeaveIfError(iArchive->DrmFilesL().CurrentContainer().SearchL(array, *mimeType, recursive));

	// Write the object out to a buffer, send to client
	CBufFlat* buf = CBufFlat::NewL(50);
	CleanupStack::PushL(buf);
	
	// create write stream
	RBufWriteStream writeStream(*buf);
	CleanupClosePushL(writeStream);

	// write the directory to the stream
	array.ExternalizeL(writeStream);
	CleanupStack::PopAndDestroy(&writeStream);
	
	TPtr8 bufPtr = buf->Ptr(0);
	
	WriteL(aMessage, 0, bufPtr);
	CleanupStack::PopAndDestroy(3, &array); // buf, mimeType, array
	}

void CRtaContentHandler::GetAttributeL(const RMessage2& aMessage)
	{
	TInt attribute;
	TPckg <TInt> attributePckg(attribute);

	TInt value;
	TPckg <TInt> valuePckg(value);			

	HBufC* uniqueId = ReadDesC16LC(aMessage, 0);

	aMessage.ReadL(1, attributePckg);
	
	iArchive->DrmFilesL().FindL(*uniqueId).GetAttribute(attribute, value);
	
	WriteL(aMessage, 2, valuePckg);
	
	CleanupStack::PopAndDestroy(uniqueId);
	}
	
void CRtaContentHandler::GetAttributeSetL(const RMessage2& aMessage)
	{
	RAttributeSet attributeSet;
	CleanupClosePushL(attributeSet);
	
	HBufC* uniqueId = ReadDesC16LC(aMessage, 0);
	
	HBufC8* value = ReadDes8LC(aMessage, 1);
	TPtr8 valuePtr = value->Des();
	
	RDesReadStream readStream(valuePtr);
	CleanupClosePushL(readStream);
	
	attributeSet.InternalizeL(readStream);
	CleanupStack::PopAndDestroy(&readStream);
	
	iArchive->DrmFilesL().FindL(*uniqueId).GetAttributeSet(attributeSet);

	// Write the object out to a buffer, send to client
	CBufFlat* buf = CBufFlat::NewL(50);
	CleanupStack::PushL(buf);
	
	// create write stream
	RBufWriteStream writeStream(*buf);
	CleanupClosePushL(writeStream);

	// write the directory to the stream
	attributeSet.ExternalizeL(writeStream);
	CleanupStack::PopAndDestroy(&writeStream);
	
	TPtr8 bufPtr = buf->Ptr(0);
	
	WriteL(aMessage, 1, bufPtr);
		
	CleanupStack::PopAndDestroy(4, &attributeSet); // buf, value, attributeSet, uniqueId
	}
	
void CRtaContentHandler::GetStringAttributeL(const RMessage2& aMessage)
	{
	TInt attribute;
	TPckg <TInt> attributePckg(attribute);
	
	HBufC* uniqueId = ReadDesC16LC(aMessage, 0);
	
	aMessage.ReadL(1, attributePckg);
	
	HBufC* value = ReadDes16LC(aMessage, 2);
	TPtr valuePtr = value->Des();
	
	User::LeaveIfError(iArchive->DrmFilesL().FindL(*uniqueId).GetStringAttribute(attribute, valuePtr));
	
	WriteL(aMessage, 2, valuePtr);
	
	CleanupStack::PopAndDestroy(2, value); // uniqueId, value
	}
	
void CRtaContentHandler::GetStringAttributeSetL(const RMessage2& aMessage)
	{
	RStringAttributeSet attributeSet;
	CleanupClosePushL(attributeSet);
	
	HBufC* uniqueId = ReadDesC16LC(aMessage, 0);
	
	HBufC8* value = ReadDes8LC(aMessage, 0);
	TPtr8 valuePtr = value->Des();
	
	RDesReadStream readStream(valuePtr);
	CleanupClosePushL(readStream);
	
	attributeSet.InternalizeL(readStream);
	CleanupStack::PopAndDestroy(&readStream);
	
	iArchive->DrmFilesL().FindL(*uniqueId).GetStringAttributeSet(attributeSet);

	// Write the object out to a buffer, send to client
	CBufFlat* buf = CBufFlat::NewL(50);
	CleanupStack::PushL(buf);
	
	// create write stream
	RBufWriteStream writeStream(*buf);
	CleanupClosePushL(writeStream);

	// write the directory to the stream
	attributeSet.ExternalizeL(writeStream);
	CleanupStack::PopAndDestroy(&writeStream);
	
	TPtr8 bufPtr = buf->Ptr(0);
	
	WriteL(aMessage, 0, bufPtr);
		
	CleanupStack::PopAndDestroy(4, &attributeSet); // buf, value, attributeSet, uniqueid
	}
	
