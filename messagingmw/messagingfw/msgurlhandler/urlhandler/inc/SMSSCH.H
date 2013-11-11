// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This file contains the API definition for the class CSmsSchemeHandler.
// This class parses a descriptor into the component fields of a gsm-sms 
// scheme as described in "draft-antti-gsm-sms-url-04" 
// and launches the sms editor with the sms-numbers filled in.
// 
//

/**
 @file
*/

#ifndef __SMSSCH_H__
#define __SMSSCH_H__

// Local includes
//
#include "MSGBASE.H"


/**
	@struct SSmsNumber
	Comments :	Structure containing descriptor pointer of sms-number 
				and service center number 
	@since 7.0
*/
struct SSmsNumber
/**
@internalComponent
@released
*/
	{
	SSmsNumber() : iNumber(KNullDesC), iServiceCentre(KNullDesC) {}
	TPtrC iNumber;
	TPtrC iServiceCentre;
	};


/**
	@class
	Comments :	Parses a descriptor into the component fields of a gsm-sms scheme 
				as described in "draft-antti-gsm-sms-url-04"
				and launches the sms editor with the sms-numbers filled in.
				There are 2 fields; sms-number and service center number.
				These fields are valid for the lifetime of this object.
	
	Format of a typical gsm-sms url is: 
		sms:<sms-number>;via=<service center number>,<sms-number>;via=<service center number>...

	@since 7.0
*/
class CSmsSchemeHandler : public CMsgSchemeHandlerBase
/**
@internalComponent
@released
*/
	{

public:		// Methods

/**
	@fn				NewLC()
	Intended Usage	:	Static factory constructor. Uses two phase construction and 
						leaves a pointer to created object on the CleanupStack.
	@since			7.0
	@leave		KErrNoMemory.
	@return			A pointer to the newly created CSmsSchemeHandler object 
	@pre 			None
	@post			A fully constructed and initialised CSmsSchemeHandler object.
 */
	static CSmsSchemeHandler* NewLC();

/**
	@fn				NewL()
	Intended Usage	:	Static factory constructor. Uses two phase construction and 
						leaves nothing on the CleanupStack.
	@since			7.0
	@leave		KErrNoMemory.
	@return			A pointer to the newly created CSmsSchemeHandler object. 
	@pre 			None
	@post			A fully constructed and initialised CSmsSchemeHandler object.
 */
	static CSmsSchemeHandler* NewL();

/**
	@fn				~CSmsSchemeHandler()
	Intended Usage	:	Destructor.
	@since			7.0
 */
	~CSmsSchemeHandler();

private:		// Methods

/**
	@fn				CSmsSchemeHandler()
	Intended Usage	:	Constructor. First phase of two-phase construction method. Does 
						non-allocating construction.
	@since			7.0
 */
	CSmsSchemeHandler();

/**
	@fn				ConstructL()
	Intended Usage	:	Second phase of two-phase construction method. Does any 
						allocations required to fully construct the object.
	@since			7.0
	@leave		KErrNoMemory.
	@pre 			First phase of construction is complete
	@post			The object is fully constructed and initialised.
 */
	void ConstructL();

/**
	@fn				ParseUrlL(const TDesC& aUrl)
	Intended Usage	: Parses a descriptor into the component fields of a gsm-sms scheme 
					  as described in "draft-antti-gsm-sms-url-04". 
	@since			7.0
	@param			aUrl	A descriptor with the gsm-sms url.
	@pre 			None
	@post			The gsm-sms url descriptor has been parsed and field values are set.
 */
	virtual void ParseUrlL(const TDesC& aUrl);

/**
	@fn				ParseSmsNumber(const TPtrC& aNumber, TPtrC& aParsedNumber)
	Intended Usage	:	Parses the sms number so that it only contains the allowed characters.
	@since			7.0
	@param			aNumber			A descriptor pointer to the sms-number to be parsed 
	@param			aParsedNumber	This is an output argument that is set to the 
									parsed number. 
	@return			An error value of KErrNotFound if the sms-number is NULL and
					a value of KErrNone if there is a valid sms-number.  
	@pre 			None
	@post			The sms-number is parsed and it only contains the allowed characters.
 */
	TInt ParseSmsNumber(const TPtrC& aNumber, TPtrC& aParsedNumber);

/**
	@fn				void GetServiceCenter(const TPtrC& aComponent, TPtrC& aServiceCenter)
	Intended Usage	:	Sets the service center number from the component
	@since			7.0
	@param			aComponent			A descriptor pointer to the component.
	@param			aServiceCenter		This is an output argument that is set to the 
										service center number if it exists otherwise a NULL
										descriptor is set. 
	@pre 			None
	@post			The service center number have been set.
 */
	void GetServiceCenter(const TPtrC& aComponent, TPtrC& aServiceCenter);

/**
	@fn				SendL()
	Intended Usage	:	Creates the sms message with the sms-numbers filled in 
						and launches the sms editor.
	@since			7.0
	@pre 			None
	@post			Sms message have been created and sms editor has been launched.
 */
	virtual void SendL();

private:		// Attributes

	/** Array containing SSmsNumber structure */
	RArray<SSmsNumber> iNumbers;

	// To access the private members for testing 
	friend class TSmsSchemeHandler_StateAccessor;
	};

#endif
