// Copyright (c) 2004-2010 Nokia Corporation and/or its subsidiary(-ies).
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
 @internalComponent
 @released
*/

#ifndef __RIGHTSOBJECT_H__
#define __RIGHTSOBJECT_H__

#include <f32file.h>
#include <caf/cafplatform.h>

class RReadStream;
class RWriteStream;

using namespace ContentAccess;

namespace ReferenceTestAgent
	{
	class CIntent;
	
	class CRightsObject : public CBase
		{
	public:
		static CRightsObject* NewL();
		IMPORT_C static CRightsObject* NewL(RReadStream& aStream, TBool aHaveDeleteKey);
		virtual ~CRightsObject();

		IMPORT_C const TDesC8& Cid();
		IMPORT_C TInt EvaluateIntent(TIntent aIntent);
		IMPORT_C void ExecuteIntentL(TIntent aIntent);
		IMPORT_C void MergeL(CRightsObject& aRightsObject);
		IMPORT_C void WriteDescription(RFile& aFile);
		IMPORT_C void ExternalizeL(RWriteStream& aStream);
		IMPORT_C void SetDeleteKeyL(const TDesC8& aDeleteKey);
		IMPORT_C const TDesC8& DeleteKey();

		void AddIntentL(CIntent* aIntent);
		void AddVersionL(const TDesC8& aVersion);
		void AddUidL(const TDesC8& aUid);
		void AddKeyValueL(const TDesC8& aKeyValue);
		void AddCountL(const TDesC8& aCount);
	private:
		CRightsObject();
		CRightsObject(TBool aForwardLock, TBool aPending, TInt aPendingDelay);
		void ConstructL(const TDesC8& aCid);
		void InternalizeL(RReadStream& aStream, TBool aHaveDeleteKey);

		TBool Pending();
		TBool ForwardLock();
		TInt PendingDelay();
		const RStreamablePtrArray <CIntent>& IntentArray() const;
		
	private:
		RStreamablePtrArray <CIntent> iIntents;
		HBufC8* iCid;
		
		TBool iForwardLock;
		TBool iPending;
		TInt iPendingDelay;
		HBufC8* iDeleteKey;
		HBufC8* iVersion;
		HBufC8* iKeyValue;
		HBufC8* iCount;
		};
	}

#endif

