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
* Description:     CTestPosLmIdListCriteria class
*
*/






#ifndef C_TESTPOSLMIDLISTCRITERIA_H
#define C_TESTPOSLMIDLISTCRITERIA_H

#include <e32base.h>

/**
 *  This class will test methods of CPosLmIdListCriteria class
 *  @p CTestPosLmIdListCriteria clss contians functions to test methods of
 *  CPosLmIdListCriteria class
 *
 *  @lib testlmksearchapi.lib
 *  @since S60 v3.2
 */
class CTestPosLmIdListCriteria : public CBase
    {

public:


    /**
     * Two-phased constructor.
     */
    static CTestPosLmIdListCriteria* NewLC();
    

    /**
    * Destructor.
    */
    virtual ~CTestPosLmIdListCriteria();
    
    /**
     * This function is used to check NewLC function of CPosLmIdListCriteria class.
     */
     void TestNewLC();
     
     /**
     * This function is used to check SetLandmarkIdsL function of CPosLmIdListCriteria class.
     */
	 void TestSetLandmarkIdsL();
	 
	 /**
     * This function is used to check GetLandmarkIdsL function of CPosLmIdListCriteria class.
     */
     void TestGetLandmarkIdsL();

    
private:
	
	/**
    * Constructor.
    */
    CTestPosLmIdListCriteria();


    };




#endif // C_TESTPOSLMIDLISTCRITERIA_H
