/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface for fetching/saving the vmbx entries.
*                Also notify on a number change is supported.
*  Interface   : External, CVoiceMailbox
*
*/

#ifndef C_VOICEMAILBOX_H
#define C_VOICEMAILBOX_H

//  INCLUDES
#include <voicemailboxdefs.h>

// FORWARD DECLARATIONS
class MVoiceMailboxObserver;
class CVoiceMailboxImpl;
class CVoiceMailboxEntry;

/**
 *  Parameter package for Voice Mailbox API operations.
 *
 *  @lib vmbxengine.lib
 */
class TVoiceMailboxParams
    {
public:
    /**
     * Version of parameter package.
     */
    IMPORT_C TInt Version();

    /**
     * Default constructor.
     */
    IMPORT_C TVoiceMailboxParams();

    /**
     * Constructor for CVoiceMailboxEntry
     */
    IMPORT_C TVoiceMailboxParams( const CVoiceMailboxEntry& aEntry );

public:

    /**
     * Mailbox type.
     */
    TVmbxType iType;

    /**
     * Mailbox service id.
     */
    TServiceId iServiceId;

    /**
     * Mailbox line type.
     * Initialized to EVmbxAlsLineDefault.
     */
    TVmbxAlsLineType iLineType;

protected:

    /**
     * TVoiceMailboxParams package version.
     */
    TInt iVersion;

};

/**
 *  CVoiceMailbox offers an interface for fetching and saving
 *  voice mailbox entries.
 *  The API offers also services for querying a new number or to
 *  change an existing one.
 *
 *  @code
 *
 *  In this example user selects type of a mailbox to use. Then number
 *  of selected mailbox is fetched. If number wasn't set, then user
 *  is queried to enter a number for the selected mailbox.
 *
 *  // Creates instance of CVoiceMailbox
 *  CVoiceMailbox* voicemailbox = CVoiceMailbox::NewLC();
 *
 *  // Selected mailbox type
 *  TVoiceMailboxParams vmbxParams;
 *
 * //Launch type selection query
 *  TInt error = voicemailbox->QueryVmbxType( vmbxParams );
 *
 *  CVoiceMailboxEntry* vmbxEntry = NULL;
 *  
 *  if ( KErrNone == error )
 *      {   
 *      if ( KErrNone == 
 *              voicemailbox->GetStoredEntry( vmbxParams, vmbxEntry ) )
 *          {            
 *          // Number retrieved succesfully, do appropriate tasks, e.g.:
 *          TPtrC ptrNumber( KNullDesC );
 *          TInt numError = vmbxEntry->GetVmbxNumber( ptrNumber );
 *          if ( KErrNone == numError && ptrNumber.Length() )
 *              {
 *              // Do appropriate tasks, e.g dial a call.
 *              } 
 *          }
 *      } 
 *  else if ( KErrNotFound == error )
 *      {
 *      // No number defined, query new entry from user
 *      error = voicemailbox->QueryNewEntry( vmbxParams, vmbxEntry );
 *      if ( KErrNone == error )
 *          {
 *          // Do appropriate tasks, e.g. save and dial a call.
 *          error = voicemailbox->SaveEntry( *vmbxEntry ); 
 *          // For example get the number/address using 
 *          // vmbxEntry->GetVmbxNumber() and then dial a call
 *          }
 *      // else: problem getting the new number/address, e.g. skip
 *      }
 * 
 *  else
 *      {
 *      // User cancelled or no supported mailboxes configured
 *      }
 *       
 *  delete vmbxEntry; // Entry ownership was transferred
 *  vmbxEntry = NULL;     
 *  CleanupStack::PopAndDestroy( voicemailbox );
 *  voicemailbox = NULL;
 *  @endcode
 *
 *  @lib vmbxengine.lib
 *  @since S60 v5.2
 */

