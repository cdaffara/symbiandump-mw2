/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the CbsUtils class
*
*/


#ifndef CBSUTILS_H
#define CBSUTILS_H

// INCLUDES
#include <e32std.h>
  
// FORWARD DECLARATIONS
class RResourceFile;
class RFs;

// CLASS DECLARATION

/**
*   Namespace structure for CBS helper functions.
*/
class CbsUtils 
    {
    public:

        /**
        * Searches and opens the DLL default resource file. 
        * Calls FindAndOpenResourceFileLC.
        *
        * @param   aFs         open file server handle.
        * @param   aResFile    resource file to open. If open is succesful, resource file
        *                      close operation is pushed on to the cleanup stack.
        *
        * @see CbsUtils::FindAndOpenResourceFileLC
        */
        static void FindAndOpenDefaultResourceFileLC
            ( RFs& aFs, 
            RResourceFile& aResFile );

        /**
        *   Leaves with KErrDiskFull if writing aBytesToWrite bytes 
        *   to the Flash File System would reduce the free space on FFS 
        *   under the critical level.
        *
        *   Used before every FFS write operation.
        *
        *   @param   aBytesToWrite      Number of bytes that are required 
        *                               to write to FFS
        *   @param   aFs                Open file server session
        *
        */
        static void FFSCriticalLevelCheckL( const TInt aBytesToWrite, RFs& aFs );
        
        /**
        *   Returns with KTrue if there is memory enough to allocate memory 
        *   for aBytesToWrite bytes.
        *
        *   Used before every topic allocation operation.
        *
        *   @param   aBytesToWrite      Number of bytes that are required 
        *                               to allocation
        *
        */
        static TBool RamMemCriticalLevelCheckL( const TInt aBytesToWrite);

        /**
        *   Leaves with KErrDiskFull if writing aBytesToWrite bytes 
        *   to the RAM File System would reduce the free space on RAM 
        *   under the critical level.
        *
        *   Used before every RAM write operation.
        *
        *   @param aFileName            File whose volume should be checked.
        *   @param aBytesToWrite        Number of bytes that are required
        *                               to write to RAM
        *   @param aFs                  Open file server session
        */
        static void VolumeCriticalLevelCheckL( 
            const TDesC& aFileName,
            TInt aBytesToWrite, const RFs& aFs );

        /**
        *   Returns ETrue, if the file aFile exists.
        *   
        *   The caller has to provide a file server session to this method.
        *
        *   @param aFs          Open file server session
        *   @param aFile        File name and path
        *   @return             ETrue, if the file exists.
        */
        static TBool ExistsL( const RFs& aFs, const TDesC& aFile );

        /**
        *   Deletes the given file.
        *
        *   @param  aFs         Open file server session
        *   @param  aFile       Filename
        */
        static void DeleteFileL( RFs& aFs, const TDesC& aFile );

private:
    // Prohibited default constructor
    CbsUtils();

    // Prohibited copy constructor
    CbsUtils( const CbsUtils& );
    
    // Prohibited assignment operator
    CbsUtils& operator=( const CbsUtils& );

    };

#endif  // CBSUTILS_H

// End of File


