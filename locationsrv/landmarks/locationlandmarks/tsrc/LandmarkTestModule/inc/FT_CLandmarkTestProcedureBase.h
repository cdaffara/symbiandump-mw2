/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*   ?description_line
*
*/


#ifndef CLANDMARKTESTPROCEDUREBASE_H
#define CLANDMARKTESTPROCEDUREBASE_H

//  INCLUDES
//#include <utfw.h>
#include <w32std.h>
#include <e32def.h>
#include <ss_std.h>

#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmDatabaseManager.h>
#include <EPos_CPosLmOperation.h> 
#include <EPos_CPosLandmarkSearch.h>
#include <EPos_CPosLmMultiDbSearch.h>

#include "FT_CSearchResult.h"
#include "FT_CAreaSearchResult.h"
#include "FT_CCategorySearchResult.h"
#include "FT_CCategoryLandmarkSearchResult.h"

#include <StifLogger.h>
#include <StifItemParser.h>

// Global category IDs as defined by Global Category API specification
// Global category identifier when used for searching
const TInt KAccommodation     = 3000; //Hotel, Camping site
const TInt KBusiness	      = 6000;  //Bank, Factory, Office
const TInt KCommunication     = 9000;  //Internet Access Point, Public Telephone, Wireless LAN Hot Spot
const TInt KEducational	      = 12000;//	School, College
const TInt KEntertainment     = 15000;//	Amusement park, Cinema, Concert hall, Night club
const TInt KFoodBeverage      = 18000;	//Fast food, Restaurant, Café, Bar
const TInt KGeographicalArea  = 21000;//City, City center, Town
const TInt KOutdoorActivities = 24000; //Camping site, Fishing place, Hunting, National park, Playground
const TInt KPeople	          = 27000;// My home, My friend's home, Father's summer cottage, Child's school
const TInt KPublicService	  = 30000; //Tourist information office, Government office, Library, Post office, Hospital, Police
const TInt KReligiousPlaces   = 33000; //Church, Mosque
const TInt KShopping	      = 36000; //Market place, Pharmacy, Shop, Shopping center
const TInt KSightseeing	      = 39000; //Monument, Mountain top, Museum
const TInt KSports	          = 42000	; //Bowling, Golf course, Ice hockey hall, Stadium
const TInt KTransport         =	45000; //Airport, Bus stop, Harbor, Railway station, Rest area

// CONSTANTS
_LIT(KAccessErr, "Access methods should return 'Not initialized' when db has been created");
_LIT(KInitErr, "Method InitializedNeeded should return ETrue after db has been created");
_LIT8(KLmxMimeType, "application/vnd.nokia.landmarkcollection+xml");
_LIT8(KGpxMimeType, "application/gps+xml");
_LIT8(KKmlMimeType,"application/vnd.google-earth.kml+xml");
_LIT8(KKmzMimeType,"application/vnd.google-earth.kmz");
_LIT(KLmxFileExt,".lmx");
_LIT(KGpxFileExt,".gpx");
_LIT(KXmlFileExt,".xml");

const TInt KMsgBufSize = 255;
const TInt KMaxLineLength = 1024;

// FORWARD DECLARATIONS
class CPosLmMultiDbSearch;
class CPosLmSearchCriteria;

// DATA TYPES
typedef RArray<TPosLmItemId> RIdArray;

// CLASS DECLARATION
class CPosLandmarkCategory;
class CActiveLmOperation;

