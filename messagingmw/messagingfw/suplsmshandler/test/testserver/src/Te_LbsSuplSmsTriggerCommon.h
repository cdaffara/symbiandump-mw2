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
// All global SUPL SMS Trigger suite consts and enums
// 
//

#if (!defined __TE_LBSSUPLSMSTRIGGERCOMMON__)
#define __TE_LBSSUPLSMSTRIGGERCOMMON__

/**
@file
@internalComponent
@prototype
*/

#include <e32base.h>

/** The UID of this unit test suite module*/
const TUid KModuleUid = {0x10283755};

/**
The ids of the errors that are simulated in the test code.
*/
enum 
	{
	ECWapBoundDatagramService_AwaitRecvDataSize,
	ECWapBoundDatagramService_RecvFrom
	};

	
#endif //__TE_LBSSUPLSMSTRIGGERCOMMON__
