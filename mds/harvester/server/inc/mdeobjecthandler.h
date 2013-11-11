/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*/

#ifndef MDEOBJECTHANDLER_H
#define MDEOBJECTHANDLER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <f32file.h>

// CLASS DECLARATION
class CMdESession;
class CHarvesterData;
class CMdENamespaceDef;
class CMdEObject;

/**
*  CMdeObjectHandler
* 
*/
class CMdeObjectHandler : public CBase
{
public: // Constructors and destructor

	/**
		* Destructor.
		*/
	~CMdeObjectHandler();

		/**
		* Two-phased constructor.
		*/
	static CMdeObjectHandler* NewL( CMdESession& aSession );

		/**
		* Two-phased constructor.
		*/
	static CMdeObjectHandler* NewLC( CMdESession& aSession );
	
	void SetMetadataObjectL( CHarvesterData& aHD );
	CMdEObject* GetMetadataObjectL( CHarvesterData& aHD, const TDesC& aObjectDefStr );
	
private:

	/**
		* Constructor for performing 1st stage construction
		*/
	CMdeObjectHandler( CMdESession& aSession );

	/**
		* default constructor for performing 2nd stage construction
		*/
	void ConstructL();
	
	CMdESession* iMdeSession;
	CMdENamespaceDef* iDefaultNamespace;
	RFs iFs;
};

#endif // MDEOBJECTHANDLER_H

