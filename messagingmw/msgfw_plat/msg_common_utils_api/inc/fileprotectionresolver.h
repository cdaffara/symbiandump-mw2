/*
* Copyright (c) 2005, 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
*           Utility class for resolving file protection status.
*
*/



#ifndef __FILEPROTECTIONRESOLVER_H
#define __FILEPROTECTIONRESOLVER_H

// INCLUDES

#include <e32std.h>
#include <e32base.h> // CBase
#include <apmstd.h>  // TDatatype

// CONSTANTS

// MACROS

// FORWARD DECLARATIONS
class CCommonContentPolicy;
class RFs;
class RFile;

// DATA TYPES

enum TFileProtectionStatus
    {
    EFileProtNoProtection       = 0x0000,
    EFileProtForwardLocked      = 0x0001,
    EFileProtSuperDistributable = 0x0002,
    EFileProtClosedContent      = 0x0004,
    // Flag indicates whether file is DRM2 protected. This is not related to sending permission
    EFileProtDrm2               = 0x0008    
    };

// FUNCTION PROTOTYPES

// CLASS DECLARATION
/**
* CFileProtectionResolver...
*
* @lib ???
* @since 3.0
*/
class CFileProtectionResolver : public CBase
    {
    public:  // New methods

        /**
        * Factory method.
        *
        * @param aFs        IN Fileserver session.
        * @return Pointer to instance in CleanupStack
        */
        IMPORT_C static CFileProtectionResolver* NewLC( RFs& aFs );
         
        /**
        * Factory method.
        *
        * @param aFs        IN Fileserver session.
        * @return Pointer to instance
        */
        IMPORT_C static CFileProtectionResolver* NewL( RFs& aFs );

        /**
        * Destructor
        */
        virtual ~CFileProtectionResolver();

        /**
        * Get Common Content Policy
        *
        * @return Pointer to CCommonContentPolicy
        */
        inline CCommonContentPolicy* CommonContentPolicy() const;

        /**
        * Finds out whether the file is DRM protected or
        * is on the Closed Content List
        *
        * @param aFile      IN File handle
        * @param aMimeType  IN Already recognized mime type.
        * @return TFileProtectionStatus
        */
        IMPORT_C TInt ProtectionStatusL(
            RFile& aFile,
            TDataType& aMimeType );

        /**
        * Finds out whether the file is DRM protected or
        * is on the Closed Content List
        *
        * @param aFilePath  IN File path
        * @param aMimeType  IN Already recognized mime type.
        * @return TFileProtectionStatus
        */
        IMPORT_C TInt ProtectionStatusL(
            const TDesC& aFilePath,
            TDataType& aMimeType );

        /**
        * Finds out whether the file is DRM protected or
        * is on the Closed Content List
        *
        * @param aFile          IN File handle
        * @param aMimeType      IN Already recognized mime type.
        * @param aContentUri    IN/OUT possible DRM content URI, NULL if not found
        * @return TFileProtectionStatus
        */
        IMPORT_C TInt ProtectionStatusL(
            RFile& aFile,
            TDataType& aMimeType,
            HBufC8*& aContentUri );

        /**
        * Finds out whether the file is DRM protected or
        * is on the Closed Content List
        *
        * @param aFilePath      IN File path
        * @param aMimeType      IN Already recognized mime type.
        * @param aContentUri    IN/OUT possible DRM content URI, NULL if not found
        * @return TFileProtectionStatus
        */
        IMPORT_C TInt ProtectionStatusL(
            const TDesC& aFilePath,
            TDataType& aMimeType,
            HBufC8*& aContentUri );

    private:

        /**
        * Constructor.
        *
        * @param aFs        IN Fileserver session.
        */
        CFileProtectionResolver( RFs& aFs );

        /**
        * 2nd phase constructor.
        */
        void ConstructL();

    private: // data

        RFs&                    iFs;
        CCommonContentPolicy*   iClosedContent;

    };
        
// -----------------------------------------------------------------------------
// CFileProtectionResolver::CommonContentPolicy
// ACCESSOR
// -----------------------------------------------------------------------------
inline CCommonContentPolicy* CFileProtectionResolver::CommonContentPolicy() const
    {
    return iClosedContent;
    }

#endif // __FILEPROTECTIONRESOLVER_H
