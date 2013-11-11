/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MPX back stepping utility singleton
*
*/

 
#ifndef CMPXBACKSTEPPINGUTILITYIMP_H
#define CMPXBACKSTEPPINGUTILITYIMP_H

// INCLUDES
#include <e32base.h>
#include <mpxbacksteppingutility.h>

// FORWARD DECLARATIONS
class MLiwInterface;
class CLiwGenericParamList;
class CLiwServiceHandler;

// CLASS DECLARATION

/**
*  Music Player common utility class
 *
 *  @lib mpxbacksteppingutility.lib
 *  @since S60 v3.2
*/
NONSHARABLE_CLASS( CMPXBackSteppingUtilityImp )
    : public CBase
    , public MMPXBackSteppingUtility
    {
public: 

    /**
     * Retrieve a back stepping utility of this thread.  All clients in this thread
     * shares the same utility (singleton)
     * Call Close() when this object is not needed anymore.
     *
     * @since 3.2
     * @return Pointer to backstepping utility. Ownership not transferred.
     */
    static MMPXBackSteppingUtility* UtilityL();

    /**
     * This method must be called when this object can be freed. This object 
     * will be destroyed when no one is using it.
     *
     * @since 3.2
     */
    void Close();

    /**
     * Forwards activation event
     *
     * @since S60 v3.2
     * @param aState state of application like view ids
     * @param aEnter indicate if it is entry/exit activation
     * @return result code retutned by BS Service interface
     * @leave leaves from HandleResultL are propagated
     */
    TInt ForwardActivationEventL(
        const TDesC8& aState,
        const TBool aEnter );
    
    /**
     * Handles back command
     *
     * @since S60 v3.2
     * @param aState state of application like view ids
     * @return result code retutned by BS Service interface
     * @leave leaves from HandleResultL are propagated
     */
    TInt HandleBackCommandL( const TDesC8& aState );

    /**
     * Initializes BS Service
     * @param aUid client application UID
     * @leave KErrNotSupported or KErrArgument 
     *        (if arguments passed to BS Service are incorrect 
     *        or could not initialize BS)
     *        KErrNotFound (if there was no return value)
     */
    void InitializeL( const TUid aUid );
    
private:

    /**
    * C++ constructor
    */
    CMPXBackSteppingUtilityImp();

    /**
    *  Destuctor
    */ 
    virtual ~CMPXBackSteppingUtilityImp();
   
    /**
     * 2nd phase constructor
     */
    void ConstructL();
    
    /**
    *  Two phased contructor
    */ 
    static CMPXBackSteppingUtilityImp* NewL(); 
    static CMPXBackSteppingUtilityImp* NewLC(); 

    /**
     * Store utility to TLS.
     *
     * @param aUtility Utility object. Ownership not transferred.
     */
    static void StoreUtility( MMPXBackSteppingUtility* aUtility );

    /**
     * Handles the result of a LIW command
     * @return result code retutned by BS Service interface
     * @leave KErrNotSupported or KErrArgument 
     *        (if arguments passed to BS Service are incorrect)
     *        KErrNotFound (if there was no return value)
     */
    TInt HandleResultL();

private: // data
    
    /**
     * Reference count.
     */
    TInt iRefCount;
    
    /**
     * AIW Service Handler
     * Own.
     */
    CLiwServiceHandler* iServiceHandler;
    
    /**
     * BS Service interface returned by LIW
     * Own.
     */
    MLiwInterface* iBSInterface;
    
    /**
     * In param list
     * Not own.
     */
    CLiwGenericParamList* iInParamList;
    
    /**
     * Out param list
     * Not own.
     */
    CLiwGenericParamList* iOutParamList;

    };

#endif  // CMPXBACKSTEPPINGUTILITYIMP_H

// End of File
