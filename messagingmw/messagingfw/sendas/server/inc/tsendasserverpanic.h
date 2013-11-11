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

#ifndef __TSENDASSERVERPANIC_H__
#define __TSENDASSERVERPANIC_H__

#include <e32std.h>

/** Sendas server panic class
@internalComponent
@released
*/
class TSendAsServerPanic
	{
public:
	enum TSendAsServerPanics
		{
		ESendAsBadDescriptor						= 0,
		ESendAsAttachmentBadState					= 1
		};
		
	static void Panic(TInt aPanic);
	};

#endif // __TSENDASSERVERPANIC_H__
