/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Iterator class for going through ongoing calls.
*
*/


#ifndef C_CALLINFOITER_H
#define C_CALLINFOITER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// forward declarations 
class CCallInfos;
class MCall;

// CLASS DECLARATION

/**
 *  Class to be used for going through ongoing calls. 
 *
 *  @code
 *  1) Go trough all calls, no need to call Count() first. 
 *  CCallInfoIter& iter = iCallInfo->GetCallsL();    
 *    
 *  for( iter.First(); !iter.IsDone(); iter.Next() )
 *     {
 *     const MCall& call( iter.Current() );
 *     DoSomethingWhenCallDetected( call ); 
 *     }
 *             
 *  2) Check if there are any calls
 *  CCallInfoIter& iter = iCallInfo->GetCallsL(); 
 *  TInt amountOfCalls = iter.Count();
 *  
 *  if ( amountOfCalls == 0)
 *      {
 *      DoSomethingBecauseAllCallsHaveEnded();
 *      }      
 *             
 *  @endcode
 *
 *  @lib telephonyservice.lib
 *  @since S60 v5.1 
 */
NONSHARABLE_CLASS( CCallInfoIter ) : public CBase
    {
public:    

    /**
     * Two-phased constructor.
     * @param aCallInfos Container for call information. 
     */
    static CCallInfoIter* NewL( CCallInfos& aCallInfos );

    /**
     * Two-phased constructor.
     * @param aCallInfos Container for call information.
     */
    static CCallInfoIter* NewLC( CCallInfos& aCallInfos );
    
    /**
     * Destructor.
     */
    ~CCallInfoIter();
    
    /**
     * Intializes the current call to the first call
     * 
     * @since S60 v5.1  
     */
    IMPORT_C void First();
    
    /**
     * Tests whether all calls are went through
     * 
     * @since S60 v5.1  
     */  
    IMPORT_C TBool IsDone() const;
    
    /**
     * Advances the current call to the next call
     * 
     * @since S60 v5.1  
     */  
    IMPORT_C void Next();
    
    /**
     * Returns the count of calls
     * 
     * @since S60 v5.1
     * @return Amount of calls  
     */
    IMPORT_C TInt Count() const;
       
    /**
     * Returns the current call. Panic happens if there are no calls.
     * 
     * @since S60 v5.1
     * @return Reference to the call.  
     */
    IMPORT_C const MCall& Current() const;
    

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CCallInfoIter( CCallInfos& aCallInfos );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();
    
private: // data    
     
    CCallInfos& iCallInfos;    
    
    TInt iCurrentIndex;

    };

#endif // C_CALLINFOITER_H
