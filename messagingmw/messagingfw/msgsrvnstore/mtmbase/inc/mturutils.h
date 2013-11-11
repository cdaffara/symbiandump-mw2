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
//

#ifndef __MTURUTILS_H__
#define __MTURUTILS_H__

#include <msvstd.h>

class MturUtils
/** Utility class that is used to start an application to 
edit a specified message.

@publishedAll
@released
*/
{
public:
	IMPORT_C static void LaunchEditorL(TMsvId aId);
	IMPORT_C static void LaunchEditorAndWaitL(TMsvId aId);
	IMPORT_C static void LaunchEditorL(TMsvId aId, RThread& aThread, TRequestStatus& aStatus);
};

#endif
