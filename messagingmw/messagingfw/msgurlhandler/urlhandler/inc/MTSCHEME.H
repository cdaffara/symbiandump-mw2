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
// This file contains the API definition for the class CMailtoSchemeHandler.
// This class parses a descriptor Url into the component fields of a mailto
// scheme as described in RFC2368 
// and launches the mailto editor with the field values filled in.
// 
//

/**
 @file
*/

#ifndef __MTSCHEME_H__
#define __MTSCHEME_H__

// Local includes
//
#include "MSGBASE.H"

/**
	@class
	Dependencies :	CMsgSchemeHandlerBase, EscapeUtils, TMailtoUrlParser, 
					TDelimitedAddressParser, TDelimitedBodyParser, TextUtils, 
					CSendAs, TMsvId
	Comments :	Parses a descriptor into the component fields of a mailto scheme 
				as described in RFC2368 
				and launches the mailto editor with the field values filled in.
				There are 5 fields; to, cc, bcc, subject and body.
	
	Format of a typical mailto url is: 
		mailto:<address>,<address>?cc=<address>&subject=<subject>&body=<body>

	@since 7.0
*/
class CMailtoSchemeHandler : public CMsgSchemeHandlerBase
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
	@return			A pointer to the newly created CMailtoSchemeHandler object 
	@pre 			None
	@post			A fully constructed and initialised CSmsSchemeHandler object.
 */
	static CMailtoSchemeHandler* NewLC();

/**
	@fn				NewL()
	Intended Usage	:	Static factory constructor. Uses two phase construction and 
						leaves nothing on the CleanupStack.
	@since			7.0
	@leave		KErrNoMemory.
	@return			A pointer to the newly created CMailtoSchemeHandler object. 
	@pre 			None
	@post			A fully constructed and initialised CMailtoSchemeHandler object.
 */
	static CMailtoSchemeHandler* NewL();

/**
	@fn				~CMailtoSchemeHandler()
	Intended Usage	:	Destructor.
	@since			7.0
 */
	~CMailtoSchemeHandler();


private:	// Methods

/**
	@fn				CMailtoSchemeHandler()
	Intended Usage	:	Constructor. First phase of two-phase construction method. 
						Does non-allocating construction.
	@since			7.0
 */
	CMailtoSchemeHandler();

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
	Intended Usage	: Parses a descriptor into the component fields of a mailto scheme 
					  as described in RFC2368. 
	@since			7.0
	@param			aUrl	A descriptor with the mailto url.
	@pre 			None
	@post			The mailto url descriptor has been parsed and field values are set.
 */
	virtual void ParseUrlL(const TDesC& aUrl);


/**
	@enum	TField
	Enum defining the mailto url fields
	@since	7.0
*/
	enum TField
		{
		/** Specifies 'To' field */
		ETo, 
		/** Specifies 'Body' field */
		EBody, 
		/** Specifies 'Subject' field */
		ESubject, 
		/** Specifies 'Cc' field */
		ECc, 
		/** Specifies 'Bcc' field */
		EBcc, 
		/** Specifies unknown field */
		EUnknown
		};

/**
	@fn				SetFieldL(TField aField, const TDesC& aFieldValue)
	Intended Usage	:	Sets the field values.
	@since			7.0
	@param			aField			The enum specifiying the mailto field.
	@param			aFieldValue		A descriptor with the mailto field value.
	@pre 			None
	@post			Field values have been set.
 */
	void SetFieldL(TField aField, const TDesC& aFieldValue);

/**
	@fn				GetFieldAndValue(const TDesC& aComponent, TPtrC& aFieldValue)
	Intended Usage	:	Returns the field and field value of the component
	@since			7.0
	@param			aComponent		a descriptor with the mailto component.
	@param			aFieldValue		This is an output argument that is set to the 
									field value. 
	@return			The enum specifying the field of the component.
	@pre 			None
	@post			The field and field value of the component have been returned.
 */
	TField GetFieldAndValue(const TDesC& aComponent, TPtrC& aFieldValue) const;


/**
	@fn				SendL()
	Intended Usage	:	Creates the email message with the field values filled in 
						and launches the mailto editor.
	@since			7.0
	@pre 			None
	@post			Email message have been created and mailto editor has been launched.
 */
	virtual void SendL();

private:		// Attributes

	/** The descriptor array that contains the addresses */
	RArray<TPtrC> iTo;
	/** The descriptor pointer that contains the subject */
	TPtrC iSubject;
	/** The descriptor pointer that contains the body */ 
	TPtrC iBody;

	/** The descriptor buffer that contains the mailto part of the url
		i.e mailto:<mailto part> 
		This provides the storage for the text pointed to by iTo & iSubject.
	*/
	HBufC* iMailtoPart;

	// To access the private members for testing.
	friend class TMailtoSchemeHandler_StateAccessor;
	};

#endif
