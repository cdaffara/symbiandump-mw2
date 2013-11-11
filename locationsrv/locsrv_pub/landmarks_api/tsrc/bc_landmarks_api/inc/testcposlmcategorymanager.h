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
* Description:  Provides testing of CPosLmCategoryManager class methods
*
*/



#ifndef CTESTPOSLMCATEGORYMGR_H
#define CTESTPOSLMCATEGORYMGR_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <EPos_CPosLandmarkCategory.h>

// CLASS 

class CTestPosLmCategoryManager
	{
	public:	//Public constructor of class
	
		/**
		*
		*/
		CTestPosLmCategoryManager(CStifLogger* aLog);
		
		static CTestPosLmCategoryManager* NewL(CStifLogger* aLog);
		
		void ConstructL();
		
		~CTestPosLmCategoryManager();
		
	public:	//Public methods
	
		/**
		*
		*/
		TInt NewLCreateCategoryManagerL(CStifItemParser& aItem);
		
		/**
		*
		*/
		TInt AddAndRemoveCategoryL(CStifItemParser& aItem);
		
		/**
		*
		*/
		TInt UpdateCategoryL(CStifItemParser& aItem);
		
		/**
		*
		*/
		TInt AddCatToLandmarksL(CStifItemParser& aItem);
		
		/**
		*
		*/
		TInt RemoveCatFromLandmarksL(CStifItemParser& aItem);
		
		/**
		*
		*/
		TInt CategoryIteratorL(CStifItemParser& aItem);
		
		/**
		*
		*/
		TInt ReferencedIteratorL(CStifItemParser& aItem);
		
		/**
		*
		*/
		TInt ReadCategoryL(CStifItemParser& aItem);
		
		/**
		*
		*/
		TInt GetCategoryL(CStifItemParser& aItem);
		
		/**
		*
		*/
		TInt RemoveCategoriesL(CStifItemParser& aItem);
		
		/**
		*
		*/
		TInt GetGlobalCategoryL(CStifItemParser& aItem);

		/**
		*
		*/
		TInt GlobalCategoryNameL(CStifItemParser& aItem);
		
		/**
		*
		*/
		TInt ResetGlobalCategoriesL(CStifItemParser& aItem);
		
	private:
		
		// Private members
		CStifLogger* iLog;	//Log	
	};
	
#endif	// CTESTPOSLMCATEGORYMGR_H