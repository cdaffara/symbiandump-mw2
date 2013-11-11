// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CWAPPBOOKMARK_H__
#define __CWAPPBOOKMARK_H__

#include <e32base.h>
#include <e32std.h>

class CIpAddress;

	//Class used for holding a bookmark - WAPP message can contain as many of these
	// as desired!
class CWappBookmark: public CBase
/**
@internalComponent
@released
*/
	{
	public:   // None of these are exported functions - class only used internally within DLL
		static CWappBookmark* NewLC();
		static CWappBookmark* NewL();
		~CWappBookmark();
		const TDesC& Name() const;
		const TDesC& Url() const;
		void SetNameL(const TDesC& aDes);
		void SetUrlL(const TDesC& aDes);
	private:
		void ConstructL();
		HBufC*	iBmkName;  // Name of the bookmark entry 
		CIpAddress*	iBmkUrl;	  // URL of the bookmark entry
	};

#endif // __CWAPPBOOKMARK_H__
