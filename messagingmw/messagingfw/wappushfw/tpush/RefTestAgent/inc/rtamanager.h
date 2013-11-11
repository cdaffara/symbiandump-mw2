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

#ifndef __RTAMANAGER_H__
#define __RTAMANAGER_H__

#include <e32std.h>
#include "rtaclient.h"

class CDir;
using namespace ContentAccess;
namespace ContentAccess
	{
	class TVirtualPathPtr;
	class RAttributeSet;
	class RStringAttributeSet;
	}

namespace ReferenceTestAgent
	{
	class CRightsObject;
	class RRtaManager : public RRtaClient
		{
	public:
		// protected constructor so only derived classes can access it
		IMPORT_C RRtaManager();
		IMPORT_C TInt Connect();
		IMPORT_C TInt DeleteFile(const TDesC& aFileName) const;
		IMPORT_C TInt CopyFile(const TDesC& aSource, const TDesC& aDestination) const;
		IMPORT_C TInt CopyFile(RFile& aSource, const TDesC& aDestination) const;
		IMPORT_C TInt RenameFile(const TDesC& aOldFileName, const TDesC& aNewFileName) const;
		IMPORT_C TInt MkDir(const TDesC& aPath) const;
		IMPORT_C TInt MkDirAll(const TDesC& aPath) const;
		IMPORT_C TInt RenameDir(const TDesC& aOldPath, const TDesC& aNewPath) const;		
		IMPORT_C TInt RmDir(const TDesC& aPath) const;
		IMPORT_C TInt GetDirL(const TDesC& aPath, TUint aEntryMask, TUint aEntrySortKey, CDir*& aEntryList) const;
		IMPORT_C TInt GetDirL(const TDesC& aPath, TUint aEntryMask, TUint aEntrySortKey, CDir*& aEntryList, CDir*& aFileList) const;
		IMPORT_C TInt GetDirL(const TDesC& aPath, TUidType aEntryUid, TUint aEntrySortKey, CDir*& aFileList) const;
		IMPORT_C TInt GetAttribute (const ContentAccess::TVirtualPathPtr& aVirtualPath, TInt aAttribute, TInt &aValue) const;
		IMPORT_C TInt GetAttributeSetL(const ContentAccess::TVirtualPathPtr& aVirtualPath, RAttributeSet &aAttributeSet) const;
		IMPORT_C TInt GetStringAttribute (const ContentAccess::TVirtualPathPtr& aVirtualPath, TInt aAttribute, TDes &aValue) const;
		IMPORT_C TInt GetStringAttributeSetL(const ContentAccess::TVirtualPathPtr& aVirtualPath, RStringAttributeSet &aStringAttributeSet) const;
		};
	} // namespace
#endif
