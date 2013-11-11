// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// The definitions of the transition classes upon the CMailtoSchemeHandler class methods.
// 
//

/**
 @file
 @SYMCreationDate Created : 24/05/01 15:57:46
*/

#ifndef __MAILTOSCHEMEHANDLERTRANSITIONVALIDATION_H__
#define __MAILTOSCHEMEHANDLERTRANSITIONVALIDATION_H__

#include "MailtoSchemeHandlerTransitions.h"

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Provide all the CtorUnit Test's specific
	validatation for the state of a transition before and after its execution.
	on the CMailtoSchemeHandler test class for a transition.
 */
class TMailtoSchemeHandler_Ctor_TransitionValidator : public TTransitionValidator
	{
public:
	/**
		@fn				TMailtoSchemeHandler_Ctor_TransitionValidator(CUnitTestContext& aUTContext)
		Intended Usage	:	
		@leave		KErrNoMemory
		@since			6.0
		@param			aUTContext The context within which this transition is executing
	 */
	inline TMailtoSchemeHandler_Ctor_TransitionValidator(CUnitTestContext& aUTContext);

	/**
		@fn				ValidatePreConditions()
		Intended Usage	: Implemented by the developer to check the
						end state of the transition behaviour.
		Error Condition	: Invalid pre-conditions
		@since			6.0
		@return			TBool ETrue if the pre-conditions were valid, EFalse otherwise.
		@pre 			TMailtoSchemeHandler_Ctor_TransitionValidator is fully constructed.
		@post			No change to the iUTContext class.
	*/
	virtual inline TBool ValidatePreConditions();

	/**
		@fn				ValidatePostConditions(TTestBedAsyncState aAsyncState)
		Intended Usage	:	Implemented by the developer to check the
						end state of the transition behaviour.
						When overriding, if the transition calls an asynchronous function
						ValidatePostConditions will be called twice. Firstly, after the
						asynchronous function has been called and, secondly, after the 
						asynchronous request has completed. The parameter aAsyncState can
						be used to distinguish between these two cases.
		Error Condition	: Invalid post-conditions.
		@since			6.0
		@param			aAsyncState EAsyncCalled if the async function has been just been called,
						EAsyncCompleted if the function has completed.
		@return			TBool ETrue if the post-conditions were valid, EFalse otherwise.
		@pre 			TMailtoSchemeHandler_Ctor_TransitionValidator is fully constructed.
		@post			No change to the iUTContext class.
	*/
	virtual inline TBool ValidatePostConditions(TTestBedAsyncState aAsyncState);
	};	// TMailtoSchemeHandler_Ctor_TransitionValidator

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Provide all the DtorUnit Test's specific
	validatation for the state of a transition before and after its execution.
	on the CMailtoSchemeHandler test class for a transition.
 */
class TMailtoSchemeHandler_Dtor_TransitionValidator : public TTransitionValidator
	{
public:
	/**
		@fn				TMailtoSchemeHandler_Dtor_TransitionValidator(CUnitTestContext& aUTContext)
		Intended Usage	:	
		@leave		KErrNoMemory
		@since			6.0
		@param			aUTContext The context within which this transition is executing
	 */
	inline TMailtoSchemeHandler_Dtor_TransitionValidator(CUnitTestContext& aUTContext);

	/**
		@fn				ValidatePreConditions()
		Intended Usage	: Implemented by the developer to check the
						end state of the transition behaviour.
		Error Condition	: Invalid pre-conditions
		@since			6.0
		@return			TBool ETrue if the pre-conditions were valid, EFalse otherwise.
		@pre 			TMailtoSchemeHandler_Dtor_TransitionValidator is fully constructed.
		@post			No change to the iUTContext class.
	*/
	virtual inline TBool ValidatePreConditions();

	/**
		@fn				ValidatePostConditions(TTestBedAsyncState aAsyncState)
		Intended Usage	:	Implemented by the developer to check the
						end state of the transition behaviour.
						When overriding, if the transition calls an asynchronous function
						ValidatePostConditions will be called twice. Firstly, after the
						asynchronous function has been called and, secondly, after the 
						asynchronous request has completed. The parameter aAsyncState can
						be used to distinguish between these two cases.
		Error Condition	: Invalid post-conditions.
		@since			6.0
		@param			aAsyncState EAsyncCalled if the async function has been just been called,
						EAsyncCompleted if the function has completed.
		@return			TBool ETrue if the post-conditions were valid, EFalse otherwise.
		@pre 			TMailtoSchemeHandler_Dtor_TransitionValidator is fully constructed.
		@post			No change to the iUTContext class.
	*/
	virtual inline TBool ValidatePostConditions(TTestBedAsyncState aAsyncState);
	};	// TMailtoSchemeHandler_Dtor_TransitionValidator


// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Provide all the ParseUrlTest1Unit Test's specific
	validatation for the state of a transition before and after its execution.
	on the CMailtoSchemeHandler test class for a transition.
 */
class TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator : public TTransitionValidator
	{
public:
	/**
		@fn				TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator(CUnitTestContext& aUTContext)
		Intended Usage	:	
		@leave		KErrNoMemory
		@since			6.0
		@param			aUTContext The context within which this transition is executing
	 */
	inline TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator(CUnitTestContext& aUTContext);

	/**
		@fn				ValidatePreConditions()
		Intended Usage	: Implemented by the developer to check the
						end state of the transition behaviour.
		Error Condition	: Invalid pre-conditions
		@since			6.0
		@return			TBool ETrue if the pre-conditions were valid, EFalse otherwise.
		@pre 			TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator is fully constructed.
		@post			No change to the iUTContext class.
	*/
	virtual inline TBool ValidatePreConditions();

	/**
		@fn				ValidatePostConditions(TTestBedAsyncState aAsyncState)
		Intended Usage	:	Implemented by the developer to check the
						end state of the transition behaviour.
						When overriding, if the transition calls an asynchronous function
						ValidatePostConditions will be called twice. Firstly, after the
						asynchronous function has been called and, secondly, after the 
						asynchronous request has completed. The parameter aAsyncState can
						be used to distinguish between these two cases.
		Error Condition	: Invalid post-conditions.
		@since			6.0
		@param			aAsyncState EAsyncCalled if the async function has been just been called,
						EAsyncCompleted if the function has completed.
		@return			TBool ETrue if the post-conditions were valid, EFalse otherwise.
		@pre 			TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator is fully constructed.
		@post			No change to the iUTContext class.
	*/
	virtual inline TBool ValidatePostConditions(TTestBedAsyncState aAsyncState);
	};	// TMailtoSchemeHandler_ParseUrlTest1_TransitionValidator


// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Provide all the ParseUrlTest9Unit Test's specific
	validatation for the state of a transition before and after its execution.
	on the CMailtoSchemeHandler test class for a transition.
 */
