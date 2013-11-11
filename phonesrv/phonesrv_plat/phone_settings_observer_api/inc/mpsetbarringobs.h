/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*      MPsetBarringObserver class defines protocol for reporting
*      barring events to barring observer.     
*
*
*/


#ifndef     MPSETBARRINGOBS_H
#define     MPSETBARRINGOBS_H

//  INCLUDES
#include "nwdefs.h"
#include "mpsetcallbarring.h" 

//  CLASS DEFINITIONS 
/**
*  MPsetBarringObserver class is virtual class for call barring observers.
*  @lib phonesettings.lib
*  @since 1.0
*/
class MPsetBarringObserver
    {
    public:      

        /**
        * Handles mode changed notes.
        *
        * @param aType Type of barring program.
        * @param aStatus Status of the barring program.
        * @param aPlural Plural notes should be used.
        */
        virtual void HandleBarringModeChangedL( TBarringProgram aType, 
              TBarringStatus aStatus, TBool aPlural ) = 0;        
        
         /**
        * Handles barring mode inquiry notes.
        *
        * @param aType Type of barring program.
        * @param aStatus Status of the barring program.
        */
        virtual void HandleBarringModeStatusL( TUint8 aBsc[KPSetNumberOfBsc], 
                    TBarringStatus aStatus ) = 0;
        
        /**
        * Handles errors in call barring requests.
        *
        * @param aReason Reason for error.
        */
        virtual void HandleBarringErrorL( TInt aReason ) = 0;

        /**
        * Handles requesting notes.
        *
        * @param aTrue Is there a request going on.
        * @param aInterrupted Request needs to be immediately cancelled.
        */
        virtual void HandleCBRequestingL( TBool aTrue, 
            TBool aInterrupted ) = 0; 
        
        /**
        * Sets pointer so that message is passed succesfully
        * from requester to observer.
        *
        * @param aBarringEngine Source of events to observer.
        */
        virtual void SetEngineContact( MPsetCallBarring* aBarringEngine ) = 0;

        /**
        * Handles notes relating to cb password change requests.
        *
        * @param aSuccess Result of teh change request.
        */
        virtual void CbPasswordChangedL( TBool aSuccess ) = 0;

    };
#endif      //  MPSETBARRINGOBS_H
// End of File
