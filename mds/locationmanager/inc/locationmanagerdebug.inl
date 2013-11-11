/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Common trace-information file.
*
*/


inline void MemPrint()
    {
    TUint32 id = RThread().SecureId().iId;
    TInt allocSize = 0;
    User::Heap().AllocSize( allocSize );
    _LIT( KMsg, "[MEMORY] ID: %d\t AllocSize: %d" );
    RDebug::Print( KMsg, id, allocSize );
    }


