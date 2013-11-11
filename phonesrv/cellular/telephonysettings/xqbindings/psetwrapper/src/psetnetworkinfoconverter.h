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

#ifndef PSETNETWORKINFOCONVERTER_H
#define PSETNETWORKINFOCONVERTER_H

#include <psetnetworkwrapper.h>
#include <mpsetnetworkselect.h>

class NetworkInfoConverter
{
public:
    
    static MPsetNetworkSelect::TNetworkInfo ConvertToSymbianType(
        const PSetNetworkWrapper::NetworkInfo &networkInfo);
    
    static MPsetNetworkSelect::TNetworkId ConvertToSymbianType(
        const PSetNetworkWrapper::NetworkId &networkId);
    
    static MPsetNetworkSelect::TNetworkStatus ConvertToSymbianType(
        const PSetNetworkWrapper::NetworkStatus &networkStatus);
    
    static MPsetNetworkSelect::TSelectMode ConvertToSymbianType(
        const PSetNetworkWrapper::NetworkSelectionMode &selectionMode);
    
    static MPsetNetworkSelect::TNetworkAccess ConvertToSymbianType(
        const PSetNetworkWrapper::NetworkAccessType &accessType);
    
    static PSetNetworkWrapper::NetworkSelectionMode ConvertToQtType(
        const MPsetNetworkSelect::TSelectMode &selectMode);
};

#endif // PSETNETWORKINFOCONVERTER_H
