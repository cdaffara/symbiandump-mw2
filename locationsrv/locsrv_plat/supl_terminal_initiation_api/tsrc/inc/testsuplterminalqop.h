/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Class declaration of TestSuplTerminalQop
*
*/



#ifndef TESTSUPLTERMINITQOP_H
#define TESTSUPLTERMINITQOP_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

// CLASS 

class TestSuplTerminalQop
	{
	public:	//Public constructor of class
	
		/**
		*
		*/
		TestSuplTerminalQop(CStifLogger* aLog);
	
		
		~TestSuplTerminalQop();
		
		
	public:	//Public methods
	
		/**
		*
		*/
		TInt SetL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetHorAccuracyL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetVerAccuracyL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetMaxLocationAgeL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetDelayL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt GetL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt GetHorAccuracyL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt GetVerAccuracyL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt GetMaxLocationAgeL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt GetDelayL( CStifItemParser& aItem );
		
		
				
			
	protected:

					
	private:
		
		// Private members
		CStifLogger* iLog;	//Log		
	};
	
#endif // TESTSUPLTERMINITQOP_H
	