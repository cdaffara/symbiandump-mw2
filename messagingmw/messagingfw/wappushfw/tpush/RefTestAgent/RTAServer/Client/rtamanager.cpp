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
#include <f32file.h>
#include <caf/caf.h>
#include "clientserver.h"
#include "rtamanager.h"


using namespace ReferenceTestAgent;

EXPORT_C RRtaManager::RRtaManager()
	{
	
	}

EXPORT_C TInt RRtaManager::Connect()
	{
	TInt err = KErrNone;
	err = RRtaClient::Connect();
	if(err == KErrNone)
		{
		// Don't really care whether client has DRM capability at this point
		err = SendReceive(ENoEnforceCreateManagerMessageHandler, TIpcArgs());
		if(err != KErrNone)
			{
			Close();
			}
		}
	return err;
	}

EXPORT_C TInt RRtaManager::DeleteFile(const TDesC& aFileName) const
	{
	TInt err = KErrNone;
	if(IsDrmCapabilityEnforced())
		{
		err = SendReceive(EManagerDeleteFile, TIpcArgs(&aFileName));
		}
	else
		{
		err = SendReceive(ENoEnforceManagerDeleteFile, TIpcArgs(&aFileName));
		}
	return err;
	}

EXPORT_C TInt RRtaManager::CopyFile(const TDesC& aSource, const TDesC& aDestination) const
	{
	TInt err = KErrNone;
	if(IsDrmCapabilityEnforced())
		{
		err = SendReceive(EManagerCopyFile, TIpcArgs(&aSource, &aDestination));
		}
	else
		{
		err = SendReceive(ENoEnforceManagerCopyFile, TIpcArgs(&aSource, &aDestination));
		}
	return err;
	}

EXPORT_C TInt RRtaManager::CopyFile(RFile& aSource, const TDesC& aDestination) const
	{
	TInt err = KErrNone;
	TIpcArgs ipcArgs;

	err = aSource.TransferToServer(ipcArgs,0,1);
	if (err==KErrNone)
		{
		ipcArgs.Set(2,&aDestination);
		if(IsDrmCapabilityEnforced())
			{
			err = SendReceive(EManagerCopyFileHandle, ipcArgs);
			}
		else
			{
			err = SendReceive(ENoEnforceManagerCopyFileHandle, ipcArgs);
			}
		}
	return err;
	}

EXPORT_C TInt RRtaManager::RenameFile(const TDesC& aOldFileName, const TDesC& aNewFileName) const
	{
	TInt err = KErrNone;
	if(IsDrmCapabilityEnforced())
		{
		err = SendReceive(EManagerRenameFile, TIpcArgs(&aOldFileName, &aNewFileName));
		}
	else
		{
		err = SendReceive(ENoEnforceManagerRenameFile, TIpcArgs(&aOldFileName, &aNewFileName));
		}
	return err;
	}

EXPORT_C TInt RRtaManager::MkDir(const TDesC& aPath) const
	{
	TInt err = KErrNone;
	if(IsDrmCapabilityEnforced())
		{
		err = SendReceive(EManagerMkDir, TIpcArgs(&aPath));
		}
	else
		{
		err = SendReceive(ENoEnforceManagerMkDir, TIpcArgs(&aPath));
		}
	return err;
	}
	
	
EXPORT_C TInt RRtaManager::MkDirAll(const TDesC& aPath) const
	{
	TInt err = KErrNone;
	if(IsDrmCapabilityEnforced())
		{
		err = SendReceive(EManagerMkDirAll, TIpcArgs(&aPath));
		}
	else
		{
		err = SendReceive(ENoEnforceManagerMkDirAll, TIpcArgs(&aPath));
		}
	return err;
	}
	

EXPORT_C TInt RRtaManager::RenameDir(const TDesC& aOldPath, const TDesC& aNewPath) const
	{
	TInt err = KErrNone;
	if(IsDrmCapabilityEnforced())
		{
		err = SendReceive(EManagerRenameDir, TIpcArgs(&aOldPath, &aNewPath));
		}
	else
		{
		err = SendReceive(ENoEnforceManagerRenameDir, TIpcArgs(&aOldPath, &aNewPath));
		}
	return err;
	}

