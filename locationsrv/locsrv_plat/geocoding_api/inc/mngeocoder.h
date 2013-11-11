/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CMnGeocoder class
*
*/


#ifndef MN_GEOCODER_H
#define MN_GEOCODER_H

#include <e32base.h>

class CApaServerAppExitMonitor;
class MAknServerAppExitObserver;
class CMnProvider;
class CPosLandmark;
class RMnGeocodingServiceClient;

/** @brief Provides access to geocoding features.
 *
 *  Both geocoding (address to coordinate) and "reverse geocoding"
 *  (coordinate to address) features are provided by this class. Further in
 *  documentation they are all referred to as "geocoding" if not specified otherwise.
 *
 *  Geocoding operations are asynchronous, they can be cancelled by @ref Cancel().
 *  However only one asynchronous request can be issued at a time, otherwise client
 *  will be panicked with KMnPanicDuplicateRequest.
 *
 *  To create an instance of the class, use @ref NewL or @ref NewChainedL methods. They
 *  require an instance of CMnProvider, defined in Map and Navigation Provider Discovery API.
 *
 *  Main methods are @ref FindCoordinateByAddressL (two overloads) and
 *  @ref FindAddressByCoordinateL.
 *  Other methods define optional information that changes behavior of the
 *  class and others help to retrieve results of main methods' execution.
 *
 *  @since 3.1
 *  @lib mnclientlib.dll
 *  @ingroup MnGeocodingAPI
 */
