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
* Description:  This class handles actions for CLI events. 
*
*
*/


#ifndef     MPSETCLIOBSERVER_H
#define     MPSETCLIOBSERVER_H

//  INCLUDES
#include <e32base.h>
#include <psuiconstants.h> 
#include "psetconstants.h" 
#include "mpsetcli.h" 

//  CLASS DEFINITIONS 
/**
*  MPsetCliObserver class is virtual class for CLI observers.
*  @lib phonesettings.lib
*  @since 1.0
*/
class   MPsetCliObserver
    {
    public:

        /**
        * Sets pointer so that message is passed succesfully
        * from requester to observer.
        *
        * @param aEngine Source of events to observer.
        */
        virtual void SetEngineContact( MPsetCli* aEngine ) = 0;

        /**
        * Handles requesting notes.
        *
        * @param aOngoing Is request already going on.
        * @param aInterrupted Request needs to be immediately cancelled.
        */
        virtual void HandleCliRequestingL( TBool aOngoing, 
            TBool aInterrupted ) = 0; 

        /**
        * Handles notes for changing any of CLI features.
        *
        * @param aType Type of the CLI affected.
        */
        virtual void CliInformationL( TPsuiCli aType ) = 0;

        /**
        * Checks the CLI status from network.
        *
        * @param aBsc List of basic service groups.
        * @param aMode Which CLI feature was inquiried for.
        */
        virtual void HandleCliStatusL( TUint8 aBsc[KPSetNumberOfBsc], TPsuiCli aMode ) = 0;

        /**
        * Checks the CNAP status from network.
        * @since 1.2
        *
        * @param aStatus CNAP status.
        */        
        virtual void HandleCnapStatusL( TInt aStatus ) =  0;

        /**
        * Handles errors.
        * @since 2.0
        *
        * @param aError error code.
        */
        virtual void HandleCliErrorL( TInt aError ) = 0;
    };
#endif // MPSETCLIOBSERVER_H
// end of file
