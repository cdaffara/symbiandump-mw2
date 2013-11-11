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
* Description:   SIMPLE document
*
*/




#ifndef simpledocument_H
#define simpledocument_H

#include <e32std.h>
#include "simplecommon.h"
#include "msimpledocument.h"
#include "simplebasedocument.h"

// FORWARD DECLARATION
class CSimpleNamespace;
class CSimpleContent;
class CBodyPart;
class MSimpleContent;
// class RPointerArray;



                   


/**
 *  CSimpleDocument
 *
 *  SIMPLE Engine core.
 *
 *  @lib simplexmlutils
 *  @since S60 3.2
 */

class CSimpleDocument : public CSimpleBaseDocument, public MSimpleDocument
    {
    
public:

   
public:

    /**
     * Two-phased constructor.
     * @since S60 3.2
     * @return CSimpleDocument
     */
    static CSimpleDocument* NewL( );

    /**
     * Two-phased constructor.
     * @since S60 3.2
     * @param aXml XML document
     * @return CSimpleDocument
     */
    static CSimpleDocument* NewL( const TDesC8& aXml );
    
    /**
     * Two-phased constructor.
     * @since S60 3.2
     * @param aData MIME multipart data     
     * @param aBoundary MIME multipart boundary     
     * @param aStart MIME multipart start content-id  
     * @return CSimpleDocument
     */
    static CSimpleDocument* NewInMultiPartL( 
        const TDesC8& aData, const TDesC8& aBoundary, const TDesC8& aStart );    

    /**
     * Destructor.
     */
    virtual ~CSimpleDocument();
    
    /**
     * Validates root element name
     * Leaves if not valid
     * @param aName element name
     */
    void ValidateXmlL( const TDesC8& aName );

// from base class MSimpleDocument


    /**
     * From MSimpleDocument
     * Entity's URI getter. URI is specified in a factory method
     *
     * @since S60 3.2
     * @return URI
     */
    const TDesC8* EntityURI();

    /**
     * From MSimpleDocument
     * Entity's URI setter.
     *
     * @since S60 3.2
     * @return URI
     */
    void SetEntityURIL( const TDesC8& aValue );

    /**
     * From MSimpleDocument
     * Document's default namespace accessor. It is PIDF namespace.
     *
     * @since S60 3.2
     * @return default namespace URI
     */
    TPtrC8 DefaultNamespace();

    /**
     * From MSimpleDocument
     * Add new newspace
     *
     * @since S60 3.2
     * @param aPrefix:  Namespace prefix
     * @param aUri: Namespace URI
     */
    void AddNamespaceL( const TDesC8& aPrefix,
                        const TDesC8& aUri);

    /**
     * From MSimpleDocument
     * Gets all namespaces of this element in an array.
     *
     * @since S60 3.2
     * @return array of namespaces. Array will be empty if element has
     *         no namespaces.
     *         Any modifications made on the returned items
     *         DOES NOT modify the real childs.
     *         Do NOT call ResetAndDestroy() for the array,
     *         you may call Reset() only. The array is valid only till
     *         the next call of this method in this entity.      
     */
    RPointerArray<MSimpleNamespace>& NamespacesL();
    
    /**
     * Copies content from given element to this element appending to the 
	 * existing content if there is any.
     * @since S60 3.2
     * @param aSource The source element.
     */
	void CopyFromL( MSimpleDocument& aSource );

    /**
     * Externalize the document into write stream.
     *
     * @since S60 3.2
     * @param aStream write stream
     */
    void ExternalizeL( RWriteStream& aStream );
    
    /**
     * Get direct contents
     * @param aContents array of contents [out]. 
     *        This is walid till the AddDirectContentL method. 
     */     
    void GetDirectContentsL( RPointerArray<MSimpleContent>& aContents );
    
    /**
     * Add content
     * @param aContent content, the data content is copied into document.
     * @param aCopyContent ETrue if buffer of each content is copied or 
     *        ownership is transferred just without copy.     
     */
    void AddDirectContentL( MSimpleContent& aContent, TBool aCopyContent );
    
    
// from base class MSimpleEntity

    /**
     * Destroy this element entity and all its descendant elements
     *
     * @since S60 3.2
     */
    void Close();

    /**
     * Getter for Element's local name.
     * LocalName and Namespace are given in factory method.
     * @return KNullDesC if not set
     */
    const TDesC8& LocalName();

    /**
     * Getter for Element's default namespace.
     *
     * @since S60 3.2
     * @return const pointer to the CSenNamespace object of this Element.
     *        NULL if not set. Ownership is transferred.
     */
    MSimpleNamespace* DefNamespaceL();

    /**
     * Method for checking if the element has any content within.
     *
     * @since S60 3.2
     * @return ETrue if has content, EFalse if not.
     */
    TBool HasContent();

    /**
     * Getter for the content of the element, unicode version.
     *
     * @since S60 3.2
     * @return content as unicode. Ownership IS TRANSFERRED to the caller.
     */
    HBufC* ContentUnicodeL();

    /**
     * Sets the content to the element. Old content is overwritten.
     *
     * @since S60 3.2
     * @param aContent: The content to be set. Can be KNullDesC
     */
    void SetContentUnicodeL(const TDesC& aContent);


    /**
     * Getting the child elements of this element matching the given criteria.
     * @param aElementArray:    Array to be filled with the matching elements,
     *                          or empty array if no matching found.
     *                          Any modifications made on the returned items
     *                          modify the real childs too.
     * @return KErrNone     ok
     *         KErrNotFound No child elements exist.
     */
    TInt SimpleElementsL( RPointerArray<MSimpleElement>& aElementArray );

    /**
     * Gets the value of the given attribute.
     *
     * @since S60 3.2
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
     * Adds an attribute. If attribute is already existing,
     * the value of the attribute will be replaced.
     *
     * @since S60 3.2
     * @param aName     Name of the attribute to be added.
     * @param aValue    Value of the attribute to be added.
     */
    void AddAttrL(const TDesC8& aName, const TDesC& aValue);

    /**
     * From MSimpleElement
     * Gets all the attributes of this element in an array.
     *
     * @since S60 3.2
     * @param aArray: Array to be filled with the attributes,
     *                       or empty array if no matching found.
     * @return KErrNone ok
     *         KErrNotFound No child elements exist.
     */
    TInt SimpleAttributesL( RPointerArray<MSimpleAttribute>& aArray );

    /**
     * Gets the parent element of this element.
     *
     * @since S60 3.2
     * @return the parent element or NULL if no parent set.
     *        Ownership IS TRANSFERRED to the caller.
     */
    MSimpleElement* SimpleParentL();

    /**
     * Detach the element from its parent.
     * If the element, or one of its children, is dependent
     * on a namespace declared in the scope of the parent
     * copy those namespace declarations to this element.
     * Leaves with KErrNotFound if parent was not set and
     * nothing to detach.
     *
     * @since S60 3.2
     */
    void DetachSimpleL();

    /**
     * Constructs and adds a new element to the children elements.
     * Sets this element to be the new parent of the given element.
     *
     * @since S60 3.2
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
     * Constructs and adds a new element to the children elements.
     * Sets this element to be the new parent of the given element.
     * Note: Element is created with no specific namespace, default namespace
     * of some of the upper level elements are in effect if there is such a
     * namespace.
     *
     * @since S60 3.2
     * @param aLocalName:   local name of the new element
     * @return the added Element. OWNERSHIP IS TRANSFERRED.
     *  Leave codes:
     *      KErrSenInvalidCharacters if aLocalName contains illegal characters.
     *      KErrSenZeroLengthDescriptor if aLocalName is zero length.
     */
    MSimpleElement* AddSimpleElementL(
        const TDesC8& aLocalName );

    /**
     * Remove an element from the childs.
     *
     * @since S60 3.2
     * @param   aNsUri:     the namespace URI of the element to be removed.
     * @param   aLocalName: the local name of the element to be removed.
     */
    void RemoveSimpleElement(
        const TDesC8& aNsUri,
        const TDesC8& aLocalName);


protected:


private:

    /**
     * Two-phase constructor
     */
    void ConstructL(
        const TDesC8& aNsUri,
        const TDesC8& aLocalName );
        
    /**
     * Two-phase constructor
     */
    void ConstructMultiPartL(
        const TDesC8& aData, const TDesC8& aBoundary, const TDesC8& aStart );        

    /**
     * Two-phase constructor
     */
    void ConstructL(
        const TDesC8& aXml );              

    /**
     * constructor
     */
    CSimpleDocument();
    
	/**
	 * ClenupStack operation
	 * aPtrArray RPointerArray<CBodyPart>
	 */
	static void ResetAndDestroy( TAny* aPtrArray );	 
	
    /**
     * DoConstructL
     * Handles XML parsing of MIME multiparts
     * @param aParts MIME multipart parts
     * @param aStart MIME multipart start content-id     
     */
    void DoConstructL( RPointerArray<CBodyPart>& aParts, const TDesC8& aStart );
    
    HBufC8* DoComposeMultiPartL( RPointerArray<CBodyPart>& aBodyArray,
                                const TDesC8& aBoundary);  
                                
    void DoExternalizeMultiPartL( 
        RPointerArray<CBodyPart>& aBodies, 
        RPointerArray<HBufC8>& aBuffers,
        RWriteStream& aStream );                              	   


private:    // data

    /**
     * Direct contents in MIME multipart 
     */
    RPointerArray<CSimpleContent> iContents;

    };


#endif      // simpledocument_H

// End of File
