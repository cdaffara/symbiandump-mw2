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
#include "rtacontent.h"

using namespace ReferenceTestAgent;


EXPORT_C RRtaContent::RRtaContent()
	{
	
	}

EXPORT_C TInt RRtaContent::Open(const TDesC& aFileName, TUint aMode)
	{
	TPckg <TUint> fileModePckg(aMode);
	TInt err = RRtaClient::Connect();
	
	if(err == KErrNone)
		{
		if(IsDrmCapabilityEnforced())
			{
			err = SendReceive(ECreateContentMessageHandler, TIpcArgs());
			}
		else
			{
			err = SendReceive(ENoEnforceCreateContentMessageHandler, TIpcArgs());
			}
		}

	if(err == KErrNone)
		{
		ShareAuto();
		if(IsDrmCapabilityEnforced())
			{
			err = SendReceive(EOpenFile, TIpcArgs(&aFileName, &fileModePckg));
			}
		else
			{
			err = SendReceive(ENoEnforceOpenFile, TIpcArgs(&aFileName, &fileModePckg));
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
	
EXPORT_C TInt RRtaContent::Open(RFile& aFile)
	{
	TIpcArgs ipcArgs;
	aFile.TransferToServer(ipcArgs, 0, 1);
	
	TInt err = RRtaClient::Connect();
	if(err == KErrNone)
		{
		if(IsDrmCapabilityEnforced())
			{
			err = SendReceive(ECreateContentMessageHandler, TIpcArgs());
			}
		else
			{
			err = SendReceive(ENoEnforceCreateContentMessageHandler, TIpcArgs());
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

EXPORT_C TInt RRtaContent::OpenContainer(const TDesC& aUniqueId) const
	{
	TInt err = KErrNone;
	
	if(IsDrmCapabilityEnforced())
		{
		err = SendReceive(EOpenContainer, TIpcArgs(&aUniqueId));
		}
	else
		{
		err = SendReceive(ENoEnforceOpenContainer, TIpcArgs(&aUniqueId));
		}
	return err;
	}
EXPORT_C TInt RRtaContent::CloseContainer() const
	{
	TInt err = KErrNone;
	
	if(IsDrmCapabilityEnforced())
		{
		err = SendReceive(ECloseContainer, TIpcArgs());
		}
	else
		{
		err = SendReceive(ENoEnforceCloseContainer, TIpcArgs());
		}
	return err;
	
	}
	
EXPORT_C void RRtaContent::GetEmbeddedObjectsL(RStreamablePtrArray< CEmbeddedObject > &aArray) const
	{
	TInt err = KErrOverflow;
	
	// arbitary length for array of embedded objects
	TInt length = 40 * KMaxPath;
	TPckg<TInt> lengthPckg(length);
	
	while(err == KErrOverflow)
		{
		HBufC8* transferBuffer = HBufC8::NewLC(length);
		TPtr8 transferPtr = transferBuffer->Des();
	
		// attempt to retrieve the attributes
		err = SendReceive(EGetEmbeddedObjects1, TIpcArgs(&transferPtr));	
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
			aArray.InternalizeL(readStream);
			CleanupStack::PopAndDestroy(&readStream);
			}
		CleanupStack::PopAndDestroy(transferBuffer);	
		}
		
	// if any other error occured, leave
	User::LeaveIfError(err);
	}
	
EXPORT_C void RRtaContent::GetEmbeddedObjectsL(RStreamablePtrArray< CEmbeddedObject > &aArray, TEmbeddedType aType) const
	{
	// arbitary length for array of embedded objects
	TInt length = 40 * KMaxPath;
	TPckg<TInt> lengthPckg(length);
	
	TPckg<TEmbeddedType> typePckg(aType);
	
	TInt err = KErrOverflow;
	
	while(err == KErrOverflow)
		{
		HBufC8* transferBuffer = HBufC8::NewLC(length);
		TPtr8 transferPtr = transferBuffer->Des();
	
		// attempt to retrieve the attributes
		err = SendReceive(EGetEmbeddedObjects2, TIpcArgs(&transferPtr, &typePckg));	
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
			aArray.InternalizeL(readStream);
			CleanupStack::PopAndDestroy(&readStream);
			}
		CleanupStack::PopAndDestroy(transferBuffer);	
		}
		
	// if any other error occured, leave
	User::LeaveIfError(err);
	}
	
EXPORT_C TInt RRtaContent::SearchL(RStreamablePtrArray< CEmbeddedObject > &aArray, const TDesC8 &aMimeType, TBool aRecursive) const
	{
	// arbitary length for array of embedded objects
	TInt length = 40 * KMaxPath;
	TPckg<TInt> lengthPckg(length);
	TPckg<TBool> recursivePckg(aRecursive);
	TInt err = KErrOverflow;
	
	while(err == KErrOverflow)
		{
		HBufC8* transferBuffer = HBufC8::NewLC(length);
		TPtr8 transferPtr = transferBuffer->Des();
	
		// attempt to retrieve the attributes
		err = SendReceive(EGetEmbeddedObjects1, TIpcArgs(&transferPtr, &aMimeType, &recursivePckg));	
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
			aArray.InternalizeL(readStream);
			CleanupStack::PopAndDestroy(&readStream);
			}
		CleanupStack::PopAndDestroy(transferBuffer);	
		}
		
	return err;
	}


EXPORT_C TInt RRtaContent::GetAttribute (const TDesC& aUniqueId, TInt aAttribute, TInt &aValue) const
	{
	TPckg <TInt> attributePckg(aAttribute);
	TPckg <TInt> valuePckg(aValue);			
	return SendReceive(EGetAttribute, TIpcArgs(&aUniqueId, &attributePckg, &valuePckg));	
	}
	
EXPORT_C TInt RRtaContent::GetAttributeSetL(const TDesC& aUniqueId, RAttributeSet &aAttributeSet) const
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
		err = SendReceive(EGetAttributeSet, TIpcArgs(&aUniqueId, &transferPtr));	
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
		
EXPORT_C TInt RRtaContent::GetStringAttribute (const TDesC& aUniqueId, TInt aAttribute, TDes &aValue) const
	{
	TPckg <TInt> attributePckg(aAttribute);
	return SendReceive(EGetStringAttribute, TIpcArgs(&aUniqueId, &attributePckg, &aValue));
	}
		
EXPORT_C TInt RRtaContent::GetStringAttributeSetL(const TDesC& aUniqueId, RStringAttributeSet &aStringAttributeSet) const
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
		err = SendReceive(EGetStringAttributeSet, TIpcArgs(&aUniqueId, &transferPtr));	
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

