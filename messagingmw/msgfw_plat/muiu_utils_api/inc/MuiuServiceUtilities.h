/*
* Copyright (c) 2002 - 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: MuiuServiceUtilities  declaration
*
*/




#ifndef __MUIUSERVICEUTILITIES_H__
#define __MUIUSERVICEUTILITIES_H__

//  INCLUDES
#include <msvipc.h>

// FORWARD DECLARATIONS
class CMsvSession;
class RFs;


/**
* Static class to help service handling.
*/
class MsvUiServiceUtilities
    {
    public:

      /**
        * Collects all the services.
        * @param aSession: reference to CMsvSession object
        * @param aAlwaysListHidden: boolean to show hidden services or not.
        * @return array of all the services found.
        * NOTE: Calling module is responsible to delete array.
        */
        IMPORT_C static CMsvEntrySelection* GetListOfAccountsL(
            CMsvSession& aSession,
            TBool aAlwaysListHidden=EFalse );

        /**
        * Collects all the services of certain mtm type.
        * @param aSession: reference to CMsvSession object
        * @param aMtm: uid of the mtm type to be searched.
        * @param aAlwaysListHidden: boolean to show hidden services or not.
        * @return array of all the services found.
        * NOTE: Calling module is responsible to delete array.
        */
        IMPORT_C static CMsvEntrySelection* GetListOfAccountsWithMTML(
            CMsvSession& aSession,
            TUid aMtm,
            TBool aAlwaysListHidden=EFalse );
        /**
         * Checks if the disk storage will fall below Critical Level (CL)
         * if a number of bytes are stored to the disk. Takes account where
         * message store resides (in c: or possible in memory card).
         * The CL is defined by SysUtil.
         * Usage os this function is same as SysUtil::FFSSpaceBelowCriticalLevelL.
         * @param aSession: reference to CMsvSession object
         * @param aBytesToWrite number of bytes the caller is about to add
         *        to the disk, if known by the caller beforehand.
         *        If not known, the default value 0 checks if the current
         *        space is already below the CR.
         * @return ETrue if storage space would go below CL after adding
         *               aBytes more data, EFalse otherwise.
         *
         */
        IMPORT_C static TBool DiskSpaceBelowCriticalLevelL( CMsvSession& aSession, 
                                                            TInt aBytesToWrite );

        /**
         * Checks if the disk storage will fall below critical level when performing
         * an operation which consumes phone memory in addition to message
         * store memory.
         *
         * For example, sending consumes always memory from CDrive is current drive CDrive or not.
         *
         * If current drive is not CDrive then check that free memory of CDrive is greater than
         * aPhoneMemoryOverheadBytes plus KDRIVECCRITICALTHRESHOLD(defined in SharedDataKeys.h) . 
         * After that check that free memory of current drive is
         * greater than aBytesToWrite plus KDRIVECCRITICALTHRESHOLD.
         * 
         * If current drive is CDrive then check that free memory of CDrive is greater than
         * aPhoneMemoryOverheadBytes plus aBytesToWrite plus KDRIVECCRITICALTHRESHOLD.
         *
         * @param aSession: reference to CMsvSession object
         * @param aBytesToWrite number of bytes the caller is about to add
         *        to the disk, if known by the caller beforehand.
         *        If not known, the default value 0 checks if the current
         *        space is already below the CR.
         * @param aPhoneMemoryOverheadBytes Sending consumes always memory from C drive.
         *        So if current drive is memory card or other than CDrive we must
         *        check that free memory of CDrive is greater than aPhoneMemoryOverheadBytes plus
         *        KDRIVECCRITICALTHRESHOLD.
         * @return ETrue if storage space would go below critical level, EFalse otherwise.
         */
        IMPORT_C static TBool DiskSpaceBelowCriticalLevelWithOverheadL( CMsvSession& aSession,
                                                                TInt aBytesToWrite,
                                                                TInt aPhoneMemoryOverheadBytes );

 			 /**
        * Test if aAddress is format <name>'@'<domain>
        * If <name> is between "-marks, it can contain any chars between ' ' and Del
        * (127).
        * Otherwise it can contain chars between ' ' and Del (127),
        * but not centain special chars.
        * Valid name can not contain two dots sequense.
        * <domain>-part is tested with function IsValidDomainL.
        * @since 3.0
        * @param aAddress: email address to be checked.
        * @return Returns ETrue if aAddress is a lexically valid emailaddress.
        */
        IMPORT_C static TBool IsValidEmailAddressL( const TDesC& aAddress );

        /**
        * Test if aDomain is format <char>+{"."<char>+}+
        * Valid char is char between ' ' and Del (127).
        * Valid domain name can not include centain special chars.
        * @since 3.0
        * @param aDomain: domain to be checked.
        * @return Returns ETrue if aDomain is a lexically valid domain name.
        */
        IMPORT_C static TBool IsValidDomainL( const TDesC& aAddress );
       
       private:
        /**
        * internal
        */
        static CMsvEntrySelection* DoGetListOfAccountsL(
            CMsvSession& aSession,
            TUid aMtm,
            TBool aAlwaysListHidden=EFalse);

    };

#endif //__MUIUSERVICEUTILITIES_H__

// End of file
