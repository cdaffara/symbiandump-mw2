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
* Description:  TOMASuplSettingsMap is a utility class to determine the setting ID and name
*
*/




#ifndef T_TOMASUPLSETTINGSMAP_H
#define T_TOMASUPLSETTINGSMAP_H


#include <e32cmn.h>


_LIT8(KRootFull,"Supl");
_LIT8(KSlpAddrFull,"Supl/SLP") ;
_LIT8(KIapIdFull,"Supl/PrefConRef") ;
_LIT8(KProviderIdFull,"Supl/ProviderId") ;
_LIT8(KNameFull,"Supl/Name") ;
_LIT8(KToConRefFull,"Supl/ToConRef") ;
_LIT8(KConRefFull,"Supl/ConRef") ;
_LIT8(KAddrTypeFull,"Supl/AddrType") ;

_LIT8(KRootNode,"Supl");
_LIT8(KSlpAddr,"SLP") ;
_LIT8(KIapIdName,"PrefConRef") ;
_LIT8(KProviderId,"ProviderId") ;
_LIT8(KName,"Name") ;
_LIT8(KToConRef,"ToConRef") ;
_LIT8(KConRef,"ConRef") ;
_LIT8(KAddrType,"AddrType") ;

_LIT(KSuplFile,"supldm.txt");
_LIT(KSlpFile,"slpdm.txt");
_LIT(KIAPIDFile,"iapiddm.txt");
_LIT(KProviderIdFile,"providdm.txt");
_LIT(KNameFile,"namedm.txt");
_LIT(KToConRefFile,"toconrefdm.txt");
_LIT(KConRefFile,"conrefdm.txt");
_LIT(KAddrTypeFile,"addrtypedm.txt");


static const TUint32 KSizeOfSettingId = 0x8;


/**
* TSettingMap is a setting ID and setting name value pair
* @since S60 v3.1
*/
class TSettingMap
    {
public:
    TPtrC8 iSettingName;
    };

typedef RArray<TSettingMap> RChildList;


/**
* TOMASuplSettingsMap is a utility class to determine the setting ID and name 
* @since S60 v3.1
*/
class TOMASuplSettingsMap
    {
public: // enums

    enum TSuplSettingType
        {
        ESlp,
        EPrefConRef,
        EProviderId,
        EName,
        EToConRef,
        EConRef,
        EAddrType,
        ENone	
        };

public:

    static TInt Map(const TDesC8& aSettingName, TSuplSettingType& suplSettingType);
    
    static TInt Get(const TDesC8& aUri, RChildList& aList, RArray<TPtrC8>& aChildren);
    
    static TInt FileName(const TDesC8& aUri, const TDesC*& aFileName);

private:
    static void GetL(const TDesC8& aUri, RChildList& aList, RArray<TPtrC8>& aChildren);
    };


#endif  // T_TOMASUPLSETTINGSMAP_H
