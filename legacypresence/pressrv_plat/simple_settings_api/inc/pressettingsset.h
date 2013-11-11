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
* Description:    This class represents the data of a presence settings set.
*               PresSettingsAPI deals with the Sets of all
*               presence settings.
*
*/




#ifndef __PRESSETTINGSSET__
#define __PRESSETTINGSSET__

//includes
#include <simpleenginedomaincrkeys.h>

// FOR INTERNAL USE    
// constants
const TInt KPresSetNameMaxLength = 100;
const TInt KContentSrvrAddMaxLength = 255;
const TInt KPresDomainSyntxMaxLength = 255;
const TInt KPresProviderIDMaxLength = 255;
const TInt KPresNormalListLength = 10;

const TInt KPresIntInit = 0;
_LIT( KPresStrInit, "None"); 

/**
* This is the class representing a settings set data.
**/
class TPresSettingsSet
    {
    public:
        
        /**
        * C++ constructor
        *
        * @param none
        * @return none
        */
        IMPORT_C TPresSettingsSet();
        
    public: // data
    
        /**
        * Data members representing a settings set.
        */
        TBuf<KPresSetNameMaxLength> iSetName;
        TInt iSipProfile;
        TInt iXDMSetting;
        TInt iObjectSize;
        TBuf<KContentSrvrAddMaxLength> iContSrvrAdd;
        TInt iStatusOfProps;
        TInt iPublicationInt;
        TInt iMaxSubscriptions;
        TInt iMaxContactsInList;
        TBuf<KPresDomainSyntxMaxLength> iDomainSyntax;
        TBuf<KPresProviderIDMaxLength> iProviderID;
        
    };

#endif      //__PRESSETTINGSSET__
            
// End of File