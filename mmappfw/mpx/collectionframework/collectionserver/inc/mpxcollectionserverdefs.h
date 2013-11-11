/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Collection Server common definitions
*
*/


#ifndef MPXCOLLECTIONSERVERDEFS_H
#define MPXCOLLECTIONSERVERDEFS_H


#include<e32base.h>
#include<apmstd.h>
#include<mpxcollectionframeworkdefs.h>

// Server name
_LIT(KMPXCollectionServerName,"MPXCollectionServer");
_LIT(KMPXCollectionServerImg,"mpxcollectionserver");        // DLL/EXE name
// UID
const TUid KMPXCollectionServerUid3={0x101FFC31};  
// Serverversion number
const TUint KMPXCollectionServerMajorVersionNumber=0;
const TUint KMPXCollectionServerMinorVersionNumber=1;
const TUint KMPXCollectionServerBuildVersionNumber=1;

#endif  // MPXCOLLECTIONSERVERDEFS_H

