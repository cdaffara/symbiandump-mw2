/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Test class for SAT Icon API 
*
*/


#ifndef CSATSERVICEAPITESTMONITOR_H
#define CSATSERVICEAPITESTMONITOR_H

class CSATServiceAPI;
class CStifLogger;

class CSatServiceAPIMonitor : public CActive
    {
public: // Construtor and destructor
    
    /**
     * Constructor to use in the object creation. Initializes the necessary data.
     */
    CSatServiceAPIMonitor( CStifLogger& aLog, CSATServiceAPI& aTest ); 
          
    /**
     * Destructor
     */    
    virtual ~CSatServiceAPIMonitor();
    
public:  // New functions

    /**
     * Activate the active object
     *
     * @since S60 5.1
     */ 
    void Activate();
    
    /**
     * Return the referance of iStatus
     *
     * @since S60 5.1
     * @return TRequestStatus&, referance of iStatus
     */    
    TRequestStatus& Status();
    
protected: // Functions from CActive

    /**
     * From CActive
     * Handles the request completion.
     */
    void RunL();

    /**
     * From CActive
     * Implements the cancel protocol.
     */
    void DoCancel();
   
    /**
     * From CActive 
     * Handles a leave occurring in the request completion event 
     * handler RunL()
     * 
     * @param aError Leave from RunL().
     * @return Error code
     */    
    TInt RunError( TInt aError );
    
private: // Data

    /* 
     * Ref to logger.
     */
    CStifLogger& iLog; 
    
    /* 
     * Ref to CSATServiceAPI.
     */    
    CSATServiceAPI& iTest;    
    
    };


#endif      // CSATSERVICEAPITESTMONITOR_H
