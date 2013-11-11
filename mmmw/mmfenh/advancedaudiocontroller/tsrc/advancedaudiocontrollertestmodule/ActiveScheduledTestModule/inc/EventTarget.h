/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Active Scheduler Test Module
*
*/

/*

  Author: Daniel Kurejwowski, dkurejwowski@dextratech.com
  Version: 2.0

*/

#ifndef __EVENT_TARGET
#define __EVENT_TARGET

#include <e32std.h>
#include <e32base.h>
#include "Parameters.h"

class MEventTarget
	{
public:
	virtual TInt ExecuteL(CParameters *aParams)=0;
	};

#endif	//__EVENT_TARGET
