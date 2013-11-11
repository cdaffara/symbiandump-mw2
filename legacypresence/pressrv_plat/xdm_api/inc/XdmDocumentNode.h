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
* Description:   XDM document node
*
*/




#ifndef __XDMDOCUMENTNODE__
#define __XDMDOCUMENTNODE__

#include <e32base.h>
#include "XdmNodeInterface.h"

enum TXdmElementType
    {
    EXdmElementNode = 0,
    EXdmElementAttribute,
    EXdmElementUnspecified
    };

class CXdmEngine;
struct SXdmAttribute8;
struct SXdmAttribute16;
class MXdmNodeFactory;
class CXdmNodeAttribute;
class CXdmOperationFactory;

//CLASS DECLARATION
class CXdmDocumentNode : public CBase, public MXdmNodeInterface
    {
    public:  // Constructors and destructor
        
        /**
        * Create a new child element
        *
		* @return CXdmDocumentNode* 
        */
        IMPORT_C CXdmDocumentNode* CreateChileNodeL();
        
        /**
        * Create a new child element
        *
        * @param TDesC8& Name of the new element
		* @return CXdmDocumentNode* 
        */
        IMPORT_C CXdmDocumentNode* CreateChileNodeL( const TDesC& aChildName );

        /**
        * Create a new attribute
        *
        * @param TDesC8& Name of the new attribute
		* @return CXdmNodeAttribute* 
        */
        IMPORT_C CXdmNodeAttribute* CreateAttributeL();
        
        /**
        * Create a new attribute
        *
        * @param TDesC8& Name of the new attribute
		* @return CXdmNodeAttribute* 
        */
        IMPORT_C CXdmNodeAttribute* CreateAttributeL( const TDesC& aAttributeName );

        /**
        * Set the contents of a leaf node. Fails if this element
        * is not a leaf element.
        *
        * <leaf>leafnodecontent</leaf>
        *
        *
        * @param TDesC8& Content of the leaf element
		* @return void 
        */
        IMPORT_C void SetLeafNodeContentL( const TDesC& aLeafContent );
        
        /**
        * Set the contents of a leaf node. Fails if this element
        * is not a leaf element.
        *
        * <leaf>leafnodecontent</leaf>
        *
        *
        * @param TDesC8& Content of the leaf element
		* @return void 
        */
        IMPORT_C void SetLeafNodeContentL( const TDesC8& aLeafContent );
        
        /**
        * Define this node to be a leaf node. Default value is EFalse,
        * so this method only needs to be called when the client wants to
        * define a node a leaf node. The operation can, of course, be
        * reverted if needed.
        *
        * @param TBool Define/undefine this node to be a leaf node
		* @return void 
        */
        IMPORT_C void SetLeafNode( TBool aIsLeafNode );
        
        /**
        * Define this node to be empty. Default value is EFalse,
        * so this method only needs to be called when the client wants
        * to indicate that a node does not/will not have children.
        * The operation can be reverted if needed.
        *
        * NOTE: An attempt to create children on an empty node will
        *       result in a panic being raised. Respectively, if a
        *       client application wants to revert the state of a 
        *       node from non-empty to empty, all children that may
        *       have been created will be removed.
        *
        * @param TBool Define/undefine this node to be empty
		* @return void 
        */
        IMPORT_C void SetEmptyNode( TBool aIsEmptyNode );
        
        /**
        * Report whether or not this is a leaf element
		* @return TBool Is this a leaf element
        */
        IMPORT_C TBool IsLeafNode() const;
        
        /**
        * Report whether or not this element is empty
		* @return TBool Is this an empty element
        */
        IMPORT_C TBool IsEmptyNode() const;
        
        /**
        * Return the contents of a leaf element
		* @return TPtrC8 Contents of the leaf element
        */
        IMPORT_C TPtrC8 LeafNodeContent() const;
        
        /**
        * Return the contents of a leaf element escape-coded, if needed
		* @return TPtrC8 Contents of the leaf element
        */
        IMPORT_C HBufC8* EscapeLeafNodeContentLC() const;

        /**
        * Returns the contents of a single element without any brackets
        * and pushes the buffer onto the CleanupStack.
        *
        * For instance:
        *
        * 'element attribute1 = "value" attribute2 = "another value"'
        *
		* @return HBufC8* 
        */
        IMPORT_C HBufC8* ElementDataLC() const;
        
        /**
        * Return the parent node of this element
        *
		* @return CXdmDocumentNode* 
        */
        IMPORT_C CXdmDocumentNode* Parent() const;
        
        /**
        * Name of this element
		* @return TPtrC8 Name of this element
        */
        IMPORT_C TPtrC NodeName() const;

        /**
        * Name of this element
		* @return TPtrC8 Name of this element
        */
        IMPORT_C HBufC8* EightBitNodeNameLC() const;
        
        /**
        * (Re-)Set the name of this element
		* @param TDesC& Name of this element
        * @return void
        */
        IMPORT_C void SetNameL( const TDesC& aNodeName );
        
        /**
        * (Re-)Set the name of this element
		* @param TDesC& Name of this element
        * @return void
        */
        IMPORT_C void SetNameL( const TDesC8& aNodeName );
        
        /**
        * Returns the number of child & lealf elements
        *
        * @return TInt Number of descendants
        */
        IMPORT_C TInt NodeCount() const;

        /**
        * Returns the number of attributes of this element
        *
		* @return TInt The number of attributes 
        */
        IMPORT_C TInt AttributeCount() const;

        /**
        * Returns a child node specified by its position among the
        * descendants of an element
        *
		* @return CXdmDocument* 
        */
        IMPORT_C CXdmDocumentNode* ChileNode( TInt aPosition ) const;

        /**
        * Reference an attribute of this element specified by the position
        * of the attribute
        *
		* @return CXdmDocument* 
        */
        IMPORT_C CXdmNodeAttribute* Attribute( TInt aPosition ) const;
        
        /**
        * Reference an attribute of this element specified by the position
        * of the attribute
        *
		* @return CXdmDocument* 
        */
        IMPORT_C CXdmNodeAttribute* Attribute( const TDesC& aAttributeName ) const;
        
        /**
        * Search for nodes which match the specified name
        *
        * @param TDesC& The element name to search for
        * @param RPointerArray<CXdmDocumentNode> Results, if any
		* @return TInt Error
        */
        IMPORT_C TInt Find( const TDesC& aNodeName,
                            RPointerArray<CXdmDocumentNode>& aResultArray ) const;
                   
        /**
        * Search for nodes which match the specified node
        *
        * @param CXdmDocumentNode& The node to compare to
		* @return TInt KErrNotFound if no matching nodes are found
        */
        IMPORT_C TInt Find( const CXdmDocumentNode& aTargetNode,
                            RPointerArray<CXdmDocumentNode>& aResultArray ) const;
                   
        /**
        * Search for nodes which match the specified name & attributes
        *
        * @param TDesC& The element name to search for
        * @param RPointerArray<CXdmDocumentNode>& Results
        * @param RPointerArray<SXdmAttribute16>& Attributes to search for
		* @return TInt KErrNotFound if no matching nodes are found
        */
        IMPORT_C TInt Find( const TDesC& aNodeName,
                            RPointerArray<CXdmDocumentNode>& aResultArray,
                            const RPointerArray<SXdmAttribute16>& aAttributeArray ) const;


        /**
        * The type of this element: an element or an attribute
		* @return TXdmElementType* 
        */
        IMPORT_C TBool Match( const CXdmDocumentNode& aAnotherNode ) const;
                              
        /**
        * The type of this element: an element or an attribute
		* @return TXdmElementType* 
        */
        IMPORT_C TBool Match( const TDesC& aNodeName,
                              const RPointerArray<SXdmAttribute8>& aAttributeArray ) const;
        
        /**
        * Comparison operator
        *
		* @return TBool Are the contents of this object identical to
		*               the ones in the parameter
        */
        IMPORT_C TBool operator==( const CXdmDocumentNode& aNode ) const;
        
        /**
        * The type of this element: an element or an attribute
		* @return TXdmElementType* 
        */                    
        IMPORT_C TBool HasAttribute( const TDesC& aAttributeName ) const;
                              
        /**
        * The type of this element: an element or an attribute
		* @return TXdmElementType* 
        */
        IMPORT_C virtual TXdmElementType ElementType() const;
        
        /**
        * Remove a child node
        * @param CXdmDocumentNode The element to be removed
		* @return void 
        */
        IMPORT_C void RemoveChileNodeL( CXdmDocumentNode* aChileNode );
        
        /**
        * Append a child node
        * @param CXdmDocumentNode The element to be appended
		* @return void 
        */
        IMPORT_C void AppendChileNodeL( CXdmDocumentNode* aChileNode );
        
        /**
        * Insert a child node to the given position
        * @param TInt The position of the new element
        * @param CXdmDocumentNode The element to be inserted
		* @return void 
        */
        IMPORT_C void InsertChileNodeL( TInt aIndex, CXdmDocumentNode* aChileNode );
        
        /**
        * Replace a child node
        * @param CXdmDocumentNode The new node
        * @param CXdmDocumentNode The node to be replaced
		* @return void 
        */
        IMPORT_C void ReplaceChileNodeL( CXdmDocumentNode* aNewNode,
                                         CXdmDocumentNode* aTargetNode );
                                            
        /**
        * Destructor.
        */
        IMPORT_C ~CXdmDocumentNode();
    
    public:
    
        /**
        * Print the contents of this node to the log file
        *
		* @return void 
        */
        IMPORT_C void Print();
        
        /**
        * Return the next node in the list
		* @return CXdmDocumentNode* 
        */
        IMPORT_C CXdmDocumentNode* NextNode() const;
        
        /**
        * Set the next node in the list
        * @param The next node in the list
		* @return void 
        */
        IMPORT_C void SetNextNode( CXdmDocumentNode* aNextNode );
    
    public:
        
        /**
        * Check whether an element has the specified attribute
        *
        * @param CXdmNodeAttribute& The specified attribute
		* @return TBool Does the element have the specified attribute
        */
        TBool HasAttribute( const CXdmNodeAttribute& aNode ) const;
        
        /**
        * Check whether an element has the specified attribute
        *
        * @param SXdmAttribute8& The specified attribute
		* @return TBool Does the element have the specified attribute
        */
        TBool HasAttributeL( const SXdmAttribute8& aAttribute ) const;
        
        /**
        * Check whether an element has the specified attribute
        *
        * @param SXdmAttribute16& The specified attribute
		* @return TBool Does the element have the specified attribute
        */
        TBool HasAttribute( const SXdmAttribute16& aAttribute ) const;
        
    protected: // New functions
        
        /**
        * Escape-code the specified string
        *
        * @param TDesC8& The descriptor to escape-code
		* @return HBufC8* Escape-coded string
        */
        HBufC8* EscapeDescLC( const TDesC8& aDescriptor ) const;

    protected:
		                           
        /**
        * C++ constructor, for derived classes
        *
        * @param CXdmEngine* Reference to the engine
        * @param MXdmNodeFactory* Reference to the node factory
        * @param CXdmDocumentNode* The parent node of this element
        */
		IMPORT_C CXdmDocumentNode( CXdmEngine& aXdmEngine,
		                           MXdmNodeFactory& aNodeFactory,
		                           CXdmDocumentNode* aParentNode );
		
		/**
        * CC++ constructor, for derived classes
        *
        * @param CXdmEngine* Reference to the engine
        * @param MXdmNodeFactory* Reference to the node factory
        */
        IMPORT_C CXdmDocumentNode( CXdmEngine& aXdmEngine,
                                   MXdmNodeFactory& aNodeFactory );

        /**
        * C++ constructor, for derived classes
        *
        * @param CXdmEngine* Is this a leaf element
        * @param CXdmEngine* Reference to the engine
        * @param MXdmNodeFactory* Reference to the node factory
        * @param CXdmDocumentNode* The parent node of this element
        */
		IMPORT_C CXdmDocumentNode( const TBool aLeafNode,
		                           CXdmEngine& aXdmEngine,
		                           MXdmNodeFactory& aNodeFactory,
                                   CXdmDocumentNode* aParentNode );
       
        /**
        * Symbian OS constructor for derived classes.
        *
        * @param TDesC8& Name of this element
        * @return void
        */
		IMPORT_C void BaseConstructL( const TDesC& aNodeName );
		
		/**
        * Copy constructor
        *
        * @param CXdmDocumentNode& The element to copy
        * @param CXdmDocumentNode& The parent of this element
        * @return void
        */
		IMPORT_C virtual void CopyConstructL( const CXdmDocumentNode& aAnotherNode,
		                                      const CXdmDocumentNode& aParentNode );
    
    private:
        
        /**
        * Search for nodes which match the specified name
        *
        * @param TDesC& The element name to search for
        * @param RPointerArray<CXdmDocumentNode> Results, if any
		* @return TInt KErrNotFound if no matches were found,
		*         number of matching elements otherwise
        */
        TInt DoFind( const TDesC& aNodeName,
                     RPointerArray<CXdmDocumentNode>& aResultArray ) const;
        
        /**
        * Search for nodes which match the specified node
        *
        * @param CXdmDocumentNode& The node to compare to
		* @return TInt KErrNotFound if no matching nodes are found
        */             
        TInt DoFind( const CXdmDocumentNode& aTargetNode,
                     RPointerArray<CXdmDocumentNode>& aResultArray ) const;
        
        /**
        * Search for nodes which match the specified name & attributes
        *
        * @param TDesC& The element name to search for
        * @param RPointerArray<CXdmDocumentNode>& Results
        * @param RPointerArray<SXdmAttribute16>& Attributes to search for
		* @return TInt KErrNotFound if no matching nodes are found
        */              
        TInt DoFind( const TDesC& aNodeName,
                     RPointerArray<CXdmDocumentNode>& aResultArray,
                     const RPointerArray<SXdmAttribute16>& aAttributeArray );
                            
        /**
        * Copy the child nodes of another element.
        *
        * @param CXdmDocumentNode& The element the children of which should be copied
        * @return void
        */
        void ConstructChildrenL( const CXdmDocumentNode& aAnotherNode );
        
        /**
        * Copy the attributes of another element.
        *
        * @param CXdmDocumentNode& The element the attributes of which should be copied
        * @return void
        */
        void ConstructAttributesL( const CXdmDocumentNode& aAnotherNode );
        
        /**
        * Remove namespace attributes from this element. Namespaces must 
        * not be used when comparing two elements, which is why these 
        * attributes need to be removed before the comparison.
        *
        * @return void
        */
        void RemoveNamespaceAttributes();
    
    protected:
        
        TInt                                        iPosition;
        TBool                                       iLeafNode;
        TBool                                       iEmptyNode;
        HBufC*                                      iNodeName;
        HBufC8*                                     iLeafContent;
        CXdmEngine&                                 iXdmEngine;
        MXdmNodeFactory&                            iNodeFactory;
        friend class                                CXdmDocument;
        CXdmDocumentNode*                           iNextNode;
        CXdmDocumentNode*                           iParentNode;
        RPointerArray<CXdmDocumentNode>             iChildren;
        RPointerArray<CXdmNodeAttribute>            iAttributes;  
    };

#endif      //__XDMDOCUMENTNODE__
            
// End of File
