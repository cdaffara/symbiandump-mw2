// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "msvtestutils.h"
#include <msvstd.h>
#include "pigeonservermtm.h"
#include "msvtestutils.h"


class CScheduleTestActive : public CActive, public MMsvSessionObserver
	{
	public:

		static CScheduleTestActive* CScheduleTestActive::NewL(CMsvTestUtils& aTest);
		void TransferCommandSyncL(CPigeonServerMtm::TPigOperation aCommand, const CMsvEntrySelection& aSelection, TBool aStopAtRunL = EFalse);
		~CScheduleTestActive();
		void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*);
		
	//	TTime LastTime();
	private:
		CScheduleTestActive::CScheduleTestActive(CMsvTestUtils& aTest);
		//CScheduleTestActive(CTestUtils& aTest);
		void RunL();
		void DoCancel() {};

		CPigeonServerMtm::TPigOperation iCommand;
		CMsvTestUtils&					iTest;
		CMsvEntrySelection*				iSelection;
		CMsvSession*					iSession;
		TBool							iStopAtRunL;
	};

class CSchSendTest : public CMsvTestUtils, public MMsvSessionObserver
	{
	public:
		static CSchSendTest* NewL(RTest& aTest);
		~CSchSendTest();

		void StartL();
		void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*) {};
	
	protected:

		void Panic(TInt aPanic);
		CSchSendTest(RTest& aTest);
		void ConstructL();

		void ShowMenuL();
		void RunAutoL();

		void TestSchedulingMessageL();

		TMsvEntry CreateMessageLC(TMsvId aDestFolder, TTime aSendTime, TBool aOffPeak = EFalse);

		//From CMsvTestUtils
		void InstantiateClientMtmsL(){};
		void InstantiateServerMtmsL(){};
		void DeleteServicesL(){};
		void CreateServicesL(){};
		void FindExistingServicesL(){};
		void InstallMtmGroupsL();
		void CreateServerMtmRegsL();
	
	private:
	
		CMsvEntrySelection*		iSelection;
		CScheduleTestActive*	iSchTestActive;
	
	protected:

		TInt iCurrentTest;
	};
