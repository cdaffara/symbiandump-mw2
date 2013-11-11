// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
    
#if !defined (__STARTER_H__)
#define __STARTER_H__

#include <eiksrvc.h>
#include <apgcli.h>
#include <bautils.h>

class CThreadWatcher;
class CStarter: public CActive
	{
	public:
		static CStarter* NewL();
		~CStarter();
		void DoCancel();
		void RunL();
		void Start();
		void Done(TBool aSuccess);
		void RestartMonitoredThreadL(TInt appType, TThreadId& aThreadId, TDesC& aExeFileName, TUint32 aAppUid, TBool aViewless);
	protected: //from CActive
	 	virtual TInt RunError(TInt aError);
	private:
		void ConstructL();
		CStarter();
	private:
		void StartSplashScreenL();
		void Connect();
		void LaunchNextStartupAppL();
		void LaunchNextExtrasL();
		void RequestNotify();
	private:

		enum
			{
			ESplashScreen,
			EStarting,
			EConnected,
			EStart,
			EWait,
			EStartExtras,
			ESplashScreenEnd
			} iStage;

		RSemaphore iSema;
		REikAppUiSession iAppUiSession;	// session to the appui session
		
		RApaLsSession iLsSess;			// session to the app arc server
		RFs iFsSession;					// file server seesion
		RResourceFile iResourceFile;	// resource file describing the startup 

		HBufC8*	iExtrasList;		// list of the applications to be launched
		TResourceReader iExtrasListReader;	// object encapsulation the application list
		TInt iExtrasCount;		// count of application remaining to be launched
		
		HBufC8*	iStartupList;		// list of the applications to be launched
		TResourceReader iStartupListReader;	// object encapsulation the application list
		TInt iStartupCount;		// count of application remaining to be launched

		RArray<TPtrC> iCmdLineArgs;	// Command line args to be freed at clean-up time

		HBufC* iSplashExe;				// full path to the splash screen program

#if defined(__WINS__)
		RLibrary iLib;
		RThread iSplash;
#else
		RProcess iSplash;
#endif
		TBool	iSplashStarted;			//
		TSglQue<CThreadWatcher> *iQueHeader;	// linked list of thread watchers
		TInt iNrSemaphore;
		TInt iWaiting;
		TUid iApplicationInFocus; //app in focus when startup is done
	};
#endif
