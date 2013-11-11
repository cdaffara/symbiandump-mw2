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


#ifndef __CMTF_TEST_ACTION_H__
#define __CMTF_TEST_ACTION_H__

#include <e32base.h>

class CMtfTestActionParameters;
class CMtfTestCase;

/** Abstract base class for asynchronous test actions. Derived classes need to override
ExecuteActionL(), RunL() and DoCancel(). Each test action has an identifier and a set of action
parameters. NewL() function of derived classes must call ConstructL(). */
class CMtfTestAction : public CActive
{
public:

	// These panics signify that a test action contains a programming error.
	//
	enum TMtfTestActionPanic
	{
		EMtfNonReferenceParameter,	// a non-reference parameter is being used as a reference
									// parameter
		EMtfNegativeParameterIndex,	// negative index being used to obtain an action parameter
		EMtfParameterIndexOutOfRange,// index used to obtain an action parameter is out of range
		EMtfUnexpectedRunL,			// RunL of an action has been called but the action has 
									// not overridden RunL
		EMtfUnexpectedDoCancel,		// DoCancel of an action has been called but the action has
									// not overridden DoCanceL 
		EMtfInvalidParameter		// invalid parameter
	};
	
	
	~CMtfTestAction();
	static void Panic(TMtfTestActionPanic aPanic);
	
	virtual void ExecuteActionL()=0;
	const CMtfTestActionParameters& ActionParameters() const;
	TBool CurrentlyBeingWaitedFor() const;
	const TDesC& ActionIdL() const;
	void SetActionIdL(const TDesC& aActionId);
	void SetCurrentlyBeingWaitedFor(const TBool& aWaitedFor);
	
	/** Returns true only if this action is the wait framework action. */
	virtual TBool WaitAction() const;
	
	/** Returns true only if this action is the section complete framework action. */
	virtual TBool SectionCompleteAction() const;
   
	// 
	// virtual func from CActive
	TInt RunError(TInt aErr);
 
protected:
	CMtfTestAction(CMtfTestCase& aTestCase);
	
	/** Constructs the test action and takes ownership of action parameters. Ownership is taken
	at the END of the function. aActionParameters must not be NULL. */
	void ConstructL(CMtfTestActionParameters* aActionParams);
	
	CMtfTestCase& TestCase();
		
private:
	CMtfTestCase&						iTestCase;
	CMtfTestActionParameters*  			iActionParams;
	HBufC*								iActionId;
	TBool 								iCurrentlyBeingWaitedFor; // no need for wait list
};


#endif

