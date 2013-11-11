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
* Description:  Provides testing of CPosLandmarkDatabase class methods
*
*/



#ifndef CTESTPOSLANDMARKDATABASE_H
#define CTESTPOSLANDMARKDATABASE_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
//#include <EPos_CPosLandmarkDatabse.h>

// CLASS 
class CPosLandmarkDatabase;

class CTestPosLandmarkDatabase : public CActive
	{
	public:	//Public constructor of class
	
		/**
		*
		*/
		CTestPosLandmarkDatabase(CStifLogger* aLog);
		
		static CTestPosLandmarkDatabase* NewL(CStifLogger* aLog);
		
		void ConstructL();
		
		~CTestPosLandmarkDatabase();
		
	public:	//Public methods
	
		/**
		*
		*/
		static TInt InitTestDatabaseL( CStifLogger* aLog )	;
		
		/**
		*
		*/
		static TInt AddLandmarksToTestDbL( CPosLandmarkDatabase* aDatabase );
		
		/**
		*
		*/
		static TInt AddCategoriesToTestDbL( CPosLandmarkDatabase* aDatabase );
		
		/**
		*
		*/
		TInt OpenDefaultDatabaseL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt OpenDatabaseL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt IsInitializingNeededL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt InitializeL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt GetDatabaseUriL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt GetDatabaseSizeL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt LandmarkIteratorL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt LmkIteratorSortPrefL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt ReadLandmarkL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt AddAndRemoveLandmarkL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt UpdateLandmarkL( CStifItemParser& aItem );

		/**
		*
		*/
		TInt RemoveLandmarksL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt RemoveAllLandmarksL( CStifItemParser& aItem );

		/**
		*
		*/
		TInt CompactL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetAndGetPartialReadParamsL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt ReadPartialLandmarkL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt PrepareAndTakePartialLmksL( CStifItemParser& aItem );

		/**
		*
		*/
		TInt ExportLandmarksL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt ImportLandmarksL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt ImportSelectedLandmarksL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt ImportedLmkIteratorL( CStifItemParser& aItem );		

		/**
		*
		*/
		TInt NotifyDatabaseEventL( CStifItemParser& aItem );		
		
		/**
		*
		*/
		TInt CancelNotifyDatabaseEventL( CStifItemParser& aItem );		
		
		/**
		*
		*/
		TInt OpenExtendedDatabaseL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt ExtendedDbLandmarkCountL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt ExtendedDbCategoryCountL( CStifItemParser& aItem );
		
			
	protected:

		void RunL();
	
		void DoCancel();	
		
	private:
		
		// Private members
		CStifLogger* iLog;	//Log		
		
		TInt iLastResult;
		
		TPosLmEvent iEvent;
	};
	
#endif // CTESTPOSLANDMARKDATABASE_H	