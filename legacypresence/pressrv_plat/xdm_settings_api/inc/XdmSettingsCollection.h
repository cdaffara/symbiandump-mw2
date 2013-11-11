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
* Description:   XDM Settings collection
*
*/




#ifndef __XDMSETTINGSCOLLECTION__
#define __XDMSETTINGSCOLLECTION__

#include <e32base.h>
#include "XdmSettingsApi.h"

class CXdmSettingsProperty;
  
//CLASS DECLARATION
class CXdmSettingsCollection : public CBase
    {
    public:
        
        /**
        * Return the name of each settings collection
        * stored in the repository
        *
        * @return CDesCArray* Names of all settings collections
        */
        IMPORT_C CXdmSettingsCollection();
        
        /**
        * Return the name of each settings collection
        * stored in the repository
        *
        * @return CDesCArray* Names of all settings collections
        */
        IMPORT_C void AppendL( const TDesC& aPropertyValue,
                               TXdmSettingsProperty aPropertyName );

        /**
        * Return the name of each settings collection
        * stored in the repository
        *
        * @return CDesCArray* Names of all settings collections
        */
        IMPORT_C void RemoveL( TXdmSettingsProperty aPropertyName );
        
        /**
        * Return the name of each settings collection
        * stored in the repository
        *
        * @return CDesCArray* Names of all settings collections
        */
        IMPORT_C TPtrC Property( TXdmSettingsProperty aProperty ) const;
        
        /**
        * Return the name of each settings collection
        * stored in the repository
        *
        * @return CDesCArray* Names of all settings collections
        */
        IMPORT_C const CXdmSettingsProperty& Property( TInt aIndex ) const;
        
        /**
        * Return the name of each settings collection
        * stored in the repository
        *
        * @return CDesCArray* Names of all settings collections
        */
        IMPORT_C TInt Count() const;
        
        /**
        * Return the name of each settings collection
        * stored in the repository
        *
        * @return CDesCArray* Names of all settings collections
        */
        IMPORT_C virtual ~CXdmSettingsCollection();
    
    private:
        
        /**
        * Return the name of each settings collection
        * stored in the repository
        *
        * @return CDesCArray* Names of all settings collections
        */
        CXdmSettingsProperty* Find( TXdmSettingsProperty aProperty ) const;
        
    private:   //Data
        
        RPointerArray<CXdmSettingsProperty>         iPropCollection;
    };

#endif      //__XDMSETTINGSCOLLECTION__
            
// End of File