class CMnGeocoder : public CBase
    {
    public :
        /** Type for bitmap of @ref TOption values. */
        typedef TUint32 TOptions;

        /** @brief Options defining geocoding execution. */
        enum TOption
            {
            /** Default value */
            EOptionNone           = 0x00,
            /** Defines that no queries to user are allowed during
             *  geocoding operations */
            EOptionDialogDisabled = 0x01
            };

        /** Destructor */
        IMPORT_C ~CMnGeocoder();

        /** @brief Creates new instance of the class. Provider Application
         *  will be executed standalone.
         *
         *  @param[in] aProvider Provider Application to be used for geocoding.
         *  @return Pointer to new instance of the class.
         *
         *  @leave KErrNotSupported Selected Provider Application does not support
         *      @ref CMnProvider::EServiceGeocoding.
         */
        IMPORT_C static CMnGeocoder* NewL( CMnProvider& aProvider );

        /** @brief Creates new instance of the class. Provider Application
         *  will be executed chained (unless Provider Application defines that
         *  it should be started in background).
         *
         *  @param[in] aProvider Provider Application to be used for geocoding.
         *  @return Pointer to new instance of the class.
         *
         *  @leave KErrNotSupported Selected Provider Application does not support
         *      @ref CMnProvider::EServiceGeocoding.
         */
        IMPORT_C static CMnGeocoder* NewChainedL( CMnProvider& aProvider );

        /** @brief Retrieves current geocoding options.
         *  @return Bitmap of @ref TOption values.
         */
        IMPORT_C TOptions Options() const;

        /** @brief Sets geocoding options.
         *  @param aOptions Bitmap of @ref TOption values.
         */
        IMPORT_C void SetOptions( TOptions aOptions );

        /** @brief Retrieves result of last geocoding or reverse geocoding operation.
         *
         *  If last operation result was address then address information in the
         *  passed landmark is updated. If last operation result were coordinates,
         *  then coordinate information of the landmark is updated.
         *  Note that landmark is not stored in any database, client has to do it
         *  itself if needed.
         *
         *  Target landmark parameter does not have to be a different landmark then that
         *  was passed for geocoding or reverse geocoding. However, all address
         *  or coordinate (depending on last operation result type) information
         *  will be updated in target landmark.
         *
         *  Issuing next geocoding request discards current result.
         *
         *  @param[out] aTargetLandmark This landmark will be updated according to
         *                         last operation result.
         *
         *  @leave KErrNotFound Result is not available.
         */
        IMPORT_C void RetrieveGeocodingResultL( CPosLandmark& aTargetLandmark );

        /** @brief Initiates geocoding: translating street address to coordinates.
         *
         *  Address information from passed landmark will be used.
         *
         *  Only one coordinate will be the result of this operation.
         *  If any ambiguities arise during translation, then Provider Application
         *  may query the user to solve it. Set EOptionDialogDisabled to prevent this.
         *
         *  Use RetrieveGeocodingResultL() to retrieve results of the operation.
         *
         *  This method can leave in case of memory lack or other system error.
         *  If it leaves, no asynchronous request is outstanding.
         *
         *  @param[in] aLandmark Landmark containing address information
         *  @param[out] aRequest Contains result of the request, when it is completed.
         *      @p KErrNotFound if given address couldn't be resolved to coordinate
         *      (also due to disabling of query dialog). @p KErrCancel, if user has cancelled
         *      the operation. @p KErrArgument, if input landmark does not contain address
         *      information. @p KErrNone if request is completed successfully.
         *
         *  @panic "MnClientPanic"-KMnPanicDuplicateRequest New asynchronous request
         *          is issued before previous one is completed.
         */
        IMPORT_C void FindCoordinateByAddressL(
            const CPosLandmark& aLandmark,
            TRequestStatus& aRequest );

        /** @brief Initiates geocoding: translating street address to coordinates.
         *
         *  This is the another overload, which accepts address input as plain
         *  string.
         *
         *  Only one coordinate will be the result of this operation.
         *  If any ambiguities arise during translation, then Provider Application
         *  may query the user to solve it. Set EOptionDialogDisabled to prevent this.
         *
         *  Use RetrieveGeocodingResultL() to retrieve results of the operation.
         *
         *  This method can leave in case of memory lack or other system error.
         *  If it leaves, no asynchronous request is outstanding.
         *
         *  @param[in] aAddress Plain address. Format of this string is not defined,
         *                  this is barely how human user writes addresses down.
         *  @param[out] aRequest Contains result of the request, when it is completed.
         *      @p KErrNotFound if given address couldn't be resolved to coordinate
         *      (also due to disabling of query dialog). @p KErrCancel, if user has cancelled
         *      the operation. @p KErrNone if request is completed successfully.
         *
         *  @panic "MnClientPanic"-KMnPanicDuplicateRequest New asynchronous request
         *          is issued before previous one is completed.
         */
        IMPORT_C void FindCoordinateByAddressL(
            const TDesC& aAddress,
            TRequestStatus& aRequest );

        /** @brief Initiates reverse geocoding: translating coordinates to
         *  street address.
         *
         *  Coordinate information from passed
         *  landmark will be used. Any other helper information (like coverage
         *  radius, existing address information) can also be used. If it is not
         *  desired, then this information should be removed from landmark
         *  by client.
         *
         *  Only one address will be the result of this operation.
         *  If any ambiguities arise during translation, then Provider Application
         *  may query user to solve it. Set EOptionDialogDisabled to prevent this.
         *  Additional information (other than coordinate) found in landmark cannot
         *  be a reason for such a request.
         *
         *  Use RetrieveGeocodingResultL() to retrieve results of the operation.
         *
         *  This method can leave in case of memory lack or other system error.
         *  If it leaves, no asynchronous request is outstanding.

         *  @param[in] aLandmark Landmark containing address information
         *  @param[out] aRequest Contains result of the request, when it is completed.
         *      @p KErrNotFound if given address couldn't be resolved to coordinate
         *      (also due to disabling of query dialog). @p KErrCancel, if user has cancelled
         *      the operation. KErrArgument, if input landmark does not contain coordinate
         *      information (latitude and longitude are required).
         *      @p KErrNone if request is completed successfully.
         *
         *  @panic "MnClientPanic"-KMnPanicDuplicateRequest New asynchronous request
         *          is issued before previous one is completed.
         */
        IMPORT_C void FindAddressByCoordinateL(
            const CPosLandmark& aLandmark,
            TRequestStatus& aRequest );

        /** @brief Cancels current outstanding asynchronous request. */
        IMPORT_C void Cancel();

        /** @brief Sets observer of provider application lifetime.
         * 	Client will receive exit event from provider application.
         *  By default exit events from provider application are not monitored.
         *  @param[in] aObserver Exit observer.
         *  @leave KErrAlreadyExists if observer is already set. 
         */
        IMPORT_C void SetExitObserverL( MAknServerAppExitObserver& aObserver );

        /** @brief Removes observer of provider application lifetime. */
        IMPORT_C void RemoveExitObserver();

    private :
        /** C++ constructor */
        CMnGeocoder();

        void ConstructCommonL( CMnProvider& aProvider );
        void ConstructNewL( CMnProvider& aProvider );
        void ConstructChainedL( CMnProvider& aProvider );

    private :
        TOptions iOptions;

        RMnGeocodingServiceClient* iSession;
        CApaServerAppExitMonitor* iExitMonitor;
    };

#endif // MN_GEOCODER_H