class CLandmarkTestProcedureBase :public CBase
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        CLandmarkTestProcedureBase(TInt aModuleId, CStifLogger* iLog );

        /**
        * Destructor.
        */
        ~CLandmarkTestProcedureBase();
        
        /**
        * Initialises the test.
        * By default does nothing.
        */
        virtual void InitTestL();

        /**
        * Executes the test.
        * By default does nothing.
        */
        virtual void StartL();

        /**
        * Executes the test.
        * By default does nothing.
        */
        virtual void StartL( CStifItemParser& ) {};

        /**
        * Cleans up after a test.
        * Always called, even if the test leaves.
        * By default does nothing.
        */
        virtual void CloseTest();
   

    public: // Functions from base classes

        
       static TInt ReadLn(RFile& aFile, TDes& aDes);
       
       static void ResetAndDestroyHBufCArray( TAny* aPointer );

    protected: // New functions

        static void Trace(TBuf8<KMsgBufSize> msg) ;
        
        /**
        * Removes the default landmark database if exists
        */
        void RemoveDefaultDbL();
        
        /**
        * Removes all existing landmark databases
        */
        void RemoveAllLmDatabasesL();

        /**
        * Compare two landmarks.
        * Should only be used when they are expected to be equal.
        */
        void CompareLandmarksL(const CPosLandmark& aSource, const CPosLandmark& aTarget);

        /**
        * Compare two categories.
        * Should only be used when they are expected to be equal.
        */
        void CompareCategoriesL(
                        const TPosLmItemId& aSourceId, 
                        const TPosLmItemId& aTargetId);

        void CompareCategoriesL(
                        const CPosLandmarkCategory& aSource, 
                        const CPosLandmarkCategory& aTarget);


        TInt CompareXMLCategoriesL(
                        const CPosLandmarkCategory& aSource, 
                        const CPosLandmarkCategory& aTarget, TBool aDebug=EFalse);

        TInt CompareXMLLandmarksL(const CPosLandmark& aSource, const CPosLandmark& aTarget, TBool aDebug=EFalse);
    
        /**
        * Creates a caategory with all attributes set
        * @param the category name
        */
        CPosLandmarkCategory* CreateCategoryLC(const TDesC& aName);
    
        /**
        * Creates a landmarks with all attributes set
        * @param the landmark name
        */
        CPosLandmark* CreateLandmarkLC(const TDesC& aName);

        /**
        * Opens the landmarks database and initializes it if necearry
        * @return the landmarks database
        */
        CPosLandmarkDatabase* OpenDatabaseLC();

        /**
        * Creates a category and adds it to the database
        * @return the category id
        */
        TPosLmItemId CreateCategoryL(const TDesC& aName);

        /**
        * Adds some fields to a landmark
        * @param the landmark
        */
        void AddFieldsL(CPosLandmark* aLandmark);
        
        void AddFields2L(CPosLandmark* aLandmark);
        
        void AddFields3L(CPosLandmark* aLandmark);

        TInt CompareL(const TDesC& aFileName);

        TInt CompareL(const TDesC& aSourceFileName, const TDesC& aTargetFileName);

        /*
        * Create landmark with name, coordinate, coverage radius, icon set
        */
        CPosLandmark* CreateXMLLandmarkLC(const TDesC& aName);


        /**
        * Add all fields to a landmark that is encoded to XML
        */
        void AddAllXMLFieldsL(CPosLandmark* aLandmark);

        /**
        * Add all fields to a landmark that is encoded to XML
        * All fields are empty (_L(""))
        */
        void AddAllEmptyXMLFieldsL(CPosLandmark* aLandmark);

        /**
        * Writes the content of the buffer to the specified file
        * Removes the file if allready exists
        */
        void WriteBufferToFileL(const CBufBase* aBuffer, const TDesC& aFileName);
        
        void CheckDefaultDbExistL();

        CPosLandmarkDatabase* BackupDbFileL();

        CPosLandmarkDatabase* RestoreDbFileL();

        void CopyDbFileL(const TDesC& aFileName);
        
        void CopyTestDbFileL(const TDesC& aFileName);

        CPosLandmarkDatabase* UseGeneratedDbFileL();

        /**
        * Copy the landmarks db used for composite testing
        * to correct path
        */
        CPosLandmarkDatabase* UseCompositeLandmarksDbFileL();

        CPosLandmarkDatabase* UseGlobalCategoriesL();

        CPosLandmarkDatabase* UseEmptyDbFileL();

        void RemoveGlobalCategoriesL();

        void AppendSearchResultsL();

        void AppendAreaSearchResultsL();

        void AppendCategoryLandmarkSearchResultsL();

        void AppendCategorySearchResultsL();

        const RPointerArray<CSearchResult>& SearchResults();

        const RPointerArray<CAreaSearchResult>& AreaSearchResults();
    
        void PrintLandmark(const CPosLandmark& aLandmark, TDes& aPrint);
        
        void LandmarksSortL(RArray<TPosLmItemId>& aArray, const TPosLmSortPref& aSortPref);

        void CategoriesSortL(RArray<TPosLmItemId>& aArray, CPosLmCategoryManager::TCategorySortPref aSortPref);

        void LogToFileL(const TDesC& aFileName, TBuf8<KMsgBufSize> aMsg);
        
        void RunAsyncOperationLD(CPosLmOperation* aOperation);
        
        void RunAsyncOperationL(CPosLmOperation* aOperation);

        void RunAsyncOperationByWaitForReqL(CPosLmOperation* aOperation);

        void RunAsyncOperationAndCancelLD(CPosLmOperation* aOperation);

        void RunAsyncOperationAndCancelInCallbackLD(CPosLmOperation* aOperation, TReal32 aThreshold = 0.3);

        /**
        * Deletes a file from the file system
        * @aFile the file name
        */
		void DeleteFileL(const TDesC& aFile);

        /**
        * Makes sure that the empty file ErrRd is created in c:\system\bootdata 
        * if it is not already created. Should be called by all panic tests 
        * before any panic occurs.
        */
        void MakeSurePanicDebugFileExistsL();

        /**
        * Used as a callback method to cancel a operation after some 
        * steps have been executed.
        * @param self
        */
        TInt static CancelTest(TAny* aSelf);

        void AssertTrueSecL(TBool aCondition, const TDesC& aErrorMsg, TInt aErrorCode=KErrGeneral);

        /**
        * Checks if the server with name aServerName is alive
        */
        TBool ServerAlive(const TDesC& aServerName);
        
        CDesCArray* ListLandmarkDbsL();
   
        void ReadGlobalCategoriesFromResourceL(
        		RArray<TUint>& aCatIds, 
        		RPointerArray<HBufC>& aCatNames,
        		TInt aMaxNum = 0);
        
        void CreateCorrectXmlFileL( const TDesC& aSampleFile, TFileName& aFileName );
        
        HBufC* GetGlobalCategoryFromResourceLC( TPosLmGlobalCategory aGlobalId );
        
        HBufC* ReadDefaultDatabaseNameFromResourceLC();
        
        /**
        * Escapes the string with characters not supported in XML.
        *
        * @param aStrToEscape The string to escape.
        * @return An escaped string.
        */
         HBufC* EscapeStringToXmlLC(
        /* IN */        const TDesC& aStrToEscape
        );

         void EscapeString(
        /* IN/OUT */    TDes& aStrToEscape,
        /* IN */        const TDesC& aFromStr,
        /* IN */        const TDesC& aToStr
        );
        
         TInt CalculateEscapedStringSize(
        /* IN */        const TDesC& aStrToEscape
        );

         TInt CalculateIncrease(
        /* IN */        const TDesC& aStrToEscape,
        /* IN */        const TDesC& aFromStr,
        /* IN */        const TDesC& aToStr
        );


    public:

        //RWsSession              iWs;
        TInt                    iModuleId;

    protected:  // Data

        class COnTheFlyTester : public CActive
            {
            public:
            
                static COnTheFlyTester* NewLC();
                ~COnTheFlyTester();
                void StartSearchCampaignL(
                    CPosLmMultiDbSearch* aSearcher,
                    const RIdArray& aExpectedMatches,
                    CPosLmSearchCriteria* aSearchCriteria);
                TInt Err() {return iErr;}
                void GetErrMsg(TPtrC& aErrMsg) {aErrMsg.Set(iBuf);}
                
            protected:
            
                void RunL();
                void DoCancel();
                TInt RunError(TInt aError);
                
            private:
            
                COnTheFlyTester();
                void VerifyOnTheFlyDataL(TBool aIsFinished = EFalse);
                
            private: // Data
            
                CPosLmMultiDbSearch*    iSearcher;
                TInt                    iErr;
                TInt                    iNrOfOldItems;
                CPosLmOperation*        iOperation;
                TReal32                 iProgress;
                CPosLmDisplayData*      iDisplayData;
                TBuf<200>               iBuf;
                const RIdArray*         iExpectedMatches;
                                
            };

        enum TExecutionMode
            {
            ESynchronous = 0,
            EAsynchronous,
            EWaitForRequest
            };

        const TTimeIntervalMicroSeconds32 KFirstDelayTime;
        const TTimeIntervalMicroSeconds32 KIntervalTime;        
 
        CPosLandmarkDatabase*   iDatabase;
        CPosLandmarkSearch*     iLandmarkSearch;
        CPosLmMultiDbSearch*    iLmMultiSearch;
        
        // Search
        RPointerArray<CSearchResult>        iSearchResults;
        RPointerArray<CAreaSearchResult>    iAreaSearchResults;
        RPointerArray<CCategoryLandmarkSearchResult>   iCategoryLandmarkSearchResults;
        RPointerArray<CCategorySearchResult>   iCategorySearchResults;

        CPosLmOperation*        iOperation;
        // Use this method when using CActiveLmOperation class from multiple threads
        // this is due to that it is not possible to use the iLog from different threads
        // set to ETrue is used from multiple threads
        
        TBool               iUseLogFromThreadIsDisabled;
        CStifLogger* iLog; 
        RFs          iFileSession;
    private:    // Data
        
        //TUint                   iCallbacks;
    
       
    };

