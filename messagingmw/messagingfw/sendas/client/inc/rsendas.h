// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __RSENDAS_H__
#define __RSENDAS_H__

#include <e32base.h>

class CSendAsMessageTypes;
class CSendAsAccounts;

/**
The default number of message slots available to a session. This determines the
number of outstanding requests the client may have with the server at any one
time.
*/
const TInt KSendAsDefaultMessageSlots = 4;

/**
Encapsulates a session with the SendAs Server.

This class allows a user to determine the message types available to the SendAs
server and also provides functionality for clients to filter this list to find 
the UIDs required to create messages.

@publishedAll
@released
*/
class RSendAs : public RSessionBase
	{
public:

/**
Defines the condition-checking of message capabilities. These are progressively
added to refine the list of available MTMs.
*/
	enum TSendAsConditionType
		{
		/**
		No MTMs are removed as a result of applying this condition to the
		filtering.
		*/
		ESendAsNoCondition,
		/**
		Only MTMs whose capabilities match the given value will remain as a
		result of applying this condition to the filtering.
		*/
		ESendAsEquals,
		/**
		Only MTMs whose capabilities do not match the given value will remain 
		as a result of applying this condition to the filtering.
		*/
		ESendAsNotEquals,
		/**
		Only MTMs whose capabilities greater than the given value will remain
		as a result of applying this condition to the filtering.
		*/
		ESendAsGreaterThan,
		/**
		Only MTMs whose capabilities less than the given value will remain as a
		result of applying this condition to the filtering.
		*/
		ESendAsLessThan,
		/**
		Only MTMs whose capabilities result non-zero value when bitwise ANDed
		with the given value will remain as a result of applying this condition
		to the filtering.
		*/
		ESendAsBitwiseAnd,
		/**
		Only MTMs whose capabilities result non-zero value when bitwise ORed
		with the given value will remain as a result of applying this condition
		to the filtering.
		*/
		ESendAsBitwiseOr,
		/**
		Only MTMs whose capabilities result non-zero value when bitwise NANDed
		with the given value will remain as a result of applying this condition
		to the filtering.
		*/
		ESendAsBitwiseNand,
		/**
		Only MTMs whose capabilities result non-zero value when bitwise NORed
		with the given value will remain as a result of applying this condition
		to the filtering.
		*/
		ESendAsBitwiseNor
		};

public:
	IMPORT_C TInt Connect();
	IMPORT_C TInt Connect(TInt aMessageSlots);
	
	IMPORT_C void FilteredMessageTypesL(CSendAsMessageTypes& aMessageTypeInfo);
	IMPORT_C TInt FilterAgainstCapability(TUid aMessageCapability);
	IMPORT_C TInt FilterAgainstCapability(TUid aMessageCapability, TInt aValue, TSendAsConditionType aConditionType);
	IMPORT_C TInt ResetMessageFilter();
	
	IMPORT_C void AvailableAccountsL(TUid aMessageType, CSendAsAccounts& aAccounts);

private:
	TAny* iAny; // Future proofing.
	};

#endif	//  __RSENDAS_H__
