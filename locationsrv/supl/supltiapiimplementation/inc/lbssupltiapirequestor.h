/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: CActive Derived class for making requests to SUPL terminal 
*                initiation library   
*
*/

#ifndef CLBSSUPLTIAPIREQUESTOR_H
#define CLBSSUPLTIAPIREQUESTOR_H

#include <e32base.h>	
#include <e32std.h>		
#include <lbs/lbsnetcommon.h>
#include <epos_suplterminal.h>

//FORWARD DECLARATIONS
class COMASuplTrace;
class MLbsSuplTiObserver;

/**
 * Class:       CCLbsSuplTiApiRequestor
 *
 * Description: This is the CActive derived class that makes the actual
 *              location request to the Terminal Initiation Library.
 */
class CCLbsSuplTiApiRequestor : public CActive
    {
public:
    //Destructor for CCLbsSuplTiApiRequestor
    ~CCLbsSuplTiApiRequestor();

    // Two-phased constructor.
    static CCLbsSuplTiApiRequestor* NewL(MLbsSuplTiObserver& aObserver,RSuplTerminalServer& aServer);

    // Two-phased constructor.
    static CCLbsSuplTiApiRequestor* NewLC(MLbsSuplTiObserver& aObserver,RSuplTerminalServer& aServer);

public:
    
    /**
     * @Description:    Method to make initial location request
     *                  Calls TI API's RunSession using the requested paramaters
     * @param:          aSessionId is the Id of the location request.This id is passed 
     *                  to the TI API's request id as argument
     *                  
     * @param           aOptions should be of type TLbsNetPosRequestOptions.
     *                  This is used to retrieve the TSuplTerminalQop and 
     *                  First Req paramaters to be passed to the TI API's 
     *                  RunSession Method.
     *                  
     * @param           aMethod This parameter contains a list of the positioning methods 
     *                  that should be used to obtain the device's position.
     *                  This is used to for setCaps to be sent to the RunSession method. 
     * @return:         None      
     *
     */
    void RequestLocation(const TLbsNetSessionId& aSessionId, const TLbsNetPosRequestOptionsBase& aOptions, const TLbsNetPosRequestMethod& aMethod);

    
    /**
     * @Description:    Method to retrieve the Position obtained for request made
     *                  using the RequestLocation() method
     *                  Calls TI API's GetPosition using the requested paramaters
     * @param:          aPositionInfo the position value is returned in this
     * @return:         KErrNone if position retrieved successfully
     *                  TODO: Other error codes to be put here      
     *
     */
    TInt GetPosition(TPositionInfoBase& aPositionInfo);
    
    /**
     * @Description:    Method to retrieve the session id for which this requestor object
     *                  is making requests. The request id is set when client calls 
     *                  RequestLocation() method. Used to differentiate CCLbsSuplTiApiRequestor
     *                  objects based on Session Id.
     * @return:         returns the TLbsNetSessionId object which was passed to this 
     *                  when making initial request    
     *
     */
    TLbsNetSessionId GetSessionId();
    
private:
    // C++ constructor
    CCLbsSuplTiApiRequestor(MLbsSuplTiObserver& aObserver,RSuplTerminalServer& aServer);

    // Second-phase constructor
    void ConstructL();
    
    /**
     * @Description:    Method to convert a TLbsNetPosRequestMethod object to an integer
     *                  form to be passed as argument to RunSession for SetCaps.
     * @param :         aMethod The TLbsNetPosRequestMethod object to be converted
     * @return:         returns the setcaps in TInt form  
     *
     */
    TInt ProcessPosMethods( const TLbsNetPosRequestMethod& aMethod);

private:
    // From CActive
    // Handle completion
    void RunL();

    // For cancelling Ashnchronous requests
    void DoCancel();

    // Override to handle leaves from RunL().
    TInt RunError(TInt aError);

private:
    //Enum to indicate state of this requestor
    enum TCLbsSuplTiApiRequestorState
        {
        EUninitialized, // Uninitialized
        EInitialized, // Initalized
        };

private:
	TCLbsSuplTiApiRequestorState iState; // State of the active object

    RSuplTerminalServer& iServer;
    RSuplTerminalSubSession iSubSession;
    
    //Reference to observer to signal completion
    MLbsSuplTiObserver& iObserver;
    
    //Session id for which this request was made
    TLbsNetSessionId iNetSessionId;
    
    //For Logging
    COMASuplTrace* iTrace;
    };

#endif // CLBSSUPLTIAPIREQUESTOR_H
