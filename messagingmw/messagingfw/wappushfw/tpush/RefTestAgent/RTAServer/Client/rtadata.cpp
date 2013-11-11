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
// sisregistry - client registry session interface implementation
// 
//

/**
 @file 
 @test
 @internalComponent
*/

#include <s32mem.h>
#include "clientserver.h"
#include "rtadata.h"

using namespace ReferenceTestAgent;


EXPORT_C RRtaData::RRtaData()
	{
	
	}

EXPORT_C TInt RRtaData::Open(const TDesC& aFileName, const TDesC& aUniqueId, TUint aMode)
	{
	TPckg <TUint> fileModePckg(aMode);
	TInt err = RRtaClient::Connect();
	
	if(err == KErrNone)
		{
		if(IsDrmCapabilityEnforced())
			{
			err = SendReceive(ECreateDataMessageHandler, TIpcArgs());
			}
		else
			{
			err = SendReceive(ENoEnforceCreateDataMessageHandler, TIpcArgs());
			}
		}

	if(err == KErrNone)
		{
		ShareAuto();
		if(IsDrmCapabilityEnforced())
			{
			err = SendReceive(EOpenFile, TIpcArgs(&aFileName, &aUniqueId, &fileModePckg));
			}
		else
			{
			err = SendReceive(ENoEnforceOpenFile, TIpcArgs(&aFileName, &aUniqueId, &fileModePckg));
			}
		}
		
	// check final result
	if(err != KErrNone)
		{
		// either connect and open file or don't connect
		Close();
		}
	return err;
	}
	
EXPORT_C TInt RRtaData::Open(RFile& aFile, const TDesC& aUniqueId)
	{
	TIpcArgs ipcArgs;
	aFile.TransferToServer(ipcArgs, 0, 1);
	ipcArgs.Set(2, &aUniqueId);
	
	TInt err = RRtaClient::Connect();
	if(err == KErrNone)
		{
		if(IsDrmCapabilityEnforced())
			{
			err = SendReceive(ECreateDataMessageHandler, TIpcArgs());
			}
		else
			{
			err = SendReceive(ENoEnforceCreateDataMessageHandler, TIpcArgs());
			}
		}
		
	if(err == KErrNone)
		{
		ShareAuto();
		if(IsDrmCapabilityEnforced())
			{
			err = SendReceive(EOpenFileHandle, ipcArgs);
			}
		else
			{
			err = SendReceive(ENoEnforceOpenFileHandle, ipcArgs);
			}
		}

	if(err != KErrNone)
		{
		// either connect and open file or don't connect
		Close();
		}
	return err;
	}

EXPORT_C TInt RRtaData::Read(TDes8& aBuffer, TInt aLength)
	{
	TPckg <TInt> lengthPckg(aLength);
	if(IsDrmCapabilityEnforced())
		{
		return SendReceive(EReadData, TIpcArgs(&aBuffer, &lengthPckg));	
		}
	else
		{
		return SendReceive(ENoEnforceReadData, TIpcArgs(&aBuffer, &lengthPckg));	
		}
	}

EXPORT_C TInt RRtaData::Read(TInt aPos, TDes8& aBuffer, TInt aLength)
	{
	TPckg <TInt> posPckg(aPos);
	TPckg <TInt> lengthPckg(aLength);
	if(IsDrmCapabilityEnforced())
		{
		return SendReceive(EReadOffsetData, TIpcArgs(&posPckg, &aBuffer, &lengthPckg));	
		}
	else
		{
		return SendReceive(ENoEnforceReadOffsetData, TIpcArgs(&posPckg, &aBuffer, &lengthPckg));	
		}
	}
	

EXPORT_C TInt RRtaData::DataSize(TInt &aSize) 
	{
	TPckg <TInt> sizePckg(aSize);
	if(IsDrmCapabilityEnforced())
		{
		return SendReceive(EDataSize, TIpcArgs(&sizePckg));	
		}
	else
		{
		return SendReceive(ENoEnforceDataSize, TIpcArgs(&sizePckg));		
		}
	}
	
EXPORT_C TInt RRtaData::Seek (TSeek aMode, TInt &aPos) const 
	{
	TPckg <TSeek> modePckg(aMode);		
	TPckg <TInt> posPckg(aPos);
	
	if(IsDrmCapabilityEnforced())
		{
		return SendReceive(ESeek, TIpcArgs(&modePckg, &posPckg));	
		}
	else
		{
		return SendReceive(ENoEnforceSeek, TIpcArgs(&modePckg, &posPckg));	
		}
	}


EXPORT_C void RRtaData::EvaluateIntentL(TIntent aIntent)
	{
	TPckg <TIntent> intentPckg(aIntent);
	if(IsDrmCapabilityEnforced())
		{
		User::LeaveIfError(SendReceive(EEvaluateIntent, TIpcArgs(&intentPckg)));
		}
	else
		{
		User::LeaveIfError(SendReceive(ENoEnforceEvaluateIntent, TIpcArgs(&intentPckg)));	
		}
	}

