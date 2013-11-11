/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Publish and Subscribe key watcher
*
*/


#ifndef C_MPXPSKEYWATCHER_H
#define C_MPXPSKEYWATCHER_H

#include <e32base.h>

#include <e32property.h>

class MMPXPSKeyObserver;

/**
 *  Publish and subscribe key watcher.
 *
 *  @lib MPXCommon.lib
 */
class CMPXPSKeyWatcher : public CActive
    {
public:

    /*
    * Two Phased Constructor.
    *
    * @since S60 3.2.3
    * @param aUid PubSub UID to watch
    * @param aKey specific key to watch
    * @return Construced object
    */
    IMPORT_C static CMPXPSKeyWatcher* NewL( TUid aUid, TInt aKey,
                                            MMPXPSKeyObserver* aObserver );

    /*
    * virtual destructor.
    *
    * @since S60 3.2.3
    */
    IMPORT_C virtual ~CMPXPSKeyWatcher();

public:     // New Functions

    /*
    * Return the value of the key.
    *
    * @since S60 3.2.3
    * @param aValue
    * @return KErrNone, if successful otherwise system error
    */
    IMPORT_C TInt GetValue( TInt& aValue );
    
    /*
    * Return the value of the key.
    *
    * @since S60 3.2.3
    * @param aValue
    * @return KErrNone, if successful otherwise system error
    */
    IMPORT_C TInt GetValue( TDes8& aValue );
    
    /*
    * Return the value of the key.
    *
    * @since S60 3.2.3
    * @param aValue
    * @return KErrNone, if successful otherwise system error
    */
    IMPORT_C TInt GetValue( TDes16& aValue );
    
    /*
    * Sets the value of the key.
    *
    * @since S60 3.2.3
    * @params aValue
    * @return KErrNone, if successful otherwise system error     
    */
    IMPORT_C TInt SetValue(TInt aValue );
    
    /*
    * Sets the value of the key.
    *
    * @since S60 3.2.3
    * @params aValue
    * @return KErrNone, if successful otherwise system error     
    */
    IMPORT_C TInt SetValue( const TDesC8& aValue );
    
    /*
    * Sets the value of the key.
    *
    * @since S60 3.2.3
    * @params aValue
    * @return KErrNone, if successful otherwise system error     
    */
    IMPORT_C TInt SetValue( const TDesC16& aValue );
    
protected:  // From Base class

    /*
    * From CActive.
    *
    * @since S60 3.2.3
    */
    void RunL();
    
    /*
    * From CActive.
    *
    * @since S60 3.2.3
    */
    void DoCancel();
    
private:

    /*
    * Default Constructor.
    *
    * @since S60 3.2.3
    * @param aUid PubSub key to watch
    * @param aKey specific key to watch
    */
    CMPXPSKeyWatcher( TUid aUid,
                      TInt aKey,
                      MMPXPSKeyObserver* aObserver );

    /*
    * Second phased constructor.
    *
    * @since S60 3.2.3
    */
    void ConstructL();

private: // data
    
    RProperty             iProperty;
    MMPXPSKeyObserver*    iObserver;  // not owned
    TUid                  iUid;       // Uid that we are watching
    TInt                  iKey;       // Key that we are watching
    };

#endif // C_MPXPSKEYWATCHER_H