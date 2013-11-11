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
//

#ifndef __CMSVCHANGEBUFFER_H__
#define __CMSVCHANGEBUFFER_H__

//**********************************
// TMsvChangeRecord
//**********************************

class CMsvChangeRecord : public CBase
/**
@internalComponent
@released
*/
	{
public:
	static CMsvChangeRecord* NewL(const TDesC8& aChange, TUint32 aNotifySequence);
	~CMsvChangeRecord();
	//
	inline const TDesC8& Change() const;
	inline TUint32 NotifySequence() const;
	//
private:
	CMsvChangeRecord(TUint32 aNotifySequence);
	void ConstructL(const TDesC8& aChange);
	//
private:
	HBufC8* iChange;
	TUint32 iNotifySequence;
	};

//**********************************
// CMsvChangeBuffer
//**********************************

const TInt KMsvChangeBufferSize=16;
const TInt KMsvChangeBufferGranularity=16;

class CMsvChangeBuffer : public CBase
/**
@internalComponent
@released
*/
	{
public:
	static CMsvChangeBuffer* NewL();
	~CMsvChangeBuffer();
	//
	inline TBool IsEmpty() const;
	inline TUint32 NotifySequence() const;
	//
	void InL(const TDesC8& aChange);
	const TDesC8& Next(TUint32& aNotifySequence);
	void Out();
private:
	CMsvChangeBuffer();
	void ConstructL();
	void ShrinkArray();
	void GrowArrayL();
private:
	TInt iInPos;
	TInt iOutPos;
	CArrayPtrFlat<CMsvChangeRecord>* iArray;
	TUint iNotifySequence;
	};

#include "CMsvChangeBuffer.inl"

#endif // __CMSVCHANGEBUFFER_H__
