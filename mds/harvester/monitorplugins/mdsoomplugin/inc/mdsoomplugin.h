/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Low memory observer for havester server
*
*/


#ifndef MDSOOMPLUGIN_H
#define MDSOOMPLUGIN_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32msgqueue.h>

#include <oommonitorplugin.h>

// CLASS DECLARATION

/**
*  CMdSOomPlugin
* 
*/
class CMdSOomPlugin : public COomMonitorPluginV2
	{
public:
	// Constructors and destructor
	static CMdSOomPlugin* NewL();
	
	/**
	 * Destructor.
	 */
	virtual ~CMdSOomPlugin();	
	

	/**
	 * From COomMonitorPlugin v2
	 * FreeRam is called when the system RAM level becomes
	 * low. This plugin is requested to help free some RAM.
	 */
	void FreeRam( TInt aBytesToFree );

	/**
	 * From COomMonitorPlugin v2
	 * MemoryGood is called when the system RAM level becomes
	 * good after being low.The plugin may take this opportunity
	 * to start using RAM again.
	 * Nb It is assumed that the plugin will not immediately
	 * cause a large increase in memory use, but that memory may be
	 * used over time, otherwise the plugin may cause oscillation
	 * between low and good memory states.
	 */
	void MemoryGood();

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CMdSOomPlugin();
	
	/**
	 * default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	/**
	 * From COomMonitorPlugin
	 */
	void ExtensionInterface(TUid aInterfaceId, TAny*& aImplementation);
    
private:

	RMsgQueue<TInt> iOomMsgQueue;
	
	};

#endif // MDSOOMPLUGIN_H

