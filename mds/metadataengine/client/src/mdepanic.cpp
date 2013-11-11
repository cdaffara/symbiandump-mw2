/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Panics the current thread with the
*                Nokia Metadata Engine Client DLL panic category 
*
*/


#include <e32std.h>

#include "mdepanic.h"


void TMdEPanic::Panic(TReason aReason)
    {
    _LIT( KMdEClient, "MDE CLIENT" );
	User::Panic( KMdEClient, aReason );
    }
	
