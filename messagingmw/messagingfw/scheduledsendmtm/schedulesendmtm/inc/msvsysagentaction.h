// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// MsvSysAgentCondAction.H
// 
//

#ifndef __MSVSYSAGENTCONDACTION_H__
#define	__MSVSYSAGENTCONDACTION_H__

 


#include <msvsenderroraction.h>

//
//
//	System Agent-related Class Declarations
//
//



/** 
Defines a condition which a Publish and Subscribe Uid must satisfy.

@see RProperty

@publishedAll
@released
*/
class TMsvCondition
	{
public:
/** 
An enumeration defining the type of test to be made against a Publish 
and Subscribe property. 
*/
	enum TMsvSchSendConditionType
		{
		/** Tests that a value is equal to a state variable value. */
		EMsvSchSendEquals,		
		/** Tests that a value is unequal to a state variable value. */
		EMsvSchSendNotEquals,		
		/** Tests that a value is greater than a state variable value. */
		EMsvSchSendGreaterThan,	
		/** Tests that a value is less than a state variable value. */
		EMsvSchSendLessThan		
		};
		
public:
	/** second UID of the process which set the condition*/
	TUid iVariable;
	/** Integer state of corresponding P&S variable to be tested against*/
	TInt iState;
	/** type of test to be performed */
	TMsvSchSendConditionType iType;
	};
	

/**
Schedule Send condition must be satisfied before a message sending attempt.

@publishedAll
@released
*/ 

class TMsvSysAgentConditionAction
	{
public:
	IMPORT_C TMsvSysAgentConditionAction();


	/** Condition to meet. */
	TMsvCondition iCondition;

	/** Error action to use. */
	TMsvSendErrorAction iErrorAction;

	/** Default error action. */
	TBool iUseDefaultSysAgentAction;

private:
	};



/**
Defines an array of  schedule send conditions that must be satisfied before a message sending attempt.

@see TMsvSysAgentConditionAction

@publishedAll
@released
*/ 

class CMsvSysAgentActions : public CArrayFixFlat<TMsvSysAgentConditionAction>
	{
public:
	IMPORT_C CMsvSysAgentActions();
	IMPORT_C ~CMsvSysAgentActions();


	IMPORT_C TBool ConditionsMetL(TMsvSendErrorAction& aErrorAction);

	/** Default send error action. */
	TMsvSendErrorAction iDefault;

private:


	void GetConditionVariablesL(CArrayFix<TUid>& aUids) const;
	TBool CheckStates(const CArrayFix<TInt>& aStates, TMsvSendErrorAction& aErrorAction) const;
	TBool ConditionOK(const TMsvSysAgentConditionAction& aCondAction, const TInt aState) const;
	
	};

#endif	// __MSVSYSAGENTCONDACTION_H__
