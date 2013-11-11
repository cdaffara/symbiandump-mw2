/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the MNWNetworkTimer class
*
*/



#ifndef MNWNETWORKTIMER_H
#define MNWNETWORKTIMER_H



// CLASS DECLARATION

/**
*  This file contains the header file of the MNWNetworkTimer class
*  
*
*  @lib networkhandling.lib
*  @since Series 60_2.8
*/
class MNWNetworkTimer 
    {
    public: // New functions
        
        /**
        * This function is called after timeout.
        * @param none
        * @return none
        */
        virtual void HandleTimeOut() = 0;

    };

#endif      // MNWNETWORKTIMER_H
            
// End of File
