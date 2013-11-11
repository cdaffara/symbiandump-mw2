/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Declaration of CPosRevGeoCodePlugin class.
*
*/

#ifndef POSREVGEOCODEPLUGIN_H
#define POSREVGEOCODEPLUGIN_H

#include "posreversegeocodeinterface.h"
#include "posrevgeocodehttpclientengine.h"
#include "posrevgeocodeconnectiontimer.h"
#include "posrevgeocodexmlparser.h"


class CPosRevGeoCodePlugin : public CPosReverseGeocodeInterface,
                             public MPosRevGeoCodeXmlObserver,
                             public MPosRevGeoCodeHTTPClientObserver,
                             public MPosRevGeoCodeTimeoutObserver
    {
public:
    /**
     * Factory function to create the instance of CPosRevGeoCodePlugin object.
     */
    static CPosRevGeoCodePlugin* NewL( MPosReverseGeocodeObserver& aObserver );
    
    /**
     * Destructor
     */
    ~CPosRevGeoCodePlugin();
    
    /**
     * Gets the address information for the given geo coordinates
     */
    virtual void GetAddressByCoordinateL( CPosLandmark& aLandmark );
    
    /**
     * Cancels the current operation.
     */
    virtual void Cancel();
    
    /**
     * Function to get the appropriate language for the request. 
     */
    void GetLanguageForTheRequest( TDes8& aLanguage );

public: // From MPosRevGeoCodeXmlObserver
    virtual void OnParseCompletedL( TInt aError );
    
public: // From MPosRevGeoCodeHTTPClientObserver
    virtual void ClientEvent( const THttpStatus& aEvent );
    
    virtual void ClientBodyReceived( const TDesC8& aBodyData );

public: // From MPosRevGeoCodeTimeoutObserver
    virtual void HandleTimedoutEvent( TInt aErrorCode );
    
private:
    /**
     * Symbian 2nd phase constructor.
     */
    void ConstructL();
    
    /**
     * Constructor
     */
    CPosRevGeoCodePlugin( MPosReverseGeocodeObserver& aObserver );
     
    /**
     * Starts the timer 
     */
    void StartTimer();
    
    /**
     * Closes connection
     */
    void CloseConnection();

private: // Data members
    
    /**
     * Reference to reverse geocode observer.
     */
    MPosReverseGeocodeObserver&  iObserver;
    
    /**
     * Pointer to HTTP client engine
     * Own
     */
    CPosRevGeoCodeHTTPClientEngine* iHTTPClientEngine;
    
    /**
     * Pointer to XML parser
     * Own
     */
    CPosRevGeoCodeXmlParser* iXmlParser;
    
    /**
     * Pointer to connection timer
     * Own
     */
    CPosRevGeoCodeConnectionTimer* iConnectionTimer;
    
    /**
     * Pointer to client's landmarks object.
     * Not own.
     */
    CPosLandmark* iClientLandmark;
    
    HBufC8* iXMLBuf;
    
    TBuf8<KMaxFileName> iQueryString;
    
    TBuf8<KMaxFileName> iAuthCode;
    
    TBuf8<KMaxFileName> iRefURL;
    
    TBuf8<KMaxFileName> iLang;
    
    };

#endif /* POSREVGEOCODEPLUGIN_H */
