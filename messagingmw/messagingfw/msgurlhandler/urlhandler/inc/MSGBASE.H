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
// This file contains the API definition for the class CMsgSchemeHandlerBase.
// This is an abstract base class which parses a descriptor url into the 
// scheme specific components and launches the correct mail editor.
// 
//

/**
 @file
*/

#ifndef __MSGBASE_H__
#define __MSGBASE_H__

// System includes
//
#include <e32base.h>
#include <msvstd.h>


// Forward declaration
//
class CRichText;
class CParaFormatLayer;
class CCharFormatLayer;


/**
	@class
	Dependencies :	CBase, MSendAsObserver, CParaFormatLayer, CCharFormatLayer, CRichText,
					MturUtils
	Comments :	This is an abstract base class which parses a descriptor url into the 
				scheme specific components and launches the correct mail editor.
				There are 2 messaging schemes; mailto and sms. 
	
	Format of a url is: 
		<scheme:<scheme specific part>>

	@since 7.0
*/

class CMsgSchemeHandlerBase : public CBase

/**
@internalComponent
@released
*/
	{

public:		// Methods

/**
	@fn				~CMsgSchemeHandlerBase()
	Intended Usage	:	Destructor.
	@since			7.0
 */
	virtual ~CMsgSchemeHandlerBase();

/**
	@fn				ParseUrlAndSendL(const TDesC& aUrl)
	Intended Usage	:	Parses a descriptor url into the scheme specific components 
						and launches the correct mail editor.
	@since			7.0
	@param			aUrl	A descriptor with the messaging url.
	@pre 			None
	@post			The messaging url descriptor has been parsed, field values are set
					and the correct mail editor has been launched.
 */
	void ParseUrlAndSendL(const TDesC& aUrl);

private:		// Methods


/**
	@fn				ParseUrlL(const TDesC& aUrl) = 0
	Intended Usage	:	Pure virtual function that parses a descriptor url into
						the scheme specific components. 
	@since			7.0
	@param			aUrl		A descriptor with the messaging url.
	@pre 			None
	@post			The messaging url descriptor has been parsed and field values are set.
 */
	virtual void ParseUrlL(const TDesC& aUrl) = 0;

/**
	@fn				SendL() = 0
	Intended Usage	:	Pure virtual function that creates the message 
						and launches the mail editor.
	@since			7.0
	@pre 			None
	@post			Message has been created and the editor has been launched.
 */
	virtual void SendL() = 0;

protected:		// Methods		

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
	@fn				CMsgSchemeHandlerBase()
	Intended Usage	:	Constructor. First phase of two-phase construction method. Does 
						non-allocating construction.
	@since			7.0
 */
	CMsgSchemeHandlerBase();

/**
	@fn				LaunchEditorL(const TMsvId& aId)
	Intended Usage	:	Launches the editor.
	@since			7.0
	@param			aId		Id of the message to be launched.
	@pre 			None
	@post			Editor has been launched
 */
	void LaunchEditorL(const TMsvId& aId);

/**
	@fn				GetSchemeSpecificPartL(const TDesC& aUrl, TPtrC& aSchemePart)
	Intended Usage	:	Sets the scheme specific part of a url.
	Error Condition	:	The function leaves with KErrNotFound if the scheme is not
						found.
	@since			7.0
	@param			aUrl			A url descriptor
	@param			aSchemePart		This is an output argument that is set to the 
									scheme specific part. 
	@pre 			None
	@post			The scheme specific part of the url is returned.
 */
	void GetSchemeSpecificPartL(const TDesC& aUrl, TPtrC& aSchemePart);

protected:		// Attributes

	/** The rich text that contains the body of the message */
	CRichText* iRtBody;

private:		// Attributes

	/** Used to create the rich text */
	CParaFormatLayer* iPfl;
	/** Used to create the rich text */
	CCharFormatLayer* iCfl;
	};

#endif
