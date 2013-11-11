/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:
*
*/


#ifndef __MDCCOMMON_PAN__
#define __MDCCOMMON_PAN__

/** Panic Category */
_LIT( KMetadataEngine, "MetadataEngine" );

/** CMdEServer panic codes */
enum TMetadataPanic
    {
    EBadRequest = 100,
    /*EBadDescriptor = 101,*/
    ESrvCreateServer = 102,
    ECreateTrapCleanup = 103,
    /*EBadState = 104,*/
    EDatabaseFault = 105,
    /*EInternal = 106,
    EInternalSchemaCorrupt = 107,
    EInternalDbCorrupt = 108,
    EInternalHierarchyCorrupt = 109,
    EInternalImportFault = 110,
    EBadFormatInQueryCriteria = 111,
	EInternalNotifierCollision = 112,
	EServerCommunication = 113,
	EInternalExportFault = 114,*/
	EServerBackupOrRestore = 115
    };

#endif // __MDCCOMMON_PAN__

// End of File

