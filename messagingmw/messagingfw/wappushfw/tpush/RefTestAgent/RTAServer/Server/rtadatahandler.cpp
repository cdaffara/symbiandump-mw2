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

#include "rtadatahandler.h"
#include "RTAsession.h"
#include "RTAserver.h"
#include "rightsobject.h"
#include "rtaArchive.h"
#include "drmfilecontent.h"
#include "drmfiles.h"

using namespace ReferenceTestAgent;


CRtaDataHandler* CRtaDataHandler::NewL(const CSession2& aSession)
	{
	return new (ELeave) CRtaDataHandler(aSession);
	}

CRtaDataHandler::CRtaDataHandler(const CSession2& aSession) : CRtaMessageHandler(aSession)
	{
	}
	
CRtaDataHandler::~CRtaDataHandler()
	{
	delete iArchive;
	iFile.Close();
	}

void CRtaDataHandler::ServiceL(const RMessage2& aMessage)
//
// Handle a client request.
	{
	switch (aMessage.Function())
		{	
	// RRtaData functions
	case EOpenFile:
	case ENoEnforceOpenFile:
		OpenFileL(aMessage);
		break;
	case EOpenFileHandle:
	case ENoEnforceOpenFileHandle:
		OpenFileHandleL(aMessage);
		break;
	case EEvaluateIntent:
	case ENoEnforceEvaluateIntent:
		EvaluateIntentL(aMessage);
		break;
	case EExecuteIntent:
	case ENoEnforceExecuteIntent:
		ExecuteIntentL(aMessage);
		break;
	case EReadData:
	case ENoEnforceReadData:
		ReadDataL(aMessage);
		break;
	case EReadOffsetData:
	case ENoEnforceReadOffsetData:
		ReadOffsetDataL(aMessage);
		break;
	case ESeek:
	case ENoEnforceSeek:
		SeekL(aMessage);
		break;
	case EDataSize:
	case ENoEnforceDataSize:
		DataSizeL(aMessage);
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


void CRtaDataHandler::OpenFileL(const RMessage2& aMessage)
	{
	TUint mode;
	TPckg <TUint> fileModePckg(mode);
	
	HBufC* fileName = ReadDesC16LC(aMessage, 0);
	HBufC* uniqueId= ReadDesC16LC(aMessage, 1);
	aMessage.ReadL(2, fileModePckg);

	User::LeaveIfError(Server().CheckPermittedFileName(*fileName));
	User::LeaveIfError(iFile.Open(Server().Fs(), *fileName, mode));
	
	// create a DRM archive based upon the file handle 
	iArchive = CRefTestAgentArchive::NewL(iFile);
	iContentObject = &iArchive->DrmFilesL().FindL(*uniqueId);
	
	CleanupStack::PopAndDestroy(2, fileName); // fileName, UniqueId
	}
	
void CRtaDataHandler::OpenFileHandleL(const RMessage2& aMessage)
	{
	iFile.AdoptFromClient(aMessage, 0, 1);
	HBufC* uniqueId = ReadDesC16LC(aMessage, 2);	
	
	// create a DRM archive based upon the file handle 
	iArchive = CRefTestAgentArchive::NewL(iFile);
	iContentObject = &iArchive->DrmFilesL().FindL(*uniqueId);
	
	CleanupStack::PopAndDestroy(uniqueId); // file, UniqueId
	}

void CRtaDataHandler::EvaluateIntentL(const RMessage2& aMessage)
	{
	TIntent intent;
	TPckg <TIntent> intentPckg(intent);
	aMessage.ReadL(0, intentPckg);
	
	Server().EvaluateIntentL(iContentObject->Cid(), intent);	
	}

void CRtaDataHandler::ExecuteIntentL(const RMessage2& aMessage)
	{
	TIntent intent;
	TPckg <TIntent> intentPckg(intent);
	aMessage.ReadL(0, intentPckg);

	Server().ExecuteIntentL(iContentObject->Cid(), intent);
	}

void CRtaDataHandler::ReadDataL(const RMessage2& aMessage)
	{
	TInt length = 0;
	TPckg <TInt> lengthPckg(length);
	
	HBufC8* buffer = ReadDes8LC(aMessage, 0);
	aMessage.ReadL(1, lengthPckg);

	TPtr8 bufPtr = buffer->Des();	
	User::LeaveIfError(iContentObject->Read(bufPtr, length));
	
	WriteL(aMessage, 0, *buffer);
	
	CleanupStack::PopAndDestroy(buffer);
	}

void CRtaDataHandler::ReadOffsetDataL(const RMessage2& aMessage)
	{
	TInt length = 0;
	TInt pos = 0;
	TPckg <TInt> posPckg(pos);
	TPckg <TInt> lengthPckg(length);
	
	aMessage.ReadL(0, posPckg);
	HBufC8* buffer = ReadDes8LC(aMessage, 1);
	aMessage.ReadL(2, lengthPckg);

	TPtr8 bufPtr = buffer->Des();	
	User::LeaveIfError(iContentObject->Read(pos, bufPtr, length));
	
	WriteL(aMessage, 1, *buffer);
	
	CleanupStack::PopAndDestroy(buffer);
	}

	
void CRtaDataHandler::SeekL(const RMessage2& aMessage)
	{
	TSeek mode;
	TPckg <TSeek> modePckg(mode);
	aMessage.ReadL(0, modePckg);
	
	TInt pos;
	TPckg <TInt> posPckg(pos);
	aMessage.ReadL(1, posPckg);
	
	User::LeaveIfError(iContentObject->Seek(mode, pos));
	
	WriteL(aMessage, 1, posPckg);
	}
	
void CRtaDataHandler::DataSizeL(const RMessage2& aMessage)
	{
	TInt size;
	TPckg <TInt> sizePckg(size);
	aMessage.ReadL(0, sizePckg);
	
	User::LeaveIfError(iContentObject->Size(size));
	
	WriteL(aMessage, 0, sizePckg);
	}
	
void CRtaDataHandler::GetAttributeL(const RMessage2& aMessage)
	{
	TInt attribute;
	TPckg <TInt> attributePckg(attribute);

	TInt value;
	TPckg <TInt> valuePckg(value);			

	aMessage.ReadL(0, attributePckg);
	
	iContentObject->GetAttribute(attribute, value);
	
	WriteL(aMessage, 1, valuePckg);
	}
	
void CRtaDataHandler::GetAttributeSetL(const RMessage2& aMessage)
	{
	RAttributeSet attributeSet;
	CleanupClosePushL(attributeSet);
	
	HBufC8* value = ReadDes8LC(aMessage, 0);
	TPtr8 valuePtr = value->Des();
	
	RDesReadStream readStream(valuePtr);
	CleanupClosePushL(readStream);
	
	attributeSet.InternalizeL(readStream);
	CleanupStack::PopAndDestroy(&readStream);
	
	iContentObject->GetAttributeSet(attributeSet);

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
		
	CleanupStack::PopAndDestroy(3, &attributeSet); // buf, value, attributeSet
	}
	
void CRtaDataHandler::GetStringAttributeL(const RMessage2& aMessage)
	{
	TInt attribute;
	TPckg <TInt> attributePckg(attribute);
	aMessage.ReadL(0, attributePckg);
	
	HBufC* value = ReadDes16LC(aMessage, 1);
	TPtr valuePtr = value->Des();
	
	User::LeaveIfError(iContentObject->GetStringAttribute(attribute, valuePtr));
	
	WriteL(aMessage, 1, valuePtr);
	
	CleanupStack::PopAndDestroy(value);
	}
	
void CRtaDataHandler::GetStringAttributeSetL(const RMessage2& aMessage)
	{
	RStringAttributeSet attributeSet;
	CleanupClosePushL(attributeSet);
	
	HBufC8* value = ReadDes8LC(aMessage, 0);
	TPtr8 valuePtr = value->Des();
	
	RDesReadStream readStream(valuePtr);
	CleanupClosePushL(readStream);
	
	attributeSet.InternalizeL(readStream);
	CleanupStack::PopAndDestroy(&readStream);
	
	iContentObject->GetStringAttributeSet(attributeSet);

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
		
	CleanupStack::PopAndDestroy(3, &attributeSet); // buf, value, attributeSet
	}
	
