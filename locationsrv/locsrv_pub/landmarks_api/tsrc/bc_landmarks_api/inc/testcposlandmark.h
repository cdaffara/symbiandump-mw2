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
* Description:  Provides testing of CPosLandmark class methods
*
*/



#ifndef CTESTPOSLANDMARK_H
#define CTESTPOSLANDMARK_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <EPos_CPosLandmark.h>

// CLASS 

class CTestPosLandmark
	{
	public:	// Public constructor
	
		/**
		*
		*/
		CTestPosLandmark(CStifLogger* aLog);
		
		static CTestPosLandmark* NewL(CStifLogger* aLog);
		
		void ConstructL();
		
		~CTestPosLandmark();
	
	public:	// Public methods of class
	
		/**
		*
		*/
		TInt CreateNewLandmarkL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt CreateNewLandmarkLC( CStifItemParser& aItem );

		/**
		*
		*/
		TInt CreateLandmarkCopyL( CStifItemParser& aItem );

		/**
		*
		*/
		TInt CreateLandmarkCopyLC( CStifItemParser& aItem );

		/**
		*
		*/
		TInt SetLandmarkNameL( CStifItemParser& aItem );

		/**
		*
		*/
		TInt GetLandmarkNameL( CStifItemParser& aItem );

		/**
		*
		*/
		TInt SetAndGetLmkDescL( CStifItemParser& aItem );

		/**
		*
		*/
		TInt SetAndGetCovRadiusL( CStifItemParser& aItem );

		/**
		*
		*/
		TInt SetAndGetPositionL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetAndGetPositionFieldL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt NumOfPosFieldsL( CStifItemParser& aItem );

		/**
		*
		*/
		TInt IsPosFieldAvailableL( CStifItemParser& aItem );

		/**
		*
		*/
		TInt RemovePositionFieldL( CStifItemParser& aItem );

		/**
		*
		*/
		TInt RemoveAttributesL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt PositionFieldIdL( CStifItemParser& aItem );

		/**
		*
		*/
		TInt LandmarkIdL( CStifItemParser& aItem );

		/**
		*
		*/
		TInt IsPartialL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetAndGetIconL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt AddAndRemoveLmCategoryL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt GetCategoriesL( CStifItemParser& aItem );
		
		/**
        *
        */
		TInt SetAndGetPlaceId( CStifItemParser& aItem );
        /**
        *
        */
		TInt SetAndGetTimeStamp( CStifItemParser& aItem );

					
	private: //Private methods
	
		/**
		*
		*/
		TInt RemoveAttrName( CPosLandmark* aLandmark );
	
		/**
		*
		*/
		TInt RemoveAttrPosition( CPosLandmark* aLandmark );
		
		/**
		*
		*/
		TInt RemoveAttrCovRadius( CPosLandmark* aLandmark );
		
		/**
		*
		*/
		TInt RemoveAttrDescription( CPosLandmark* aLandmark );

				
		/**
		*
		*/
		//TInt RemoveAttributes( CStifItemParser& aItem );
		
		//Private data

		CStifLogger* iLog;	//Log			
	};

#endif      // CTESTPOSLANDMARK_H

// End of File
