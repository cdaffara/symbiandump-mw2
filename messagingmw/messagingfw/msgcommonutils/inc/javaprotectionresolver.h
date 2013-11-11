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
* Description:  
*       Resolves the protection status for a Java file.
*
*/



#ifndef __JAVAPROTECTIONRESOLVER_H
#define __JAVAPROTECTIONRESOLVER_H

// INCLUDES

#include <e32std.h>
#include <e32base.h> // CBase
#include <apmstd.h>  // TDatatype

// CONSTANTS

// MACROS

// FORWARD DECLARATIONS
class RFs;
class RFile;

// DATA TYPES

// FUNCTION PROTOTYPES

// CLASS DECLARATION
/**
* CJavaProtectionResolver...
*
* @lib muiu.lib
* @since 3.1
*/
NONSHARABLE_CLASS( CJavaProtectionResolver ) : public CBase
    {
    public:  // New methods

        /**
        * Factory method.
        *
        * @param aFs        IN Fileserver session.
        * @return Pointer to instance in CleanupStack
        */
        static CJavaProtectionResolver* NewLC( RFs& aFs );
         
        /**
        * Destructor
        */
        virtual ~CJavaProtectionResolver();

        /**
        * Checks if file is a superdistributable java archive
        * @return ETrue, if file is SD JAR and Java is supported,
        *         EFalse, otherwise.
        */
        TBool IsSuperDistributionPackageL( RFile& aFile );

    private:

        /**
        * Constructor.
        *
        * @param aFs        IN Fileserver session.
        */
        CJavaProtectionResolver( RFs& aFs );

    private: // data

        RFs&                    iFs;

    };

#endif // __JAVAPROTECTIONRESOLVER_H
