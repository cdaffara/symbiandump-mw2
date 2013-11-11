/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CPosLandmarkDatabase class
*
*/


#ifndef CPOSLANDMARKDATABASE_H
#define CPOSLANDMARKDATABASE_H

#include <e32base.h>
#include <s32strm.h>
#include "EPos_Landmarks.h"
#include "EPos_CPosLandmark.h"
#include "EPos_CPosLmPartialReadParameters.h"
#include "EPos_CPosLmItemIterator.h"
#include "EPos_TPosLmSortPref.h"
#include "EPos_CPosLmOperation.h"

class CPosLandmarkParser;
class CPosLandmarkEncoder;


/**
*  Handle to a landmark database.
*
*  This class contains functions for creating, iterating, reading, modifying
*  and deleting landmarks.
*
*  Operations on the database may fail with error code @p KErrCorrupt if the
*  database is damaged. The client can try to recover the database by calling
*  @ref CPosLandmarkDatabase::InitializeL().
*
*  Operations on the database may fail with error code @p KErrLocked if another
*  client is writing to the database. Write operations can also fail with this
*  error code if another client is currently reading from the database.
*
*  If @ref CPosLandmarkDatabase is used, the client must call the global
*  function @ref ReleaseLandmarkResources() before terminating in order to
*  release all used landmark resources, otherwise the client may receive
*  an ALLOC panic.
*
*  @p NetworkServices capability is required for remote databases.
*
*  @since S60 3.0
*  @lib eposlandmarks.lib.
*/
class CPosLandmarkDatabase : public CBase
    {
    public:

        /**
        *  Bitmap for specifying a group of transfer options
        *  defined by @ref _TAttributes.
        */
        typedef TUint TTransferOptions;

        /**
        *  Specifies options for importing and exporting landmarks.
        */
        enum _TTransferOptions
            {
            EDefaultOptions = 0                 /**<
                None of the transfer option flags are set. */,
            EIncludeCategories = 0x01           /**<
                Export/Import the categories of the landmarks. */,
            EIncludeGlobalCategoryNames = 0x02  /**<
                Only useful in combination with EIncludeCategories.
                If set, global category names will be used in export/import
                even if user has renamed them. For import it means that the
                names of the global categories in the database are overwritten
                by the imported names. For export it means that predefined
                names of global categories in the current language will be
                exported instead of user-defined names. */,
            ESupressCategoryCreation = 0x04     /**<
                Only useful in combination with EIncludeCategories.
                If set, no new categories are created in the database when
                importing landmarks. This means that connections from
                imported landmarks will be established only to already
                existing categories, according to the import information. */
            };

        /**
        *  Encapsulates size information for a landmark database.
        */
        struct TSize
            {
            TInt iFileSize;   /**<
                The size of the database in bytes. */
            TReal32 iUsage;   /**<
                How many percent of the database which is currently in use.
                This value lies in the interval [0.0, 1.0]. */
            };

    public:

        /**
        * Opens the default landmark database.
        *
        * The client takes ownership of the returned database handle.
        *
        * The database may have to be initialized before it can be used, see
        * @ref IsInitializingNeeded and @ref InitializeL.
        *
        * This function requires @p ReadUserData capability.
        *
        * @returns A handle to the open database.
        */
        IMPORT_C static CPosLandmarkDatabase* OpenL();

        /**
        * Opens a specific landmark database.
        *
        * The client refers to a database by URI. The URI consists of a
        * protocol specifier and the database location: "protocol://location".
        * If the client does not specify a protocol, "file://" will be assumed.
        *
        * For local landmark databases, the URI consists of the drive and the
        * database file name, e.g. "c:landmarks.ldb". The path cannot be
        * specified by the client. The extension of the database file name must
        * be "ldb" otherwise the client will get the error @p KErrArgument.
        *
        * If the client specifies a local database and does not specify the
        * drive letter, e.g. "landmarks.ldb", default database drive will be assumed.
        *
        * The client takes ownership of the returned database handle.
        *
        * The database may have to be initialized before it can be used, see
        * @ref IsInitializingNeeded and @ref InitializeL.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param[in] aDatabaseUri The URI of the database to open.
        * @returns A handle to the open database.
        *
        * @leave KErrArgument Extension of the local database name is not "ldb".
        * @leave KErrNotSupported The protocol specified in URI is not supported.
        */
        IMPORT_C static CPosLandmarkDatabase* OpenL(
               const TDesC&  aDatabaseUri
        );

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CPosLandmarkDatabase();

    public:

        /**
        * Checks if the database is in need of initialization.
        *
        * If the database needs to be initialized, the client must call
        * @ref InitializeL, otherwise the database may not be possible to
        * access. Access functions may leave with @p KErrPosLmNotInitialized.
        *
        * Initialization may be needed also if the database becomes damaged.
        * The client can then try to call @ref InitializeL to try to recover
        * the database.
        *
        * @returns @p ETrue if the database is in need of initialization,
        *   otherwise @p EFalse.
        */
        virtual TBool IsInitializingNeeded() const = 0;

        /**
        * Initializes the database.
        *
        * This function may have to be called right after the database is
        * opened. @ref IsInitializingNeeded can be called to find out if
        * initialization is needed.
        *
        * If the database becomes damaged, the client can call
        * @ref InitializeL  to try to recover the database.
        *
        * It is ok to call @InitializeL even if initialization is not needed.
        * In this case, the operation will not do anything.
        *
        * If the database needs to be initialized, the client must call
        * @ref InitializeL, otherwise the database may not be possible to
        * access. Access functions may leave with @p KErrPosLmNotInitialized.
        *
        * The function returns an operation object which can be run in
        * incremental mode. If it is run incrementally the client can supervise
        * the progress of the operation.
        *
        * If the @ref CPosLmOperation object is deleted before the operation
        * is complete, it is possible that the database is not yet initialized.
        *
        * The client takes ownership of the returned operation object.
        *
        * This function requires @p ReadUserData capability.
        *
        * @returns A handle to the operation.
        */
        virtual CPosLmOperation* InitializeL() = 0;

        /**
        * Reads a landmark from the database.
        *
        * @pre Database is initialized (see @ref IsInitializingNeeded).
        *
        * The client takes ownership of the returned database landmark object.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param The ID of the landmark to read.
        * @returns The requested landmark. The landmark object is put on the
        *   cleanup stack.
        *
        * @leave KErrNotFound The requested landmark does not exist in the database.
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual CPosLandmark* ReadLandmarkLC(
               TPosLmItemId  aLandmarkId
        ) = 0;

        /**
        * Returns an object for iterating the landmarks in the database.
        *
        * @pre Database is initialized (see @ref IsInitializingNeeded).
        *
        * The iterator object is reset, so that the first
        * @ref CPosLmItemIterator::NextL call will return the first landmark
        * ID.
        *
        * The client takes ownership of the returned iterator object.
        *
        * This function requires @p ReadUserData capability.
        *
        * @return The landmark iterator.
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual CPosLmItemIterator* LandmarkIteratorL() = 0;

        /**
        * Returns an object for iterating the landmarks in the database.
        *
        * @pre Database is initialized (see @ref IsInitializingNeeded).
        *
        * The iterator object is reset, so that the first
        * @ref CPosLmItemIterator::NextL call will return the first landmark
        * ID.
        *
        * This overload of the iterator function takes a sort preference object
        * as input. The sort preference object specifies how the landmarks
        * should be sorted by the iterator. Only sorting by landmark name is supported.
        *
        * The client takes ownership of the returned iterator object.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param[in] aSortPref A sort preference object.
        * @return The landmark iterator.
        *
        * @leave KErrNotSupported Sorting by another attribute than name is
        *   requested.
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual CPosLmItemIterator* LandmarkIteratorL(
               const TPosLmSortPref&  aSortPref
        ) = 0;

        /**
        * Adds a new landmark to the database and returns its ID.
        *
        * @pre Database is initialized (see @ref IsInitializingNeeded).
        *
        * A landmark can contain the IDs of the categories it belongs to. If
        * any of these categories does not exist in the database, the add
        * operation will still complete successfully but the category which
        * was not found will be ignored.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @post Landmark is added to the database and landmark object
        *   has database item set (CPosLandmark::LandmarkId()).
        *
        * @param[in,out] aLandmark The landmark to add.
        * @return The ID of the new landmark.
        *
        * @leave KErrAccessDenied The database is read-only.
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual TPosLmItemId AddLandmarkL(
            CPosLandmark&  aLandmark
        ) = 0;

        /**
        * Updates a landmark in the database.
        *
        * @pre Database is initialized (see @ref IsInitializingNeeded).
        *
        * Only landmark objects containing full landmark information can be
        * used to update a landmark. If a partial landmark (see
        * @ref ReadPartialLandmarkLC and @ref CPosLandmark::IsPartial) is
        * passed to this function it will leave with @p KErrArgument.
        *
        * Note that any updates in the database made since the landmark
        * object was read from the database will be overwritten by this
        * operation.
        *
        * A landmark can contain the IDs of the categories it belongs to. If
        * any of these categories does not exist in the database, the update
        * operation will still complete successfully but the category which
        * was not found will be ignored.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @param[in] aLandmark The new landmark data.
        *
        * @leave KErrArgument A partial landmark is passed.
        * @leave KErrAccessDenied The database is read-only.
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual void UpdateLandmarkL(
               const CPosLandmark&  aLandmark
        ) = 0;

        /**
        * Removes a landmark from the database.
        *
        * @pre Database is initialized (see @ref IsInitializingNeeded).
        *
        * If the landmark does not exist in the database, nothing happens.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @param aLandmarkId The ID of the landmark to remove.
        *
        * @leave KErrAccessDenied The database is read-only.
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual void RemoveLandmarkL(
               TPosLmItemId  aLandmarkId
        ) = 0;

        /**
        * Removes a set of landmarks from the database.
        *
        * @pre Database is initialized (see @ref IsInitializingNeeded).
        *
        * If any of the specified landmarks don't exist in the database,
        * nothing happens for those landmarks.
        *
        * The function returns an operation object which can be run in
        * incremental mode. If it is run incrementally the client can supervise
        * the progress of the operation.
        *
        * If the @ref CPosLmOperation object is deleted before the operation
        * is complete, it is possible that only a subset of the landmarks
        * have been deleted.
        *
        * The client takes ownership of the returned operation object.
        *
        * While removing landmarks, this operation will acquire a write-lock on
        * the database.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @param[in] aLandmarkIdArray The IDs of the landmarks to remove.
        * @returns A handle to the operation.
        *
        * @leave KErrAccessDenied The database is read-only.
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual CPosLmOperation* RemoveLandmarksL(
               const RArray<TPosLmItemId>&  aLandmarkIdArray
        ) = 0;

        /**
        * Removes all landmarks from the database.
        *
        * @pre Database is initialized (see @ref IsInitializingNeeded).
        *
        * The function returns an operation object which can be run in
        * incremental mode. If it is run incrementally the client can supervise
        * the progress of the operation.
        *
        * If the @ref CPosLmOperation object is deleted before the operation
        * is complete, it is possible that only a subset of the landmarks
        * have been deleted.
        *
        * The client takes ownership of the returned operation object.
        *
        * While removing landmarks, this operation will acquire a write-lock on
        * the database.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @returns A handle to the operation.
        *
        * @leave KErrAccessDenied The database is read-only.
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual CPosLmOperation* RemoveAllLandmarksL() = 0;

        /**
        * Returns the partial read parameters for this database handle.
        *
        * Partial read parameters are used to define which landmark data should
        * be returned when @ref ReadPartialLandmarkLC is called.
        *
        * The client takes ownership of the returned parameter object.
        *
        * @returns The current partial read parameters.
        */
        virtual CPosLmPartialReadParameters* PartialReadParametersLC() = 0;

        /**
        * Sets the partial read parameters for this database handle.
        *
        * Partial read parameters are used to define which landmark data should
        * be returned when @ref ReadPartialLandmarkLC is called.
        *
        * @param[in] aPartialSettings The new partial read parameters.
        */
        virtual void SetPartialReadParametersL(
               const CPosLmPartialReadParameters&  aPartialSettings
        ) = 0;

        /**
        * Reads partial data from a landmark in the database.
        *
        * @pre Database is initialized (see @ref IsInitializingNeeded).
        *
        * Partial settings define which landmark data should be returned in
        * this call. Partial read attributes are defined by calling
        * @ref SetPartialReadParametersL. If no partial read parameters have
        * been set, the landmarks will not contain any attributes.
        *
        * The client takes ownership of the returned landmark object.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param The ID of the landmark to read.
        * @returns The requested landmark. The landmark object will only
        *   contain the attributes defined in the partial read attributes. The
        *   landmark object is put on the cleanup stack.
        *
        * @leave KErrNotFound The requested landmark does not exist in the database.
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual CPosLandmark* ReadPartialLandmarkLC(
               TPosLmItemId  aLandmarkId
        ) = 0;

        /**
        * Reads partial data from a set of landmarks in the database.
        *
        * @pre Database is initialized (see @ref IsInitializingNeeded).
        *
        * Partial settings define which landmark data should be read. Partial
        * read attributes are defined by calling @ref SetPartialReadParametersL.
        * If no partial read parameters have been set, the landmarks will not
        * contain any attributes.
        *
        * Note that the returned data may be very big if all attributes in
        * each landmark are requested. A typical use for this function is to
        * retrieve the names for a set of landmarks.
        *
        * When the request is completed, the result can be retrieved by calling
        * @ref TakePreparedPartialLandmarksL.
        *
        * The function returns an operation object which can be run in
        * incremental mode. If it is run incrementally the client can supervise
        * the progress of the operation.
        *
        * The client takes ownership of the returned operation object.
        *
        * While preparing landmark information, this operation will acquire a
        * read-lock on the database.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param[in] aLandmarkIdArray An array with IDs of the landmarks to read.
        * @returns A handle to the operation.
        *
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual CPosLmOperation* PreparePartialLandmarksL(
               const RArray<TPosLmItemId>&  aLandmarkIdArray
        ) = 0;

        /**
        * Fetches the result from a call to @ref PreparePartialLandmarksL.
        *
        * @pre A call to this function must be preceeded by successful call to
        *   @ref PreparePartialLandmarksL.
        *
        * The returned array will have the same length as the ID array passed
        * in the @ref PreparePartialLandmarksL call and it will have the same
        * order.
        *
        * If reading a landmark failed during preparation, the corresponding
        * pointer value in the returned array will be NULL. For instance,
        * reading can fail if the specified ID does not exist in the database.
        *
        * The client takes ownership of the returned array object including
        * the contained landmark objects.
        *
        * @param[in] aPreparePartialLandmarkOperation The operation object returned
        *   by the @ref PreparePartialLandmarksL function.
        *
        * @returns An array containing the prepared partial landmark objects.
        *
        * @leave KErrNotFound @ref PreparePartialLandmarksL hasn't been called yet or
        *   it didn't succeed or this function has been called already since then.
        */
        virtual CArrayPtr<CPosLandmark>* TakePreparedPartialLandmarksL(
               CPosLmOperation*  aPreparePartialLandmarkOperation
        ) = 0;

        /**
        * Returns a URI which points to the landmark database storage.
        *
        * The URI may point to a file in the terminal file system or on a
        * remote file.
        *
        * The client takes ownership of the returned descriptor.
        *
        * @returns The database URI. The URI is put on the cleanup stack.
        */
        virtual HBufC* DatabaseUriLC() = 0;

        /**
        * Returns size information for the database.
        *
        * This function requires @p ReadUserData capability.
        *
        * @returns Size information for the database.
        */
        virtual TSize SizeL() = 0;

        /**
        * Compacts the landmark database.
        *
        * @pre Database is initialized (see @ref IsInitializingNeeded).
        *
        * Compaction means that any unused space in the database is removed.
        *
        * The function returns an operation object which can be run in
        * incremental mode. If it is run incrementally the client can supervise
        * the progress of the operation.
        *
        * The client takes ownership of the returned operation object.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @returns A handle to the operation.
        *
        * @leave KErrAccessDenied The database is read-only.
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual CPosLmOperation* CompactL() = 0;

        /**
        * Listens for database events.
        *
        * This function is asynchronous and it will complete the request
        * status when an event occurs. At this time @p aEvent input parameter
        * is updated and the client can read event information from it.
        *
        * Event listening can be cancelled by calling
        * @ref CancelNotifyDatabaseEvent.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param[out] aEvent Upon completion contains the event information.
        * @param[out] aStatus Upon completion contains status of the request.
        *   @p KErrNotSupported if events are not supported.
        *   @p KErrNone if an event occured, otherwise an error code if some error was encountered.
        *
        * @panic "Landmarks Client"-EPosEventNotifierAlreadyHasOutstandingRequest
        *   Client already has an outstanding @ref NotifyDatabaseEvent request.
        */
        virtual void NotifyDatabaseEvent(
           TPosLmEvent& aEvent,
           TRequestStatus& aStatus
        ) = 0;

        /**
        * Cancels a call to @ref NotifyDatabaseEvent.
        *
        * This function requires @p ReadUserData capability.
        *
        * @return @p KErrNone if the request was successfully cancelled,
        *   otherwise a system wide error code.
        */
        virtual TInt CancelNotifyDatabaseEvent() = 0;

        /**
        * Exports a number of landmarks.
        *
        * @pre Database is initialized (see @ref IsInitializingNeeded).
        *
        * To export a set of landmarks, the client must first create a
        * @ref CPosLandmarkEncoder object for the landmark content format in
        * which the landmarks should be encoded. The client can add some
        * information of the landmark collection in the encoder as well.
        *
        * The client must also provide a list of the landmarks which should be
        * exported. If one of the landmarks are not found in the database, the returned
        * operation fails with error code @p KErrNotFound.
        *
        * The client does not have to add any landmarks to the encoder object.
        * This function will add the ones specified in the ID array.
        *
        * The function returns an operation object which can be run in
        * incremental mode. If it is run incrementally the client can supervise
        * the progress of the operation.
        *
        * If the @ref CPosLmOperation object is deleted before the operation
        * is complete, it is possible that only a subset of the landmarks have
        * been exported.
        *
        * The client takes ownership of the returned operation object.
        *
        * When all landmarks have been exported the client must finalize
        * encoding by calling @ref CPosLandmarkEncoder::FinalizeEncodingL.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param[in] aLandmarkEncoder A landmark encoder object.
        * @param[in] aLandmarkIdArray The landmarks which should be exported.
        * @param[in] aTransferOptions A bitmap representing the options for the
        *   export operation. The bitmap values are defined by
        *   @ref _TTransferOptions.
        *
        * @returns A handle to the operation.
        *
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        *
        * @panic "Landmarks Client"-EPosLmInvalidArgument
        *   Client specified invalid transfer option values.
        */
        virtual CPosLmOperation* ExportLandmarksL(
               CPosLandmarkEncoder&  aLandmarkEncoder,
               const RArray<TPosLmItemId>&  aLandmarkIdArray,
               TTransferOptions  aTransferOptions
        ) = 0;

        /**
        * Import a set of landmarks.
        *
        * @pre Database is initialized (see @ref IsInitializingNeeded).
        *
        * To import landmark content, the client must first create a parser
        * object which can parse the landmark content. The client does not have
        * to call @ref CPosLandmarkParser::ParseContentL first. If the content
        * is not already parsed, this will be handled by the import operation.
        *
        * The function returns an operation object which can be run in
        * incremental mode. If it is run incrementally the client can supervise
        * the progress of the operation.
        *
        * If the @ref CPosLmOperation object is deleted before the operation
        * is complete, it is possible that only a subset of the landmarks have
        * been imported.
        *
        * The client takes ownership of the returned operation object.
        *
        * After completion @ref ImportedLandmarksIteratorL can be called to
        * retrieve the IDs of the added landmarks.
        *
        * The @p NextStep function in the operation object cannot be executed
        * synchronously using @p User::WaitForRequest. Doing so may cause the
        * operation to hang. @p NextStep must be run using an active object
        * for this operation.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @param[in] aLandmarkParser An object which can parse landmark content.
        * @param[in] aTransferOptions A bitmap representing the options for the
        *   import operation. The bitmap values are defined by
        *   @ref _TTransferOptions.
        *
        * @returns A handle to the operation.
        *
        * @leave KErrAccessDenied The database is read-only.
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        *
        * @panic "Landmarks Client"-EPosLmInvalidArgument
        *   Client specified invalid transfer option values.
        */
        virtual CPosLmOperation* ImportLandmarksL(
               CPosLandmarkParser&  aLandmarkParser,
               TTransferOptions  aTransferOptions
        ) = 0;

        /**
        * Import a set of landmarks.
        *
        * @pre Database is initialized (see @ref IsInitializingNeeded).
        *
        * To import landmark content, the client must first create a parser
        * object which can parse the landmark content. The client does not have
        * to call @ref CPosLandmarkParser::ParseContentL first. If the content
        * is not already parsed, this will be handled by the import operation.
        *
        * In this overload of the function, the client can pass an array
        * defining a subset of the landmarks in the parser object. This way
        * the client can select to import only a part of the landmark content.
        *
        * The function returns an operation object which can be run in
        * incremental mode. If it is run incrementally the client can supervise
        * the progress of the operation.
        *
        * If the @ref CPosLmOperation object is deleted before the operation
        * is complete, it is possible that only a subset of the landmarks have
        * been imported.
        *
        * The client takes ownership of the returned operation object.
        *
        * After completion @ref ImportedLandmarksIteratorL can be called to
        * retrieve the IDs of the added landmarks.
        *
        * The @p NextStep function in the operation object cannot be executed
        * synchronously using @p User::WaitForRequest. Doing so may cause the
        * operation to hang. @p NextStep must be run using an active object
        * for this operation.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @param[in] aLandmarkParser An object which can parse landmark content.
        * @param[in] aLandmarkSelection An array defining which of the parsed
        *   landmarks to import. The array items refer to the order in which
        *   @ref CPosLandmarkParser accesses the landmarks. 0 means the first
        *   parsed landmark, 1 means the second, etc. If the parser supports
        *   indexing, these numbers correspond to the index values used to
        *   access the landmarks in @ref CPosLandmarkParser::LandmarkLC. The
        *   index values must be less than the number of landmarks accessed by
        *   the parser, otherwise the operation object will panic with error
        *   code @p EPosInvalidIndex during execution. @p Note: The indexes
        *   can be used regardless of whether the parser supports indexing or
        *   not.
        * @param aTransferOptions A bitmap representing the options for the
        *   import operation. The bitmap values are defined by
        *   @ref _TTransferOptions.
        * @returns A handle to the operation.
        *
        * @leave KErrAccessDenied The database is read-only.
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        *
        * @panic "Landmarks Client"-EPosLmInvalidArgument
        *   Client specified invalid transfer option values.
        */
        virtual CPosLmOperation* ImportLandmarksL(
               CPosLandmarkParser&  aLandmarkParser,
               const RArray<TUint>&  aLandmarkSelection,
               TTransferOptions  aTransferOptions
        ) = 0;

        /**
        * Returns an object for iterating the landmarks added in an import
        * operation.
        *
        * To import landmarks @ref ImportLandmarksL is used.
        *
        * If @ref ImportedLandmarksIteratorL is called before the
        * @ref ImportLandmarksL operation has completed, the iterator will
        * iterate the landmarks imported so far. Landmarks imported after
        * the iterator is obtained will not affect the iterator. A new
        * iterator must be obtained to iterate these new landmarks.
        *
        * The client takes ownership of the returned iterator object.
        *
        * @param[in] aImportOperation This object is returned by
        *   @ref ImportLandmarksL methods.
        * @returns An object for iterating the landmarks added in the latest
        *   import operation.
        */
        virtual CPosLmItemIterator* ImportedLandmarksIteratorL(
               CPosLmOperation*  aImportOperation
        ) = 0;

        /**
        * @internal */
        /*
        * Returns the database access implementation ID.
        *
        * @returns The @ref CPosLandmarkDatabase implementation ID.
        */
        TUid ImplementationId() const;

    protected:

        // C++ constructor.
        IMPORT_C CPosLandmarkDatabase();

    private:

        // Prohibit copy constructor
        CPosLandmarkDatabase( const CPosLandmarkDatabase& );
        // Prohibit assigment operator
        CPosLandmarkDatabase& operator= ( const CPosLandmarkDatabase& );

    protected:

        // Implementation Uid
        TUid iDtorIdKey;
    };

#endif      // CPOSLANDMARKDATABASE_H


