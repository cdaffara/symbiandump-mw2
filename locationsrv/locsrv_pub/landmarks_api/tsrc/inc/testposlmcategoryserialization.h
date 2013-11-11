/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides testing of PosLmCategorySerialization class methods
*
*/



#ifndef TESTPOSLMKCATSERIALIZATION_H
#define TESTPOSLMKCATSERIALIZATION_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <EPos_CPosLmItemIterator.h>

// CLASS 

class TestPosLmkCatSerialization
	{
	public:	//Public constructor of class
	
		/**
		*
		*/
		TestPosLmkCatSerialization(CStifLogger* aLog);
		
	public:	//Public methods
	
		/**
		*
		*/
		TInt PackCategoryL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt UnpackCategoryL( CStifItemParser& aItem );
		
				
	private:
		
		// Private members
		CStifLogger* iLog;	//Log		
	};
	
#endif // TESTPOSLMKCATSERIALIZATION_H
	