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
* Description:  Class declaration of TestRSuplTerminalServer
*
*/



#ifndef TESTRSUPLTERMINALSERVER_H
#define TESTRSUPLTERMINALSERVER_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

// CLASS 

class TestRSuplTerminalServer
	{
	public:	//Public constructor of class
	
		/**
		*
		*/
		TestRSuplTerminalServer(CStifLogger* aLog);
	
		
		~TestRSuplTerminalServer();
		
		
	public:	//Public methods
	
		/**
		*
		*/
		TInt ConnectCloseL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt VersionL( CStifItemParser& aItem );
		
		/**
		*
		*/
		//TInt SetVerAccuracyL( CStifItemParser& aItem );
		
		
		
				
			
	protected:

					
	private:
		
		// Private members
		CStifLogger* iLog;	//Log		
	};
	
#endif // TESTRSUPLTERMINALSERVER_H
	