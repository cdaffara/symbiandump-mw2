// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CMsvChangeBuffer.h
// CMsvChangeBuffer
// 
//

inline TBool CMsvChangeBuffer::IsEmpty() const
	{
	return iInPos==iOutPos;
	}

inline TUint32 CMsvChangeBuffer::NotifySequence() const
	{
	return iNotifySequence;
	}

//**********************************
//CMsvChangeRecord
//**********************************

inline const TDesC8& CMsvChangeRecord::Change() const
	{
	return *iChange;
	}

inline TUint32 CMsvChangeRecord::NotifySequence() const
	{
	return iNotifySequence;
	}
