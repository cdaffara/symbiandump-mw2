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


#ifndef __CMTF_TEST_ACTION_POP3_POPULATE_H__
#define __CMTF_TEST_ACTION_POP3_POPULATE_H__




#include "CMtfTestActionPop3TopBase.h"



_LIT(KTestActionPop3Populate,"Pop3TopPopulate");


class CMtfTestActionPop3Populate : public CMtfTestActionPop3TopBase
	{

	public:
		static CMtfTestAction* NewL( CMtfTestCase& aTestCase, CMtfTestActionParameters* aActionParameters );
		virtual ~CMtfTestActionPop3Populate(); 
		virtual void ExecuteActionL();
	private:
		CMtfTestActionPop3Populate( CMtfTestCase& aTestCase );
	};


#endif // __CMTF_TEST_ACTION_POP3_POPULATE_H__
