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

#include "psetnetworkinfoconverter.h"

/*!
    \class NetworkInfoConverter
    \brief Offers converting services between Symbian and Qt data types.
*/

/*!
  NetworkInfoConverter::ConvertToSymbianType
 */
MPsetNetworkSelect::TNetworkInfo NetworkInfoConverter::ConvertToSymbianType(
        const PSetNetworkWrapper::NetworkInfo &networkInfo)
{
    MPsetNetworkSelect::TNetworkInfo convertedType;
    convertedType.iId = ConvertToSymbianType(networkInfo.m_id);
    convertedType.iStatus = ConvertToSymbianType(networkInfo.m_status);
    convertedType.iMode = ConvertToSymbianType(networkInfo.m_mode);
    convertedType.iAccess = ConvertToSymbianType(networkInfo.m_access);
    convertedType.iShortName.Copy(networkInfo.m_shortName.utf16());
    convertedType.iLongName.Copy(networkInfo.m_longName.utf16());
    return convertedType;
}

/*!
  NetworkInfoConverter::ConvertToSymbianType
 */
MPsetNetworkSelect::TNetworkId NetworkInfoConverter::ConvertToSymbianType(
        const PSetNetworkWrapper::NetworkId &networkId)
{
    MPsetNetworkSelect::TNetworkId convertedType;
    convertedType.iCountryCode.Copy(networkId.m_countryCode.utf16());
    convertedType.iNetworkCode.Copy(networkId.m_networkCode.utf16());
    return convertedType;
}

/*!
  NetworkInfoConverter::ConvertToSymbianType
 */
MPsetNetworkSelect::TNetworkStatus NetworkInfoConverter::ConvertToSymbianType(
        const PSetNetworkWrapper::NetworkStatus &networkStatus)
{
    MPsetNetworkSelect::TNetworkStatus convertedType =
        MPsetNetworkSelect::ENetStatUnknown;
    switch (networkStatus)
    {
        case PSetNetworkWrapper::StatusUnknown:
            convertedType = MPsetNetworkSelect::ENetStatUnknown;
            break;
        case PSetNetworkWrapper::StatusAvailable:
            convertedType = MPsetNetworkSelect::ENetStatAvailable;
            break;
        case PSetNetworkWrapper::StatusCurrent:
            convertedType = MPsetNetworkSelect::ENetStatCurrent;
            break;
        case PSetNetworkWrapper::StatusForbidden:
            convertedType = MPsetNetworkSelect::ENetStatForbidden;
            break;
        default:
            Q_ASSERT_X(false, "switch default", "unknown enumerator");
            break;
    }
    
    return convertedType;
}

/*!
  NetworkInfoConverter::ConvertToSymbianType
 */
MPsetNetworkSelect::TSelectMode NetworkInfoConverter::ConvertToSymbianType(
        const PSetNetworkWrapper::NetworkSelectionMode &selectionMode)
{
    MPsetNetworkSelect::TSelectMode convertedType =
        MPsetNetworkSelect::ENetSelectModeAutomatic;
    switch (selectionMode)
    {
        case PSetNetworkWrapper::SelectionModeAutomatic:
            convertedType = MPsetNetworkSelect::ENetSelectModeAutomatic;
            break;
        case PSetNetworkWrapper::SelectionModeManual:
            convertedType = MPsetNetworkSelect::ENetSelectModeManual;
            break;
        default:
            Q_ASSERT_X(false, "switch default", "unknown enumerator");
            break;
    }
    
    return convertedType;
}

/*!
  NetworkInfoConverter::ConvertToSymbianType
 */
MPsetNetworkSelect::TNetworkAccess NetworkInfoConverter::ConvertToSymbianType(
        const PSetNetworkWrapper::NetworkAccessType &accessType)
{
    MPsetNetworkSelect::TNetworkAccess convertedType =
        MPsetNetworkSelect::ENetNetworkGSM;
    switch (accessType)
    {
        case PSetNetworkWrapper::AccessTypeGsm:
            convertedType = MPsetNetworkSelect::ENetNetworkGSM;
            break;
        case PSetNetworkWrapper::AccessTypeWcdma:
            convertedType = MPsetNetworkSelect::ENetNetworkWCDMA;
            break;
        default:
            Q_ASSERT_X(false, "switch default", "unknown enumerator");
            break;
    }
    
    return convertedType;
}

/*!
  NetworkInfoConverter::ConvertToQtType
 */
PSetNetworkWrapper::NetworkSelectionMode NetworkInfoConverter::ConvertToQtType(
    const MPsetNetworkSelect::TSelectMode &selectMode)
{
    PSetNetworkWrapper::NetworkSelectionMode convertedType =
        PSetNetworkWrapper::SelectionModeAutomatic;
    switch (selectMode)
    {
        case MPsetNetworkSelect::ENetSelectModeAutomatic:
            convertedType = PSetNetworkWrapper::SelectionModeAutomatic;
            break;
        case MPsetNetworkSelect::ENetSelectModeManual:
            convertedType = PSetNetworkWrapper::SelectionModeManual;
            break;
        default:
            Q_ASSERT_X(false, "switch default", "unknown enumerator");
            break;
    }
    
    return convertedType;
}