class TMailtoSchemeHandler_ParseUrlTest9_TransitionValidator : public TTransitionValidator
	{
public:
	/**
		@fn				TMailtoSchemeHandler_ParseUrlTest9_TransitionValidator(CUnitTestContext& aUTContext)
		Intended Usage	:	
		@leave		KErrNoMemory
		@since			6.0
		@param			aUTContext The context within which this transition is executing
	 */
	inline TMailtoSchemeHandler_ParseUrlTest9_TransitionValidator(CUnitTestContext& aUTContext);

	/**
		@fn				ValidatePreConditions()
		Intended Usage	: Implemented by the developer to check the
						end state of the transition behaviour.
		Error Condition	: Invalid pre-conditions
		@since			6.0
		@return			TBool ETrue if the pre-conditions were valid, EFalse otherwise.
		@pre 			TMailtoSchemeHandler_ParseUrlTest9_TransitionValidator is fully constructed.
		@post			No change to the iUTContext class.
	*/
	virtual inline TBool ValidatePreConditions();

	/**
		@fn				ValidatePostConditions(TTestBedAsyncState aAsyncState)
		Intended Usage	:	Implemented by the developer to check the
						end state of the transition behaviour.
						When overriding, if the transition calls an asynchronous function
						ValidatePostConditions will be called twice. Firstly, after the
						asynchronous function has been called and, secondly, after the 
						asynchronous request has completed. The parameter aAsyncState can
						be used to distinguish between these two cases.
		Error Condition	: Invalid post-conditions.
		@since			6.0
		@param			aAsyncState EAsyncCalled if the async function has been just been called,
						EAsyncCompleted if the function has completed.
		@return			TBool ETrue if the post-conditions were valid, EFalse otherwise.
		@pre 			TMailtoSchemeHandler_ParseUrlTest9_TransitionValidator is fully constructed.
		@post			No change to the iUTContext class.
	*/
	virtual inline TBool ValidatePostConditions(TTestBedAsyncState aAsyncState);
	};	// TMailtoSchemeHandler_ParseUrlTest9_TransitionValidator

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Provide all the ParseUrlTest12Unit Test's specific
	validatation for the state of a transition before and after its execution.
	on the CMailtoSchemeHandler test class for a transition.
 */
class TMailtoSchemeHandler_ParseUrlTest12_TransitionValidator : public TTransitionValidator
	{
public:
	/**
		@fn				TMailtoSchemeHandler_ParseUrlTest12_TransitionValidator(CUnitTestContext& aUTContext)
		Intended Usage	:	
		@leave		KErrNoMemory
		@since			6.0
		@param			aUTContext The context within which this transition is executing
	 */
	inline TMailtoSchemeHandler_ParseUrlTest12_TransitionValidator(CUnitTestContext& aUTContext);

	/**
		@fn				ValidatePreConditions()
		Intended Usage	: Implemented by the developer to check the
						end state of the transition behaviour.
		Error Condition	: Invalid pre-conditions
		@since			6.0
		@return			TBool ETrue if the pre-conditions were valid, EFalse otherwise.
		@pre 			TMailtoSchemeHandler_ParseUrlTest12_TransitionValidator is fully constructed.
		@post			No change to the iUTContext class.
	*/
	virtual inline TBool ValidatePreConditions();

	/**
		@fn				ValidatePostConditions(TTestBedAsyncState aAsyncState)
		Intended Usage	:	Implemented by the developer to check the
						end state of the transition behaviour.
						When overriding, if the transition calls an asynchronous function
						ValidatePostConditions will be called twice. Firstly, after the
						asynchronous function has been called and, secondly, after the 
						asynchronous request has completed. The parameter aAsyncState can
						be used to distinguish between these two cases.
		Error Condition	: Invalid post-conditions.
		@since			6.0
		@param			aAsyncState EAsyncCalled if the async function has been just been called,
						EAsyncCompleted if the function has completed.
		@return			TBool ETrue if the post-conditions were valid, EFalse otherwise.
		@pre 			TMailtoSchemeHandler_ParseUrlTest12_TransitionValidator is fully constructed.
		@post			No change to the iUTContext class.
	*/
	virtual inline TBool ValidatePostConditions(TTestBedAsyncState aAsyncState);
	};	// TMailtoSchemeHandler_ParseUrlTest12_TransitionValidator

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : Provide all the ParseUrlHandler14Unit Test's specific
	validatation for the state of a transition before and after its execution.
	on the CMailtoSchemeHandler test class for a transition.
 */
