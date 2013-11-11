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
* Description:  Iterator class for going through remote party info
*               of ongoing calls.
*
*/

#ifndef CCALLREMOTEPARTYINFOITER_H_
#define CCALLREMOTEPARTYINFOITER_H_

// forward declarations
class CCallRemotePartyInfos;

// CLASS DECLARATION

/**
 *  Class to be used for going through ongoing calls. 
 *
 *  @code
 *  1) Go trough all items, no need to call Count() first. 
 *  CCallRemotePartyInfoIter& iter = iRemoteInfos->GetRemotePartyInformationL();    
 *    
 *  for( iter.First(); !iter.IsDone(); iter.Next() )
 *     {
 *     const MCallRemotePartyInfo& remoteInfo( iter.Current() );
 *     DoSomethingWhenCallDetected( remoteInfo ); 
 *     }
 *             
 *  2) Check if there are any calls
 *  CCallRemotePartyInfoIter& iter = iRemoteInfos->GetRemotePartyInformationL(); 
 *  TInt amountOfCalls = iter.Count();
 *  
 *  if ( amountOfCalls == 0)
 *      {
 *      DoSomething();
 *      }      
 *             
 *  @endcode
 *
 *  @lib telephonyservice.lib
 *  @since S60 v5.2
 */
NONSHARABLE_CLASS( CCallRemotePartyInfoIter ) : public CBase
    {
public:    

    /**
     * Two-phased constructor.
     * @param aRemotePartyInfos Container for remote party information. 
     */
    static CCallRemotePartyInfoIter* 
        NewL( CCallRemotePartyInfos& aRemotePartyInfos );

    /**
     * Two-phased constructor.
     * @param aRemotePartyInfos Container for remote party information.
     */
    static CCallRemotePartyInfoIter*
        NewLC( CCallRemotePartyInfos& aRemotePartyInfos );
    
    /**
     * Destructor.
     */
    ~CCallRemotePartyInfoIter();
    
    /**
     * Intializes the current item to the first remote party information.
     * 
     * @since S60 v5.1  
     */
    IMPORT_C void First();
    
    /**
     * Tests whether the last item has been reached.
     * 
     * @since S60 v5.1  
     */  
    IMPORT_C TBool IsDone() const;
    
    /**
     * Advances the current item to the next remote party information.
     * 
     * @since S60 v5.1  
     */  
    IMPORT_C void Next();
    
    /**
     * Returns the count of items.
     * 
     * @since S60 v5.1
     * @return Amount of calls  
     */
    IMPORT_C TInt Count() const;
       
    /**
     * Returns the current remote party information.
     * Panic happens if there are no items.
     * 
     * @since S60 v5.1
     * @return Reference to the call.  
     */
    IMPORT_C const MCallRemotePartyInfo& Current() const;

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CCallRemotePartyInfoIter( CCallRemotePartyInfos& aRemotePartyInfos );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();
    
private: // data    
    
    CCallRemotePartyInfos&   iRemotePartyInfos;    
    TInt iCurrentIndex;

    };

#endif /* CCALLREMOTEPARTYINFOITER_H_ */
