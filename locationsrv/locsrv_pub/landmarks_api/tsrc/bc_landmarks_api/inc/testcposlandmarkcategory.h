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
* Description:  Provides testing of CTestPosLandmarkCategory class methods
*
*/



#ifndef CTESTPOSLANDMARKCATEGORY_H
#define CTESTPOSLANDMARKCATEGORY_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <EPos_CPosLandmarkCategory.h>

// CLASS 

class CTestPosLandmarkCategory
	{
	public:	//Public constructor of class
	
		/**
		*
		*/
		CTestPosLandmarkCategory(CStifLogger* aLog);
		
		static CTestPosLandmarkCategory* NewL(CStifLogger* aLog);
		
		void ConstructL();
		
		~CTestPosLandmarkCategory();
		
	public:	//Public methods
	
		/**
		*
		*/
		TInt NewLCreateCategoryL(CStifItemParser& aItem);
		
		/**
		*
		*/
		TInt NewLCCreateCategoryLC(CStifItemParser& aItem);

		/**
		*
		*/
		TInt NewLCreateCategoryCopyL(CStifItemParser& aItem);
		
		/**
		*
		*/
		TInt NewLCCreateCategoryCopyLC(CStifItemParser& aItem);
		
		/**
		*
		*/
		TInt SetAndGetCategoryNameL(CStifItemParser& aItem);
		
		/**
		*
		*/
		TInt SetAndGetCategoryIconL(CStifItemParser& aItem);
		
		/**
		*
		*/
		TInt CategoryIdL(CStifItemParser& aItem);
		
		/**
		*
		*/
		TInt CheckGlobalL(CStifItemParser& aItem);
		
		/**
		*
		*/
		TInt RemoveCategoryAttributesL(CStifItemParser& aItem);
	
	private:
	
		// Private members
		CStifLogger* iLog;	//Log
	};

#endif //CTESTPOSLANDMARKCATEGORY_H
