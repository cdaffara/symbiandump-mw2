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
* Description:  CTestPosLmMultiDbSearch class
*
*/



#ifndef C_TESTPOSLMMULTIDBSEARCH_H
#define C_TESTPOSLMMULTIDBSEARCH_H


#include <e32base.h>


/**
 *  This class will test methods of CPosLmMultiDbSearch class
 *  @p CTestPosLmMultiDbSearch clss contians functions to test methods of
 *  CPosLmMultiDbSearch class
 *
 *  @lib testlmksearchapi.lib
 *  @since S60 v3.2
 */
 
class CTestPosLmMultiDbSearch : public CBase
    {

public:
    /**
     * Two-phased constructor.
     */
     static CTestPosLmMultiDbSearch* NewLC();
    

    /**
    * Destructor.
    */
    virtual ~CTestPosLmMultiDbSearch();

 	/**
     * This function is used to check NewL function of CPosLmMultiDbSearch class.
     */
     void TestNewL(TInt);
     
    /**
     * This function is used to check SetDatabasesToSearchL function of CPosLmMultiDbSearch class.
     */ 
     void TestSetDatabasesToSearchL();
     
     /**
     * This function is used to check DatabasesToSearchL function of CPosLmMultiDbSearch class.
     */
     void TestDatabasesToSearchL();
     
     /**
     * This function is used to check SetMaxNumOfMatches function of CPosLmMultiDbSearch class.
     */
     void TestSetMaxNumOfMatchesL(TInt);
     
     /**
     * This function is used to check StartLandmarkSearchL function of CPosLmMultiDbSearch class.
     */
     void TestStartLandmarkSearchL1L();
     
     /**
     * This function is used to check overloaded StartLandmarkSearchL function of CPosLmMultiDbSearch class.
     */
     void TestStartLandmarkSearchL2L();
		
	/**
     * This function is used to check overloaded StartLandmarkSearchL function of CPosLmMultiDbSearch class.
     */
     void TestStartLandmarkSearchL3L();
     
     /**
     * This function is used to check StartCategorySearchL function of CPosLmMultiDbSearch class.
     */
     void TestStartCategorySearchL();
     
     /**
     * This function is used to check  NumOfSearchErrors function of CPosLmMultiDbSearch class.
     */
     void TestNumOfSearchErrorsL();
     
     /**
     * This function is used to check  GetSearchError function of CPosLmMultiDbSearch class.
     */
     void TestGetSearchErrorL();
     
     /**
     * This function is used to check TotalNumOfMatches function of CPosLmMultiDbSearch class.
     */
     void TestTotalNumOfMatchesL();
     
     /**
     * This function is used to check NumOfMatches function of CPosLmMultiDbSearch class.
     */
     void TestNumOfMatchesL(TInt);
     
     /**
     * This function is used to check NumOfDatabasesToSearch function of CPosLmMultiDbSearch class.
     */
     void TestNumOfDatabasesToSearchL();
     
     /**
     * This function is used to check DatabaseUriPtr function of CPosLmMultiDbSearch class.
     */
     void TestDatabaseUriPtrL(TInt);
	
	 /**
     * This function is used to check TestMatchIteratorL function of CPosLmMultiDbSearch class.
     */
	 void TestMatchIteratorL();
	 /**
     * This function is used to check SetDisplayData function of CPosLmMultiDbSearch class.
     */
     void TestSetDisplayDataL();
    
	 /**
     * This function is used to check UnsetDisplayData function of CPosLmMultiDbSearch class.
     */
     void TestUnsetDisplayDataL();
     
     /**
     * This function is used to check MaxNumOfMatches function of CPosLmMultiDbSearch class.
     */
	 void TestMaxNumOfMatchesL();
    	    
private:
	/**
    * Constructor.
    */
    CTestPosLmMultiDbSearch();
    
    /**
    * Second phase construction.
    */
    void ConstructL();
    
    /**
    * All the landmarks in the database are deleted here.
    */
    void DeleteLandmarksL();
    
    /**
    * Landmarks used for testing are created here.
    */

    void CreateLandmarkL();

private: // data

    /**
     * Pointer to landmark object
     * Own
     */
     CPosLandmark* iLandmark;

    /**
     * Pointer to landmark database object
     * Own.  
     */
     CPosLandmarkDatabase* iLandmarkDatabase;

    /**
     * Landmark id object
     */
     TPosLmItemId iLandmarkId;
    


    };



#endif // C_TESTPOSLMMULTIDBSEARCH_H
