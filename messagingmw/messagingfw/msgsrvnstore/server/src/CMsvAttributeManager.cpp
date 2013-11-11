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

#include "CMsvAttributeManager.h"
#include <cmsvattachment.h>

CDesC8Attribute::CDesC8Attribute()
	{
	}
	
CDesC8Attribute::~CDesC8Attribute()
	{
	delete iAttribute;
	}

CMsvAttributeManager* CMsvAttributeManager::NewL(CMsvAttachment& aAttachment)
	{
	CMsvAttributeManager* self = new(ELeave) CMsvAttributeManager(aAttachment);
	return self;
	}

CMsvAttributeManager::CMsvAttributeManager(CMsvAttachment& aAttachment)
	: iAttachment(aAttachment)
	{
	}	

CMsvAttributeManager::~CMsvAttributeManager()
	{
	iDesC8Attributes.ResetAndDestroy();
	iIntAttributes.Reset();
	}

void CMsvAttributeManager::CloneL(CMsvAttributeManager& aAttributeManager)
	{
	iDesC8Attributes.ResetAndDestroy();
	for( TInt ii=0; ii<aAttributeManager.iDesC8Attributes.Count(); ++ii )
		{
		CDesC8Attribute* attrib = new(ELeave) CDesC8Attribute();
		CleanupStack::PushL(attrib);
		CDesC8Attribute* attribToCopy = aAttributeManager.iDesC8Attributes[ii];
		attrib->iUid = attribToCopy->iUid;
		attrib->iAttribute = attribToCopy->iAttribute->AllocL();
		User::LeaveIfError(iDesC8Attributes.Append(attrib));
		CleanupStack::Pop(attrib);
		}
		
	iIntAttributes.Reset();
	for( TInt ii=0; ii<aAttributeManager.iIntAttributes.Count(); ++ii )
		{
		TIntAttribute attrib = aAttributeManager.iIntAttributes[ii];
		User::LeaveIfError(iIntAttributes.Append(attrib));
		}
	}

void CMsvAttributeManager::InternalizeL(RReadStream& aStream)
	{
	// DesC8 attributes
	iDesC8Attributes.ResetAndDestroy();
	TInt attributeCount = aStream.ReadInt32L();
	for( TInt ii=0; ii<attributeCount; ++ii )
		{
		CDesC8Attribute* attrib = new(ELeave) CDesC8Attribute();
		CleanupStack::PushL(attrib);
		attrib->iUid = TUid::Uid(aStream.ReadInt32L());
		attrib->iAttribute = HBufC8::NewL(aStream, KMaxTInt);
		User::LeaveIfError(iDesC8Attributes.Append(attrib));
		CleanupStack::Pop(attrib);
		}
		
	// Int attributes
	iIntAttributes.Reset();
	attributeCount = aStream.ReadInt32L();
	for( TInt ii=0; ii<attributeCount; ++ii )
		{
		TIntAttribute attrib;
		attrib.iUid = TUid::Uid(aStream.ReadInt32L());
		attrib.iAttribute = aStream.ReadInt32L();
		User::LeaveIfError(iIntAttributes.Append(attrib));
		}
	}
	
void CMsvAttributeManager::ExternalizeL(RWriteStream& aStream) const
	{
	// DesC8 attributes
	TInt attributeCount = iDesC8Attributes.Count();
	aStream.WriteInt32L(attributeCount);
	for( TInt ii=0; ii<attributeCount; ++ii )
		{
		CDesC8Attribute* attrib = iDesC8Attributes[ii];
		aStream.WriteInt32L(attrib->iUid.iUid);
		aStream << *(attrib->iAttribute);
		}
		
	// Int attributes
	attributeCount = iIntAttributes.Count();
	aStream.WriteInt32L(attributeCount);
	for( TInt ii=0; ii<attributeCount; ++ii )
		{
		TIntAttribute attrib = iIntAttributes[ii];
		aStream.WriteInt32L(attrib.iUid.iUid);
		aStream.WriteInt32L(attrib.iAttribute);
		}
	}
	
void CMsvAttributeManager::SetDesC8AttributeL(TUid aAttributeId, const TDesC8& aAttribute)
	{
	TInt attribIndex = FindDesC8Attribute(aAttributeId);
	if( attribIndex != KErrNotFound )
		{
		delete iDesC8Attributes[attribIndex];
		iDesC8Attributes.Remove(attribIndex);
		}
	
	CDesC8Attribute* attrib = new(ELeave) CDesC8Attribute();
	CleanupStack::PushL(attrib);
	User::LeaveIfError(iDesC8Attributes.Append(attrib));
	attrib->iUid = aAttributeId;
	attrib->iAttribute = aAttribute.AllocL();
	CleanupStack::Pop(attrib);
	}
	
TInt CMsvAttributeManager::GetDesC8Attribute(TUid aAttributeId, TPtrC8& aAttribute) const
	{
	TInt attribIndex = FindDesC8Attribute(aAttributeId);
	if( attribIndex != KErrNotFound )
		{
		aAttribute.Set(*(iDesC8Attributes[attribIndex]->iAttribute));
		return KErrNone;
		}
	
	return KErrNotFound;
	}
	
void CMsvAttributeManager::RemoveDesC8Attribute(TUid aAttributeId)
	{
	TInt attribIndex = FindDesC8Attribute(aAttributeId);
	if( attribIndex != KErrNotFound )
		{
		delete iDesC8Attributes[attribIndex];
		iDesC8Attributes.Remove(attribIndex);
		}
	}
	
TInt CMsvAttributeManager::FindDesC8Attribute(TUid aUid) const
	{
	for( TInt ii=0; ii<iDesC8Attributes.Count(); ++ii )
		{
		if( iDesC8Attributes[ii]->iUid == aUid )
			return ii;
		}
	
	return KErrNotFound;
	}
	
void CMsvAttributeManager::SetIntAttributeL(TUid aAttributeId, TInt aAttribute)
	{
	TInt attribIndex = FindIntAttribute(aAttributeId);
	if( attribIndex != KErrNotFound )
		iIntAttributes.Remove(attribIndex);
	
	TIntAttribute attrib;
	attrib.iUid = aAttributeId;
	attrib.iAttribute = aAttribute;
	User::LeaveIfError(iIntAttributes.Append(attrib));
	}
	
TInt CMsvAttributeManager::GetIntAttribute(TUid aAttributeId, TInt& aAttribute) const
	{
	TInt attribIndex = FindIntAttribute(aAttributeId);
	if( attribIndex != KErrNotFound )
		{
		aAttribute = iIntAttributes[attribIndex].iAttribute;
		return KErrNone;
		}
	
	return KErrNotFound;
	}
	
void CMsvAttributeManager::RemoveIntAttribute(TUid aAttributeId)
	{
	TInt attribIndex = FindIntAttribute(aAttributeId);
	if( attribIndex != KErrNotFound )
		iIntAttributes.Remove(attribIndex);
	}
	
TInt CMsvAttributeManager::FindIntAttribute(TUid aUid) const
	{
	for( TInt ii=0; ii<iIntAttributes.Count(); ++ii )
		{
		if( iIntAttributes[ii].iUid == aUid )
			return ii;
		}
	
	return KErrNotFound;
	}

