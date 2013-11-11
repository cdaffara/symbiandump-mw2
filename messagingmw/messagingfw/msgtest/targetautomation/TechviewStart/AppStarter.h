// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __APPSTARTER_H__
#define __APPSTARTER_H__

//#include <e32base.h>
#include <apgcli.h>

class CThreadWatcher;
class CStarter;

class CAppStarter : public CTimer
	{
	public:
		static void NewLD(TInt aAppType, TUint32 aAppUid, TDesC& aExeFileName, TDesC& aDllFileName, TDesC* aCmdLineArgs, TUint32 aStackSize,
							TUint32 aMinHeapSize, TUint32 aMaxHeapSize, TBool aMonitoring, TBool aViewless,
							TBool aSemaphore, TSglQue<CThreadWatcher>* aQue, RApaLsSession& aSession, CStarter* aStarter);
		virtual void RunL();
	protected: //from CActive
	 	virtual TInt RunError(TInt aError);
	private:
		CAppStarter(RApaLsSession& aSession);
		~CAppStarter();

		void StartL();
		void Done(TBool aSucces);
		void Next();
		void TryStartViewlessAppL(TApaAppInfo aInfo, TThreadId aThreadId);


		enum
			{
			EStartApp,
			EAppStarted,
			EAppFailed
			} iState;

		TInt iAppType;
		TUint32 iAppUid;
		TFullName iExeFileName;
		TFullName iDllFileName;
		TDesC* iCmdLineArgs;
		TUint32 iStackSize;
		TUint32 iMinHeapSize;
		TUint32 iMaxHeapSize;
		TBool iMonitoring;
		TBool iViewless;
		TBool iSemaphore;
		TSglQue<CThreadWatcher>* iQue;
		RApaLsSession& iSession;
		CStarter* iStarter;

		TInt iRetries;
	};
#endif //__APPSTARTER_H__
