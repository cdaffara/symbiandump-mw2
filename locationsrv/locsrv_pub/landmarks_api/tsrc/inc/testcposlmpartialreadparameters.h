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
* Description:  Provides testing of CPosLmPartialReadParameters class methods
*
*/



#ifndef CTESTPOSLMPARTIALREADPARAMS_H
#define CTESTPOSLMPARTIALREADPARAMS_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <EPos_CPosLmItemIterator.h>

// CLASS 

class CTestPosLmPartialReadParams
	{
	public:	//Public constructor of class
	
		/**
		*
		*/
		CTestPosLmPartialReadParams(CStifLogger* aLog);
		
		static CTestPosLmPartialReadParams* NewL(CStifLogger* aLog);
		
		void ConstructL();
		
		~CTestPosLmPartialReadParams();
		
	public:	//Public methods
	
		/**
		*
		*/
		TInt CreatePartialReadParamsL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetAndGetRequestedAttrL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetAndGetRequestedPosFieldL( CStifItemParser& aItem );
		
		
		
	private:
		
		// Private members
		CStifLogger* iLog;	//Log		
	};
	
#endif // CTESTPOSLMPARTIALREADPARAMS_H
	