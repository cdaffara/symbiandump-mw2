/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Read instructions from message file
*
*/


#include <f32file.h>
#include <e32cmn.h>
#include <utf.h>
#include <barsc.h>
#include <barsread.h>
#include <epos_comasuplpospayload.h>
#include "epos_comasuplmessagefilereader.h"
#include <postester.rsg>
#include "epos_comasuplmessageinfo.h"
#include "epos_comasuplpostesterlogger.h"


_LIT(KResourcePath, "\\resource\\");
_LIT(KResourceName, "postester.rsc");


_LIT8(KConnectErr, "Error: Could not connect to server.");
_LIT8(KErrOpen, "Error: Could not open file");
_LIT8(KParseErr, "Error: Error in Parsing");
_LIT8(KPayloadErr, "Error: Wrong payload type.");
_LIT8(KInstructionErr, "Error: Wrong instruction type.");
_LIT8(KResourceNotFound, "Resource file not found.");

void ResetAndDestroy(TAny* aArray)
    {
    	((RPointerArray<COMASuplMessageInfo>*)aArray)->ResetAndDestroy();
    	delete aArray;
    }

COMASuplMessageFileReader* COMASuplMessageFileReader::
											NewL( TFileName& aFilePath
										, COMASuplPosTesterLogger* aLog)
	{
	COMASuplMessageFileReader* self = new( ELeave )
								COMASuplMessageFileReader(aLog);
	CleanupStack::PushL( self );
	self->ConstructL( aFilePath );
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
COMASuplMessageFileReader::~COMASuplMessageFileReader()
	{
	iRFile.Close();
	iRFs.Close();
	delete iFileData;
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
RPointerArray< COMASuplMessageInfo >& COMASuplMessageFileReader::
														GetMessageListL()
	{
	TBool noEoF = ETrue;
	RPointerArray< COMASuplMessageInfo >* messageArray = 
							new(ELeave)RPointerArray< COMASuplMessageInfo >;
	CleanupStack::PushL(TCleanupItem(*ResetAndDestroy, messageArray));
	while(noEoF)
		{
		//Read one Message
		HBufC8* messageBlock = NULL;
		if(ReadOneMessageBlockL(messageBlock))
			{
			CleanupStack::PushL(messageBlock);
			COMASuplMessageInfo* msgInfo = NULL;
			GetMsgInfoL(messageBlock, msgInfo);
			messageArray->Append(msgInfo);
			CleanupStack::Pop(messageBlock);
			}
		else
			{
			noEoF = EFalse;
			}
		delete messageBlock;
		}
	CleanupStack::Pop(messageArray);
	return *messageArray;
	}

COMASuplMessageFileReader::COMASuplMessageFileReader(COMASuplPosTesterLogger* aLog)
	{
	iLogger = aLog;
	}

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//

void COMASuplMessageFileReader::ConstructL(TFileName& aFilePath)
	{
	TInt err = KErrGeneral;
	err = iRFs.Connect();
	if(err != KErrNone)
		{
		iLogger->WriteLine(KConnectErr);
		User::LeaveIfError(err);
		}
	if(0 == aFilePath.Length())
		{
		ReadFileInfoFromResourceL();
		}
	else
		{
		iMsgFileName.Copy(aFilePath);
		}
	
	HBufC* filename = HBufC::NewL(iMsgFileName.Length() + 2 * KExtraBuffer);
	CleanupStack::PushL(filename);
	
	TPtr filenamePtr = filename->Des();
	filenamePtr.Copy(_L("Info: Path of Message File Used is: "));
	filenamePtr.Append(iMsgFileName);
	
	HBufC8* displayName = 
				CnvUtfConverter::ConvertFromUnicodeToUtf8L(filenamePtr);

	iLogger->WriteLine(*displayName);
	CleanupStack::PopAndDestroy(filename);
	delete displayName;
	
	err = iRFile.Open(iRFs, iMsgFileName, EFileShareReadersOnly);
	if(err != KErrNone)
		{
		iLogger->WriteLine(KErrOpen);
		User::LeaveIfError(err);
		}
	FileDataReaderL();
	}

// ---------------------------------------------------------------------------
// Reads complete data of a file in a buffer on heap.
// ---------------------------------------------------------------------------
//
TDesC8& COMASuplMessageFileReader::FileDataReaderL()
	{
	TInt fileSize = 0;
	User::LeaveIfError(iRFile.Size(fileSize));
	iFileData = HBufC8::NewL(fileSize);
	TPtr8 data = (iFileData->Des());
	User::LeaveIfError(iRFile.Read(data));
	return *iFileData;
	}

// ---------------------------------------------------------------------------
// Reads one message block from the buffer which contains complete data
// of the file.
// ---------------------------------------------------------------------------
//
TBool COMASuplMessageFileReader::ReadOneMessageBlockL(HBufC8*& aMessageBlock)
	{
	TInt msgBlockEnd = iFileData->Find(KMsgBlockDelimiter);
	if(msgBlockEnd > 0)
		{
		aMessageBlock = HBufC8::NewL(msgBlockEnd 
									+ KMsgBlockDelimiter().Length());
		aMessageBlock->Des().Copy(iFileData->Ptr(), msgBlockEnd 
									+ KMsgBlockDelimiter().Length());
		iFileData->Des().Delete(0, msgBlockEnd 
									+ KMsgBlockDelimiter().Length());
		return ETrue;
		}
	if(iFileData->Length() > 0)
		{
		iLogger->WriteLine(KParseErr);
		User::Leave(KErrNotFound);
		}
	return EFalse;
	}

// ---------------------------------------------------------------------------
// Extracts "instruction", "payload type" and "payload" from the message 
// block.
// ---------------------------------------------------------------------------
//

TInt COMASuplMessageFileReader::ParseMessageBlockL(HBufC8& aMessageBlock
						, COMASuplMessageInfo::TOMASuplOperationType& aOpType
						, COMASuplPosPayload& aPayload)
	{
	
	TInt payloadType = KErrNotFound;
	//find \r\n
	TInt typeDelimLoc = KErrNotFound;
	TInt opDelimLoc = KErrNotFound;
	TPtrC8 block = aMessageBlock.Mid(0);
	opDelimLoc = block.Find(KTypeDelimiter);
	if(opDelimLoc > 0)
		{
		TPtrC8 opType(block.Left(opDelimLoc));
		aOpType = GetOperationTypeL(opType);
		if(COMASuplMessageInfo::ESend == aOpType)
			{
			TPtrC8 payloadPart = block.Mid(opDelimLoc + KTypeDelimiter().Length());
			typeDelimLoc = payloadPart.Find(KTypeDelimiter);
			if(typeDelimLoc  > 0)
				{
				TPtrC8 type (payloadPart.Left(typeDelimLoc));

				HBufC8* typeToTrim = type.AllocL();
				typeToTrim->Des().Trim();
				TLex8 lex(typeToTrim->Des());
				TInt err = lex.Val(payloadType);
				delete typeToTrim;

				if(err != KErrNone)
					{
					iLogger->WriteLine(KParseErr);
					User::Leave(err);
					}
				TInt endPos = 0;
				TPtrC8 payloadPtr = payloadPart.Mid(typeDelimLoc + 
												KTypeDelimiter().Length());
				endPos = payloadPtr.Find(KMsgBlockDelimiter);
				TPtrC8 onlyPayload = payloadPtr.Left(endPos);
				HBufC8* payLoadData = onlyPayload.AllocL();//need not delete
				CleanupStack::PushL(payLoadData);
				PreparePayLoadL(aPayload, *payLoadData, payloadType);
				CleanupStack::Pop(payLoadData);
				}
			else
				{
				iLogger->WriteLine(KParseErr);
				User::Leave(KErrNotFound);
				}
			}
		else
			{
			return KErrNone;
			}
		}
	else
		{
		iLogger->WriteLine(KParseErr);
		User::Leave(KErrNotFound);
		}
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// Prepares COMASuplPosPayload object from a descriptor.
// ---------------------------------------------------------------------------
//
void COMASuplMessageFileReader::PreparePayLoadL(
									COMASuplPosPayload& aPayloadObj
									, HBufC8& aPayload
									, TInt aPayloadType)
	{
	COMASuplPosPayload::TOMASuplPosPayloadType payloadType = 
											COMASuplPosPayload::ETIA801;
	switch(aPayloadType)
		{
		case 0:
			{
			payloadType = COMASuplPosPayload::ETIA801;
			break;
			}
		case 1:
			{
			payloadType = COMASuplPosPayload::ERRLP;
			break;
			}
		case 2:
			{
			payloadType = COMASuplPosPayload::ERRC;
			break;
			}
		default:
			{
			iLogger->WriteLine(KPayloadErr);
			User::Leave(KErrNotFound);
			}
			
		}
	aPayloadObj.SetPosPayload(&aPayload, payloadType);
	}

// ---------------------------------------------------------------------------
// Creates a COMASuplMessageInfo object from a message block.
// ---------------------------------------------------------------------------
//
void COMASuplMessageFileReader::GetMsgInfoL(HBufC8* aMessageBlock
							, COMASuplMessageInfo*& msgInfo)
	{
	delete msgInfo;
	
	COMASuplPosPayload* posPayLoad = COMASuplPosPayload::NewL();
	CleanupStack::PushL(posPayLoad);
	COMASuplMessageInfo::TOMASuplOperationType opType = COMASuplMessageInfo::EWait;
	ParseMessageBlockL(*aMessageBlock, opType, *posPayLoad);
	msgInfo = COMASuplMessageInfo::NewL(opType, *posPayLoad);
	CleanupStack::Pop(posPayLoad);
	delete posPayLoad;
	}

// ---------------------------------------------------------------------------
// Provides the type of instruction/operation.
// ---------------------------------------------------------------------------
//
COMASuplMessageInfo::TOMASuplOperationType 
COMASuplMessageFileReader::GetOperationTypeL(const TDesC8& aOpType)
	{
	if(aOpType.Compare(KSendOp) == 0)
		{
		return COMASuplMessageInfo::ESend;
		}
	else 
		{
		if(aOpType.Compare(KWaitOp) == 0)
			{
			return COMASuplMessageInfo::EWait;
			}
		else
			{
			iLogger->WriteLine(KInstructionErr);
			User::Leave(KErrNotFound);
			}
		}
	return COMASuplMessageInfo::EWait;
	}

// ---------------------------------------------------------------------------
// Reads default message file path and name from the resource file.
// ---------------------------------------------------------------------------
//
void COMASuplMessageFileReader::ReadFileInfoFromResourceL()
	{
	TBuf<128> resourceFileName;
	GetFileAbsolutePathL(resourceFileName);
	
    RResourceFile resourceFile;
    CleanupClosePushL(resourceFile);
    resourceFile.OpenL(iRFs, resourceFileName);
    resourceFile.ConfirmSignatureL(0);
    HBufC8* dataBuffer = resourceFile.AllocReadLC(R_POS_TESTER_MSG_FILE_PATH_NAME);
    TResourceReader theReader;
	theReader.SetBuffer(dataBuffer);
	TPtrC textdata = theReader.ReadTPtrC();
	iMsgFileName.Copy(textdata);
	CleanupStack::PopAndDestroy(dataBuffer);
	CleanupStack::PopAndDestroy(&resourceFile);//closing resource file
	}

void COMASuplMessageFileReader::GetFileAbsolutePathL(TDes& aResourceFileName)
	{
	TFindFile findResource(iRFs);
	TInt err = findResource.FindByDir(KResourceName(), KResourcePath());
	if(err != KErrNone)
		{
		iLogger->WriteLine(KResourceNotFound);
		User::Leave(err);
		}
	aResourceFileName.Copy(findResource.File());
	}
