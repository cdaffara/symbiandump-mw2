/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Test module to implement Firing of startup trigger and Listing of trigger related test cases
*
*/




#ifndef T_TRIGITERATOR_H
#define T_TRIGITERATOR_H

 //  INCLUDES

  #include <lbtcommon.h>
  #include <lbttriggerfiringeventobserver.h>


  // FORWARD DECLARATIONS
  
  class RLbt; 
  class CLbtTriggerFiringEventNotifier;
  class CLbtTriggerEntry;
     


class CTrigIterator : public CIdle
    {
    public:
        static CTrigIterator* NewL( RLbt& aLbt, CActiveSchedulerWait* aWait );
        CTrigIterator( RLbt& aLbt, CActiveSchedulerWait* aWait );
    private:
        void ConstructL();
        static TInt CallbackL( TAny* aAny );
        TInt StepL();
    private:
        RLbt& iLbt;
        CActiveSchedulerWait* iWait;
    };
#endif