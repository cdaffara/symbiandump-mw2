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

#ifndef __CMTF_TEST_CASE_H__
#define __CMTF_TEST_CASE_H__

#include <test/testexecutestepbase.h>
#include "TMtfTestParameterType.h"
#include "CMtfTestParameter.h"
#include "CMtfTestParameterStore.h"
#include "CMtfConfigurationType.h"
#include "CMtfEnumeratorConverter.h"
#include "CMtfTestAction.h"

class CMtfTestServer;
class CMtfTestAction;

_LIT(KMtfDontCare,"_");
_LIT(_,"_");						// Short form for nonscripted test scripts


/** Base class from which all test cases are derived. To create a new test case class the functions
SetupTestCaseL(), ExecuteTestCaseL() and CheckPostConditionsL() must be overridden. */
class CMtfTestCase: public CTestStep
{
public:

	// These panics signify that the test case script is invalid in case of a scripted
	// test case. In case of a non-scripted test case, the overridden sections of the test 
	// case are invalid. The test actions are not used correctly.
	//
	enum TMtfTestCasePanic
	{
		EMtfGeneralTypeMismatch,	// mismatch of general parameter types
		EMtfSpecificTypeMismatch,	// mismatch of specific parameter types
		EMtfMissingParameters,		// a test action was used without providing all
									// required parameters
		EMtfUnexpectedQueuedAction,	// the test case expected iQueuedActions to be empty
		EMtfInvalidParameter,		// invalid parameter
		EMtfInternalError
	};
	
	static void Panic(TMtfTestCasePanic aPanic);
	virtual ~CMtfTestCase();
	
	/** Required by TestExecute */
	virtual TVerdict doTestStepPreambleL();
	
	/** Required by TestExecute */
	virtual TVerdict doTestStepL();
	
	/** Required by TestExecute */
	virtual TVerdict doTestStepPostambleL();
	
	/** Notifies the test case that an action has been completed. Used by test actions. */
	void ActionCompletedL(CMtfTestAction& aTestAction);
	
	/** Notifies the test case that a section has been completed. Used by the framework. */
	void SectionCompletedL();
	
	/** Queues an action for execution. Takes ownership  */
	void QueueTestActionL(CMtfTestAction* aTestAction);
	
	TBool ScriptedTestCase() const;
	
	/** Sets the test case to wait for the specified action to complete before executing
	any other actions. */
	void WaitForActionL(const TDesC& aActionId);
	
	/** Sets the test case to wait for the completion of all currently executing actions
	before starting the execution of any other actions. */
	void WaitForAllActionsL();
	
	TBool IsSynchronous();
	
	void DeleteParameterL(const TDesC& aName);
	
	/** Obtains the configuration file corresponding to the required type. Configuration file returned was either set
	from the main script, set as the test case default, or a global default one is used.*/
	const TDesC& GetConfigurationFileL(const CMtfConfigurationType::TMtfConfigurationType& aType, TInt aIndex) const;
	const TDesC& GetConfigurationFileL(const CMtfConfigurationType::TMtfConfigurationType& aType) const;
	
	/** Sets the default configuration for this test case only.*/
	void SetTestCaseDefaultConfigurationTypeL(CMtfConfigurationType* aConfiguration);
	
	TInt ConvertEnumeratorL(const TDesC& aParameter) const;
	
	const CMtfTestParameterStore& ParameterStore() const;
	void StoreParameterL(CMtfTestParameter* aParameter);
	const CMtfTestParameter& ObtainParameterL(const TDesC& aName) const;
	
	enum TMtfTestCaseState
	{
		EMtfTestCaseInitialState,
		EMtfTestCaseExecutingSetup,
		EMtfTestCaseExecutingTest,
		EMtfTestCaseCheckingPostconditions,
		EMtfTestCaseTestCompleted
	};
	
	const CMtfTestServer& Server() { return iTestServer; }

protected:
	CMtfTestCase(const CMtfTestServer& aTestServer, const TBool& aScripted, const TBool& aSynchronous);
	void ConstructL();
	
private:

