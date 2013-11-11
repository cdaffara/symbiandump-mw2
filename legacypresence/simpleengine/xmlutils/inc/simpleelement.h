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
* Description:   SIMPLE element
*
*/




#ifndef simpleelement_H
#define simpleelement_H

#include <e32std.h>
#include <SenBaseElement.h>
#include "msimpleelement.h"
#include "msimpleattribute.h"

// FORWARD DECLARATION
class MSimpleNamespace;
class CSenElement;
class CSimpleAttribute;
class CSimpleElement;

/**
 *  CSimpleElement
 *
 *  SIMPLE element.
 *
 *  @lib simplexmlutils
 *  @since S60 v3.1
 */

class CSimpleElement : public CBase, public MSimpleElement
    {

public:

    /**
     * Two-phased constructor.
     * @since S60 v3.1
     */
    static CSimpleElement* NewL( const TDesC8& aNsUri,
        const TDesC8& aLocalName  );

    /**
     * Two-phased constructor.
     * @since S60 v3.1
     * @param aElement base element
     * @param aOwn whethre ownership is transferred
     */
    static CSimpleElement* NewL(
        CSenElement* aElement,
        TBool aOwn );

    /**
     * Destructor.
     */
    virtual ~CSimpleElement();

    /**
     * @return Base class implementation
     */
    CSenElement* BaseElement();
    
    /**
     * From MSimpleElement
     * Adds an attribute. If attribute is already existing,
     * the value of the attribute will be replaced.
     *
     * @since Series60 3.1
     * @param aName     Name of the attribute to be added.
     * @param aValue    Value of the attribute to be added.
     */
    void AddAttr8L(const TDesC8& aName, const TDesC8& aValue);    

// from base class MSimpleElement


    /**
     * From MSimpleElement
     * Destroy this element entity and all its descendant elements
     *
     * @since S60 v3.1
     */
    void Close();

    /**
     * From MSimpleElement
     * Getter for Element's local name.
     * LocalName and Namespace are given in factory method.
     * @return KNullDesC if not set
     */
    const TDesC8& LocalName();

    /**
     * From MSimpleElement
     * Getter for Element's default namespace.
     *
     * @since S60 v3.1
     * @return const pointer to the CSenNamespace object of this Element.
     *        NULL if not set. Ownership is transferred.
     */
    MSimpleNamespace* DefNamespaceL();

    /**
     * From MSimpleElement
     * Method for checking if the element has any content within.
     *
     * @since S60 v3.1
     * @return ETrue if has content, EFalse if not.
     */
    TBool HasContent();

    /**
     * From MSimpleElement
     * Getter for the content of the element, unicode version.
     *
     * @since S60 v3.1
     * @return content as unicode. Ownership IS TRANSFERRED to the caller.
     */
    HBufC* ContentUnicodeL();

    /**
     * From MSimpleElement
     * Sets the content to the element. Old content is overwritten.
     *
     * @since S60 v3.1
     * @param aContent: The content to be set. Can be KNullDesC
     */
    void SetContentUnicodeL(const TDesC& aContent);

    /**
     * From MSimpleElement
     * Getting the child elements of this element.
     *
     * @since Series60 3.1
     * @param aElementArray:    Array to be filled with the matching elements,
     *                          or empty array if no matching found.
     *                          Any modifications made on the returned items
     *                          modify the real childs too.
     * @return KErrNone     ok
     *         KErrNotFound No child elements exist.
     */
    TInt SimpleElementsL( RPointerArray<MSimpleElement>& aElementArray );

    /**
     * From MSimpleElement
     * Gets the value of the given attribute.
     *
     * @since Series60 3.1
     * @param aName:    Name of the attribute in question.
     * @return the value of the attribute, or NULL if not found. Ownership is
     *        TRANSFERRED.
     */
    HBufC* AttrValueLC( const TDesC8& aName );

    /**
     * Gets the value of the given attribute.
     * @param aName:    Name of the attribute in question.
     * @return the value of the attribute, or NULL if not found. Ownership is
     *         NOT TRANSFERRED.
     */
    const TDesC8* AttrValue(const TDesC8& aName);

    /**
     * From MSimpleElement
     * Adds an attribute. If attribute is already existing,
     * the value of the attribute will be replaced.
     *
     * @since Series60 3.1
     * @param aName     Name of the attribute to be added.
     * @param aValue    Value of the attribute to be added.
     */
    void AddAttrL(const TDesC8& aName, const TDesC& aValue);

    /**
     * From MSimpleElement
     * Gets all the attributes of this element in an array.
     *
     * @since Series60 3.1
     * @param aArray: Array to be filled with the attributes,
     *                       or empty array if no matching found.
     * @return KErrNone ok
     *         KErrNotFound No child elements exist.
     */
    TInt SimpleAttributesL( RPointerArray<MSimpleAttribute>& aArray );

    /**
     * From MSimpleElement
     * Gets the parent element of this element.
     *
     * @since Series60 3.1
     * @return the parent element or NULL if no parent set.
     *        Ownership is IS TRANSFERRED to the caller.
     */
    CSimpleElement* SimpleParentL();

    /**
     * From MSimpleElement
     * Detach the element from its parent.
     * If the element, or one of its children, is dependent
     * on a namespace declared in the scope of the parent
     * copy those namespace declarations to this element.
     * Leaves with KErrNotFound if parent was not set and
     * nothing to detach.
     *
     * @since Series60 3.1
     */
    void DetachSimpleL();

    /**
     * From MSimpleElement
     * Constructs and adds a new element to the children elements.
     * Sets this element to be the new parent of the given element.
     *
     * @since Series60 3.1
     * @param aNsUri:       namespace URI of the new element
     * @param aLocalName:   local name of the new element
     * @return the added Element
     * Leave codes:
     *      KErrSenInvalidCharacters if aLocalName contains illegal characters.
     *      KErrSenZeroLengthDescriptor if aLocalName is zero length.
     */
    MSimpleElement* AddSimpleElementL(
        const TDesC8& aNsUri,
        const TDesC8& aLocalName);

    /**
     * From MSimpleElement
     * Constructs and adds a new element to the children elements.
     * Sets this element to be the new parent of the given element.
     * Note: Element is created with no specific namespace, default namespace
     * of some of the upper level elements are in effect if there is such a
     * namespace.
     *
     * @since Series60 3.1
     * @param aLocalName:   local name of the new element
     * @return the added Element. OWNERSHIP IS TRANSFERRED.
     * Leave codes:
     *      KErrSenInvalidCharacters if aLocalName contains illegal characters.
     *      KErrSenZeroLengthDescriptor if aLocalName is zero length.
     */
    MSimpleElement* AddSimpleElementL(
        const TDesC8& aLocalName );


    /**
     * From MSimpleElement
     * Remove an element from the childs.
     *
     * @since Series60 3.1
     * @param   aNsUri:     the namespace URI of the element to be removed.
     * @param   aLocalName: the local name of the element to be removed.
     */
    void RemoveSimpleElement( const TDesC8& aNsUri,
                              const TDesC8& aLocalName);

protected:

    /**
     * Two-phase constructor
     * @param aNsUri Namespace URI
     * @param aLocalName local name
     */
    void ConstructL( const TDesC8& aNsUri,
        const TDesC8& aLocalName );

    void ConstructL(
        CSenElement* aElem,
        TBool aOwn );

    /**
     * constructor
     */
    CSimpleElement();

private: // data

    /**
     * Base element when created from scratch
     * Own.
     */
    CSenElement*    iBase;

    /**
     * Base element when only reference to base element
     * Not own.
     */
    CSenElement*    iRef;

    /**
     * Attributes. 
     */
    RPointerArray<CSimpleAttribute> iAttributes;

    /**
     * Attributes. 
     */
    RPointerArray<CSimpleElement> iElements;

    };




#endif      // ?INCLUDE_H

// End of File
