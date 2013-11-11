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
* Description:    SIMPLE data element
*
*/




#ifndef M_simpleelement_H
#define M_simpleelement_H

#include <e32std.h>
#include <msimpleattribute.h>

// FORWARD DECLARATION
class MSimpleNamespace;

/**
 *  MSimpleDocument
 *
 *  SIMPLE data document
 *
 *  @lib simplexmlutils
 *  @since S60 3.2
 */

class MSimpleElement
    {

public:

    /**
     * Destroy this element entity and all its descendant elements.
     * @since S60 3.2
     */
    virtual void Close() = 0;

    /**
     * Getter for Element's local name.
     * LocalName and Namespace are given in factory method.
     * @since S60 3.2
     * @return KNullDesC8 if not set
     */
    virtual const TDesC8& LocalName() = 0;

    /**
     * Getter for Element's own default namespace.
     *
     * @since S60 3.2
     * @return pointer to the MSimpleNamespace object of this Element.
     *        NULL if not set for this element. Ownership is transferred.
     */
    virtual MSimpleNamespace* DefNamespaceL() = 0;

    /**
     * Method for checking if the element has any content within.
     *
     * @since S60 3.2
     * @return ETrue if has content, EFalse if not.
     */
    virtual TBool HasContent() = 0;

    /**
     * Getter for the content of the element, unicode version.
     *
     * @since S60 3.2
     * @return content as unicode. Ownership IS TRANSFERRED to the caller.
     */
    virtual HBufC* ContentUnicodeL()= 0;

    /**
     * Sets the content to the element. Old content is overwritten.
     *
     * @since S60 3.2
     * @param aContent: The content to be set. Can be KNullDesC
     */
    virtual void SetContentUnicodeL( const TDesC& aContent ) = 0;

    /**
     * Getting the child elements of this element.
     * @param aElementArray: Array to be filled with the child elements,
     *         or empty array if no childs.
     *         Any modifications made on the returned items
     *         modify the real childs.
     *         Do NOT call ResetAndDestroy() for the array,
     *         you may call Reset() only. The array is valid only till
     *         the next call of this method in this entity. New elements are
     *         not attached to the array automatically, so that you have
     *         to re-call this method if you wish to see all the elements
     *         after calling AddSimpleElementL() method.
     * @return KErrNone     ok
     *         KErrNotFound No child elements exist.
     */
    virtual TInt SimpleElementsL( RPointerArray<MSimpleElement>& aElementArray ) = 0;

    /**
     * Gets the value of the given attribute.
     *
     * @since S60 3.2
     * @param aName:    Name of the attribute in question.
     * @return the value of the attribute, or NULL if not found. Ownership is
     *        TRANSFERRED.
     */
    virtual HBufC* AttrValueLC( const TDesC8& aName ) = 0;

    /**
     * Gets the value of the given attribute.
     * @param aName:    Name of the attribute in question.
     * @return the value of the attribute, or NULL if not found. Ownership is
     *         NOT TRANSFERRED.
     */
    virtual const TDesC8* AttrValue( const TDesC8& aName ) = 0;

    /**
     * Adds an attribute. If attribute is already existing,
     * the value of the attribute will be replaced.
     *
     * @since S60 3.2
     * @param aName     Name of the attribute to be added.
     * @param aValue    Value of the attribute to be added.
     */
    virtual void AddAttrL( const TDesC8& aName, const TDesC& aValue ) = 0;

    /**
     * From MSimpleElement
     * Gets all the attributes of this element in an array.
     *
     * @since S60 3.2
     * @param aArray: Array to be filled with the attributes,
     *         or empty array if no childs.
     *         Any modifications made on the returned items
     *         modify the real attributes.
     *         Do NOT call ResetAndDestroy() for the array,
     *         you may call Reset() only. The array is valid only till
     *         the next call of this method in this entity. New attributes are
     *         not attached to the array automatically, so that you have
     *         to re-call this method if you wish to see all the elements
     *         after calling AddAttrL() method.
     * @return KErrNone ok
     *         KErrNotFound No child elements exist.
     */
    virtual TInt SimpleAttributesL( RPointerArray<MSimpleAttribute>& aArray ) = 0;

    /**
     * Gets the parent element of this element.
     *
     * @since S60 3.2
     * @return the parent element or NULL if no parent set.
     *        Ownership is NOT transferred to the caller.
     */
    virtual MSimpleElement* SimpleParentL() = 0;

    /**
     * Detach the element from its parent.
     * Notice that the current element instance should not be
     * used after this method is called successfully.
     * Leaves with KErrNotFound if parent was not set and
     * nothing to detach.
     *
     * @since S60 3.2
     */
    virtual void DetachSimpleL() = 0;

    /**
     * Constructs and adds a new element to the children elements.
     * Sets this element to be the new parent of the given element.
     *
     * @since S60 3.2
     * @param aNsUri:       namespace URI of the new element
     * @param aLocalName:   local name of the new element
     * @return the added Element.
     * OWNERSHIP IS TRANSFERRED.
     * Leave if aLocalName contains illegal characters or
     *       if aLocalName is zero length.
     */
    virtual MSimpleElement* AddSimpleElementL(
        const TDesC8& aNsUri,
        const TDesC8& aLocalName ) = 0;

    /**
     * Constructs and adds a new element to the children elements.
     * Sets this element to be the new parent of the given element.
     * Note: Element is created with no specific namespace, default namespace
     * of some of the upper level elements are in effect if there is such a
     * namespace.
     *
     * @since S60 3.2
     * @param aLocalName:   local name of the new element
     * @return the added Element. OWNERSHIP IS TRANSFERRED.
     * Leave if aLocalName contains illegal characters.
     */
    virtual MSimpleElement* AddSimpleElementL(
        const TDesC8& aLocalName ) = 0;

    /**
     * Remove an element from the childs.
     *
     * @since S60 3.2
     * @param   aNsUri:     the namespace URI of the element to be removed.
     * @param   aLocalName: the local name of the element to be removed.
     */
    virtual void RemoveSimpleElement(
        const TDesC8& aNsUri,
        const TDesC8& aLocalName ) = 0;

    };

#endif

// End of File
