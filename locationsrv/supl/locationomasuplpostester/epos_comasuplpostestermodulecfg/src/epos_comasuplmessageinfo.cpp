/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Class to hold messages read from message file.
*
*/

#include "epos_comasuplmessageinfo.h"
#include "epos_comasuplPosPayload.h"

// ---------------------------------------------------------------------------
// Named constructor
// ---------------------------------------------------------------------------
//
COMASuplMessageInfo* COMASuplMessageInfo::NewL(TOMASuplOperationType aOpType
						, COMASuplPosPayload& aPayload)
	{
	COMASuplMessageInfo* self = new(ELeave)COMASuplMessageInfo(aOpType);
	CleanupStack::PushL(self);
	self->ConstructL(aPayload);
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
COMASuplMessageInfo::~COMASuplMessageInfo()
	{
	delete iPayload;
	}

COMASuplMessageInfo::COMASuplMessageInfo(TOMASuplOperationType aOpType)
									
	{
	iOpType = aOpType;
	}

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void COMASuplMessageInfo::ConstructL(COMASuplPosPayload& aPayload)
	{
	iPayload = static_cast<COMASuplPosPayload*>(aPayload.CloneL());
	}

