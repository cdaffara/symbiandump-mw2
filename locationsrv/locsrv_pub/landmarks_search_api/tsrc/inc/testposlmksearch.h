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
* Description:  Declaration of CTestPosLmkSearch  class
*
*/



#ifndef C_TESTPOSLMKSEARCH_H
#define C_TESTPOSLMKSEARCH_H


#include <e32base.h>
#include <EPos_CPosLandmarkSearch.h>
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkDatabase.h>
/**
 *  This class will test methods of CPosLandmarkSearch class
 *  @p CTestPosLmkSearch clss contians functions to test methods of
 *  CPosLandmarkSearch class
 *
 *  @lib testlmksearchapi.lib
 *  @since S60 v3.2
 */
class CTestPosLmkSearch :public CBase
    {

public:

    /**
     * Two-phased constructor.
     */
    
     static CTestPosLmkSearch* NewLC();
    

    /**
    * Destructor.
    */
    
    virtual ~CTestPosLmkSearch();

    /**
     * This function is used to check NewL function of CPosLandmarkSearch class
     *
     */
    void TestNewL();
    
    /**
     * This function is used to check MaxNumOfMatches function of CPosLandmarkSearch class.
     *
     */
    void TestMaxNumOfMatchesL();
    
    /**
     * This function is used to check SetMaxNumOfMatches function of CPosLandmarkSearch class
     * with default value
     * @param aNumOfMatches Number of matches
     */
    void TestSetMaxNumOfMatchesL(TInt aNumOfMatches=-1);
	
	/**
     * This function is used to check StartLandmarkSearchL function of CPosLandmarkSearch class
     * with default value
     * @param aBool If this is true,landmark is searched in the last search result else
     * from the database 
     */
     
	void TestStartLandmarkSearchL(TBool aBool=EFalse);
	
	/**
     * This function is used to check StartLandmarkSearchL function of CPosLandmarkSearch class
     * with default value and with sorting option.
     */

	void TestStartLandmarkSearchL1L();
	
	/**
     * This function is used to check StartLandmarkSearchL function of CPosLandmarkSearch class
     * with default value.With sorting option and category of sort is specified.
     */

	void TestStartLandmarkSearchL2L();
	
	/**
     * This function is used to check NumOfMatches function of CPosLandmarkSearch class.
     */
	
	void TestNumOfMatchesL();
	
	/**
     * This function is used to check MatchIteratorL function of CPosLandmarkSearch class.
     */
	void TestMatchIteratorL();

 	/**
     * This function is used to check SetDisplayData function of CPosLandmarkSearch class.
     */
	void TestSetDisplayDataL(); 
	
	/**
     * This function is used to check UnsetDisplayData function of CPosLandmarkSearch class.
     */
	void TestUnsetDisplayDataL(); 
	
private:

	/**
    * Constructor.
    */
    CTestPosLmkSearch();
    
    /**
    * Landmarks used for testing are created here.
    */

    void CreateLandmarkL();
	
	/**
    * All landmarks are deleted when this function is called.
    */

	void DeleteAllLandMarksL();
	
	    /**
    * Second phase construction.
    */
    void ConstructL();
   
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




#endif // C_TESTPOSLMKSEARCH_H
