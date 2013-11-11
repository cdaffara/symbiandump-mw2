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
* Description:  Provides testing of CPosLandmarkEncoder class methods
*
*/



#ifndef CTESTPOSLANDMARKENCODER_H
#define CTESTPOSLANDMARKENCODER_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <EPos_CPosLmItemIterator.h>

// CLASS 

class CTestPosLandmarkEncoder
	{
	public:	//Public constructor of class
	
		/**
		*
		*/
		CTestPosLandmarkEncoder(CStifLogger* aLog);
		
		static CTestPosLandmarkEncoder* NewL(CStifLogger* aLog);
		
		void ConstructL();
		
		~CTestPosLandmarkEncoder();
		
	public:	//Public methods
	
		/**
		*
		*/
		TInt CreateEncoderL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetOutputFileL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetOutputBufferL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt AddCollectionDataL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt AddLandmarkToEncodeL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt AddCategoryForLatestLandmarkL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt FinalizeEncodingL( CStifItemParser& aItem );
		
				
	private:
		
		// Private members
		CStifLogger* iLog;	//Log		
	};
	
#endif // CTESTPOSLANDMARKENCODER_H
	