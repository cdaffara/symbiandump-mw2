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
* Description:  NPP PSY position requester header file
*
*/



#ifndef NPPPSYREQUESTER_H
#define NPPPSYREQUESTER_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CNppPsyPositioner;
class CPositioner;
class CNppPsySingletonHandler;

// CLASS DECLARATION

/**
*  This class implements the position requester.
*  
*  Position requester is responsible for loading appropriate PSY vi ECom Plug-in 
*  and forward location request to the loaded PSY.
*
*  This class may owns an instance of CPositioner. 
*
*  @lib nlanpppsy.dll
*  @since 3.1
*/
class CNppPsyRequester : public CActive
    {
    public:  // Constructors and destructor
        
        /**
        * Two - phased constructor.
        * @param aPositioner Reference to CNppPsyPositioner
        */
        static CNppPsyRequester* NewL( CNppPsyPositioner& aPositioner );
        
        /**
        * Destructor.
        */
        ~CNppPsyRequester();
        
    public:
        
        /**
        * Notify position update
        * @param aPosInfo Reference to TPositionerInfoBase, used to store
        *                 fix information.
        * @param aSTatus  Reference to TRequestStatus, used to complete location
        *                 request.
        * @since 3.1
        */
        void NotifyPositionUpdate  ( 
            TPositionInfoBase& aPosInfo,  
            TRequestStatus&    aStatus ); 
    
    private: // Functions from base classes

        /**
        * From CActive
        */
        void RunL(); 

        /**
        * From CActive
        */
        void DoCancel(); 

        /**
        * From CActive
        */
        TInt RunError( TInt aErr ); 

    private:

        /**
        * C++default constructor.
        */
        CNppPsyRequester( CNppPsyPositioner& aPositioner );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Make location request to the first PSY 
        */
        void MakeLocationRequest();
        
        /**
        * Complete location request
        */
        void CompleteRequest( TInt aErr );

    private:    // Data
        // Reference to NPP PSY Positioner
        CNppPsyPositioner& iNppPsyPositioner;

        // Singleton handler
        CNppPsySingletonHandler* iSingletonHandler;

        // Pointer to position info
        TPositionInfoBase* iPositionInfo;

        // Request status
        TRequestStatus* iRequestStatus;

        //PSY list
        RArray < TUid > iPsyList;
        
        // Current loaded positioner
        CPositioner* iPositioner;

    };

#endif      // NPPPSYREQUESTER_H   
            
// End of File
