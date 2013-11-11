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
* Description:  CTestPosLmDisplayData.h
*
*/



#ifndef C_TESTPOSLMDISPLAYDATA_H
#define C_TESTPOSLMDISPLAYDATA_H

#include <e32base.h> 


/**
 *  This class will test methods of CPosLmDisplayData class
 *  @p CTestPosLmDisplayData clss contians functions to test methods of
 *  CPosLmDisplayData class
 *
 *  @lib testlmksearchapi.lib
 *  @since S60 v3.2
 */
class CTestPosLmDisplayData : public CBase
    {

public:

    /**
     * Two-phased constructor.
     */
    static CTestPosLmDisplayData* NewLC();
    

    /**
    * Destructor.
    */
    virtual ~CTestPosLmDisplayData();
	
	/**
     * This function is used to check NewL function of CPosLmDisplayData class.
     */
    void TestNewL();
    
    /**
     * This function is used to check NewItemIndex function of CPosLmDisplayData class.
     */
     void TestNewItemIndexL();
     
     /**
     * This function is used to check Count function of CPosLmDisplayData class.
     */
     void TestCountL();
     
     /**
     * This function is used to check Reset function of CPosLmDisplayData class.
     */
     void TestResetL();
     
     /**
     * This function is used to check DisplayItem function of CPosLmDisplayData class.
     */
     void TestDisplayItemL(TInt);
     
     /**
     * This function is used to check SetPartialReadParametersL function of CPosLmDisplayData class.
     */
     void TestSetPartialReadParametersL(TInt);
     
      /**
     * This function is used to check UnsetPartialReadParameters function of CPosLmDisplayData class.
     */
     void TestUnsetPartialReadParametersL();

private:

    /**
    * Constructor.
    */
    CTestPosLmDisplayData();
    
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

#endif //  C_TESTPOSLMDISPLAYDATA_H
