/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/

#include "ut_psetnetworkinfoconverter.h"
#include "psetnetworkinfoconverter.h"
#include "qtestmains60.h"


/*!
  UT_PSetNetworkInfoConverter::UT_PSetNetworkInfoConverter
 */
UT_PSetNetworkInfoConverter::UT_PSetNetworkInfoConverter() 
{
}

/*!
  UT_PSetNetworkInfoConverter::~UT_PSetNetworkInfoConverter
 */
UT_PSetNetworkInfoConverter::~UT_PSetNetworkInfoConverter()
{

}

/*!
  UT_PSetNetworkInfoConverter::init
 */
void UT_PSetNetworkInfoConverter::init()
{
    initialize();
}

/*!
  UT_PSetNetworkInfoConverter::cleanup
 */
void UT_PSetNetworkInfoConverter::cleanup()
{
    reset();
}

/*!
  UT_PSetNetworkInfoConverter::t_convertToSymbianTypeNetworkInfo
 */
void UT_PSetNetworkInfoConverter::t_convertToSymbianTypeNetworkInfo()
{
    _LIT(KCountryCode, "1234");         // country code max length is 4
    _LIT(KNetworkCode, "12345678");     // network code max length is 8
    PSetNetworkWrapper::NetworkId networkId = 
        { 
            QString::fromUtf16(KCountryCode().Ptr(), KCountryCode().Length()),
            QString::fromUtf16(KNetworkCode().Ptr(), KNetworkCode().Length()),
        };
    
    _LIT(KShortName, "shortNameMaxLength20");
    _LIT(KLongName, "longNameMaxLengthIs30LikeThis_");
    PSetNetworkWrapper::NetworkInfo info =
        {
            networkId,
            PSetNetworkWrapper::StatusUnknown,
            PSetNetworkWrapper::SelectionModeManual,
            PSetNetworkWrapper::AccessTypeGsm,
            QString::fromUtf16(KShortName().Ptr(), KShortName().Length()),
            QString::fromUtf16(KLongName().Ptr(), KLongName().Length()),
        };
    
    MPsetNetworkSelect::TNetworkInfo convertedInfo =
        NetworkInfoConverter::ConvertToSymbianType(info);
    
    QVERIFY(convertedInfo.iId.iCountryCode == KCountryCode());
    QVERIFY(convertedInfo.iId.iNetworkCode == KNetworkCode());
    QVERIFY(convertedInfo.iStatus == MPsetNetworkSelect::ENetStatUnknown);
    QVERIFY(convertedInfo.iMode == MPsetNetworkSelect::ENetSelectModeManual);
    QVERIFY(convertedInfo.iAccess == MPsetNetworkSelect::ENetNetworkGSM);
    QVERIFY(convertedInfo.iShortName == KShortName());
    QVERIFY(convertedInfo.iLongName == KLongName());
}

/*!
  UT_PSetNetworkInfoConverter::t_convertToSymbianTypeNetworkId
 */
void UT_PSetNetworkInfoConverter::t_convertToSymbianTypeNetworkId()
{
    _LIT(KCountryCode, "1234");
    _LIT(KNetworkCode, "12345678");
    PSetNetworkWrapper::NetworkId networkId = 
        { 
            QString::fromUtf16(KCountryCode().Ptr(), KCountryCode().Length()),
            QString::fromUtf16(KNetworkCode().Ptr(), KNetworkCode().Length()),
        };
    
    MPsetNetworkSelect::TNetworkId  convertedType =
        NetworkInfoConverter::ConvertToSymbianType(networkId);
    QVERIFY(convertedType.iCountryCode == KCountryCode());
    QVERIFY(convertedType.iNetworkCode == KNetworkCode());
}

/*!
  UT_PSetNetworkInfoConverter::t_convertToSymbianTypeNetworkStatus
 */
void UT_PSetNetworkInfoConverter::t_convertToSymbianTypeNetworkStatus()
{
    MPsetNetworkSelect::TNetworkStatus convertedType =
        NetworkInfoConverter::ConvertToSymbianType(
            PSetNetworkWrapper::StatusUnknown);
    QVERIFY(MPsetNetworkSelect::ENetStatUnknown == convertedType);
    
    convertedType = NetworkInfoConverter::ConvertToSymbianType(
        PSetNetworkWrapper::StatusAvailable);
    QVERIFY(MPsetNetworkSelect::ENetStatAvailable == convertedType);
    
    convertedType = NetworkInfoConverter::ConvertToSymbianType(
        PSetNetworkWrapper::StatusCurrent);
    QVERIFY(MPsetNetworkSelect::ENetStatCurrent == convertedType);
    
    convertedType = NetworkInfoConverter::ConvertToSymbianType(
        PSetNetworkWrapper::StatusForbidden);
    QVERIFY(MPsetNetworkSelect::ENetStatForbidden == convertedType);
}

/*!
  UT_PSetNetworkInfoConverter::t_convertToSymbianTypeNetworkSelectionMode
 */
void UT_PSetNetworkInfoConverter::t_convertToSymbianTypeNetworkSelectionMode()
{
    MPsetNetworkSelect::TSelectMode convertedType =
        NetworkInfoConverter::ConvertToSymbianType(
            PSetNetworkWrapper::SelectionModeAutomatic);
    QVERIFY(MPsetNetworkSelect::ENetSelectModeAutomatic == convertedType);
    
    convertedType = NetworkInfoConverter::ConvertToSymbianType(
        PSetNetworkWrapper::SelectionModeManual);
    QVERIFY(MPsetNetworkSelect::ENetSelectModeManual == convertedType);
}

/*!
  UT_PSetNetworkInfoConverter::t_convertToSymbianTypeNetworkAccessType
 */
void UT_PSetNetworkInfoConverter::t_convertToSymbianTypeNetworkAccessType()
{
    MPsetNetworkSelect::TNetworkAccess convertedType =
        NetworkInfoConverter::ConvertToSymbianType(
            PSetNetworkWrapper::AccessTypeGsm);
    QVERIFY(MPsetNetworkSelect::ENetNetworkGSM == convertedType);
    
    convertedType = NetworkInfoConverter::ConvertToSymbianType(
        PSetNetworkWrapper::AccessTypeWcdma);
    QVERIFY(MPsetNetworkSelect::ENetNetworkWCDMA == convertedType);
}

/*!
  UT_PSetNetworkInfoConverter::t_convertToQtTypeNetworkSelectionMode
 */
void UT_PSetNetworkInfoConverter::t_convertToQtTypeNetworkSelectionMode()
{
    PSetNetworkWrapper::NetworkSelectionMode convertedType =
        NetworkInfoConverter::ConvertToQtType(
            MPsetNetworkSelect::ENetSelectModeAutomatic);
    QVERIFY(PSetNetworkWrapper::SelectionModeAutomatic == convertedType);
    
    convertedType = NetworkInfoConverter::ConvertToQtType(
        MPsetNetworkSelect::ENetSelectModeManual);
    QVERIFY(PSetNetworkWrapper::SelectionModeManual == convertedType);
}

QTEST_MAIN_S60(UT_PSetNetworkInfoConverter)
