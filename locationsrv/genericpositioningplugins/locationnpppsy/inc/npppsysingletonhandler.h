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
* Description:  NPP PSY singleton objects handler
*
*/



#ifndef NPPPSYSINGLETONHANDLER_H
#define NPPPSYSINGLETONHANDLER_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION
class CNppPsyStatusHandler;
class CNppPsySettingHandler;
class MPositionerStatus;

/**
*  This class implements the handler for singleton object
*  
*  The singleton object contains two PSY status handler and setting handler.
*
*  This class owns an instance of CNppPsyStatusHandler and CNppPsySettingHandler.
*
*  @lib nlanpppsy.dll
*  @since 3.1
*/
class CNppPsySingletonHandler : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Get instance to singleton handler. Release reference function
        * must be called to release the reference
        * @param aStatusInterface Referece to MPositionerStatus to report module 
        *                         status.
        * @reuturn the pointer the the singleton handler
        */
        static CNppPsySingletonHandler* GetInstanceL( 
            MPositionerStatus& aStatusInterface );

        /**
        * Release the reference to singleton handler. If there is no ojbect
        * has the reference of this object, it will delete itself.
        */
        void ReleaseInstance();

        /**
        * Destructor
        */
        ~CNppPsySingletonHandler();

        /**
        * Get reference to status handler
        * @return Reference to status handler
        */
        CNppPsyStatusHandler& StatusHandler();

        /**
        * Get reference to setting handler
        * @return Reference to setting handler
        */
        CNppPsySettingHandler& SettingHandler();
        

    private:
        /**
        * Second - phase constructor
        */
        void ConstructL( MPositionerStatus& aStatusInterface );

    private:
        //Reference count
        TInt iRefCount;

        //Status handler
        CNppPsyStatusHandler* iStatusHandler;

        //Setting handler
        CNppPsySettingHandler* iSettingHandler;
    };

#endif      // NPPPSYSINGLETONHANDLER_H
            
// End of File
