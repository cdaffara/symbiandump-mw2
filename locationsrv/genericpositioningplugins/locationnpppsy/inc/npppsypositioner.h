/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  NPP PSY Positioner header file
*
*/



#ifndef NPPPSYPOSITIONER_H
#define NPPPSYPOSITIONER_H

//  INCLUDES
#include <lbs/epos_cpositioner.h> // CPositioner

// FORWARD DECLARATIONS
class CNppPsyRequester;
class MPositionerStatus;

// CLASS DECLARATION

/**
*  This class implements Positioning Plug-in API for NPP PSY
*  
*  This class provides the interface to Location Framework as specified by
*  Positioning Plug-in API. Each client connection to NPP PSY makes an 
*  instance of this class.
*
*  This class owns a instance of CNppPsyRequester if there is location
*  request received.
*
*  @lib nlanpppsy.dll
*  @since 3.1
*/
class CNppPsyPositioner : public CPositioner
    {
    public:  // Constructors and destructor
        
        /**
        * Two - phased constructor.
        */
        static CNppPsyPositioner* NewL( TAny* aConstructionParameters );
        
        /**
        * Destructor.
        */
        ~CNppPsyPositioner();
        
        /**
        * Get status report interface
        * @return Reference to status report interface
        */
        MPositionerStatus& StatusReportInterface();

    public: // Functions from base classes

        /**
        * From CPositioner See Positioning Plug-in API
        * @since 3.1
        */
        void NotifyPositionUpdate  ( 
            TPositionInfoBase& aPosInfo,  
            TRequestStatus&    aStatus ); 


        /**
        * From CPositioner See Positioning Plug-in API.
        * @since 3.1
        */
        void CancelNotifyPositionUpdate();

    private:

        /**
        * C++default constructor.
        */
        CNppPsyPositioner();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( TAny* aConstructionParameters );

    private:    // Data
        //Position requester
        CNppPsyRequester* iRequester;

    };

#endif      // NPPPSYPOSITIONER_H   
            
// End of File
