/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides dialing services for tests.
*
*/

#ifndef CDIALER_H_
#define CDIALER_H_

#include <etelmm.h>
#include <mphcltemergencycallobserver.h>

class CPhCltEmergencyCall;
class CPhCltCommandHandler;


NONSHARABLE_CLASS( CDialer ) : public CActive,
                               protected MPhCltEmergencyCallObserver
    {
public:    
    static CDialer* NewL( RTelServer& aTelServer );    
    static CDialer* NewLC( RTelServer& aTelServer );    
    ~CDialer();    
    void Dial( const TDesC& aNumber );    
    void Dial();    
    void DialEmergency();    
    void Hangup();    
    void HangupEmergencyCall();    
    void WaitForRequest();
    
protected:    
    void RunL();    
    void DoCancel();    
    void HandleEmergencyDialL( const TInt aStatus );
    
private:    
    void ConstructL();    
    CDialer( RTelServer& aTelServer );
    
private:
    RTelServer iTelServer;
    RMobileCall iCall;
    CActiveSchedulerWait* iWait;
    CPhCltEmergencyCall* iEmergency;
    CPhCltCommandHandler* iCmdHandler;
    };


#endif /* CDIALER_H_ */
