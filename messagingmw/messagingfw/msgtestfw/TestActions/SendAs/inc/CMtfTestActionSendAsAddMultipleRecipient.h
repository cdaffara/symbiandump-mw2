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

#ifndef __CMTFTESTACTIONSENDASADDMULTIPLERECIPIENT_H__
#define __CMTFTESTACTIONSENDASADDMULTIPLERECIPIENT_H__


#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionSendAsAddMultipleRecipient,"SendAsAddMultipleRecipient");
/*@}*/

/**
SendAsAddMultipleRecipient Test Action is intended to add one more recipient to the created message.
@internalTechnology
*/
class CMtfTestActionSendAsAddMultipleRecipient : public CMtfSynchronousTestAction
	{
public:
	virtual ~CMtfTestActionSendAsAddMultipleRecipient(); 
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* ActionParameters);
	virtual void ExecuteActionL();

private:
	CMtfTestActionSendAsAddMultipleRecipient(CMtfTestCase& aTestCase);
	};


#endif //__CMTFTESTACTIONSENDASADDMULTIPLERECIPIENT_H__