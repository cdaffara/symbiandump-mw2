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


#ifndef __CMTFTESTACTIONVERIFYRECIPIENTS_H__
#define __CMTFTESTACTIONVERIFYRECIPIENTS_H__

//System Include
#include <msvstore.h>
#include <badesca.h>
#include <msvapi.h>
#include <miuthdr.h>

//User Include
#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionVerifyRecipients,"VerifyRecipients");
/*@}*/

/**
 VerifyRecipients Test Action is intended verify the recipient address set for
 the message by comparing it with the recipient addresses provided as input to
 the Test Action.
@internalTechnology
*/
class CMtfTestActionVerifyRecipients : public CMtfSynchronousTestAction
	{
public:
	virtual ~CMtfTestActionVerifyRecipients(); 

	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
								CMtfTestActionParameters* ActionParameters);
								
	virtual void ExecuteActionL();

private:
	CMtfTestActionVerifyRecipients(CMtfTestCase& aTestCase);
	
	void ObtainRecipientsL(CMsvEntry* aMsvEntry, CDesC16ArrayFlat&  aRecipientArray);
	
	TInt VerifyRecipientsL (CMsvEntry& aMsgEntry,
							CMsvStore& aMsgStore,
							TImHeaderEncodingInfo::TFieldList recipientType,
							CDesC16Array&  aRecipientArray);
	};


#endif //__CMTFTESTACTIONVERIFYRECIPIENTS_H__