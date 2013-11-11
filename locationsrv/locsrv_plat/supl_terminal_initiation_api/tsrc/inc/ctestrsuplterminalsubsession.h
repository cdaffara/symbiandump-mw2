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
* Description:  Class declaration of CTestRSuplTerminalSubSession
*
*/



#ifndef CTESTRSUPLTERMINALSUBSESSION_H
#define CTESTRSUPLTERMINALSUBSESSION_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

// CLASS 

class CTestRSuplTerminalSubSession : public CActive
	{
	public:	//Public constructor of class
	
		/**
		*
		*/
		CTestRSuplTerminalSubSession(CStifLogger* aLog);
		
		static CTestRSuplTerminalSubSession* NewL(CStifLogger* aLog);
		
		void ConstructL();
		
		~CTestRSuplTerminalSubSession();
		
	
	public:	//Public methods
	
		/**
		*
		*/
		TInt OpenCloseL( CStifItemParser& aItem );
		
				
		/**
		*
		*/
		TInt GetPositionL( CStifItemParser& aItem );
		
		
		
				
			
	protected:

		void RunL();
		
		void DoCancel();
					
	private:
		
		// Private members
		CStifLogger* iLog;	//Log		
	};
	
#endif // CTESTRSUPLTERMINALSUBSESSION_H
	