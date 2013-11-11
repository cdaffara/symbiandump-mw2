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
* Description: Class to create,manage and destroy requestors for each request
*
*/

#ifndef CLBSSUPLTIREQUESTMANAGER_H
#define CLBSSUPLTIREQUESTMANAGER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <lbs/lbsnetcommon.h>
#include <epos_suplterminal.h>

//FORWARD DECALRATIONS
class CCLbsSuplTiApiRequestor;
class COMASuplTrace;
class MLbsSuplTiObserver;

// CLASS DECLARATION

/**
 * Class:       CCLbsSuplTiRequestManager
 *
 * Description: This class creates and manages all CCLbsSuplTiApiRequestor objects
 *              using for making the location requests. Each time a new request is made
 *              using the CSuplTiApi::RequestLocation with a new session id, this class 
 *              creates a new CCLbsSuplTiApiRequestor object which is used to make a
 *              location request. The ownership of the created object is held 
 *              by this class in the iRequestorCollection array.
 */
class CCLbsSuplTiRequestManager : public CActive
    {
public:
    // Constructors and destructor

    /**
     * Destructor for CCLbsSuplTiRequestManager.
     */
    ~CCLbsSuplTiRequestManager();

    /**
     * Two-phased constructor.
     */
    static CCLbsSuplTiRequestManager* NewL(MLbsSuplTiObserver& aObserver);

    /**
     * Two-phased constructor.
     */
    static CCLbsSuplTiRequestManager* NewLC(MLbsSuplTiObserver& aObserver);

public:
    
    /**
     * @Description:    Method to make the initial location request.
     *                  Each time a location request is made this methos
     *                  searches the iRequestorCollection for the session id.
     *                  If the session id is not found then creates a new requestor object
     *                  and initaites the location request.
     *                  
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
     *                  
     * @return:         None    
     *
     */
    void RequestLocation(const TLbsNetSessionId& aSessionId, const TLbsNetPosRequestOptionsBase& aOptions, const TLbsNetPosRequestMethod& aMethod);
    
    /**
     * @Description:    Method to cancel the request made using the RequestLocation() method.
     *                  This method searches the iRequestorCollection for the session id.
     *                  If the session id is found then this method uses the corresponding 
     *                  requestor object to cancel the asynchronous request.
     *                  If the session id is not found then it returns an error. 
     * @param:          aPositionInfo the position value is returned in this
     * @return:         KErrNone if position retrieved successfully
     *                  KErrNotFound if session id not found
     *                  TODO: Other error codes to be put here      
     *
     */
    
    TInt CancelRequest(const TLbsNetSessionId& aSessionId);
    
    /**
     * @Description:    Method to retrieve the Position obtained for request made
     *                  using the RequestLocation() method.
     *                  This method searches the iRequestorCollection for the session id.
     *                  If the session id is found then this method uses the corresponding 
     *                  requestor object to retrieve the position present in that requestor.
     *                  If the session id is not found then it returns an error. 
     * @param:          aPositionInfo the position value is returned in this
     * @return:         KErrNone if position retrieved successfully
     *                  KErrNotFound if session id not found
     *                  TODO: Other error codes to be put here      
     *
     */
    
    TInt GetPosition(const TLbsNetSessionId& aSessionId, TPositionInfoBase& aPositionInfo);
    
    /*
     * @Description:    Method to close open sessions with server
     * @return          None
     * 
     */
    void CloseSession();
    
private:
    // From CActive
    // Handle completion
    void RunL();

    // For cancelling Ashnchronous requests
    void DoCancel();

    // Override to handle leaves from RunL().
    TInt RunError(TInt aError);

    /**
     * Constructor for performing 1st stage construction
     */
    CCLbsSuplTiRequestManager(MLbsSuplTiObserver& aObserver);

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();
    
    /**
      * @Description:    Method to search and determine if a requestor exists
      *                  corresponding to a particular session id.
      *                  Searches the iRequestorCollection for the session id.
      * @param:          aSessionId Session Id to search
      * @return:         index of the requestor in the iRequestorCollection array if found
      *                  KErrNotFound otherwise      
      *
      */
    TInt FindSessionId(const TLbsNetSessionId& aSessionId);
    
  	/**
      * @Description:    Method to initiate the location request once a valid server handle is established.
      *									 If a valid server handle exists already this method is called to initiate location
      *									 request.
      * @param:          none
      * @return:         none      
      *
      */
    void InitiateLocationRequest();
private :
    
    //To signal completion
    MLbsSuplTiObserver& iObserver;
    //Hold a pointer to all requestor objects that were used to make location requests
    RPointerArray<CCLbsSuplTiApiRequestor> iRequestorCollection;
    
    RSuplTerminalServer iServer;
    
    /*For Logging*/
    COMASuplTrace* iTrace;
    
    TLbsNetSessionId iSessionId;
    TLbsNetPosRequestOptionsBase iOptions;
    TLbsNetPosRequestMethod iMethod;
    };

#endif // CLBSSUPLTIREQUESTMANAGER_H
