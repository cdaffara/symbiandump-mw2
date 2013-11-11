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

#include "t_schsendutils.h"

class TMsvSchedulePackage;

class CSchSendTest : public CSchSendTestUtils
	{
	public:
		static CSchSendTest* NewL(RTest& aTest);

	protected:

		CSchSendTest(RTest& aTest);

		void ShowMenuL() {RunAutoL();}
		void RunAutoL();
		void TestPackageL();
		void TestPackageL(const TMsvSchedulePackage& aPackage);

		void TestSendErrorActionsL();
		void TestSendErrorActions1L();
		void TestSendErrorActions2L();
		void TestSendErrorActions3L();
		void TestSendErrorActions4L();
		void TestSendErrorActions5L();

		void TestDefaultL();
		void TestErrorsL();
		void TestStoringL();
		void TestRemovingL();

		void TestOffPeakTimesL();
		void TestOffPeakTimes1L();
		void TestOffPeakTimes2L();
		void TestOffPeakTimes3L();

		void TestSysAgentActionsL();

		void TestSchedulingMessagesL();
		void TestSchedulingOffPeakMessagesL();
		void TestSchedulingOldMessagesL();
		void TestSchedulingBadMessagesL();
		void TestSchedulingMessagesTwiceL();
		void TestReschedulingMessagesL();
		void TestReschedulingNonScheduledMessagesL();
		void TestDeletingMessagesL();
		void TestDeletingNonScheduledMessagesL();
		void TestCheckScheduleL();
	
		void TestScheduleSettingsL();

		TBool DoTest(void (CSchSendTest::*aTest)(), TPtrC aText);
		TBool HeapTest(void (CSchSendTest::*test)(), TPtrC aText);

		const TSchSendTestProgress& TestL(TSchSendTestOperation aOp, CMsvEntrySelection& aSel, TBool aBool = EFalse);
	};