	virtual void SetupTestCaseL()=0;
	virtual void ExecuteTestCaseL()=0;
	virtual void CheckPostConditionsL()=0;
	
	/** Can be overridden if this section is not synchronous. By default returns true. */ 
	virtual TBool SetupTestCaseIsSynchronous() const;
	
	/** Can be overridden if this section is not synchronous. By default returns true. */ 
	virtual TBool ExecuteTestCaseIsSynchronous() const;
	
	/** Can be overridden if this section is not synchronous. By default returns true. */ 
	virtual TBool CheckPostconditionsIsSynchronous() const; 
	
	void ExecuteActionsL();
	void ChangeState();
	
private:
	RPointerArray<CMtfTestAction> iQueuedActions;
	RPointerArray<CMtfTestAction> iCurrentActions;
	RPointerArray<CMtfTestAction> iCompletedActions;
	TInt 	iWaitListCount;
	CMtfTestParameterStore*     	iParameterStore;
	TMtfTestCaseState 				iCurrentState;
	TBool 							iScriptedTestCase;
	TBool 							iActiveSchedulerRunning;
	TBool 							iSynchronous;	// used if scripted
	CActiveScheduler* 				iScheduler;
	RPointerArray<CMtfConfigurationType> 	iTestCaseDefaultConfigurations;
	const CMtfTestServer& 					iTestServer;
};


/** Used for optional parameters. This function requires a default value for the optional parameter to be
provided. This function should be a member function of CMtfTestCase but unfortunately member template functions
are not supported by all three compilers.  */
template<class T>
T* ObtainParameterReferenceL(CMtfTestCase& aTestCase, const TDesC& aParameterName,T* aDefault)
{
	// if parameter is not provided use default
	if (aParameterName == KMtfDontCare)
	{
		return aDefault;
	}
	
	return ObtainParameterReferenceL<T>(aTestCase,aParameterName);
}

/** Used to obtain non-optional reference parameters. This function should be a member function of CMtfTestCase 
but unfortunately member template functions are not supported by all three compilers. Type T and the type of the
requested parameter must match exactly, otherwise the function leaves. T must be either a CBase derived class or HBufC. */
template<class T>
T* ObtainParameterReferenceL(CMtfTestCase& aTestCase, const TDesC& aParameterName)
{
	// if optional leave
	if (aParameterName == KMtfDontCare)
	{
		User::Leave(KErrNotFound);
	}

	// obtain type information for T
	TMtfTestParameterType<T> parameterType;
	TMtfTestParameterGeneralType generalTypeId = parameterType.GeneralTypeId();
	TMtfTestParameterSpecificType specificTypeId = parameterType.SpecificTypeId();

	// This function must only be called with T being either a CBase derived class
	// or HBufC, otherwise it is a programming error.
	// can also be called with an RArray type ptr
	
	__ASSERT_ALWAYS( (generalTypeId == EMtfCBaseType) || (generalTypeId == EMtfHBufCType) || (generalTypeId == EMtfValueType) ,
		CMtfTestAction::Panic(CMtfTestAction::EMtfNonReferenceParameter));
		
	T* returnPtr = NULL;
	
	if (generalTypeId == EMtfHBufCType) 
	{
		// if hbufc check if it is a constant descriptor
		if (aParameterName[0] == '\"')
		{
			if (aParameterName[aParameterName.Length()-1] == '\"')
			{
				// find parameter in const store first
				
				if (specificTypeId == EMtfHBufC)
				{
					// add the constant to parameter store
					
					HBufC* buf = HBufC::NewLC(aParameterName.Length()-2);
					
					for(TInt c=0; c<aParameterName.Length()-2; c++)
							buf->Des().Append(aParameterName[c+1]);
				
					// parameter store takes ownership
					
					CMtfTestParameter* newParameter = CMtfTestParameter::NewL(aParameterName,generalTypeId,specificTypeId,reinterpret_cast<TAny*>(buf));
					CleanupStack::PushL(newParameter);
					aTestCase.StoreParameterL(newParameter);				
					CleanupStack::Pop(2,buf);
					returnPtr = reinterpret_cast<T*>(buf);
				}
				else if (specificTypeId == EMtfHBufC8)
				{
					HBufC8* buf = HBufC8::NewLC(aParameterName.Length()-2);
					
					for(TInt c=0; c<aParameterName.Length()-2; c++)
							buf->Des().Append(aParameterName[c+1]);
				
					CMtfTestParameter* newParameter = CMtfTestParameter::NewL(aParameterName,generalTypeId,specificTypeId,reinterpret_cast<TAny*>(buf));
					CleanupStack::PushL(newParameter);
					aTestCase.StoreParameterL(newParameter);				
					CleanupStack::Pop(2,buf);
					returnPtr = reinterpret_cast<T*>(buf);
				}
				else
					User::Panic(KMtfTypeMismatch,0);
			}
		}
	}
	
	if(returnPtr == NULL) // not a constant descriptor
		{
		const CMtfTestParameter& parameter = aTestCase.ObtainParameterL(aParameterName);

		__ASSERT_ALWAYS(generalTypeId == parameter.GeneralTypeId(), User::Panic(KMtfTypeMismatch,0));
		__ASSERT_ALWAYS(specificTypeId == parameter.SpecificTypeId(), User::Panic(KMtfTypeMismatch,0));
		
		returnPtr = reinterpret_cast<T*>(parameter.Parameter());
		}
	return returnPtr;
}


