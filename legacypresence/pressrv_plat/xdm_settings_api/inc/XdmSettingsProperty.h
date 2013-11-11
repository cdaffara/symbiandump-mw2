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
* Description:   XDM Settings property
*
*/




#ifndef __XDMSETTINGSPROPERTY__
#define __XDMSETTINGSPROPERTY__

#include <e32base.h>

//CLASS DECLARATION
NONSHARABLE_CLASS ( CXdmSettingsProperty ) : public CBase
    {
    public:
        
        /**
        * Return the name of each settings collection
        * stored in the repository
        *
        * @return CDesCArray* Names of all settings collections
        */
        static CXdmSettingsProperty* NewL( const TDesC& aPropertyValue,
                                           TXdmSettingsProperty aPropertyName );
        
        /**
        * Return the name of each settings collection
        * stored in the repository
        *
        * @return CDesCArray* Names of all settings collections
        */
        IMPORT_C TXdmSettingsProperty PropertyName() const;

        /**
        * Return the name of each settings collection
        * stored in the repository
        *
        * @return CDesCArray* Names of all settings collections
        */
        IMPORT_C TPtrC PropertyValue() const;
        
        /**
        * Return the name of each settings collection
        * stored in the repository
        *
        * @return CDesCArray* Names of all settings collections
        */
        IMPORT_C virtual ~CXdmSettingsProperty();
    
    private:
        
        /**
        * Return the name of each settings collection
        * stored in the repository
        *
        * @return CDesCArray* Names of all settings collections
        */
        CXdmSettingsProperty( TXdmSettingsProperty aPropertyName );
        
        /**
        * Return the name of each settings collection
        * stored in the repository
        *
        * @return CDesCArray* Names of all settings collections
        */
        void ConstructL( const TDesC& aPropertyValue );

    private:   //Data
        
        HBufC*                              iPropertyValue;
        TXdmSettingsProperty                iPropertyName;
    };

#endif      //__XDMSETTINGSPROPERTY__
            
// End of File
