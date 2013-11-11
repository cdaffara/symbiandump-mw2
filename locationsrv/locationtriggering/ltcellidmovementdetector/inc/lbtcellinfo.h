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
* Description: Data class representing Cell information in GSM/WCDMA
* PLMN.
*
*/



#ifndef _LBTCELLINFO_H
#define _LBTCELLINFO_H


#include <e32base.h>
#include <rmmcustomapi.h>

#include "lbtcellidmvmtdetconsts.h"


/*
 * Container class to hold cell information
 */
class CLbtCellInfo : public CBase
    {

public:

    class TCellInfoArrayIterator;
    
    /**
     *
     */
    static CLbtCellInfo* NewL( RMmCustomAPI::TMmCellInfo& aCellInfo );
    
    /**
     *
     */
    RArray< TGsmNMR >& GetNMR()
        {
        return iGsmNMR;
        }
    
    /**
     *
     */
    RArray< TWcdmaNMR >& GetWcdmaNMR()
        {
        return iWcdmaNMR;
        }
    
    /**
     *
     */
    void AddNMRData(RMmCustomAPI::TMmCellInfo& aCellInfo );     
     
    /**
     *
     */
    TInt CellChangeCount(); 
    
    /**
     *
     */
    void Reset();      
    /**
     *
     */
    TInt GetTA()
        {
        return iTA;
        }
        
    /**
     *
     */
    ~CLbtCellInfo()
        {
        iGsmNMR.Close();
        iWcdmaNMR.Close();
        iCellIdArray.Close();
        }
        
    /**
     *
     */
    TCellInfoArrayIterator Begin();
    
    
    /**
     *
     */
    TBool Find( TInt& pos, const TGsmNMR& nmr );
    
    /**
     * Returns count of NMR values
     */
    TInt Count();
    
    
    /**
     *
     */
    static TBool FindCellInfo( const TGsmNMR& src, const TGsmNMR& dst );
    
    /**
     * ****** Iterator class ******
     */
    class TCellInfoArrayIterator
        {
        
    public:

        /**
         *
         */
        TCellInfoArrayIterator( const RArray< TGsmNMR >* aNMRList );
        
        /**
         *
         */
        TCellInfoArrayIterator( const RArray< TWcdmaNMR >* aNMRList );

        /**
         *
         */
        void Reset();

        /**
         *
         */
        TBool Next( TGsmNMR& aNMR );
        
        /**
         *
         */
        TBool Next( TWcdmaNMR& aNMR );
       
    private:
    
        /**
         *
         */
        TCellInfoArrayIterator();
    
        /**
         *
         */
        const RArray< TGsmNMR >*   iGsmNMRList;
        
        /**
         *
         */
        const RArray< TWcdmaNMR >*   iWcdmaNMRList;
    
        /**
         *
         */
        TInt                    iIndex;
        
        };
    
private:

    /**
     *
     */
    CLbtCellInfo()
        {
        }
        
    /**
     *
     */
    void ConstructL( RMmCustomAPI::TMmCellInfo& aCellInfo );
    
private:
    /**
     *
     */
    RMmCustomAPI::TMmCellInfo::TNwMode iMode; 
    
    /**
     * 
     */
    RArray<TInt>       iCellIdArray; 
    /**
     *
     */
    RArray< TGsmNMR >  iGsmNMR;
    
    /**
     *
     */
   RArray< TWcdmaNMR > iWcdmaNMR;     
   
    /**
     *
     */
    TInt            iTA;
    
    };

#endif
