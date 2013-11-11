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
* Description: Implementation of data class representing Cell 
* information in GSM/WCDMA PLMN.
*
*/



#include "lbtcellinfo.h"
#include "lbtlogger.h"


//------------------------------------------------------------
// CLbtCellInfo::NewL
//------------------------------------------------------------
//
CLbtCellInfo* CLbtCellInfo::NewL( RMmCustomAPI::TMmCellInfo& aCellInfo )
    {
    FUNC_ENTER("CLbtCellInfo::NewL");
    CLbtCellInfo* self = new ( ELeave ) CLbtCellInfo();
    CleanupStack::PushL( self );
    self->ConstructL( aCellInfo );
    CleanupStack::Pop();
    return self;
    }


//------------------------------------------------------------
// CLbtCellInfo::Begin
//------------------------------------------------------------
//
CLbtCellInfo::TCellInfoArrayIterator CLbtCellInfo::Begin()
    {
    FUNC_ENTER("CLbtCellInfo::Begin");
    if( iMode == RMmCustomAPI::TMmCellInfo::EGSM )
        {
        return TCellInfoArrayIterator( &iGsmNMR );
        }
    return TCellInfoArrayIterator( &iWcdmaNMR );
    }
    
//------------------------------------------------------------
// CLbtCellInfo::CellChangeCount
//------------------------------------------------------------
//    
TInt CLbtCellInfo::CellChangeCount()
    {
    FUNC_ENTER("CLbtCellInfo::CellChangeCount");
    if( iMode == RMmCustomAPI::TMmCellInfo::EWCDMA )
        {
        return iCellIdArray.Count();
        }
    return 0;    
    }
    
//------------------------------------------------------------
// CLbtCellInfo::AddNMRData
//------------------------------------------------------------
//
void CLbtCellInfo::AddNMRData(RMmCustomAPI::TMmCellInfo& aCellInfo )
    {
    FUNC_ENTER("CLbtCellInfo::AddNMRData");
    if( iMode == RMmCustomAPI::TMmCellInfo::EGSM )
        {
        // This is not used for Gsm
        }
    else if( iMode == RMmCustomAPI::TMmCellInfo::EWCDMA )   
        {
        if( aCellInfo.iWcdmaCellInfo.iNwkMeasureReport[0].iCellMeasuredResult[0].iCID != -1 )
            {
            if( !iCellIdArray.Count() ||
		         iCellIdArray.Find( aCellInfo.iWcdmaCellInfo.iNwkMeasureReport[0].iCellMeasuredResult[0].iCID ) == KErrNotFound )
                {
                TInt error = iCellIdArray.Append( aCellInfo.iWcdmaCellInfo.iNwkMeasureReport[0].iCellMeasuredResult[0].iCID );
                if( error != KErrNone )
                     {
                     LOG1("Failed to append cell id into the array:%d",error);
                     }
                }
            TWcdmaNMR wcdmaNMR;
            // Only current cell reading are considered for calculation.
            wcdmaNMR.iCarrierRSSI = aCellInfo.iWcdmaCellInfo.iNwkMeasureReport[0].iCarrierRSSI;
            wcdmaNMR.iCpichEcN0 = aCellInfo.iWcdmaCellInfo.iNwkMeasureReport[0].iCellMeasuredResult[0].iFddInfo.iCpichEcN0;
            wcdmaNMR.iCpichRscp = aCellInfo.iWcdmaCellInfo.iNwkMeasureReport[0].iCellMeasuredResult[0].iFddInfo.iCpichRscp;
            wcdmaNMR.iPathloss = aCellInfo.iWcdmaCellInfo.iNwkMeasureReport[0].iCellMeasuredResult[0].iFddInfo.iPathloss;
            
            TInt error = iWcdmaNMR.Append( wcdmaNMR );
            if( error != KErrNone )
                {
                LOG1("Failed to append WCDMA NMR info:%d",error);
                }
            }
        }
    }

//------------------------------------------------------------
// CLbtCellInfo::Find
//------------------------------------------------------------
//
TBool CLbtCellInfo::Find( TInt& pos, const TGsmNMR& nmr )
    {
    FUNC_ENTER("CLbtCellInfo::Find");
    pos = iGsmNMR.Find( nmr,  TIdentityRelation< TGsmNMR >( FindCellInfo ) );
    if( KErrNotFound == pos )
        {
        return EFalse;
        }
    return ETrue;
    }
    

//------------------------------------------------------------
// CLbtCellInfo::Count
//------------------------------------------------------------
//
TInt CLbtCellInfo::Count()
    {
    FUNC_ENTER("CLbtCellInfo::Count");
    if( iMode == RMmCustomAPI::TMmCellInfo::EGSM )
        {
        return iGsmNMR.Count();
        }
    return iWcdmaNMR.Count();
    }

