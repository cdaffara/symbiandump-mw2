/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Landmarks Server's utilities
*
*/


#ifndef EPOS_POSLMSERVERUTILITIES_
#define EPOS_POSLMSERVERUTILITIES_

#include <e32base.h>
#include <f32file.h>
#include <d32dbms.h>

class CPosLmLocalDbAccess;

class PosLmServerUtility
    {
    public:
    
        enum TLmOperation
            {
            ECreateFieldsLmIdIndex,
            /**< Create index in fields table on Lm ID field */
            ECreateCategoriesLmIdIndex,
            /**< Create index in categories table on Lm ID field */
            };

    public: // New functions
        
        /**
        * Method to estimate disk size impact for creating
        * foreign index in a table.
        * @return Estimated number of bytes.
        */
        static TInt EstimatedDiskSizeOfIndex(
            TLmOperation aOperation,
            TInt aCountOfRecords );

        /**
        * Reads the data of a long text column.
        *
        * @param[in] aView Prepared view.
        * @param[in] aCol The number of the column to read from.
        * @return Data from the column.
        */
        static HBufC* ReadFromLongTextColumnLC(
            RDbRowSet& aView,
            TDbColNo aCol );

        /** Returns number of rows in a table 
         * @param aTableName Name of the table which rowcount is needed */
        static TUint GetTableRowCountL( 
            CPosLmLocalDbAccess& aDbAccess,
            const TDesC& aTableName );

        static TUint TotalLandmarkCountL( CPosLmLocalDbAccess& aDbAccess );

        /** Checks whether given index exists on given table */
        static TBool IndexExistsL(
            const RDbNamedDatabase& aDatabase,
            const TDesC& aTable,
            const TDesC& aIndex );

        /**
        * Reads the data of a long text column.
        *
        * @param[in] aView Prepared view.
        * @param[in] aCol The number of the column to read from.
        * @return Data from the column.
        */
        //Duplicate Declaration
        /*HBufC* ReadFromLongTextColumnLC(
            RDbRowSet& aView,
            TDbColNo aCol );*/
    };

#endif /*EPOS_POSLMSERVERUTILITIES_*/
