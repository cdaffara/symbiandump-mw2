// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32std.h>
#include <tmsvsystemprogress.h>


EXPORT_C TMsvSystemProgress::TMsvSystemProgress() : iVersion(1), iErrorCode(KErrNone), iId(0)
	{
	}
	
EXPORT_C TMsvSystemProgress::TMsvSystemProgress(const TMsvSystemProgress& aSysProg) : iVersion(1), iErrorCode(aSysProg.iErrorCode), iId(aSysProg.iId)
	{
	}

EXPORT_C TMsvSystemProgress& TMsvSystemProgress::operator=(const TMsvSystemProgress& aSysProg) 
	{
	iErrorCode = aSysProg.iErrorCode;
	iId = aSysProg.iId;
	return *this;
	}
