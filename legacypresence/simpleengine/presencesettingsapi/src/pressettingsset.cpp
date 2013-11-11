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




#include "pressettingsset.h"

// -----------------------------------------------------------------------------
// TPresSettingsSet::TPresSettingsSet( )
// -----------------------------------------------------------------------------
//
EXPORT_C TPresSettingsSet::TPresSettingsSet()
    {
    iSetName = KNullDesC;
    iSipProfile = KErrNotFound;
    iXDMSetting = KErrNotFound;
    iObjectSize = KErrNotFound;
    iContSrvrAdd.Copy(KNullDesC);
    iStatusOfProps = NULL; // All settings are open
    iPublicationInt = KErrNotFound;
    iMaxSubscriptions = KErrNotFound;
    iMaxContactsInList = KErrNotFound;
    iDomainSyntax = KNullDesC;
    iProviderID = KNullDesC;
    }
        
// End of File
