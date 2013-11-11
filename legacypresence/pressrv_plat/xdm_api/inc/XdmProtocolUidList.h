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
* Description:   XDM protocols
*
*/




#ifndef __XDMPROTOCOLUIDLIST__
#define __XDMPROTOCOLUIDLIST__

#include <e32base.h>

//Constants
const TInt KXcapProtocol                = 0x10207423;
const TInt KLocalProtocol               = 0x10207424;

// Application usage interface Uid
const TInt KXdmXcapAppUsageInterfaceUid     = 0x10207417;

// Application usage Uids
const TInt KXdmCapabilityUsageUid         = 0x10207418;
const TInt KXdmDirectoryUsageUid          = 0x10207419;
const TInt KXdmTestAppUsageUid            = 0x1020741A;
const TInt KXdmPocGroupUsageUid           = 0x1020741B;
const TInt KXdmPocUserAccessUsageUid      = 0x1020741C;
const TInt KXdmSharedXDMUsageUid          = 0x1020741D;
const TInt KXdmRlsServicesUsageUid        = 0x1020741E;
const TInt KXdmResourceListsUsageUid      = 0x10275080;
const TInt KXdmIetfCommonPolicyUsageUid   = 0x10275081;
const TInt KXdmOmaCommonPolicyUsageUid    = 0x10275082;
const TInt KXdmIetfPresRulesUsageUid      = 0x10275083;
const TInt KXdmOmaPresRulesUsageUid       = 0x102750AB;

#endif      //__XDMPROTOCOLUIDLIST__
            
// End of File
