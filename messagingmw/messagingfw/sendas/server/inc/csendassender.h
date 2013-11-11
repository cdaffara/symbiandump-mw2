// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CSENDASSENDER_H__
#define __CSENDASSENDER_H__

#include <e32base.h>
#include <msvapi.h>

#if (defined SYMBIAN_USER_PROMPT_SERVICE)
#include <ups/upsclient.h>
using namespace UserPromptService;
#endif

class MSendAsSenderObserver;
class CBaseMtm;
class CMsvOperation;
class CMsvEntrySelection;

/**
Objects of this class are used to track the progress of a send operation.
They are created and owned by CSendAsMessage sub-session objects.

When a background send is requested, ownership is transferred to the sendas
server. This allows the session to be closed before the background send 
operation has completed.

@internalComponent
@released
*/
class CSendAsSender : public CActive
	{
public:
	static CSendAsSender* NewL(MSendAsSenderObserver& aObserver);
	virtual ~CSendAsSender();
	//
	void SendMessage();
	void SendMessage(const TSecurityInfo& aSecurityInfo, const TUid& aNotifierUid);

#if (defined SYMBIAN_USER_PROMPT_SERVICE)
	void SendMessageL(const RMessage2 aMessage, TBool aHasCapability);
#endif	
	//
	void SetClientMtm(CBaseMtm& aClientMtm);
	void ProgressL(TMsvSendOperationProgress& aProgress);
	void FinalProgress(TMsvSendOperationProgress& aProgress);
private:
	CSendAsSender(MSendAsSenderObserver& aObserver);
	void ConstructL();
	//
	void SetEntrySelectionL();
	//
	void InvokeMoveOperationL();
	void InvokeSendOperationL();
	void InvokeConfirmOperationL();
	//	
	void DeleteOperation();
	void Complete(TInt aError);
	// methods from CActive
	virtual void RunL();
	virtual void DoCancel();
	virtual TInt RunError(TInt aError);
	
private:
	enum TSendAsSenderState
		{
		ESenderIdle			=0,
		ESenderStartingConfirmedSend,
		ESenderStartingUnconfirmedSend,
		ESenderConfirming,
		ESenderMoving,
		ESenderSending,
		ESenderDone,
		ESenderAuthoriseSend
		};
private:
	MSendAsSenderObserver*		iObserver;
	TSendAsSenderState			iState;
	//
	RNotifier					iNotifier;
	TUid                        iNotifierUid;
	HBufC8* 					iBuffer;
	TPckgBuf<TInt> 				iNotifierResult;
	CBaseMtm*					iClientMtm;
	CMsvOperation*				iOperation;
	CMsvEntrySelection*			iSelection;
	TMsvSendOperationProgress	iFinalProgress;
	TSecurityInfo				iSecurityInfo;
	TMsvEntry					iEntry;
	
	/** For interaction with the UPS Server */
#if (defined SYMBIAN_USER_PROMPT_SERVICE)	
	RUpsSession 		 		iUpsSession;
	RUpsSubsession 				iUpsSubsession;
	TUpsDecision				iDecision;
#endif	
	};

#endif	// __CSENDASSENDER_H__
