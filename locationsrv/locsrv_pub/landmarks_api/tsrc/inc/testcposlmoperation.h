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
* Description:  Provides testing of CPosLmOperation class methods
*
*/



#ifndef CTESTPOSLMOPERATION_H
#define CTESTPOSLMOPERATION_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <EPos_CPosLmOperation.h>

// CLASS 

class CTestPosLmOperation : public CActive
	{
	public:	//Public constructor of class
	
		/**
		*
		*/
		CTestPosLmOperation(CStifLogger* aLog);
		
		static CTestPosLmOperation* NewL(CStifLogger* aLog);
		
		void ConstructL();
		
		~CTestPosLmOperation();
		
	public:	//Public methods
	
		/**
		*
		*/
		TInt DestructOperationL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt ExecuteL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt ExecuteAndDeleteOperationL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt NextStepL( CStifItemParser& aItem );
		
	
		
	protected:

		void RunL();
	
		void DoCancel();
	
		
	private:
		
		// Private members
		CStifLogger* iLog;	//Log
		
		TReal32 iProgress; // Progress of operation
		
		CPosLmOperation* iOperation;
		
		TInt iLastResult;
		
		CPosLandmarkDatabase* iDatabase;
	};
	
#endif // CTESTPOSLMOPERATION_H	
