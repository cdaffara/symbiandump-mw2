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
// RTA - client Reference Test Server interface
// 
//

/**
 @file 
 @test
 @internalComponent
*/

#ifndef __RTACONTENT_H__
#define __RTACONTENT_H__

#include <e32std.h>
#include <caf/caf.h>
#include "rtaclient.h"

using namespace ContentAccess;
namespace ReferenceTestAgent
	{
	class RRtaContent : public RRtaClient
		{
	public:
		IMPORT_C RRtaContent();
		IMPORT_C TInt Open(const TDesC& aFileName, TUint aMode);
		IMPORT_C TInt Open(RFile& aFile);
		IMPORT_C TInt OpenContainer(const TDesC& aUniqueId) const;
		IMPORT_C TInt CloseContainer() const;
		IMPORT_C void GetEmbeddedObjectsL(RStreamablePtrArray< CEmbeddedObject > &aArray) const;
		IMPORT_C void GetEmbeddedObjectsL (RStreamablePtrArray< CEmbeddedObject > &aArray, TEmbeddedType aType) const; 
		IMPORT_C TInt SearchL(RStreamablePtrArray< CEmbeddedObject > &aArray, const TDesC8 &aMimeType, TBool aRecursive) const;
		IMPORT_C TInt GetAttribute (const TDesC& aUniqueId, TInt aAttribute, TInt &aValue) const;
		IMPORT_C TInt GetAttributeSetL(const TDesC& aUniqueId, RAttributeSet &aAttributeSet) const; 
		IMPORT_C TInt GetStringAttribute (const TDesC& aUniqueId, TInt aAttribute, TDes &aValue) const; 
		IMPORT_C TInt GetStringAttributeSetL(const TDesC& aUniqueId, RStringAttributeSet &aStringAttributeSet) const; 
		};
	} // namespace
#endif
