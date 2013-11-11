/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Active object for each asynchronous request in RRadioSession.
*				 Notifies MRadioObserver upon request completion.
*
*/



#ifndef RADIOREQUEST_H
#define RADIOREQUEST_H

//  INCLUDES
#include <e32base.h>
#include "RadioSession.h"
#include "RadioClientServer.h"

// CLASS DECLARATION

/**
*  Implements active object for outstanding asynchronous request.
*
*  @lib RadioSession.lib
*  @since Series 60 3.0
*/
class CRadioRequest : public CActive
    {
public:  // Constructors and destructor

	/**
	 * Two-phased constructor.
	 */
	static CRadioRequest* NewLC( RRadioSession& aSession, MRadioObserver& aObserver,
        			TRadioServerRequest aRequest );

	/**
	 * Destructor.
	 */
	virtual ~CRadioRequest();

public: // New functions

	/**
	 * Completes the outstanding request.
	 * @since Series 60 3.0
	 * @param aRequest Request being completed
	 * @param aError A standard system error code
	 */
	void CompleteRequest( TRadioServerRequest aRequest, TInt aError );

private:  // Functions from base classes

	/**
	 * From CActive
	 * Cancel outstanding request
	 */
	void DoCancel();

	/**
	 * From CActive
	 * Implementation of CActive::RunL. Called when server request has completed.
	 **/
	void RunL();

private:

	/**
	 * C++ default constructor.
	 */
	CRadioRequest( RRadioSession& aSession, MRadioObserver& aObserver,
			TRadioServerRequest aRequest );

	/**
	 * By default Symbian 2nd phase constructor is private.
	 */
	void ConstructL();

private:    // Data

	// Radio session
	RRadioSession& iSession;
	// Radio observer
	MRadioObserver& iObserver;
	// Request
	TRadioServerRequest iRequest;

	// Boolean package buffer
	TBool iBool;
	TPckg<TBool> iBoolPckg;

	// Integer package buffer
	TInt iInt;
	TPckg<TInt> iIntPckg;

private:    // Friend classes
	friend class RRadioSession;
    };

#endif      // RADIOREQUEST_H

// End of File
