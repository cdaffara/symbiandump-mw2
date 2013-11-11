/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Monitoring startup and reading/writing to shared data.
*
*/



#ifndef CSOSENPOLICYENLISTHANDLER_H
#define CSOSENPOLICYENLISTHANDLER_H


// INCLUDES
#include    "csosemergencynumberpolicyhandler.h" 
#include    <rphcltserver.h> 
#include    <cenrepnotifyhandler.h>

// FORWARD DECLARATIONS


// CLASS DECLARATION

/**
*  It is the monitor for startup status, and provides services for
*  reading and writing to/from shared data.
*
*  @since Series60_2.6
*/
NONSHARABLE_CLASS( CSosEnPolicyENListHandler ) : public CBase,
    public MCenRepNotifyHandlerCallback
    {
    public:  // Constructors and destructor

        /**
        * Creates a new monitor and starts it.
        *
        * @return Pointer to created CSosEnPolicyENListHandler instance.
        */

        static CSosEnPolicyENListHandler* NewL(); 
            
        /**
        * Destructor.
        */
        ~CSosEnPolicyENListHandler();

    public: // New functions

        /** 
        * Reads product specific numbers.
        *
        * @return Returns space separated list of emergency numbers, in case
        *         of error, empty list is returned.
        */
        const TDesC& ReadProductSpecificNumbers();

        /** 
        * Adds Emergency number to list to be written to Central Repository
        *
        * @param aNumber This is emergency number to be added.
        * @param aPermanent If this is set to ETrue, number will not be
        *                   cleared with ClearCurrentList function.
        *                   For numbers which are read in the startup.
        *   Permanent numbers should be written first. Also adding new permanent
        *   number removes all non permanent numbers.
        */
        void AddToList( const TDesC& aNumber, TBool aPermanent = EFalse );

        /** 
        * Clears Current list.
        * Does not touch to permanent numbers.
        */
        void ClearCurrentList();

        /** 
        * Writes current list to Central Repository.
        */
        void WriteListToCentralRepository();

        /** 
        * Is number in current list with normal mode.
        *
        * @param aNumber This is number to be checked. 
        * @return Returns ETrue if in list, otherwice EFalse.
        */
        TBool IsInListByNormalMode ( const TDesC& aNumber );

        /** 
        * Is number in current list with advanced mode.
        *
        * @param aNumber This is number to be checked. 
        * @return Returns ETrue if in list, otherwice EFalse.
        */
        TBool IsInListByAdvancedMode ( TDes& aNumber );

    private: // From MCenRepNotifyHandlerCallback

        void HandleNotifyString( TUint32 aId, const TDesC16& aNewValue );

    private:

        /** 
        * C++ constructor.
        *
        */

        CSosEnPolicyENListHandler();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:  // Data

        // Central repository instance.
        CRepository*    iRepository;

        // The product specific emergency numbers.
        TBuf< KEnPolicySDEmergencyNumberListLength >   iProductSpecificList;

        // The current emergency number list.
        TBuf< KEnPolicySDEmergencyNumberListLength >   iCurrentList;

        // The current place in the emergency number list.
        TInt    iCurrentListPlace;

        // The start point of current emergency number list.
        TInt    iCurrentListStart;

        /**
         * CenRep listener
         * 
         * Own
         */
        CCenRepNotifyHandler* iListener;

    };

#endif  // CSOSENPOLICYENLISTHANDLER_H


// End of File
