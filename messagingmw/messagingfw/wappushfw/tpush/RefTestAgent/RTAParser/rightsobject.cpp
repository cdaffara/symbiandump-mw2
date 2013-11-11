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

#include <s32strm.h>
#include <caf/caf.h>
#include "rightsobject.h"
#include "intent.h"

using namespace ReferenceTestAgent;


CRightsObject* CRightsObject::NewL()
	{
	CRightsObject* self = new (ELeave) CRightsObject();
	return self;
	}

EXPORT_C CRightsObject* CRightsObject::NewL(RReadStream& aStream, TBool aHaveDeleteKey)
	{
	CRightsObject* self = new (ELeave) CRightsObject;
	CleanupStack::PushL(self);
	self->InternalizeL(aStream, aHaveDeleteKey);
	CleanupStack::Pop(self);
	return self;
	}


CRightsObject::CRightsObject()
	{
	}

CRightsObject::CRightsObject(TBool aForwardLock, TBool aPending, TInt aPendingDelay)
 : iForwardLock(aForwardLock), iPending(aPending), iPendingDelay(aPendingDelay)
	{
	}

CRightsObject::~CRightsObject()
	{
	iIntents.Close();
	delete iCid;
	delete iDeleteKey;
	delete iVersion;
	delete iKeyValue;
	delete iCount;
	}

void CRightsObject::ConstructL(const TDesC8& aCid)
	{
	iCid = aCid.AllocL();
	}


EXPORT_C const TDesC8& CRightsObject::Cid()
	{
	return *iCid;
	}

EXPORT_C TInt CRightsObject::EvaluateIntent(TIntent aIntent)
	{
	TInt err = KErrCANoRights;
	if(iForwardLock)
		{
		err = KErrNone;
		}
	else if(iPending)
		{
		err = KErrCAPendingRights;
		}
	else
		{
		for(TInt i = 0; i < iIntents.Count(); i++)
			{
			err = KErrCANoPermission;		// at least one intent type 
			if(iIntents[i]->Intent() == aIntent)
				{
				err = iIntents[i]->Evaluate();
				break;
				}
			}
		}
	return err;
	}
	
EXPORT_C void CRightsObject::ExecuteIntentL(TIntent aIntent)
	{
	TInt err = KErrCANoRights;
	if(!iForwardLock)
		{
		if(iPending)
			{
			User::Leave(KErrCAPendingRights);
			}
		else
			{
			for(TInt i = 0; i < iIntents.Count(); i++)
				{
				err = KErrCANoPermission; // at least one intent type
				if(iIntents[i]->Intent() == aIntent)
					{
					iIntents[i]->ExecuteL();
					return;
					}
				}
			// couldn't find matching intent
			// leave with either KErrCANoRights if there were no rights
			// or KErrCANoPermission if the speicifed intent was not found
			User::Leave(err);
			}
		}
	}

void CRightsObject::AddIntentL(CIntent* aIntent)
	{
	TBool merged = EFalse;
	for(TInt i = 0; i < iIntents.Count(); i++)
		{
		if(iIntents[i]->Intent() == aIntent->Intent())
			{
			iIntents[i]->MergeL(aIntent);
			delete aIntent;
			aIntent = NULL;
			merged = ETrue;
			break;
			}
		}
	if(!merged)
		{
		iIntents.AppendL(aIntent);	
		}
	}

EXPORT_C void CRightsObject::WriteDescription(RFile& aFile)
	{
	TBuf8 <10> delay;
	aFile.Write(_L8("\r\ncid: "));
	aFile.Write(*iCid);
	aFile.Write(_L8("\r\n"));

	TPtrC8 deleteKey(KNullDesC8);
	if (iDeleteKey)
		{
		deleteKey.Set(*iDeleteKey);
		}
	aFile.Write(_L8("delete key: "));
	aFile.Write(deleteKey);
	aFile.Write(_L8("\r\n"));

	if(iForwardLock)
		{
		aFile.Write(_L8("	 Forward Lock use enabled\r\n"));
		}
	else if(iPending)
		{
		aFile.Write(_L8("	 Rights pending, expected in "));
		delay.Num(iPendingDelay);
		aFile.Write(delay);
		aFile.Write(_L8(" seconds\r\n"));
		}
	else
		{
		for(TInt i = 0; i < iIntents.Count(); i++)
			{
			iIntents[i]->WriteDescription(aFile);
			}
		}
	}

const RStreamablePtrArray <CIntent>& CRightsObject::IntentArray() const
	{
	return iIntents;	
	}


