// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// WWW Hotlist Item Settings
// 
//

#ifndef __WWWP_H__
#define __WWWP_H__

#include <ipaddr.h>

class CWWWHotlistItem  : public CBase
/**
@internalComponent
@released
*/
	{
public:
	CWWWHotlistItem();
	~CWWWHotlistItem();
	const TDesC& Name() const;
	const TDesC& Url() const;

	void SetNameL(const TDesC& aDes);
	void SetUrlL(const TDesC& aDes);

private:
	HBufC*	iBmkName;	// Name of the bookmark entry 
	CIpAddress*	iBmkUrl;	// URL of the bookmark entry
	};


class CWWWHotlistParser : public CBase
/**
@internalComponent
@released
*/
	{
public:
	static CWWWHotlistParser* NewL(RFs& aFs);
	~CWWWHotlistParser();
	void ParseL(CParsedFieldCollection& aIacpFields);
	void ProcessL(CMsvEntry& aEntry); 

private:
	CWWWHotlistParser(RFs& aFs);
	void ConstructL();
	void StoreL(CMsvEntry& aEntry);

private:
	CArrayPtrSeg<CWWWHotlistItem>*	iHotlistItemList;   // Array of bookmarks to store
	RFs&							iFs;
	};
#endif  
