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
* Description:  Provides testing of CPosLandmarkParser class methods
*
*/



#ifndef CTESTPOSLANDMARKPARSER_H
#define CTESTPOSLANDMARKPARSER_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <EPos_CPosLmItemIterator.h>

// CLASS 

class CTestPosLandmarkParser
	{
	public:	//Public constructor of class
	
		/**
		*
		*/
		CTestPosLandmarkParser(CStifLogger* aLog);
		
		static CTestPosLandmarkParser* NewL(CStifLogger* aLog);
		
		void ConstructL();
		
		~CTestPosLandmarkParser();
		
	public:	//Public methods
	
		/**
		*
		*/
		TInt CreateParserL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetInputBufferL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetInputFileL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetInputFileHandleL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt ParseContentL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt NumOfParsedContentL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt FirstAndNextCollDataIdL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt GetCollectionDataL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt RetrieveParsedLmkL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt RetrieveCategoryL( CStifItemParser& aItem );
				
		
	private:
		
		// Private members
		CStifLogger* iLog;	//Log		
	};
	
#endif // CTESTPOSLANDMARKPARSER_H
	