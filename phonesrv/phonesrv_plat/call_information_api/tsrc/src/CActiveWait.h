/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/
#ifndef CACTIVEWAIT_H
#define CACTIVEWAIT_H

#include <e32base.h>

/**
 * To wait TRequestStatus completion.
 *
 * Usage:
 *
 * wait = CActiveWait::NewL();
 * someservice->MakeRequest( wait->Status() );
 * wait->WaitForRequest();
 */
NONSHARABLE_CLASS( CActiveWait ) : public CActive
    {
public:
    static CActiveWait* NewL(); 
    static CActiveWait* NewLC(); 
        
    ~CActiveWait();
        
public:

    TRequestStatus& Status();

    void WaitForRequest();
        
protected:
    
    CActiveWait();
        
    void ConstructL();
        
    void DoCancel();
        
    void RunL();

protected:

    CActiveSchedulerWait* iWait;
    };
    
    
#endif // CACTIVEWAIT_H