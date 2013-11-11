/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: This file contains activeobject header for handling callbacks
*
*/



#include <e32base.h>
#include <e32svr.h>
#include <StifParser.h>
#include <StifLogger.h>
#include <StifTestInterface.h>
#include <systemtoneservice.h>

 


class CStsActiveObj : public CActive
{
    
    public:
    static CStsActiveObj* NewL( MStsPlayAlarmObserver* aObserver,CStifLogger* aLogger );
    ~CStsActiveObj();
       
    public:     // Functions from base classes

    /**
    * RunL derived from CActive handles the completed requests.
    */
    void RunL();

    /**
    * DoCancel derived from CActive handles the cancel
    */
    void DoCancel();

    /**
    * RunError derived from CActive handles errors from active handler.
    */
    TInt RunError( TInt aError );
    
    private:

        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL(MStsPlayAlarmObserver* aObserver, CStifLogger* aLogger);

        /**
        * C++ constructor.
        */
        CStsActiveObj();
    

    private:    // Data
    MStsPlayAlarmObserver*   iObserver;
    CStifLogger* 			 iLog;
   

};
