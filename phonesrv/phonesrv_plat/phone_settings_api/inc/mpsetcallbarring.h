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
*      MPsetCallBarring defines interface to get and set call barring
*      setting in the network.                                        
*
*
*/


#ifndef MPSETCALLBARRING_H
#define MPSETCALLBARRING_H

//  INCLUDES
#include <e32base.h>
#include <nwdefs.h>

//  FORWARD DECLARATIONS
class MPsetBarringObserver;

//  CLASS DEFINITIONS 
/**
*  MPsetCallBarring class is virtual class for call barring.
*  @lib phonesettings.lib
*  @since 1.0
*/
class MPsetCallBarring
    { 
    public:        

        /**
        * Sets barring status to the network.
        *
        * @param aBarring New status of barring.
        * @param aBsc List of basic service groups.
        */
        virtual void SetBarringL( const TCallBarringSetting& aBarring, 
            TBasicServiceGroups aBsc ) = 0;

        /**
        * Checks the barring status from network
        *
        * @param aGroup Service group concerned.
        * @param aMode Barring program mode.
        */
        virtual void GetBarringStatusL( const TServiceGroup aGroup, 
            const TBarringProgram aMode ) = 0;

        /**
        * Cancels the pending request.
        *
        * @return Returns error code
        */
        virtual TInt CancelCurrentRequest()=0;
    };
#endif //MPSETCALLBARRING_H
//End of file
