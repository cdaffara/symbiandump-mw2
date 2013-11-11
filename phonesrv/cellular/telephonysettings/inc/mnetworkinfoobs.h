/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observes completions of network info and change requests.
*
*
*/


#ifndef     __MNETWORKINFOOBS_H
#define     __MNETWORKINFOOBS_H

//  INCLUDES
#ifndef     __E32DEF_H__
#include    <e32def.h>
#endif

#ifndef     __MNETWORKSELECT_H
#include    "mnetworkselect.h" 
#endif

//  DATA TYPES  
typedef CArrayFixFlat<MNetworkSelect::TNetworkInfo> CNetworkInfoArray;

//  CLASS DEFINITIONS 

class   MNetworkInfoObserver
    {
    public:            
        virtual void HandleNetworkInfoReceivedL( const CNetworkInfoArray* aInfoArray, const TInt aResult)=0;
        virtual void HandleCurrentNetworkInfoL( const MNetworkSelect::TCurrentNetworkInfo& aCurrentInfo, const TInt aResult)=0;
        virtual void HandleNetworkChangedL( const MNetworkSelect::TNetworkInfo& aCurrentInfo, const MNetworkSelect::TCurrentNetworkStatus aStatus, const TInt aResult)=0;    
    };

#endif // __MNETWORKINFOOBS_H

// end of file
