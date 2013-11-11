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
//

// User includes
#include <t_utilscentralrepository.h>

// epoc includes
#include <msvids.h>

/**
CT_MsgUtilsCentralRepository()
Constructor
*/
EXPORT_C CT_MsgUtilsCentralRepository::CT_MsgUtilsCentralRepository()
	{
	}


/**
~CT_MsgUtilsCentralRepository()
Destructor
*/
CT_MsgUtilsCentralRepository::~CT_MsgUtilsCentralRepository()
	{
	}


/**
GetDefaultSmtpServiceIdL()
This function return the default Smtp Service Id. This method requires that a
related IMAP4 or POP3 account has been previously created in the central
repository - this is because SMTP accounts do not exist independently but only
as part of an IMAP4 or POP3 account

@return
Entry ID of the default SMTP service entry
*/
EXPORT_C TMsvId CT_MsgUtilsCentralRepository::GetDefaultSmtpServiceIdL()
	{
	CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();
	
	TSmtpAccount smtpAccount;
	smtpAccount.iSmtpService = KMsvNullIndexEntryId;
	User::LeaveIfError(emailAccounts->DefaultSmtpAccountL(smtpAccount));

	CleanupStack::PopAndDestroy(emailAccounts);
	
	return smtpAccount.iSmtpService;
	}


/**
GetImapServiceIdL()
This function returns the Service Id of the IMAP account with the specified name

@param aName
The Imap account name

@return
It returns entry Id of an IMAP service
*/
EXPORT_C TMsvId CT_MsgUtilsCentralRepository::GetImapServiceIdL(TDes& aName)
	{
	TImapAccount imapAccount;
	TMsvId imapServiceId = KMsvNullIndexEntryId;

	TBool returnValue = GetImapAccountL( aName, imapAccount );
	
	if ( returnValue )
		{
		imapServiceId = imapAccount.iImapService;
		}
	return imapServiceId;
	}


/**
GetPopServiceIdL()
This function returns the Service Id of the POP account with the specified name

@param aName
The Pop account name

@return
Returns entry Id of an POP service
*/
EXPORT_C TMsvId CT_MsgUtilsCentralRepository::GetPopServiceIdL(TDes& aName)
	{
	TPopAccount popAccount;
	TMsvId popServiceId = KMsvNullIndexEntryId;

	TBool returnValue = GetPopAccountL( aName, popAccount );
	
	if ( returnValue )
		{
		popServiceId = popAccount.iPopService;
		}
	return popServiceId;
	}

/**
GetSmtpServiceIdL()
This function returns the Service Id of the SMTP account with the specified name

@param aName
The Smtp account name

@return
Returns entry Id of an SMTP service
*/
EXPORT_C TMsvId CT_MsgUtilsCentralRepository::GetSmtpServiceIdL(TDes& aName)
	{
	TSmtpAccount smtpAccount;
	TMsvId smtpServiceId = KMsvNullIndexEntryId;

	TBool returnValue = GetSmtpAccountL(aName, smtpAccount);
	
	if (returnValue)
		{
		smtpServiceId = smtpAccount.iSmtpService;
		}
	return smtpServiceId;
	}

/**
GetImapAccountL()
Searches the IMAP account with the specified name and 
returns the TImapAccount oject associted to it.

@param aName
The Imap account name

@param aImapAccount
An object of type TImapAccount

@leave KErrNotFound
Leaves if no account found with the supplied name.

@return
ETrue if the account is found, else EFalse
*/
EXPORT_C TBool CT_MsgUtilsCentralRepository::GetImapAccountL(TDes& aName, TImapAccount& aImapAccount )
	{
	TImapAccount imapAccount;
	imapAccount.iImapService = KMsvNullIndexEntryId;

	// Get the list of IMAP accounts
	RArray<TImapAccount> accountIds;

	CEmailAccounts *emailAccounts = CEmailAccounts::NewLC();	
	emailAccounts->GetImapAccountsL(accountIds);
	
	// Search for the account with the given name in the list of IMAP accounts
	TBool accountFound = EFalse;
	for( TInt index = 0; index < accountIds.Count(); index++)
		{
		TImapAccount tempAccount = accountIds[index];
		TPtrC accountPtr( tempAccount.iImapAccountName );

		if( accountPtr.Compare(aName) == 0 )
			{
			aImapAccount = tempAccount;
			accountFound = ETrue;
			break;
			}
		}
	if(!accountFound)
		{
		User::Leave(KErrNotFound);
		}
	CleanupStack::PopAndDestroy(emailAccounts);
	accountIds.Reset();
	
	return accountFound;
	}


/**
GetPopAccountL()
Searches the POP account with the specified name and 
returns the TPopAccount oject associted to it.

@param aName
The Imap account name

@param aPopAccount
An object of type TPopAccount

@leave KErrNotFound
Leaves if no account found with the supplied name.

@return
ETrue if the account is found, else EFalse
*/
EXPORT_C TBool CT_MsgUtilsCentralRepository::GetPopAccountL(TDes& aName, TPopAccount& aPopAccount )
	{
	TPopAccount popAccount;
	popAccount.iPopService = KMsvNullIndexEntryId;

	// Get the list of POP accounts
	RArray<TPopAccount> accountIds;

	CEmailAccounts *emailAccounts = CEmailAccounts::NewLC();	
	emailAccounts->GetPopAccountsL(accountIds);
	
	// Search for the account with the given name in the list of POP accounts
	TBool accountFound = EFalse;
	for( TInt index = 0; index < accountIds.Count(); index++)
		{
		TPopAccount tempAccount = accountIds[index];
		TPtrC accountPtr( tempAccount.iPopAccountName );

		if( accountPtr.Compare(aName) == 0 )
			{
			aPopAccount = tempAccount;
			accountFound = ETrue;
			break;
			}
		}
	if(!accountFound)
		{
		User::Leave(KErrNotFound);
		}
	CleanupStack::PopAndDestroy(emailAccounts);
	accountIds.Reset();

	return accountFound;
	}

/**
GetSmtpAccountL()
Searches the SMTP account with the specified name and 
returns the TSmtpAccount oject associted to it.

@param aName
The Smtp account name

@param aSmtpAccount
An object of type TSmtpAccount

@leave KErrNotFound
Leaves if no account found with the supplied name.

@return
ETrue if the account is found, else EFalse
*/
EXPORT_C TBool CT_MsgUtilsCentralRepository::GetSmtpAccountL(TDes& aName, TSmtpAccount& aSmtpAccount)
	{
	TSmtpAccount smtpAccount;
	smtpAccount.iSmtpService = KMsvNullIndexEntryId;

	// Get the list of SMTP accounts
	RArray<TSmtpAccount> accountIds;

	CEmailAccounts *emailAccounts = CEmailAccounts::NewLC();	
	emailAccounts->GetSmtpAccountsL(accountIds);
	
	// Search for the account with the given name in the list of SMTP accounts
	TBool accountFound = EFalse;
	for( TInt index = 0; index < accountIds.Count(); index++)
		{
		TSmtpAccount tempAccount = accountIds[index];
		TPtrC accountPtr( tempAccount.iSmtpAccountName );

		if( accountPtr.Compare(aName) == 0 )
			{
			aSmtpAccount = tempAccount;
			accountFound = ETrue;
			break;
			}
		}
	if(!accountFound)
		{
		User::Leave(KErrNotFound);
		}
	CleanupStack::PopAndDestroy(emailAccounts);
	accountIds.Reset();

	return accountFound;
	}
