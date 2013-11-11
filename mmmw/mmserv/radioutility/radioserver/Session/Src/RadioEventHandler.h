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
* Description:  Active object for each spontaneous event from Radio Server.
*				 Notifies MRadioEventObserver upon receiving a new event.
*
*/



#ifndef RADIOEVENTHANDLER_H
#define RADIOEVENTHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <e32property.h>

#include "RadioSession.h"

// CLASS DECLARATION

/**
*  Implements active object for spontaneous event.
*
*  @lib RadioSession.lib
*  @since Series 60 3.0
*/
class CRadioEventHandler : public CActive
    {
public:  // Constructors and destructor

	/**
	 * Two-phased constructor.
	 */
	static CRadioEventHandler* NewLC( MRadioObserver& aObserver, RRadioSession& aSession, TUint aKey );

	/**
	 * Destructor.
	 */
	virtual ~CRadioEventHandler();

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
	CRadioEventHandler( MRadioObserver& aObserver,RRadioSession& aSession, TUint aKey );

	/**
	 * By default Symbian 2nd phase constructor is private.
	 */
	void ConstructL();

private:    // Data

	// Radio session
	RRadioSession& iSession;
	// Property
	RProperty iProperty;
	// Property key
	TUint iKey;

	MRadioObserver& iObserver;
    };

#endif      // RADIOEVENTHANDLER_H

// End of File
