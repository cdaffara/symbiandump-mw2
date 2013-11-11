// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// TMSVSYSTEMPROGRES.H
// 
//

#ifndef __TMSVSYSTEMPROGRESS_H__
#define __TMSVSYSTEMPROGRESS_H__

#include <msvstd.h>

const TUint KUIDMsgMsvSystemProgress = 0x1028308F;

/**
Progress information that is defined by and understood by the system as well as client MTM's

@publishedAll
@released
*/
class TMsvSystemProgress
	{
public:
	IMPORT_C TMsvSystemProgress();
	IMPORT_C TMsvSystemProgress(const TMsvSystemProgress& aSysProg);
	IMPORT_C TMsvSystemProgress& operator=(const TMsvSystemProgress& aSysProg);
	
public:
	/** the version number of class */
	const TInt			iVersion;
	/** Operation error code.*/
	TInt				iErrorCode; 
	/** the id of an entry in an operation*/
	TMsvId				iId;	
	};

#endif // #define __TMSVSYSTEMPROGRESS_H__
