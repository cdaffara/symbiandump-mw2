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
* Description:   XDM node attribute
*
*/




#ifndef __XDMNODEATTRIBUTE__
#define __XDMNODEATTRIBUTE__

#include <e32base.h>
#include "XdmDocumentNode.h"

class CXdmEngine;

//CLASS DECLARATION
class CXdmNodeAttribute : public CXdmDocumentNode
    {
    public:
        
        /**
        * Return the value of this attribute
        *
        * NOTE! The name of an attribute is retrieved through a call
        *       to the NodeName() member function in the base class. 
        *
        * @return TPtrC8 The value of this attribute
        */     
        IMPORT_C TPtrC AttributeValue() const;

        /**
        * Return the value of this attribute
        *
        * @return HBufC8* The value of this attribute
        */     
        IMPORT_C HBufC8* EightBitValueLC() const;
        
        /**
        * Return the escaped value of this attribute
        *
        * @return HBufC8* The value of this attribute
        */     
        IMPORT_C HBufC8* EscapedValueLC() const;
        
        /**
        * Set the value of this attribute
        *
        * @param TDesC8& Value of the attribute
        * @return void
        */     
        IMPORT_C void SetAttributeValueL( const TDesC& aAttributeValue );
        
        /**
        * Set the value of this attribute
        *
        * @param TDesC8& Value of the attribute
        * @return void
        */     
        IMPORT_C void SetAttributeValueL( const TDesC8& aAttributeValue );
        
        /**
        * The type of this element: an element or an attribute
        *
		* @return TXdmElementType* 
        */
        IMPORT_C virtual TXdmElementType ElementType() const;
          
        /**
        * Destructor.
        */      
        IMPORT_C virtual ~CXdmNodeAttribute();

    protected:  //Methods
        
        /**
        * Symbian OS constructor for derived classes.
        *
        * @param CXdmEngine& Reference to the engine
        * @param MXdmNodeFactory* Reference to the node factory
        */     
        IMPORT_C CXdmNodeAttribute( CXdmEngine& aXdmEngine,
                                    MXdmNodeFactory& aNodeFactory );
                                    
        /**
        * Symbian OS constructor for derived classes.
        *
        * @param CXdmEngine& Reference to the engine
        * @param MXdmNodeFactory* Reference to the node factory
        * @param CXdmDocumentNode* Pointer to the parent element
        */    
        IMPORT_C CXdmNodeAttribute( CXdmEngine& aXdmEngine,
                                    MXdmNodeFactory& aNodeFactory,
                                    CXdmDocumentNode* aParentNode );
        
        /**
        * Symbian OS base class constructor.
        *
        * @param TDesC& Name of the attribute
        * @return void
        */     
        IMPORT_C void BaseConstructL( const TDesC& aAttributeName );
    
    private:
      
        /**
        * Set the value of this element
        *
        * @param TDesC Attribute name
        * @return void
        */   
        void SetValueL( const TDesC& aAttributeName );

    private:  //Data
        
        HBufC*                         iAttributeValue;
    };

#endif      //__XDMNODEATTRIBUTE__
            
// End of File