//------------------------------------------------------------
// CLbtCellInfo::Reset
//------------------------------------------------------------
//
void CLbtCellInfo::Reset()
    {
    FUNC_ENTER("CLbtCellInfo::Reset");
    if( iMode == RMmCustomAPI::TMmCellInfo::EGSM )
        {
        iGsmNMR.Reset();
        }
    else if( iMode == RMmCustomAPI::TMmCellInfo::EWCDMA )
        {
        iWcdmaNMR.Reset();
        iCellIdArray.Reset();
        } 
    }

//------------------------------------------------------------
// CLbtCellInfo::FindCellInfo
//------------------------------------------------------------
//
TBool CLbtCellInfo::FindCellInfo( const TGsmNMR& src, const TGsmNMR& dst )
    {
    FUNC_ENTER("CLbtCellInfo::FindCellInfo");
    if( src.BSIC == dst.BSIC && src.ARFCN == dst.ARFCN )
        {
        return ETrue;
        }
    return EFalse;
    }


//------------------------------------------------------------
// CLbtCellInfo::TCellInfoArrayIterator::TCellInfoArrayIterator
//------------------------------------------------------------
//
CLbtCellInfo::TCellInfoArrayIterator::TCellInfoArrayIterator( const RArray< TGsmNMR >* aNMRList )
    {
    FUNC_ENTER("TCellInfoArrayIterator::TCellInfoArrayIterator-Gsm");
    iGsmNMRList = aNMRList;
    iIndex = 0;
    }

//------------------------------------------------------------
// CLbtCellInfo::TCellInfoArrayIterator::TCellInfoArrayIterator
//------------------------------------------------------------
//
CLbtCellInfo::TCellInfoArrayIterator::TCellInfoArrayIterator( const RArray< TWcdmaNMR >* aNMRList )
    {
    FUNC_ENTER("TCellInfoArrayIterator::TCellInfoArrayIterator-Wcdma");
    iWcdmaNMRList = aNMRList;
    iIndex = 0;
    }


//------------------------------------------------------------
// CLbtCellInfo::TCellInfoArrayIterator::Reset
//------------------------------------------------------------
//
void CLbtCellInfo::TCellInfoArrayIterator::Reset()
    {
    iIndex = 0;
    }
    
    
//------------------------------------------------------------
// CLbtCellInfo::TCellInfoArrayIterator::Next
//------------------------------------------------------------
//
TBool CLbtCellInfo::TCellInfoArrayIterator::Next( TGsmNMR& aNMR )
    {
    FUNC_ENTER("TCellInfoArrayIterator::Next");
    TInt count = iGsmNMRList->Count();
    while( count && iIndex < count )
        {
        aNMR = ( *iGsmNMRList )[ iIndex ];
        ++iIndex;
        return ETrue;
        }
    return EFalse;
    }

//------------------------------------------------------------
// CLbtCellInfo::TCellInfoArrayIterator::Next
//------------------------------------------------------------
//
TBool CLbtCellInfo::TCellInfoArrayIterator::Next( TWcdmaNMR& aNMR )
    {
    FUNC_ENTER("TCellInfoArrayIterator::Next-WCDMA");
    TInt count = iWcdmaNMRList->Count();
    while( count && iIndex < count )
        {
        aNMR = ( *iWcdmaNMRList )[ iIndex ];
        ++iIndex;
        return ETrue;
        }
    return EFalse;
    }

  
//------------------------------------------------------------
// CLbtCellInfo::TCellInfoArrayIterator::TCellInfoArrayIterator
//------------------------------------------------------------
//
 CLbtCellInfo::TCellInfoArrayIterator::TCellInfoArrayIterator()
    {
    
    }
    

//------------------------------------------------------------
// CLbtCellInfo::ConstructL
//------------------------------------------------------------
//
void CLbtCellInfo::ConstructL( RMmCustomAPI::TMmCellInfo& aCellInfo )
    {
    FUNC_ENTER("TCellInfoArrayIterator::ConstructL");
    iMode = aCellInfo.iMode;
    
    if( iMode == RMmCustomAPI::TMmCellInfo::EGSM )
        {
        iTA = aCellInfo.iGsmCellInfo.iTA;
        for( TInt i = 0; i < 15; i++ )
            {
            if( aCellInfo.iGsmCellInfo.iNmr[i].iARFCN != -1 && 
                aCellInfo.iGsmCellInfo.iNmr[i].iBSIC != -1 )
                {
                TGsmNMR nmr;
                nmr.BSIC = aCellInfo.iGsmCellInfo.iNmr[i].iBSIC;
                nmr.ARFCN = aCellInfo.iGsmCellInfo.iNmr[i].iARFCN;
                nmr.RxLEV = aCellInfo.iGsmCellInfo.iNmr[i].iRxLEV;
                iGsmNMR.AppendL( nmr );
                }
            else
                {
                continue;
                }
            }
        }
    else if( iMode == RMmCustomAPI::TMmCellInfo::EWCDMA )
        {
        AddNMRData( aCellInfo );
        }    
    }
