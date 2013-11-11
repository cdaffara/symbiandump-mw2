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
*       This class is virtual abstract class for setting/cancelling and
*       inquiring divert(s).
*
*
*/


#ifndef     MPSETCALLDIVERTING_H
#define     MPSETCALLDIVERTING_H

//  INCLUDES
#include <e32base.h>
#include <nwdefs.h>

//  CLASS DEFINITIONS 
/**
*  MPsetCallDiverting class is virtual class for Diverting.
*  @lib phonesettings.lib
*  @since 1.0
*/
class   MPsetCallDiverting
    {
    public:

        /**
        * Sets diverting to the network.
        *
        * @param aDivert New divert settings.
        * @param aBsc List of basic service groups .
        * @param aVmbx Divert to voice mailbox number.
        */
        virtual void SetDivertingL( const TCallDivertSetting& aDivert, 
            TBasicServiceGroups aBsc, TBool aVmbx ) = 0;        
          
        /**
        * Checks the diverting status from network.
        *
        * @param aServiceGroup Service group indication.
        * @param aCondition Diverting conditions.
        * @param aBsc List of basic service groups.
        */
        virtual void GetDivertingStatusL( const TServiceGroup aServiceGroup, 
            const TCallDivertingCondition aCondition, 
            TBasicServiceGroups aBsc ) = 0;
          
        /**
        * Cancels current request.
        *
        * @return Returns error code.
        */
        virtual TInt CancelCurrentRequest() = 0;

    };

#endif      //  MPSETCALLDIVERTING_H
// End of File
