// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __PUSHENTRYPANIC_H__
#define __PUSHENTRYPANIC_H__

enum TPushEntryPanic	
	{
	/** Null Ids for entries not valid (Service, &Msg). */
	EPushEntryNullMsgId,
	/** Only save messages under the Service Entry. */
	EPushEntryServiceIdMismatch,
	/** Not a WAP Push  MTM. */
	EPushEntryWrongMTMtype,
	/** iBioType value in TMsvEntry don't match Uid for Push Entry class. */
	EPushEntryWrongMsgtype
	};

/** Panic name and codes */
_LIT(KPushPanicMoniker, "PUSHENTRY");

#endif
