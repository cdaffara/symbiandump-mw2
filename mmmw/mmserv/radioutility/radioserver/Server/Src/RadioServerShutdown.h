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
* Description:  This class implements the shutdown timer that is used by the RadioServer
*				 to shutdown the server 2 seconds after the last client has disconnected.
*
*/



#ifndef RADIOSERVERSHUTDOWN_H
#define RADIOSERVERSHUTDOWN_H

//  INCLUDES
#include <e32base.h>

// CLASS DECLARATION

/**
*  Shutdown timer after the last client has disconnected.
*
*  @lib RadioServer.exe
*  @since S60 3.0
*/
class CRadioServerShutdown : public CTimer
    {
public:  // Constructors and destructor

	/**
	 * Two-phased constructor.
	 */
	static CRadioServerShutdown* NewL();

	/**
	 * Destructor.
	 */
	virtual ~CRadioServerShutdown();

public: // New functions

	/**
	 * Starts the shutdown count down.
	 * @since S60 3.0
	 */
	void Start();

private:  // Functions from base classes

	/**
	 * From CTimer
	 * Timer has expired, stop the RadioServer.
	 **/
	void RunL();

private:

	/**
	 * C++ default constructor.
	 */
	CRadioServerShutdown();

	/**
	 * By default Symbian 2nd phase constructor is private.
	 */
	void ConstructL();
    };

#endif      // RADIOSERVERSHUTDOWN_H

// End of File
