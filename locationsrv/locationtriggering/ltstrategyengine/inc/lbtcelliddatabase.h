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
* Description:  This header file describes the class that handles cell id to
*                co-ordinate mapping database
*
*/

#ifndef LBTCELLIDDATABASE_H_
#define LBTCELLIDDATABASE_H_

#include <e32base.h>
#include <lbsposition.h>
/**
 *  This class provides the inteface to access Cell ID Database
 *
 *  @since S60 v5.1
 */
class CLbtCellIdDatabase : public CBase
    {
    /**
     * Data to store cell information
     */ 
    struct TCellIdCoordinateData
        {
        TInt32      iCountryCode; 
        TInt32      iNetworkID;
        TInt32      iLac;
        TInt32      iCellId;
        TLocality   iLocality;
        };
    
public: // Constructor and destructor
    /**
     * Constructs a new instance of CLbtCellIdDatabase.
     *
     * @return The new instance of CLbtCellIdDatabase.
     * @leave System wide error code if the object creation fails.         
     */
    static CLbtCellIdDatabase* NewL( );       
    
    /**
     * Destructor.
     */
    ~CLbtCellIdDatabase();

public: // Methods to for functionality
    /**
     * Inserts a recods which maps a perticular CGI information
     * to co-ordinates
     * 
     * @param[in] aCountryCode the country code of the cell
     * @param[in] aNetworkID the network identifier
     * @param[in] aLac the location area code
     * @param[in] aCellId the cell identifier
     * @param[in] aLocality the co-ordinate information which is 
     *            encompassed in the cell id. As of now only the latitude
     *            longitude and horizontal accuracy values are stored from
     *            specified aCoordinate
     *
     * @Leave Any one of the symbian error codes. Specifically
     *      - KErrNone of the information is commited into the database
     *      - KErrNoMemory if the system does not container enough memory to
     *                     process the request
     */
     void InsertCidLocation( TInt32 aCountryCode, 
                             TInt32 aNetworkID, 
                             TInt32 aLac, 
                             TInt32 aCellId,
                             TLocality aLocality );
    
    /**
     * For a specified CGI information, retreives the location information.
     * In the current implementation the retreived location information only 
     * contains the latitude, logitude and horizontal accuracy information
     *
     * @param[in]  aCountryCode the country code of the cell
     * @param[in]  aNetworkID the network identifier
     * @param[in]  aLac the location area code
     * @param[in]  aCellId the cell identifier
     * @param[out] aLocality the co-ordinate information which is 
     *             encompassed in the cell id. As of now only the latitude
     *             longitude and horizontal accuracy values are stored in
     *             aLocality 
     *
     * @return Any one of the symbian error codes. Specifically
     *      - KErrNotSupported if this method is invoked before invoking 
     *                         method CacheDataForNetwork()
     *      - KErrNone of the information is commited into the database
     *      - KErrNotFound if no location information is found for the database
     *      - KErrNoMemory if the system does not container enough memory to
     *                     process the request
     * 
     */ 
     TInt GetLocationForCid( TInt32 aCountryCode, 
                             TInt32 aNetworkID, 
                             TInt32 aLac,
                             TInt32 aCellId,
                             TLocality& aLocality );

private:
    /**
     * C++ default constructor
     */
    CLbtCellIdDatabase();
    
    /**
     * Symbian second phase contructor
     */
    void ConstructL();
    
    /**
     * Method to check if an entry is already present in the database
     */ 
    TBool IsDuplicateEntry( TInt32 aCountryCode, 
                            TInt32 aNetworkID, 
                            TInt32 aLac, 
                            TInt32 aCellId,
                            TLocality aLocality );
    
private://data
    // Write request array
    RArray<TCellIdCoordinateData> iCellIdDbArray;
    };

#endif /*LBTCELLIDDATABASE_H_*/
