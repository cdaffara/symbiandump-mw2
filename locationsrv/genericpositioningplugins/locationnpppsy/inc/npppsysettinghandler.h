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
* Description:  NPP PSY status event report handler header file
*
*/



#ifndef NPPPSYSETTINGHANDLER_H
#define NPPPSYSETTINGHANDLER_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION
class CRepository;
class CNppPsyStatusHandler;

/**
*  This class implements the handler that responsible for read and write
*  PSY list from Central Repository.
*  
*  This class owns an instance of CRepository.
*
*  @lib nlanpppsy.dll
*  @since 3.1
*/
class CNppPsySettingHandler : public CActive
    {
    public:  // Constructors and destructor
        
        /**
        * Two - phased constructor.
        * @param aStatusHandler Referece to CNppPsyStatusHandler
        * @return Constructed object of CNppPsySettingHandler
        */
        static CNppPsySettingHandler* NewL( 
            CNppPsyStatusHandler& aStatusHandler );
        
        /**
        * Destructor.
        */
        ~CNppPsySettingHandler();

    public: // Functions from base classes
        /**
        * Get PSY list from setting
        * @param aList On return contains the PSY list
        */
        void GetPsyListL( RArray < TUid > &aList );

        /**
        * Move PSY to last
        * @param aPsy The Uid of the PSY that should be moved to the end
        *        of PSY priority list
        */
        void MovePsyLast( TUid aPsy );
        
        /**
        * Remove PSY from the list
        * @param aPsy The Uid of the PSY to be removed from setting
        */
        void RemovePsy( TUid aPsy );
        
    private: // From base classes
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
        TInt RunError( TInt aError );

    private:

        /**
        * C++default constructor.
        */
        CNppPsySettingHandler( CNppPsyStatusHandler& aStatusHandler );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Parse PSY list setting from CenRep
        */
        void ParseSettingL(
            TInt& aListFrozen, 
            RArray < TUid >& aPsyList );
        
        /**
        * Check if the key value in ROM contains different PSY or 
        * frozen flag. If it's different, then reset the key and
        * use the key value in ROM. 
        * 
        * This operation is needed when phone is restore to a old 
        * version of backup.
        */
        void ValidateRomKeyValueL();

        /**
        * Write setting. If the setting can't be writen, this function
        * will just return.
        */
        void WriteSetting(); 
        
        /**
        * This function will remove PSY UID which are there already in cenrep key which 
        * is configured during runtime.
        */
        void RemoveDuplicatePsyUid(RArray < TUid >& aToPsyList ,RArray < TUid >& aFromPsyList );
        

    private:    // Data
        //Status handler
        CNppPsyStatusHandler& iStatusHandler;
        
        //Central repository
        CRepository* iRepository;

        //Indicate if PSY list is frozen and frozen mode
        TInt iListFrozen;

        //Psy list
        RArray < TUid > iPsyList;

    };

#endif      // NPPPSYSETTINGHANDLER_H
            
// End of File
