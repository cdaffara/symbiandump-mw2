/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/



/**
 @file
*/


#ifndef __CMTF_TEST_ACTION_POP3_POPULATE_ALL_H__
#define __CMTF_TEST_ACTION_POP3_POPULATE_ALL_H__




#include "CMtfTestActionPop3TopBase.h"



_LIT(KTestActionPop3PopulateAll,"Pop3TopPopulateAll");


class CMtfTestActionPop3PopulateAll : public CMtfTestActionPop3TopBase
	{

	public:
		static CMtfTestAction* NewL( CMtfTestCase& aTestCase, CMtfTestActionParameters* aActionParameters );
		virtual ~CMtfTestActionPop3PopulateAll(); 
		virtual void ExecuteActionL();
	private:
		CMtfTestActionPop3PopulateAll( CMtfTestCase& aTestCase );
	};


#endif // __CMTF_TEST_ACTION_POP3_POPULATE_ALL_H__