EXPORT_C void CRightsObject::MergeL(CRightsObject& aRightsObject)
	{
	if(!iForwardLock && !aRightsObject.Pending() && !aRightsObject.ForwardLock())
		{
		iPending = EFalse;
		iPendingDelay = 0;
		for(TInt i = 0; i < aRightsObject.IntentArray().Count(); i++)
			{
			CIntent* intent = aRightsObject.IntentArray()[i]->CopyL();
			CleanupStack::PushL(intent);
			AddIntentL(intent);
			CleanupStack::Pop(intent);
			}
		}
	else
		{
		// Content is already forward locked, adding the new 
		// rights object would only introduce restritions
		// so ignore it
		}
	}


TBool CRightsObject::ForwardLock()
	{
	return iForwardLock;
	}
	
TBool CRightsObject::Pending()
	{
	return iPending;
	}
	
TInt CRightsObject::PendingDelay()
	{
	return iPendingDelay;
	}

void CRightsObject::InternalizeL(RReadStream& aStream, TBool aHaveDeleteKey)
	{
	// read Cid length
	TInt cidLength = aStream.ReadInt32L();	
	iCid = HBufC8::NewL(cidLength);
	TPtr8 cidPtr = iCid->Des();
	aStream.ReadL(cidPtr, cidLength);
	

	TInt verLength = aStream.ReadInt32L();	
	if(verLength)
		{
		iVersion = HBufC8::NewL(cidLength);
		TPtr8 verPtr = iVersion->Des();
		aStream.ReadL(verPtr, verLength);
		}
	
	TInt keyLength = aStream.ReadInt32L();	
	if(keyLength)
		{
		iKeyValue = HBufC8::NewL(keyLength);
		TPtr8 keyPtr = iKeyValue->Des();
		aStream.ReadL(keyPtr, keyLength);
		}

	TInt cntLength = aStream.ReadInt32L();	
	if(cntLength)
		{
		iVersion = HBufC8::NewL(cntLength);
		TPtr8 cntPtr = iCount->Des();
		aStream.ReadL(cntPtr, cntLength);
		}
	// Version 0 of rights.db does not have the deleteKey.
	if (aHaveDeleteKey)
		{
		if (iDeleteKey)
			{
			delete iDeleteKey;
			iDeleteKey = NULL;
			}

		TInt deleteKeyLen = aStream.ReadInt32L();
		if (deleteKeyLen)
			{
			iDeleteKey = HBufC8::NewL(deleteKeyLen);
			TPtr8 ptr8 = iDeleteKey->Des();
			aStream.ReadL(ptr8, deleteKeyLen);
			}
		}
	}
	
EXPORT_C void CRightsObject::ExternalizeL(RWriteStream& aStream)
	{
	aStream.WriteInt32L(iCid->Des().Length());
	TPtr8 cidPtr = iCid->Des();
	aStream.WriteL(cidPtr);
	if(iVersion) 
		{
		//Write the version
		aStream.WriteInt32L(iVersion->Des().Length());
		TPtr8 versionPtr = iVersion->Des();
		aStream.WriteL(versionPtr);
		}
	else
		{
		aStream.WriteInt32L(0);
		}
	if(iKeyValue) 
		{
		//Write the key value
		aStream.WriteInt32L(iKeyValue->Des().Length());
		TPtr8 keyPtr = iKeyValue->Des();
		aStream.WriteL(keyPtr);
		}
	else
		{
		aStream.WriteInt32L(0);
		}	
	if(iCount) 
		{
		//Write the count
		aStream.WriteInt32L(iCount->Des().Length());
		TPtr8 countPtr = iCount->Des();
		aStream.WriteL(countPtr);
		}
	else
		{
		aStream.WriteInt32L(0);
		}		
	TInt deleteKeyLen = iDeleteKey ? iDeleteKey->Length() : 0;
	aStream.WriteInt32L(deleteKeyLen);
	if (deleteKeyLen)
		{
		aStream.WriteL(*iDeleteKey);
		}
	}

EXPORT_C void CRightsObject::SetDeleteKeyL(const TDesC8& aDeleteKey)
	{
	if (aDeleteKey == KNullDesC8)
		{
		delete iDeleteKey;
		iDeleteKey = NULL;
		}
	else if (iDeleteKey == NULL)
		{
		iDeleteKey = aDeleteKey.AllocL();
		}
	else
		{
		User::Leave(KErrAlreadyExists);
		}
	}

EXPORT_C const TDesC8& CRightsObject::DeleteKey()
	{
	return (iDeleteKey ? *iDeleteKey : KNullDesC8());
	}

void CRightsObject::AddVersionL(const TDesC8& aVersion)
	{
	iVersion = aVersion.AllocL();
	}

void CRightsObject::AddUidL(const TDesC8& aUid)
	{
	iCid = aUid.AllocL();
	}	

void CRightsObject::AddKeyValueL(const TDesC8& aKeyValue)
	{
	iKeyValue = aKeyValue.AllocL();
	}

void CRightsObject::AddCountL(const TDesC8& aCount)
	{
	iCount = aCount.AllocL();
	}
