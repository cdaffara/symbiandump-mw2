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
* Description:   XDM node interface
*
*/




#ifndef __XDMNODEINTERFACE__
#define __XDMNODEINTERFACE__

#include <e32base.h>

//CLASS DECLARATION
class MXdmNodeInterface
    {
    public:
        
        /**
        * Return the prefix for this element
        * @return TPtrC8 Prefix descriptor
        */
        virtual TPtrC8 Prefix() const = 0;
        
        /**
        * Set prefix for this element
        * @param TDesC8& Prefix descriptor
        * @return void
        */
        virtual void SetPrefixL( const TDesC8& aNamespacePrefix ) = 0;
    };

#endif      //__XDMNODEINTERFACE__
            
// End of File