EXPORT_C void RRtaData::ExecuteIntentL(TIntent aIntent)
	{
	TPckg <TIntent> intentPckg(aIntent);
	if(IsDrmCapabilityEnforced())
		{
		User::LeaveIfError(SendReceive(EExecuteIntent, TIpcArgs(&intentPckg)));
		}
	else
		{
		User::LeaveIfError(SendReceive(ENoEnforceEvaluateIntent, TIpcArgs(&intentPckg)));	
		}
	}

EXPORT_C TInt RRtaData::GetAttribute (TInt aAttribute, TInt &aValue) const
	{
	TPckg <TInt> attributePckg(aAttribute);
	TPckg <TInt> valuePckg(aValue);			
	return SendReceive(EGetAttribute, TIpcArgs(&attributePckg, &valuePckg));	
	}
	
EXPORT_C TInt RRtaData::GetAttributeSetL(RAttributeSet &aAttributeSet) const
	{
	TInt err = KErrOverflow;
	TInt length = 0;
	TPckg <TInt> lengthPckg(length);
	
	// Write the object out to a buffer, send to client
	CBufFlat* buf = CBufFlat::NewL(50);
	CleanupStack::PushL(buf);
	
	// create write stream
	RBufWriteStream writeStream(*buf);
	CleanupClosePushL(writeStream);

	// write the directory to the stream
	aAttributeSet.ExternalizeL(writeStream);
	CleanupStack::PopAndDestroy(&writeStream);
	
	
	TPtr8 bufPtr = buf->Ptr(0);
	// allocate arbitary receive buffer twice the size of the source buffer
	length = bufPtr.Length() * 2;
	
	while(err == KErrOverflow)
		{
		HBufC8* transferBuffer = HBufC8::NewLC(length);
		transferBuffer->Des().Copy(bufPtr);
		TPtr8 transferPtr = transferBuffer->Des();
	
		// attempt to retrieve the attributes
		err = SendReceive(EGetAttributeSet, TIpcArgs(&transferPtr));	
		if(err == KErrOverflow)
			{
			// Find out the length required to receive the resulting attribute set
			lengthPckg.Copy(transferPtr.Left(lengthPckg.MaxLength()));	
			}
		else if(err == KErrNone)
			{
			// read in the attribute values from the buffer
			RDesReadStream readStream(transferPtr);
			CleanupClosePushL(readStream);
			aAttributeSet.InternalizeL(readStream);
			CleanupStack::PopAndDestroy(&readStream);
			}
		CleanupStack::PopAndDestroy(transferBuffer);	
		}

	CleanupStack::PopAndDestroy(buf);
	return err;
	}
		
EXPORT_C TInt RRtaData::GetStringAttribute (TInt aAttribute, TDes &aValue) const
	{
	TPckg <TInt> attributePckg(aAttribute);
	return SendReceive(EGetStringAttribute, TIpcArgs(&attributePckg, &aValue));
	}
		
EXPORT_C TInt RRtaData::GetStringAttributeSetL(RStringAttributeSet &aStringAttributeSet) const
	{
	TInt err = KErrOverflow;
	TInt length = 0;
	TPckg <TInt> lengthPckg(length);
	
	// Write the object out to a buffer, send to client
	CBufFlat* buf = CBufFlat::NewL(50);
	CleanupStack::PushL(buf);
	
	// create write stream
	RBufWriteStream writeStream(*buf);
	CleanupClosePushL(writeStream);

	// write the directory to the stream
	aStringAttributeSet.ExternalizeL(writeStream);
	CleanupStack::PopAndDestroy(&writeStream);
	
	
	TPtr8 bufPtr = buf->Ptr(0);
	// allocate arbitary receive buffer much larger than the size of the source buffer
	length = bufPtr.Length() * 15;
	
	while(err == KErrOverflow)
		{
		HBufC8* transferBuffer = HBufC8::NewLC(length);
		transferBuffer->Des().Copy(bufPtr);
		TPtr8 transferPtr = transferBuffer->Des();
	
		// attempt to retrieve the attributes
		err = SendReceive(EGetStringAttributeSet, TIpcArgs(&transferPtr));	
		if(err == KErrOverflow)
			{
			// Find out the length required to receive the resulting attribute set
			lengthPckg.Copy(transferPtr.Left(lengthPckg.MaxLength()));	
			}
		else if(err == KErrNone)
			{
			// read in the attribute values from the buffer
			RDesReadStream readStream(transferPtr);
			CleanupClosePushL(readStream);
			aStringAttributeSet.InternalizeL(readStream);
			CleanupStack::PopAndDestroy(&readStream);
			}
		CleanupStack::PopAndDestroy(transferBuffer);	
		}

	CleanupStack::PopAndDestroy(buf);
	return err;
	}