class MProgressObserver
    {
    public: // New functions

        /**
        * Notifies the progress of the execution of a CPosLmOperation object.
        *
        * @param sProgress the progress of the operation
        */
        virtual void NotifyProgress(TReal aProgress) = 0;

    };

class CActiveLmOperation : public CActive
    {
    public:
        
        /**
        * C++ constructor.
        */
        CActiveLmOperation(
            CStifLogger* aLog, 
            TBool aUseLogFromThreadIsDisabled = EFalse,
            TReal32 aThreshold = 0.3);
        
        /**
        * Destructor.
        */
        ~CActiveLmOperation();

    public: 
        
        void Start(CPosLmOperation* aOperation, MProgressObserver* aObserver = NULL);    
        
        void CheckOperationL();

        void DeleteOperationD();

        TInt Status() const;

        TReal Progress() const;
        
        CPosLmOperation* Operation();

        void SetCancelTest();
        
        TReal32 Threshold();
        
        TReal32 DeltaProgress();

        /**
        * From CActive
        * Handles request completion event.
        */
        void DoCancel();
    
        /**
        * From CActive.
        *
        * Handles request completion event.
        */
        void RunL();
    
         
    private:

        
    private: // data
        
        CPosLmOperation*    iOperation;
        
        TReal32             iPassedProgress;
        TReal32             iLastProgress;
        TReal32             iDeltaProgress;
        TReal32             iThreshold;

        TBool               iAsyncProgressErr;
        TBool               iPassedProgressErr;

        TBool               iCancelTest;

        TBool               iUseLogFromThreadIsDisabled;

        MProgressObserver*  iObserver;     
        CStifLogger*        iLog;   
    };


#endif      // CLANDMARKTESTPROCEDUREBASE_H

// End of File
