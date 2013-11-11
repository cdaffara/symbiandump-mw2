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
* Description:  MmMtpDataProvider
*
*/


#ifndef TMMMTPDPPANIC_H
#define TMMMTPDPPANIC_H

#include <e32std.h>

/**
* define the MM data provider panic code
*/
enum TMmMTPDpPanic
    {
    EMmMTPDpNoMatchingProcessor = 0,
    EMmMTPDpObjectNull = 1,
    EMmMTPDpFormatNotSupported = 2,
    EMmMTPDpSendObjectStateInvalid = 3,
    EMmMTPDpReferencesNull = 4,
    EMmMTPDpRequestCheckNull = 5,
    EMmMTPDpHandlesNull = 6,
    EMmMTPDpUnsupportedProperty = 7,
    EMmMTPDpObjectPropertyNull = 8,
    EMmMTPDpCannotRenameDir = 9,
    };

_LIT( KMmMTPDpPanic, "MM MTP Data Provider" );

inline void Panic( TMmMTPDpPanic aCode )
    {
    User::Panic( KMmMTPDpPanic, aCode );
    }

#endif // TMMMTPDPPANIC_H
