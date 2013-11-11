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
*
*/

#ifndef MDEOBJECTWRAPPER_H
#define MDEOBJECTWRAPPER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION
class CMdEObject;
class CMdEPropertyDef;

/**
*  CMdeObjectWrapper
* 
*/
NONSHARABLE_CLASS( CMdeObjectWrapper ) : public CBase
{
public: // Constructors and destructor

	/**
		* Destructor.
		*/
	~CMdeObjectWrapper();

		/**
		* Two-phased constructor.
		*/
	IMPORT_C static CMdeObjectWrapper* NewL();

		/**
		* Two-phased constructor.
		*/
	IMPORT_C static CMdeObjectWrapper* NewLC();
	
	IMPORT_C void HandleObjectPropertyL(CMdEObject& aMdeObject, const TDesC& aPropertyType, TAny* aData, TBool aIsAdd = ETrue);

	IMPORT_C static void HandleObjectPropertyL(CMdEObject& aMdeObject, CMdEPropertyDef& aPropertyDef, TAny* aData, TBool aIsAdd = ETrue);
	
private:

	/**
		* Constructor for performing 1st stage construction
		*/
	CMdeObjectWrapper();

	/**
		* default constructor for performing 2nd stage construction
		*/
	void ConstructL();

};

#endif // MDEOBJECTWRAPPER_H

