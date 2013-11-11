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


#ifndef __CMTFTESTACTIONSENDASCHECKMULTIPLERECIPIENTFLAG_H__
#define __CMTFTESTACTIONSENDASCHECKMULTIPLERECIPIENTFLAG_H__

//System Include
#include <msvstore.h>
#include <badesca.h>
#include <msvapi.h>
#include <miuthdr.h>

//User Include
#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionSendAsCheckMultipleRecipientFlag,"SendAsCheckMultipleRecipientFlag");
/*@}*/

/**
 CheckMultipleRecipientFlag Test Action is intended verify if the Multiple Recipient Flag has been set
@internalTechnology
*/
class CMtfTestActionSendAsCheckMultipleRecipientFlag : public CMtfSynchronousTestAction
	{
public:
	virtual ~CMtfTestActionSendAsCheckMultipleRecipientFlag(); 

	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
								CMtfTestActionParameters* ActionParameters);
								
	virtual void ExecuteActionL();

private:
	CMtfTestActionSendAsCheckMultipleRecipientFlag(CMtfTestCase& aTestCase);
	
	void ObtainRecipientsL(CMsvEntry* aMsvEntry, CDesC16ArrayFlat&  aRecipientArray);
	
	TInt CheckFlag (CMsvEntry& aMsgEntry);
	};


#endif //__CMTFTESTACTIONSENDASCHECKMULTIPLERECIPIENTFLAG_H__