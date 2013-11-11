/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Definition of clinet keys. Used by restore watcher and*
*/


#ifndef CLIENTKEYWATCHERKEYS_H
#define CLIENTKEYWATCHERKEYS_H

// SYSTEM INCLUDE
#include <e32def.h>
#include <e32cmn.h>

const TUid KPSRestoreWatcherCategory = { 0x200009F5 }; // Gategory
const TUint KPSRestoreWatcherClientsKey = 0x00000001;  // Observed key

_LIT( KRestoreFile, ":\\private\\200009F5\\restoredone" ); // HARCODED PATH!

#endif /*CLIENTKEYWATCHERKEYS_H*/
