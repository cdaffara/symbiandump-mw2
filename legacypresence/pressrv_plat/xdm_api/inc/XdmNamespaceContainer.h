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
* Description:   XDM namespace
*
*/




#ifndef __XDMNAMESPACECONTAINER__
#define __XDMNAMESPACECONTAINER__

//CLASS DECLARATION
class MXdmNamespaceContainer
    {
    public:
        
        /**
        * Return the number of namespace declarations
        *
        * @return TInt Namespace declaration count
        */
        virtual TInt Count() const = 0;
        
        /**
        * Append a new namespace to the document
        *
        * @param TDesC8& URI of the namespace
        * @param TDesC8& Prefix of the namespace
        */
        virtual void AppendNamespaceL( const TDesC8& aUri, const TDesC8& aPrefix ) = 0;
        
        /**
        * Remove a namespace from the document
        *
        * @param TDesC8& URI of the namespace
        * @param TDesC8& Prefix of the namespace
        */
        virtual void RemoveNamespace( const TDesC8& aUri ) = 0;
        
        /**
        * Fetch the URI that corresponds to the prefix in the parameter
        *
        * @param TDesC8 Prefix of the namespace
        * @return TPtrC URI that corresponds to the prefix in the parameter
        */
        virtual TPtrC8 Uri( const TDesC8& aPrefix ) const = 0;
        
        /**
        * Fetch the aIndex-th URI
        *
        * @param TInt The index of the desired URI string
        * @return TPtrC URI string
        */
        virtual TPtrC8 Uri( TInt aIndex ) const = 0;
        
        /**
        * Fetch the aIndex-th prefix
        *
        * @param TDesC8 The index of the desired prefix string
        * @return TPtrC Prefix string
        */
        virtual TPtrC8 Prefix( TInt aIndex ) const = 0;
        
        /**
        * Resets namespaces from the document
        *
        * @return void
        */
        virtual void ResetNamespaces( ) = 0;
    };

#endif      //__XDMNAMESPACECONTAINER__
            
// End of File
