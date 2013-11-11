/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: muiudomainpan  declaration
*
*/




#ifndef __MUIUDOMAINPAN_H__
#define __MUIUDOMAINPAN_H__

//  INCLUDES
#include <e32base.h>

// DATA TYPES
enum TMuiuDomainPanic
	{
	//
	// --- Operation watcher ---
	EMuiuOperationWatcherUnknownOperation,
	EMuiuOperationWatcherNoCompletedOperations,
	//
	// --- Editor watcher ---
	EMuiuEditorWatcherActiveInFinalProgress,
	EMuiuEmbeddedWatcherDocumentAlreadyExists,
	EMuiuEmbeddedWatcherDocumentShouldNotStart,
	//
	// --- Message Iterator ---
	EMuiuMsgIteratorNoPreviousMessage,
	EMuiuMsgIteratorNoNextMessage,
	EMuiuMsgIteratorInvalidCurrentIndex,
	EMuiuMsgIteratorEntryHasNoParent,
	EMuiuMsgIteratorParentEntryNull,
	EMuiuMsgIteratorParentEntryMoved,
	EMuiuMsgIteratorParentEntryDeleted,
	EMuiuMsgIteratorChildEntryNotFound,
	//
	// --- Single Op Watcher ---
	EMuiuSingleOpWatcherAlreadyActive,
	EMuiuSingleOpWatcherOperationAlreadySet,
	EMuiuSingleOpWatcherOperationNull,
	//
	// --- Operation Wrapped Dialog ---
	EMuiuOpPrgNullOperation,
	EMuiuOpPrgOperationAlreadySet,
	EMuiuOpPrgBadLocalOpType,
	//
	// --- MTM Store ---
	EMuiuMtmStoreNullMtmPtr,
	EMuiuMtmStoreNullMtmUiPtr,
	EMuiuMtmStoreNullTimerPtr,
	EMuiuMtmStoreMissingMtm,
	EMuiuMtmStoreNegativeCounterValue
	};

GLREF_C void Panic( TMuiuDomainPanic aPanic );

#endif //__MUIUDOMAINPAN_H__


// End of File
