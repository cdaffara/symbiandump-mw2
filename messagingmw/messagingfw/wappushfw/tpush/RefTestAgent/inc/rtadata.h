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

#ifndef __RTADATA_H__
#define __RTADATA_H__

#include <e32std.h>
#include <caf/caf.h>
#include "rtaclient.h"

using namespace ContentAccess;
namespace ReferenceTestAgent
	{
	class RRtaData : public RRtaClient
		{
	public:
		IMPORT_C RRtaData();
		IMPORT_C TInt Open(const TDesC& aFileName, const TDesC& aUniqueId, TUint aMode);
		IMPORT_C TInt Open(RFile& aFile, const TDesC& aUniqueId);
		IMPORT_C TInt Read(TDes8& aBuffer, TInt aLength);
		IMPORT_C TInt Read(TInt aPos, TDes8& aBuffer, TInt aLength);
		IMPORT_C TInt DataSize(TInt &aSize); 
		IMPORT_C TInt Seek (TSeek aMode, TInt &aPos) const; 
		IMPORT_C void EvaluateIntentL(TIntent aIntent);
		IMPORT_C void ExecuteIntentL(TIntent aIntent);
		IMPORT_C TInt GetAttribute (TInt aAttribute, TInt &aValue) const;
		IMPORT_C TInt GetAttributeSetL(RAttributeSet &aAttributeSet) const; 
		IMPORT_C TInt GetStringAttribute (TInt aAttribute, TDes &aValue) const; 
		IMPORT_C TInt GetStringAttributeSetL(RStringAttributeSet &aStringAttributeSet) const; 
		};
	} // namespace
#endif