/** Used for value and reference parameters. */
template<class T>
void StoreParameterL(CMtfTestCase& aTestCase,T& aPtr, const TDesC& aName)
{
	// if the parameter is "don't care" then don't store the parameter
	if (aName == KMtfDontCare)
	{
		return;
	}
	
	// obtain type information for T
	TMtfTestParameterType<T> parameterType;
	TMtfTestParameterGeneralType generalTypeId = parameterType.GeneralTypeId();
	TMtfTestParameterSpecificType specificTypeId = parameterType.SpecificTypeId();
	
	CMtfTestParameter* newParameter=NULL;
//	TPckgC<T> pack(aPtr);
//	HBufC8 *valueStorage;
	
	switch(generalTypeId)
	{
		case EMtfCBaseType:
		case EMtfHBufCType:
			{
			newParameter = CMtfTestParameter::NewL(
				aName,generalTypeId,specificTypeId,reinterpret_cast<TAny*>(&aPtr));
			CleanupStack::PushL(newParameter);
			aTestCase.StoreParameterL(newParameter);
			CleanupStack::Pop(newParameter);
			return;
			}
		case EMtfValueType:
			if(specificTypeId == EMtfTTime)
				{
				newParameter = CMtfTestParameter::NewL(
				aName,generalTypeId,specificTypeId,reinterpret_cast<TAny*>(&aPtr));
				CleanupStack::PushL(newParameter);
				aTestCase.StoreParameterL(newParameter);
				CleanupStack::Pop(newParameter);
				return;	
				}
			else
				{
				TPckgC<T> pack(aPtr);
				HBufC8 *valueStorage = pack.AllocLC();
				newParameter = CMtfTestParameter::NewL(
					aName,generalTypeId,specificTypeId,reinterpret_cast<TAny*>(valueStorage));
				CleanupStack::PushL(newParameter);
				aTestCase.StoreParameterL(newParameter);
				CleanupStack::Pop(2,valueStorage);
				return;	
				}
		case EMtfEnumeratorType:
			{
			TPckgC<T> pack(aPtr);
			HBufC8 *valueStorage = pack.AllocLC();
			newParameter = CMtfTestParameter::NewL(
				aName,generalTypeId,specificTypeId,reinterpret_cast<TAny*>(valueStorage));
			CleanupStack::PushL(newParameter);
			aTestCase.StoreParameterL(newParameter);
			CleanupStack::Pop(2,valueStorage);
			return;
			}
		default:
			User::Panic(KMtfTypeMismatch,0);	
	}
}

