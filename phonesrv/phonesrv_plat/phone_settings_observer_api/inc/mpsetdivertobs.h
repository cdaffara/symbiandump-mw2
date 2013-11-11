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
*       MPsetDivertObserver class defines protocol for reporting diverting events.
*
*
*/


#ifndef     MPSETDIVERTOBS_H
#define     MPSETDIVERTOBS_H

//  INCLUDES
#include <etelmm.h>
#include "nwdefs.h"
#include "mpsetcalldiverting.h" 

//  FORWARD DECLARATIONS
class CMobilePhoneCFList;

//  CLASS DEFINITIONS 
/**
*  MPsetDivertObserver class is virtual class for Divert observers.
*  @lib phonesettings.lib
*  @since 1.0
*/
class   MPsetDivertObserver
    {
    public:

        /**
        * Handles showing notes, when diverts change.
        *
        * @param aSetting Setting which was changed.
        * @param aPlural Plural notes to be used.
        */
        virtual void HandleDivertingChangedL( 
            const TCallDivertSetting& aSetting, TBool aPlural ) = 0;
 
        /**
        * Handles showing of status inquiry notes.
        *
        * @param aBsc List of basic services.
        * @param aSetting Divert setting which was queried.
        * @param aPlural Plural notes to be used.
        */

        virtual void HandleDivertingStatusL( CMobilePhoneCFList& aSetting, 
            TBool aPlural ) = 0;

        /**
        * Handles showing of error notes.
        *
        * @param aReason Reason for diverting error.
        */
        virtual void HandleDivertingErrorL( TInt aReason ) = 0;
        
        /**
        * Handles requesting notes.
        *
        * @param aOngoing Is there a request going on.
        * @param aInterrupted Request needs to immediately cancelled.
        */
        virtual void HandleCFRequestingL( TBool aOngoing, 
            TBool aInterrupted ) = 0;

        /**
        * Sets pointer so that message is passed succesfully
        * from requester to observer.
        *
        * @param aDivertEngine Source for the events to observer.
        */
        virtual void SetEngineContact( MPsetCallDiverting* aDivertEngine ) = 0;
    };
#endif // MPSETDIVERTOBS_H
//End of file
