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
* Description:   XDM Protocol node factory
*
*/




#ifndef __XDMNODEFACTORY__
#define __XDMNODEFACTORY__

#include <e32cmn.h>

class CXdmDocumentNode;
class CXdmNodeAttribute;

//CLASS DECLARATION
NONSHARABLE_CLASS( MXdmNodeFactory )
    {
    public:
        
        /**
        * Create a new child element
        *
        * @param TDesC8& Name of the new element
		* @return CXdmDocumentNode* 
        */
        virtual CXdmDocumentNode* ChileNodeL() = 0;
        
        /**
        * Create a new child element
        *
        * @param TDesC8& Name of the new element
		* @return CXdmDocumentNode* 
        */
        virtual CXdmDocumentNode* ChileNodeL( const TDesC& aChildName ) = 0;
        
        /**
        * Create a new child element
        *
        * @param TDesC8& Name of the new element
		* @return CXdmDocumentNode* 
        */
        virtual CXdmDocumentNode* ChileNodeL( const CXdmDocumentNode* aAnotherNode ) = 0;

        /**
        * Create a new attribute
        *
        * @param TDesC8& Name of the new attribute
		* @return CXdmNodeAttribute* 
        */
        virtual CXdmNodeAttribute* AttributeL() = 0;
        
        /**
        * Create a new attribute
        *
        * @param TDesC8& Name of the new attribute
		* @return CXdmNodeAttribute* 
        */
        virtual CXdmNodeAttribute* AttributeL( const TDesC& aAttributeName ) = 0;
    };

#endif    //__XDMNODEFACTORY__
            
// End of File
