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
* Description: Logic for bufferred publish and subcribe notifier
*
*/

#ifndef C_SPSBUFFEREDPUBLISHER_H
#define C_SPSBUFFEREDPUBLISHER_H

#include <e32base.h>

/**
 *  P&S ringbuffer implementation
 *  
 *  Ringbuffer size is 127 TUint32:s
 *
 *  @code
 *   Publisher:
 *      CSpsBufferedPublisher::SetL( KCategory, KKey, data );
 *   
 *   Subscriber:
 *      - start to fill buffer
 *        iMember = CSpsBufferedPublisher::NewL( KCategory, KKey );
 *        iMember->Start();
 *        
 *      - when changed data is read
 *        RArray<TUint32> array;
 *        iMember->GetL( array );
 *        // Do something with data
 *        array.Close();
 *        
 *  @endcode
 *
 *  @lib serviceprovidersettings.lib
 */
NONSHARABLE_CLASS( CSpsBufferedPublisher ): public CBase
    {
public: // Publisher methods
    /**
     * ?description
     *
     * @param aCategory P&S Category
     * @param aKey P&S key
     * @param aData Data to be published
     */
    static void SetL( TUid aCategory, TUint aKey, TUint32 aData );
    
public: // Subscriber methods

    /**
     * Creates P&S key if not yet created.
     * 
     * Two-phased constructor.
     * @param aCategory P&S Category
     * @param aKey P&S key
     */
    static CSpsBufferedPublisher* NewL( TUid aCategory, TUint aKey );

    /**
     * Destructor.
     */
    virtual ~CSpsBufferedPublisher();

    /**
     * Start logging of buffer changes
     * Must be stopped before it can start from current position
     */
    void Start();
    
    /**
     * Stop logging of buffer changes
     */
    void Stop();
    
    /**
     * Get logged P&S data
     * 
     * @param aData Changed data since last get
     * @leave KErrNotReady if not started
     */
    void GetL( RArray<TUint32>& aData );

private:
    
    CSpsBufferedPublisher( TUid aCategory, TUint aKey );

    void ConstructL();

private:

    static void SignalAndCloseSemaphore( TAny* aPtr );
    
    static TUint32& At( TDes8& aData, TInt aIndex );
    
    static TUint Max( TDesC8& aData );
    
    static TInt32 Base( TDesC8& aData, TInt aIndex  );
    
    static TUint32& GlobalIndex( TDes8& aData );
    
private: // data
    
    TUid iCategory;
    TUint iKey;
    TInt iIndex;
    
    };


#endif // C_CSPSBUFFEREDPUBLISHER_H
