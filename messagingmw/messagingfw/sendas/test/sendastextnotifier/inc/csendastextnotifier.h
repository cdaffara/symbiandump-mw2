// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CSENDASETEXTNOTIFIER_H__
#define __CSENDASETEXTNOTIFIER_H__

#include <twintnotifier.h>
#include <msvapi.h>

class CMsvEntrySelection;

class CSendAsTextNotifier : public CBase, public MNotifierBase2
	{
public:
	//	Construction
	static CSendAsTextNotifier* NewL();
	~CSendAsTextNotifier();
private:
	CSendAsTextNotifier();
	void ConstructL();

	//	From MNotifierBase2
	virtual void Release();
	virtual TNotifierInfo RegisterL();
	virtual TNotifierInfo Info() const;
	virtual TPtrC8 StartL(const TDesC8& aBuffer);
	virtual void StartL(const TDesC8& aBuffer, TInt aReplySlot, const RMessagePtr2& aMessage);
	virtual void Cancel();
	virtual TPtrC8 UpdateL(const TDesC8& aBuffer);

private:
	TNotifierInfo iInfo;
	RMessagePtr2  iMessage;
	TInt		  iReplySlot;

	TSecurityInfo iSecurityInfo;
	CMsvEntrySelection* iSelection;
	};
	
//**********************************
// CDummyObserver
//**********************************

class CDummyObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*) {};
	};

#endif	// __CSENDASETEXTNOTIFIER_H__


