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
* Description:  Provides testing of CPosLmItemIterator class methods
*
*/



#ifndef CTESTPOSLMITEMITERATOR_H
#define CTESTPOSLMITEMITERATOR_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <EPos_CPosLmItemIterator.h>

// CLASS 

class CTestPosLmItemIterator
	{
	public:	//Public constructor of class
	
		/**
		*
		*/
		CTestPosLmItemIterator(CStifLogger* aLog);
		
		static CTestPosLmItemIterator* NewL(CStifLogger* aLog);
		
		void ConstructL();
		
		~CTestPosLmItemIterator();
		
	public:	//Public methods
	
		/**
		*
		*/
		TInt NumOfItemsL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt NextItemL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt GetItemIdsL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt ResetIteratorL( CStifItemParser& aItem );
		
		
		
	private:
		
		// Private members
		CStifLogger* iLog;	//Log		
	};
	
#endif // CTESTPOSLMITEMITERATOR_H	