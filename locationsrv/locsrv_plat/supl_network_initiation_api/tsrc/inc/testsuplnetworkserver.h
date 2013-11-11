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
* Description:  CTestSuplNetworkServer class
*
*/



#ifndef C_TESTSUPLNETWORKSERVER_H
#define C_TESTSUPLNETWORKSERVER_H

#include <e32base.h>

 
/**
 *  This class is used to test methods of RSuplNetworkServer class
 *  @p CTestSuplNetworkServer contians functions to test methods of
 *  RSuplNetworkServer class
 *
 *  @lib testsuplinitapi.lib
 *  @since S60 v3.2
 */
 class CTestSuplNetworkServer : public CBase
    {

public:

	/**
     * Two-phased constructor.
     */
     static CTestSuplNetworkServer* NewLC();
    

    /**
    * Destructor.
    */
    virtual ~CTestSuplNetworkServer();
    
    /**
    * This function will test Connect method of RSuplNetworkServer class.
    */
    void TestConnectL();

	/**
    * This function will test Connect method of RSuplNetworkServer class
    * for negative case.
    */
    void TestConnectL(TInt);
    
   	/**
    * This function will test ForwardMessage method of RSuplNetworkServer class.
    */
    void TestForwardMessageL();
    
    /**
    * This function will test Version method of RSuplNetworkServer class.
    */
    void TestVersionL();


private: 

	/**
    * Constructor.
    */
    CTestSuplNetworkServer();
    };

#endif // C_TESTSUPLNETWORKSERVER_H