class TMailtoSchemeHandler_ParseUrlTest14_TransitionValidator : public TTransitionValidator
	{
public:
	/**
		@fn				TMailtoSchemeHandler_ParseUrlTest14_TransitionValidator(CUnitTestContext& aUTContext)
		Intended Usage	:	
		@leave		KErrNoMemory
		@since			6.0
		@param			aUTContext The context within which this transition is executing
	 */
	inline TMailtoSchemeHandler_ParseUrlTest14_TransitionValidator(CUnitTestContext& aUTContext);

	/**
		@fn				ValidatePreConditions()
		Intended Usage	: Implemented by the developer to check the
						end state of the transition behaviour.
		Error Condition	: Invalid pre-conditions
		@since			6.0
		@return			TBool ETrue if the pre-conditions were valid, EFalse otherwise.
		@pre 			TMailtoSchemeHandler_ParseUrlTest14_TransitionValidator is fully constructed.
		@post			No change to the iUTContext class.
	*/
	virtual inline TBool ValidatePreConditions();

	/**
		@fn				ValidatePostConditions(TTestBedAsyncState aAsyncState)
		Intended Usage	:	Implemented by the developer to check the
						end state of the transition behaviour.
						When overriding, if the transition calls an asynchronous function
						ValidatePostConditions will be called twice. Firstly, after the
						asynchronous function has been called and, secondly, after the 
						asynchronous request has completed. The parameter aAsyncState can
						be used to distinguish between these two cases.
		Error Condition	: Invalid post-conditions.
		@since			6.0
		@param			aAsyncState EAsyncCalled if the async function has been just been called,
						EAsyncCompleted if the function has completed.
		@return			TBool ETrue if the post-conditions were valid, EFalse otherwise.
		@pre 			TMailtoSchemeHandler_ParseUrlTest14_TransitionValidator is fully constructed.
		@post			No change to the iUTContext class.
	*/
	virtual inline TBool ValidatePostConditions(TTestBedAsyncState aAsyncState);
	};	// TMailtoSchemeHandler_ParseUrlTest14_TransitionValidator

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class

	Comments : Provide all the CreateEmailLUnit Test's specific
	validatation for the state of a transition before and after its execution.
	on the CMailtoSchemeHandler test class for a transition.
 */
class TMailtoSchemeHandler_CreateEmailL_TransitionValidator : public TTransitionValidator
	{
public:
	/**
		@fn				TMailtoSchemeHandler_CreateEmailL_TransitionValidator(CUnitTestContext& aUTContext)
		Intended Usage	:	
		@leave		KErrNoMemory
		@since			6.0
		@param			aUTContext The context within which this transition is executing
	 */
	inline TMailtoSchemeHandler_CreateEmailL_TransitionValidator(CUnitTestContext& aUTContext);

	/**
		@fn				ValidatePreConditions()
		Intended Usage	: Implemented by the developer to check the
						end state of the transition behaviour.
		Error Condition	: Invalid pre-conditions
		@since			6.0
		@return			TBool ETrue if the pre-conditions were valid, EFalse otherwise.
		@pre 			TMailtoSchemeHandler_CreateEmailL_TransitionValidator is fully constructed.
		@post			No change to the iUTContext class.
	*/
	virtual inline TBool ValidatePreConditions();

	/**
		@fn				ValidatePostConditions(TTestBedAsyncState aAsyncState)
		Intended Usage	:	Implemented by the developer to check the
						end state of the transition behaviour.
						When overriding, if the transition calls an asynchronous function
						ValidatePostConditions will be called twice. Firstly, after the
						asynchronous function has been called and, secondly, after the 
						asynchronous request has completed. The parameter aAsyncState can
						be used to distinguish between these two cases.
		Error Condition	: Invalid post-conditions.
		@since			6.0
		@param			aAsyncState EAsyncCalled if the async function has been just been called,
						EAsyncCompleted if the function has completed.
		@return			TBool ETrue if the post-conditions were valid, EFalse otherwise.
		@pre 			TMailtoSchemeHandler_CreateEmailL_TransitionValidator is fully constructed.
		@post			No change to the iUTContext class.
	*/
	virtual inline TBool ValidatePostConditions(TTestBedAsyncState aAsyncState);
	};	// TMailtoSchemeHandler_CreateEmailL_TransitionValidator

#include "MailtoSchemeHandlerTransitionValidation.inl"

#endif // __MAILTOSCHEMEHANDLERTRANSITIONVALIDATION_H__