/** Usef for optional value parameters. A default value must be provided. */
template<class T>
T ObtainValueParameterL(CMtfTestCase& aTestCase, const TDesC& aParameterName,T aDefault)
{
	if (aParameterName == KMtfDontCare)
	{
		return aDefault;
	}
	
	return ObtainValueParameterL<T>(aTestCase,aParameterName);
}

/** Used for non-optional value parameters. TInt parameters and enumerator parameters may be constant values. */
template<class T>
T ObtainValueParameterL(CMtfTestCase& aTestCase, const TDesC& aParameterName)
{
	// if optional leave
	if (aParameterName == KMtfDontCare)
	{
		User::Leave(KErrNotFound);
	}
	
	T result;
	T* resultPtr = NULL;
	
	TMtfTestParameterType<T> parameterType;
	TMtfTestParameterGeneralType generalTypeId = parameterType.GeneralTypeId();
	TMtfTestParameterSpecificType specificTypeId = parameterType.SpecificTypeId();
	
	// check that id is value
	__ASSERT_ALWAYS(((generalTypeId == EMtfValueType) || (generalTypeId == EMtfEnumeratorType)),User::Panic(KMtfTypeMismatch,0));
	
	if (generalTypeId == EMtfEnumeratorType)
	{
		// check if it is a constant enumerator
		if (CMtfEnumeratorConverter::IsConstantEnumerator(aParameterName))
		{ 
			*reinterpret_cast<TInt*>(&result) = aTestCase.ConvertEnumeratorL(aParameterName);
			resultPtr = &result;
		}
	}
	else if (specificTypeId == EMtfTInt)
	{
		// if TInt then check if it is a constant parameter
		TLex converter(aParameterName);
		
		if (converter.Val(*(reinterpret_cast<TInt*>(&result))) == KErrNone)
		{
			// const parameter
			resultPtr = &result;
		}
	}
	
	if(resultPtr == NULL)
		{
		const CMtfTestParameter& parameter=aTestCase.ObtainParameterL(aParameterName);
		
		__ASSERT_ALWAYS(generalTypeId == parameter.GeneralTypeId(), User::Panic(KMtfTypeMismatch,0));
		__ASSERT_ALWAYS(specificTypeId == parameter.SpecificTypeId(), User::Panic(KMtfTypeMismatch,0));
		
		HBufC8* buf=reinterpret_cast<HBufC8*>(parameter.Parameter());
		T value;	
		TPckgC<T> unpack(value);
		unpack.Set(*buf);
		result = unpack();
		resultPtr = &result;
		}
	
	return *resultPtr;
}

/** Deletes the named parameter. */
template<class T>
#if defined (__WINS__) 
T*		// vc6bug - doesn't expand to correct type if T is not employed as param or return 
#else
void	// normal case
#endif
DeleteParameterL(CMtfTestCase& aTestCase, const TDesC& aParameterName)
{
	TMtfTestParameterType<T> parameterType;
	TMtfTestParameterGeneralType generalTypeId = parameterType.GeneralTypeId();
	TMtfTestParameterSpecificType specificTypeId = parameterType.SpecificTypeId();

	const CMtfTestParameter& parameter = aTestCase.ObtainParameterL(aParameterName);

	__ASSERT_ALWAYS(generalTypeId == parameter.GeneralTypeId(), User::Panic(KMtfTypeMismatch,0));
	__ASSERT_ALWAYS(specificTypeId == parameter.SpecificTypeId(), User::Panic(KMtfTypeMismatch,0));
	
	aTestCase.DeleteParameterL(aParameterName);
#if defined (__WINS__) 
	return (reinterpret_cast<T*>(0)); // vc6bug
#endif
}


#endif
