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
* Description:  This class implements the server-side session.
*
*/



#ifndef RADIOSERVERSESSION_H
#define RADIOSERVERSESSION_H

//  INCLUDES

#include <MCustomInterfaceCustomCommandImplementor.h>

#include "RadioServerData.h"

// FORWARD DECLARATIONS
class CRadioServer;
class CCustomInterfaceBuilder;
class CMMFObjectContainer;
class CMMFCustomCommandParserManager;

// CLASS DECLARATION

/**
*  This class implements the server-side session.
*
*  @lib RadioServer.exe
*  @since S60 3.0
*/
class CRadioServerSession : public CSession2,
							public MCustomInterfaceCustomCommandImplementor
    {
public:  // Constructors and destructor

	/**
	 * Two-phased constructor.
	 */
	static CRadioServerSession* NewL( CRadioServer& aServer );

	/**
	 * Destructor.
	 */
	virtual ~CRadioServerSession();

private:  // Functions from base classes

	/**
	 * From CSession2
	 * Handles the servicing of a client request that has been passed to the server.
	 */
	void ServiceL( const RMessage2& aMessage );

	/**
	 * From MCustomInterfaceCustomCommandImplementor
	 * Creates a Custom Interface Builder.
	 */
	const TMMFMessageDestination& GetCustomInterfaceBuilderL();

private:

	/**
	 * C++ default constructor.
	 */
	CRadioServerSession( CRadioServer& aServer );

	/**
	 * By default Symbian 2nd phase constructor is private.
	 */
	void ConstructL();

	/**
	 * Returns a CMMFObjectContainer reference to CMMFObjectContainer.
	 * @return Reference to CMMFObjectContainer
	 */
	CMMFObjectContainer& MMFObjectContainerL();

private:    // Data

	// Reference to the radio server
	CRadioServer& iServer;
	// Pointer to the custom interface builder
	CCustomInterfaceBuilder* iCustomInterfaceBuilder;
	// Container for Custom Interface builder and message handlers created by it
	CMMFObjectContainer* iObjectContainer;
	// Container for Custom Interface Custom Command Parser object
	CMMFCustomCommandParserManager* iCustomCommandParserManager;
    };

#endif      // RADIOSERVERSESSION_H

// End of File
