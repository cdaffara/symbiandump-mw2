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
* Description: 
*       MPsetNetworkMode defines interface to get network mode information.
*       MPsetNetworkInfoObserver is used to handle completion of function 
*       calls of this class. All the methods are asynchronous.    
*       Observer handles completion                                           
*
*
*/


#ifndef     MPSETNETWORKMODE_H
#define     MPSETNETWORKMODE_H

//  INCLUDES
#include <e32base.h>

//  CLASS DEFINITIONS 
/**
*  MPsetNetworkMode class is virtual class for system network mode.
*  @lib phonesettings.lib
*  @since 2.6
*/
class   MPsetNetworkMode
    {
    public:

        /**
        * Returns the Network Mode list (GSM/UMTS/DualMode).
        * @since 2.6
        */
        //virtual void GetNetworkModeSelectionList() = 0;        

        /**
        * Returns the currently selected Network Mode (GSM/UMTS/DualMode).
        *
        * @since 2.6
        */
        virtual void GetCurrentNetworkModeSelectionL() = 0;

        /**
        * Sets the Network Mode to TSY side (GSM/UMTS/DualMode).
        *
        * @param aNetworkMode currently selected network mode.
        * @since 2.6
        */
        virtual void SetNetworkModeSelectionL( TUint32& aNetworkMode ) = 0;


    };

#endif // MPSETNETWORKMODE_H

// End of File
