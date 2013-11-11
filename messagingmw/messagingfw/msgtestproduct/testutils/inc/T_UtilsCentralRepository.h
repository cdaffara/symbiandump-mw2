// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This is the header file for CT_MsgUtilsCentralRepository class and
// contains the common utility functions that use the Central Repository
// 
//

#ifndef __T_UTILS_CENTRAL_REPOSITORY_H__
#define __T_UTILS_CENTRAL_REPOSITORY_H__


// epoc includes
#include <cemailaccounts.h>

/**
Implements the utility functions related to the Central Repository
*/
class CT_MsgUtilsCentralRepository
	{	
public:
	IMPORT_C CT_MsgUtilsCentralRepository();
	~CT_MsgUtilsCentralRepository();

	// Gets the Imap service Id based on the account name
	IMPORT_C static TMsvId GetImapServiceIdL(TDes& aAccountName);

	// Gets the Pop service Id based on the account name
	IMPORT_C static TMsvId GetPopServiceIdL(TDes& aAccountName);
	
	// Gets the Smtp service Id based on the account name
	IMPORT_C static TMsvId GetSmtpServiceIdL(TDes& aAccountName);

	// Gets the default Smtp service Id
	IMPORT_C static TMsvId GetDefaultSmtpServiceIdL();
	
	// Checks if the specified Imap account is present in the Central Repository
	IMPORT_C static TBool GetImapAccountL( TDes& aAccountName, TImapAccount& aImapAccount );

	// Checks if the specified Pop account is present in the Central Repository
	IMPORT_C static TBool GetPopAccountL( TDes& aAccountName, TPopAccount& aPopAccount );
	
	// Checks if the specified Smtp account is present in the Central Repository
	IMPORT_C static TBool GetSmtpAccountL(TDes& aAccountName, TSmtpAccount& aSmtpAccount);
	};
#endif //__T_UTILS_CENTRAL_REPOSITORY_H__