EXPORT_C TInt RRtaManager::RmDir(const TDesC& aPath) const
	{
	TInt err = KErrNone;
	if(IsDrmCapabilityEnforced())
		{
		err = SendReceive(EManagerRemoveDir, TIpcArgs(&aPath));
		}
	else
		{
		err = SendReceive(ENoEnforceManagerRemoveDir, TIpcArgs(&aPath));
		}
	return err;
	}


EXPORT_C TInt RRtaManager::GetDirL(const TDesC& aPath, TUint aEntryMask, TUint aEntrySortKey, CDir*& aEntryList) const
	{
	TInt err = KErrOverflow;
	TPckg<TUint> entryMaskPckg(aEntryMask);
	TPckg<TUint> sortKeyPckg(aEntrySortKey);
	
	// start off with an arbitary size receive buffer
	TInt length = 50 * sizeof(TEntry);
	TPckg <TInt> lengthPckg(length);
	

	while(err == KErrOverflow)
		{
		HBufC8* transferBuffer = HBufC8::NewLC(length);
		TPtr8 transferPtr = transferBuffer->Des();
	
		if(IsDrmCapabilityEnforced())
			{
			err = SendReceive(EManagerGetDir1, TIpcArgs(&aPath, &entryMaskPckg, &sortKeyPckg, &transferPtr));
			}
		else
			{
			err = SendReceive(ENoEnforceManagerGetDir1, TIpcArgs(&aPath, &entryMaskPckg, &sortKeyPckg, &transferPtr));
			}


		if(err == KErrOverflow)
			{
			// Find out the length required to receive the directory list
			lengthPckg.Copy(transferPtr.Left(lengthPckg.MaxLength()));	
			}
		else if(err == KErrNone)
			{
			// read in the attribute values from the buffer
			RDesReadStream entryListStream(transferPtr);
			CleanupClosePushL(entryListStream);
			aEntryList = ContentAccess::CDirStreamable::NewL(entryListStream);
			CleanupStack::PopAndDestroy(&entryListStream);
			}
		CleanupStack::PopAndDestroy(transferBuffer);	
		}
	return err;
	}


EXPORT_C TInt RRtaManager::GetDirL(const TDesC& aPath, TUint aEntryMask, TUint aEntrySortKey, CDir*& aEntryList, CDir*& aFileList) const
	{
	TInt err = KErrOverflow;
	TPckg<TUint> entryMaskPckg(aEntryMask);
	TPckg<TUint> sortKeyPckg(aEntrySortKey);
	
	// start off with an arbitary size receive buffer
	TInt length = 50 * sizeof(TEntry);
	TPckg <TInt> lengthPckg(length);

	TInt lengthA = length;
	TInt lengthB = length;
		
	// Pack the entry mask and sort key into the one descriptor
	// since we can only pass across four arguements
	HBufC8* doublePackage = HBufC8::NewLC(2* sortKeyPckg.MaxLength());
	doublePackage->Des().Copy(entryMaskPckg);
	doublePackage->Des().Append(sortKeyPckg);	
	
	while(err == KErrOverflow)
		{
		HBufC8* transferBufferA = HBufC8::NewLC(lengthA);
		TPtr8 transferPtrA = transferBufferA->Des();

		HBufC8* transferBufferB = HBufC8::NewLC(lengthB);	
		TPtr8 transferPtrB = transferBufferB->Des();

		if(IsDrmCapabilityEnforced())
			{
			err = SendReceive(EManagerGetDir2, TIpcArgs(&aPath, doublePackage, &transferPtrA, &transferPtrB));
			}
		else
			{
			err = SendReceive(ENoEnforceManagerGetDir2, TIpcArgs(&aPath, doublePackage, &transferPtrA, &transferPtrB));
			}


		if(err == KErrOverflow)
			{
			// If B has something in it then A must have succeeded
			// else it must have been A that overflowed
			if(transferPtrB.Length() > 0)
				{
				// Find out the length required to receive the directory list
				lengthPckg.Copy(transferPtrB.Left(lengthPckg.MaxLength()));		
				lengthB = length;
				}
			else
				{
				// Find out the length required to receive the directory list
				lengthPckg.Copy(transferPtrA.Left(lengthPckg.MaxLength()));						
				lengthA = length;
				}
				
			}
		else if(err == KErrNone)
			{
			// read in the attribute values from the buffer
			RDesReadStream entryListStream(transferPtrA);
			CleanupClosePushL(entryListStream);
			aEntryList = ContentAccess::CDirStreamable::NewL(entryListStream);
			CleanupStack::PopAndDestroy(&entryListStream);
			
			// put aEntryList on the cleanupstack in case subsequent operations fail
			CleanupStack::PushL(aEntryList);

			// read in the attribute values from the buffer
			entryListStream.Open(transferPtrB);
			CleanupClosePushL(entryListStream);
			aFileList = ContentAccess::CDirStreamable::NewL(entryListStream);
			CleanupStack::PopAndDestroy(&entryListStream);
			
			// Finished so can take aEntryList off the cleanupstack
			CleanupStack::Pop(aEntryList);
			}
		CleanupStack::PopAndDestroy(2, transferBufferA);	// TransferBufferA, TransferBufferB
		}
	CleanupStack::PopAndDestroy(doublePackage);
	return err;
	}


