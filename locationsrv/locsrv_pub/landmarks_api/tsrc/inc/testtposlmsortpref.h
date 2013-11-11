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
* Description:  Provides testing of TPosLmSortPref class methods
*
*/



#ifndef TTESTPOSLMSORTPREF_H
#define TTESTPOSLMSORTPREF_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <EPos_CPosLmItemIterator.h>

// CLASS 

class TTestPosLmSortPref
	{
	public:	//Public constructor of class
	
		/**
		*
		*/
		TTestPosLmSortPref(CStifLogger* aLog);
		
	public:	//Public methods
	
		/**
		*
		*/
		TInt CreateSortPref( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt GetSortOrder( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetAndGetSortAttr( CStifItemParser& aItem );
		
		
		
	private:
		
		// Private members
		CStifLogger* iLog;	//Log		
	};
	
#endif // TTESTPOSLMSORTPREF_H
	