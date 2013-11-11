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
// The definition of the accessor class upon the CMailtoSchemeHandler
// 
//

/**
 @file
 @SYMCreationDate Created : 24/05/01 15:57:46
*/

#ifndef __MAILTOSCHEMEHANDLERSTATEACCESSOR_H__
#define __MAILTOSCHEMEHANDLERSTATEACCESSOR_H__

#include <e32base.h>
#include <f32file.h>
#include <s32stor.h>
#include <s32file.h>

#include <ecom/test_bed/testutilities.h>
#include <ecom/test_bed/testbeddefinitions.h>
#include <ecom/test_bed/stateaccessor.h>
#include "MTSCHEME.H"

// ______________________________________________________________________________
//
/**
	@internalTechnology
	@class
	Comments : State accessor for the CMailtoSchemeHandler object under test.
 */
class TMailtoSchemeHandler_StateAccessor : public MStateAccessor
	{
public:
	/**
		@fn				InvariantTest(TAny* aTestObject)
		Intended Usage	:	
		@since			6.0
		@param			TAny* A pointer to the object under test.
		@return			TInt KErrNone if the invariants were valid, error code otherwise.
		@pre 			TMailtoSchemeHandler_StateAccessor is fully constructed.
		@post			The class under test is set to the state specified.
	 */
	inline TInt InvariantTest(TAny* aTestObject);

	/**
		@fn				Internalize(TAny* aTestObject)
		Intended Usage	: Implemented by the developer to set the state of the object under test.
		@since			6.0
		@param			aTestObject The object to be tested
		@return			KErrNone if the internalize was successful.
		@return			One of the EPOC standard error codes indicating the reason
						for the failure	to load the object state.
		@pre 			TMailtoSchemeHandler_StateAccessor is fully constructed, and
						valid test object must be passed in.
		@post			The class under test is set to the state specified
	 */
	inline virtual TInt Internalize(TAny* aTestObject);

	/**
		@fn				Externalize(TAny* aTestObject)
		Intended Usage	: Implemented by the developer to persist the state of the object under test.
		@since			6.0
		@param			aTestObject The object to be tested
		@return			KErrNone if the internalize was successful.
		@return			One of the EPOC standard error codes indicating the reason
						for the failure	to externalize the object.
		@pre 			TMailtoSchemeHandler_StateAccessor is fully constructed, and
						valid test object must be passed in.
		@post			Returns an error code dependant on the result of the test
	 */
	inline virtual TInt Externalize(TAny* aTestObject);

	/**
		@fn				InternalizeL(RFileReadStream& aStream, CMailtoSchemeHandler* aMailtoSchemeHandler)
		Intended Usage	: Implemented by the developer to set the state of the object under test.
		Error Condition	: Leaves with an error code.
		@since			6.0
		@param			aStream The filestream to read from .
		@param			aMailtoSchemeHandler A pointer to the CMailtoSchemeHandler object under test.
		@return			None.
		@pre 			TMailtoSchemeHandler_StateAccessor is fully constructed.
		@post			The class under test is set to the state specified.
	 */
		inline void InternalizeL(RFileReadStream& aStream, CMailtoSchemeHandler* aMailtoSchemeHandler);

	/**
		@fn				ExternalizeL(RFileWriteStream& aStream, CMailtoSchemeHandler* aMailtoSchemeHandler)
		Intended Usage	: Implemented by the developer to persist the state of the object under test.
		Error Condition	: Leaves with an error code.
		@since			6.0
		@param			aStream The filestream to persist into.
		@param			aMailtoSchemeHandler A pointer to the CMailtoSchemeHandler object under test.
		@return			None.
		@pre 			TMailtoSchemeHandler_StateAccessor is fully constructed.
		@post			No change to the class under test, its internal state.
						is persisted.
	*/
	inline void ExternalizeL(RFileWriteStream& aStream, CMailtoSchemeHandler* aMailtoSchemeHandler);

	inline void ParseUrlL(CMailtoSchemeHandler& aMailTo, const TDesC& aUrl);

	inline RArray<TPtrC>& GetRecipientArray(CMailtoSchemeHandler& aMailTo);

	inline TPtrC& GetSubject(CMailtoSchemeHandler& aMailTo);

	inline TPtrC& GetBody(CMailtoSchemeHandler& aMailTo);


private:
	/**
		@fn				InternalizeL(CMailtoSchemeHandler* aMailtoSchemeHandler)
		Intended Usage	: Implemented by the developer to set the state of the object under test.
		Error Condition	: Leaves with an error code.
		@since			6.0
		@param			TAny* A pointer to the object under test.
		@return			None.
		@pre 			TMailtoSchemeHandler_StateAccessor is fully constructed.
		@post			The class under test is set to the state specified.
	*/
	inline void InternalizeL(CMailtoSchemeHandler* aMailtoSchemeHandler);

	/**
		@fn				ExternalizeL(CMailtoSchemeHandler* aMailtoSchemeHandler)
		Intended Usage	: Implemented by the developer to persist the state of the object under test.
		Error Condition	: Leaves with an error code.
		@since			6.0
		@param			TAny* A pointer to the object under test.
		@return			None.
		@pre 			TMailtoSchemeHandler_StateAccessor is fully constructed.
		@post			No change to the class under test, its internal state.
						is persisted.
	*/
	inline void ExternalizeL(CMailtoSchemeHandler* aMailtoSchemeHandler);
	};	// TMailtoSchemeHandler_StateAccessor

#include "MailtoSchemeHandlerStateAccessor.inl"

#endif // __MAILTOSCHEMEHANDLERSTATEACCESSOR_H__

