/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  TNE Inline 
*
*/



#ifndef __TNECOMMON_INL__
#define __TNECOMMON_INL__


inline void TTNEPanic::Panic(TInt aPanic)
    {
    _LIT(KTNEPanicCategory, "THUMBNAIL ENGINE");

    User::Panic(KTNEPanicCategory, aPanic);
    }

#endif //__TNECOMMON_INL__
