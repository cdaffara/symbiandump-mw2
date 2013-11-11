/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   XDM credentials
*
*/




#ifndef __XDMCREDENTIALS__
#define __XDMCREDENTIALS__

#include <e32base.h>

const TInt KXdmMaxUserNameLength		               = 256;
const TInt KXdmMaxPasswordLength		               = 256;

//CLASS DECLARATION
class TXdmCredentials
    {
    public:
        
        /**
        * C++ Defualt Constructor
        */
        IMPORT_C TXdmCredentials();
        
        /**
        * Constructor with parameters
        * @param TDesC& Username
        * @param TDesC& Password
        */
        IMPORT_C TXdmCredentials( const TDesC& aUserName,
                                  const TDesC& aPassword );

    public:  //Data
        
        TBuf8<KXdmMaxUserNameLength>                   iUserName;
        TBuf8<KXdmMaxPasswordLength>                   iPassword;
    };

#endif      //__XDMCREDENTIALS__
            
// End of File
