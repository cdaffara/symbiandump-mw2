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

#ifndef __CMSVATTRIBUTEMANAGER_H__
#define __CMSVATTRIBUTEMANAGER_H__

#include <e32base.h>

class CMsvAttachment;
class RReadStream;
class RWriteStream;

class CDesC8Attribute : public CBase
	{
public:
	CDesC8Attribute();
	~CDesC8Attribute();

public:
	TUid iUid;
	HBufC8* iAttribute;
	};

class TIntAttribute
	{
public:
	TUid iUid;
	TInt iAttribute;
	};

class CMsvAttributeManager : public CBase
	{
public:
	static CMsvAttributeManager* NewL(CMsvAttachment& aAttachment);
	~CMsvAttributeManager();
	
	void CloneL(CMsvAttributeManager& aAttributeManager);

	void InternalizeL(RReadStream& aStream);
	void ExternalizeL(RWriteStream& aStream) const;

	void SetDesC8AttributeL(TUid aAttributeId, const TDesC8& aAttribute);
	TInt GetDesC8Attribute(TUid aAttributeId, TPtrC8& aAttribute) const;
	void RemoveDesC8Attribute(TUid aAttributeId);
	
	void SetIntAttributeL(TUid aAttributeId, TInt aAttribute);
	TInt GetIntAttribute(TUid aAttributeId, TInt& aAttribute) const;
	void RemoveIntAttribute(TUid aAttributeId);

private:
	CMsvAttributeManager(CMsvAttachment& aAttachment);
	
	TInt FindDesC8Attribute(TUid aUid) const;
	TInt FindIntAttribute(TUid aUid) const;
	
private:
	CMsvAttachment& iAttachment;
	RPointerArray<CDesC8Attribute> iDesC8Attributes;
	RArray<TIntAttribute> iIntAttributes;
	};
	
#endif // __CMSVATTRIBUTEMANAGER_H__
