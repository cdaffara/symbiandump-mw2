// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CMsvServerOperation
// 
//


inline TMsvOp CMsvServerOperation::Id() const
	{
	return iId;
	}

inline TUid CMsvServerOperation::MtmUid() const
	{
	return iMtmUid;
	}

inline TMsvId CMsvServerOperation::ServiceId() const
	{
	return iServiceId;
	}

inline TMsvId CMsvServerOperation::SessionId() const
	{
	return iSessionId;
	}

inline TMsvOperationState CMsvServerOperation::State() const
	{
	return iState;
	}

inline void CMsvServerOperation::SetState(TMsvOperationState aState)
	{
	iState=aState;
	}