EXPORT_C TInt RRtaManager::GetDirL(const TDesC& aPath, TUidType aEntryUid, TUint aEntrySortKey, CDir*& aFileList) const
	{
	TInt err = KErrOverflow;
	TPckg<TUidType> entryUidPckg(aEntryUid);
	TPckg<TUint> sortKeyPckg(aEntrySortKey);
	
	// start off with an arbitary size receive buffer
	TInt length = 50 * sizeof(TEntry);
	TPckg <TInt> lengthPckg(length);
	
	while(err == KErrOverflow)
		{
		HBufC8* transferBuffer = HBufC8::NewLC(length);
		TPtr8 transferPtr = transferBuffer->Des();
	
		if(IsDrmCapabilityEnforced())
			{
			err = SendReceive(EManagerGetDir1, TIpcArgs(&aPath, &entryUidPckg, &sortKeyPckg, &transferPtr));
			}
		else
			{
			err = SendReceive(ENoEnforceManagerGetDir1, TIpcArgs(&aPath, &entryUidPckg, &sortKeyPckg, &transferPtr));
			}


		if(err == KErrOverflow)
			{
			// Find out the length required to receive the directory list
			lengthPckg.Copy(transferPtr.Left(lengthPckg.MaxLength()));	
			}
		else if(err == KErrNone)
			{
			// read in the attribute values from the buffer
			RDesReadStream entryListStream(transferPtr);
			CleanupClosePushL(entryListStream);
			aFileList = ContentAccess::CDirStreamable::NewL(entryListStream);
			CleanupStack::PopAndDestroy(&entryListStream);
			}
		CleanupStack::PopAndDestroy(transferBuffer);	
		}
	return err;
	}


EXPORT_C TInt RRtaManager::GetAttribute (const ContentAccess::TVirtualPathPtr& aVirtualPath, TInt aAttribute, TInt &aValue) const
	{
	TPckg <TInt> attributePckg(aAttribute);
	TPckg <TInt> valuePckg(aValue);			
	return SendReceive(EManagerGetAttribute, TIpcArgs(&aVirtualPath.URI(), &aVirtualPath.UniqueId(), &attributePckg, &valuePckg));	
	}
	
EXPORT_C TInt RRtaManager::GetAttributeSetL(const ContentAccess::TVirtualPathPtr& aVirtualPath, RAttributeSet &aAttributeSet) const
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
		err = SendReceive(EManagerGetAttributeSet, TIpcArgs(&aVirtualPath.URI(), &aVirtualPath.UniqueId(), &transferPtr));	
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
		
EXPORT_C TInt RRtaManager::GetStringAttribute (const ContentAccess::TVirtualPathPtr& aVirtualPath, TInt aAttribute, TDes &aValue) const
	{
	TPckg <TInt> attributePckg(aAttribute);
	return SendReceive(EManagerGetStringAttribute, TIpcArgs(&aVirtualPath.URI(), &aVirtualPath.UniqueId(), &attributePckg, &aValue));
	}
		
EXPORT_C TInt RRtaManager::GetStringAttributeSetL(const ContentAccess::TVirtualPathPtr& aVirtualPath, RStringAttributeSet &aStringAttributeSet) const
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
		err = SendReceive(EManagerGetStringAttributeSet, TIpcArgs(&aVirtualPath.URI(), &aVirtualPath.UniqueId(), &transferPtr));	
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

