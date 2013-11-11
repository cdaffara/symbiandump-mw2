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
* Description:  Definition of CCbsLivecasetHandler class.
*
*/


#ifndef     CCBSLIVECASTHANDLER_H
#define     CCBSLIVECASTHANDLER_H

//  INCLUDES

#include <e32base.h>
#include "CCbsMessage.h"

//  FORWARD DECLARATIONS
class CCbsRecEtel;

//  CLASS DECLARATION 

/**
*   CCbsLivecastHandler constructs CB messages based on a page
*   received through ETel.
*
*   The selection between GSM and WCDMA messages is made when
*   constructing the message.
*/
class CCbsLivecastHandler : public CBase
    {
    public:     // New functions        

        /**
        *   Constructs a new CCbsLivecastHandler 
        *   
        *   @param      aRecEtel        CCbsRecEtel reference
        *   @return                     A new instance of CCbsLivecastHandler
        */
        static CCbsLivecastHandler* NewL( CCbsRecEtel& aRecEtel );

        /**
        *   Destructor
        */
        ~CCbsLivecastHandler();

        /**
        *   Processes Livecast messages
        */
        void HandleLivecastMessageL( CCbsMessage* aLivecastMessage );
        
    private:

        /**
        *   Default constructor               
        */  
        CCbsLivecastHandler( CCbsRecEtel& aRecEtel );

        /**
        *   2nd-phase constructor               
        */
        void ConstructL();

    private: // Data

        // CCbsRecEtel instance to which CB messages are passed.
        CCbsRecEtel& iRecEtel;

    };

#endif      //  CCBSLIVECASTHANDLER_H   
            
// End of File