class CVoiceMailbox : public CBase
    {
public:

    /**
     * Static constructor.
     * @return New instance of the object. Ownership transferred.
     */
    IMPORT_C static CVoiceMailbox* NewL();

    /**
     * Static constructor.
     * @return New instance of the object. Ownership transferred.
     */
    IMPORT_C static CVoiceMailbox* NewLC();

    /**
     * Destructor.
     */
    virtual ~CVoiceMailbox();

public:

    /**
     * Retrieves voice mailbox entry for the specified service.
     *
     * @param in Params Service id and type of the mailbox for which
     *          a number/address should be defined.
     * @param out aEntry If successful, a new result container is
     *  created and ownership passed to the caller.
     * @return - KErrNone if an entry is found.
     *         - KErrNotFound if no entry available
     *         - KErrArgument when illegal argument
     *         - Other system wide errors.
     */
    IMPORT_C virtual TInt GetStoredEntry(
                            const TVoiceMailboxParams& aParams,
                            CVoiceMailboxEntry*& aEntry ) const;

    /**
     * Saves the vmbx entry to its storage.
     *
     * Some mailbox types may not support saving all arguments.
     * In that case unsupported arguments are ignored.
     * This method may display related UI notes or queries.
     *
     * @param in aEntry The vmbx entry to save.
     * @return - KErrNone if the entry was saved
     *         - KErrNotSupported if not capable or allowed to execute this
     *              for the specified mailbox service.
     *         - KErrArgument When illegal argument was passed.
     *         - Other system wide errors.
     */
    IMPORT_C virtual TInt SaveEntry( const CVoiceMailboxEntry& aEntry );

    /**
     * Displays a query asking the user to define a mailbox number/address.
     * Returns the input which the user entered, but does not save it.
     * For that caller has to use SaveEntry().
     * This method may display related UI notes or queries.
     *
     * If editing for the specified mailbox service is not allowed, 
     * this method displays an error note and returns an error.
     * If client is interested, CheckConfiguration()
     * can be used to check permissions.
     *
     *
     * @param in aParams Service id and type of the mailbox for which
     *          a number/address should be defined.
     * @param out aEntry If successful, a new result container is
     *  created and ownership passed to the caller.
     * @return - KErrNone if the number was entered.
     *         - KErrNotFound if the number is not defined
     *                        (user entered no number or selected Cancel)
     *         - KErrArgument When illegal argument.
     *         - KErrNotSupported if not capable or allowed to execute this
     *              for the specified mailbox.
     *         - Other system wide errors.
     */
    IMPORT_C virtual TInt QueryNewEntry(
                                    const TVoiceMailboxParams& aParams,
                                    CVoiceMailboxEntry*& aEntry );

    /**
     * Displays a query asking the user to change a mailbox number/address.
     * Returns the input from user, but does not save it.
     * For that caller has to use SaveEntry().
     *
     * If editing for the specified mailbox service is not allowed, 
     * this method displays an error note and returns an error.
     * If client is interested, CheckConfiguration()
     * can be used to check permissions.
     *
     * @param in aParams Service id and type of the mailbox.
     * @param out aEntry If successful, a new result container is
     *  created and ownership passed to the caller.
     * @return - KErrNone if the number was entered.
     *           aNumber will contain the vmbx number.
     *         - KErrCancel If the user selected Cancel.
     *         - KErrArgument When illegal argument.
     *         - KErrNotSupported if not capable or allowed to execute this
     *              for the specified mailbox.
     *         - Other system wide errors.
     */
    IMPORT_C virtual TInt QueryChangeEntry( 
            const TVoiceMailboxParams& aParams, CVoiceMailboxEntry*& aEntry );

    /**
     * Displays a query on the screen asking the user to select a
     * Voice Mailbox service.
     *
     * @param out aParams If successful,
     *          contains selected mailbox info.
     * @return - KErrNone if successful.
     *         - KErrCancel if user selected Cancel
     *         - KErrNotFound if there is no defined voice number and
     *             user have selected the type to define or the default
     *             type given.
     *         - Other system wide errors.
     */
    IMPORT_C virtual TInt QueryVmbxType(
                            TVoiceMailboxParams& aParams );

    /**
     * Issues a notify request on a vmbx number/address change.
     *
     * The observer callback will be called whenever any vmbx number/address
     * has changed on those mailboxes which support observing.
     *
     * Only one notify request can be pending. Method will leave with KErrInUse
     *  if a second request is issued while one is active already.
     * Leaves if the notify request could not be served.
     *
     * @param in aObserver The observer for the notification.
     * @param in aNotifyOnActiveLineOnly if this is ETrue, notify events will
     *        be generated only if the vmbx number of the active ALS line is
     *        changed, not if the number of the other line is changed. When
     *        there is no ALS support, this has no effect ie. notification
     *        will be generated always.
     */
    IMPORT_C virtual void NotifyVmbxNumberChangeL(
                                 MVoiceMailboxObserver& aObserver,
                                 const TBool aNotifyOnActiveLineOnly );

    /**
     * Cancels a notify request on a vmbx number change.
     *
     */
    IMPORT_C virtual void NotifyVmbxNumberChangeCancel();

    /**
     * Checks the Voice Mailbox configuration to find out if a features
     * are disabled or enabled.
     *
     * @param in aParams Specifies which mailbox capabilities,
     *          the caller wants to check.
     * @param in aFlags Use TVmbxFeatureCapabilityFlag values 
     *          for this parameter.
     *          Specifies what features client wants to check.

     * @return True if feature(s) enabled.
     */
    IMPORT_C virtual TBool CheckConfiguration(
                                const TVoiceMailboxParams& aParams,
                                const TInt aFlags );


    /**
     * Checks the Voice Mailbox how many VoIP service Ids
     *
     * @param out aProfileIds
     * @return KErrNone if get service Ids successfully.
     */
    IMPORT_C virtual TInt GetServiceIds( RIdArray& aProfileIds ) const;

    /**
     * Saves provisioned entry to corresponding storage.
     *
     * Some mailbox types may not support saving all arguments.
     * In that case unsupported arguments are ignored.
     *
     * @param in aEntry The provisioned vmbx entry to save.
     * @return - KErrNone if the entry was saved
     *         - KErrNotSupported if not capable or allowed to execute this
     *              for the specified mailbox service.
     *         - KErrArgument When illegal argument was passed.
     *         - Other system wide errors.
     */
    IMPORT_C TInt SaveProvisionedEntry( const CVoiceMailboxEntry& aEntry );

protected:

    /**
     * Second phase constructor.
     *
     */
    void ConstructL();

    /**
     * Default constructor.
     *
     */
    CVoiceMailbox();

private: // data

    /**
     * Implementation of the client API.
     * Own.
     **/
    CVoiceMailboxImpl* iBody;
    };

#endif // C_VOICEMAILBOX_H
