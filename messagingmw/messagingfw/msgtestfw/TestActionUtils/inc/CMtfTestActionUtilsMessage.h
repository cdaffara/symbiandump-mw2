/**
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __CMTF_TEST_ACTION_UTILS_MESSAGE_H__
#define __CMTF_TEST_ACTION_UTILS_MESSAGE_H__

#include <e32base.h>
#include <msvstore.h>
#include <miuthdr.h>
#include <badesca.h>
#include <gsmumsg.h>


class CMtfTestActionUtilsMessage 
	{
public:
	static TInt VerifyBodyTextContentsL(CMsvEntry& aEntry, HBufC* aFileName );
	static TInt VerifyEmailRecipientsL(CMsvStore& aMsgStore, CDesC16Array& aRecipientArray,TImHeaderEncodingInfo::TFieldList aFieldType);
	static TInt VerifySmsRecipientsL(CMsvStore& aMsgStore,CDesC16Array&  aRecipientArray);
	static TInt VerifyObexRecipientL(CMsvEntry& aMsgEntry, CMsvStore& aMsgStore, CDesC16Array&  aRecipientArray);
	static TInt VerifyEmailSubjectL (CMsvEntry& aMsgEntry,HBufC*  aSubject);
	static TInt VerifyObexSubjectL (CMsvEntry& aEntry,HBufC* aSubject);
	static void FormatBtRecipientAddress(TDesC& aRecipient, TDes16& formattedAddr);
   ~CMtfTestActionUtilsMessage();

private:
	CMtfTestActionUtilsMessage();


private:
/*check for any member variable*/
	};


#endif//__CMTF_TEST_ACTION_UTILS_MESSAGE_H__