/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Ussd session cancel waiter.
*
*/



#ifndef CPHSRVUSSDSESSIONCANCELWAITER_H
#define CPHSRVUSSDSESSIONCANCELWAITER_H

// CLASS DECLARATION
/**
*  For cancelling the USSD session.
*
*  @since Series60_2.6
*/
class CPhSrvUssdSessionCancelWaiter : public CActive
    {
    public: // Constructors and destructor

        /**
        * C++ constructor.
        */
        CPhSrvUssdSessionCancelWaiter() 
            : CActive( EPriorityLow ),
              iSSRequestPckg( iSSRequest )
            { 
            CActiveScheduler::Add( this );
            }
        
        /**
        * Destructor.
        */
        ~CPhSrvUssdSessionCancelWaiter() { Cancel();}

    public: // Functions from base classes
        void SetActive() { CActive::SetActive(); }
        void RunL() {}
        void DoCancel() {}
    
    public: //Data

        // SendSSRequest information.
        RMobilePhone::TMobilePhoneSendSSRequestV3     iSSRequest;

        // Packaged version of iSSRequest.
        RMobilePhone::TMobilePhoneSendSSRequestV3Pckg iSSRequestPckg;

    };

#endif // CPHSRVUSSDSESSIONCANCELWAITER_H

// End of file.
