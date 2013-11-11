/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Sub-session for SAT services.
*
*/

#ifndef RSATSERVICE_H
#define RSATSERVICE_H

//  INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class CFbsBitmap;
class RSatSession;
class TSatIconInfo;
class TSatInternalIconInfo;
class CSatPluginMonitor;

// DATA TYPES

/**
 * Array containing the icon information
 */
typedef RArray<TSatIconInfo> RIconEf;


// CLASS DECLARATION

/**
 *  <b>Sub-session for SAT services to SIM</b>
 *
 *  In order to use SAT Service API, services of an another API is needed:
 *  <ul>
 *  <li><b>SAT Client API</b>        \n For connecting SAT Session <BR>
 *  </ul>
 *
 *  Idle mode text client needs to monitor for idle mode content 
 *  changes and call SetIdleModeTextResponse()
 *  if content is updated.
 *  Result needs to be reported also if there already is  
 *  initial content after startup, when change notification has not been 
 *  requested yet.
 *  Example sequence for Idle mode client:
 *  1. Startup
 *  2. Request notification for idle mode content updates from.
 *  3. Try to get initial idle mode text.
 *  4. Use initial content, if there was some.
 *  5. Report result if there was content.
 *  Later:
 *  6. Idle mode text change notification request is completed.
 *  7. Renew notification request.
 *  8. Read new content.
 *  9. Use new content.
 *  10. Report result.
 *
 *  @code
 *  // How to connect to SAT service :
 *  // How to request notification on SetupIdleModeText change:
 *
 *  RSatSession satSession;
 *  RSatService satService;
 *  // Connect session
 *  satSession.ConnectL();
 *  satService.OpenL( satSession );
 *  // Start observing asynchronously for changes
 *  satService.NotifySetupIdleModeTextChange( status );
 *  
 *  //How to read Idle Mode Text and icon:
 *
 *  // Read text and icon after startup and when notified about change.
 *  TSatIconQualifier iconQualif( ESatIconNoIcon );
 *  TUint8 recordId( 0 );
 *  TInt error( KErrNone );
 *  HBufC* text = NULL;
 *  satService.GetSetupIdleModeTextL( 
 *    text, iconQualif, recordId );
 *  CFbsBitmap* icon = NULL;
 *  if ( text )
 *      {
 *      // Use text, take care of cleanup.
 *      }
 *  if ( ESatIconNoIcon != iconQualif )
 *      {
 *      // There's an icon to be displayed.
 *      // Get icon records into array.
 *      RIconEf iconInfos;
 *      CleanupStack::PushL( TCleanupItem( 
 *          LocalArrayCleanupFunction, iconInfos )
 *      satService.GetIconInfoL( recordId, iconInfos );
 *
 *      // Resolve which icon to use using icon infos.
 *      // Then get the icon you selected.
 *      icon = satService.GetIconL( iconInfos[ 0 ] );
 *      CleanupStack::PopAndDestroy(); // iconInfos
 *
 *      if ( ESatIconSelfExplanatory == iconQualif )
 *          {
 *          // Only icon should be displayed.
 *          }
 *      else if ( ESatIconNotSelfExplanatory == iconQualif )
 *          {
 *          // Both text and icon should be displayed.
 *          }
 *      }
 *  // else: only text should be displayed
 *  
 *  // Display text and/or icon, take care of their cleanup.
 *
 *  @endcode
 *
 *
 *  @lib SatClient.lib
 *  @since 5.1
 */
class RSatService : public RSubSessionBase
    {

    public:  // Enumerations
        
        /**
         * Result options for idle mode text setup.
         */
        enum TSATIdleResult
            {
            
            /** 
             * Text and Icon were set up successfully
             */
            ESATIdleSuccess = 0x00,
            
            /** 
             * Text was set up successfully, but requested icon was not 
             * displayed
             */
            ESATIdleSuccessRequestedIconNotDisplayed = 0x04,
            
            /**
             * Text could not be displayed - temporary problem
             */
            ESATIdleMeUnableToProcessCmd = 0x20,
            
            /**
             * Text could not be displayed - permanent problem
             */
            ESATIdleCmdBeyondMeCapabilities = 0x30
            };
            
        /**
         * Specifies icon role.
         */
        enum TSatIconQualifier
            {
            
            /** 
             * Icon is not available.
             */
            ESatIconNoIcon = 1,
            
            /**
             * Icon is self-explanatory, 
             * i.e. if displayed, it replaces the text string.
             */
            ESatIconSelfExplanatory,
            
            /**
             * Icon is not self-explanatory, 
             * i.e. if displayed, it shall be displayed together with 
             * the text string.
             */
            ESatIconNotSelfExplanatory
            };

    public:  // Constructors

        /**
         * C++ default constructor.
         */
        IMPORT_C RSatService();

    public: // New functions

        /**
         * Opens the sub-session.
         *
         * @since 5.1
         * @param aSession Sub-session.
         * @leave Error value returned by RSubSessionBase::CreateSubSession.
         */
        IMPORT_C void OpenL( const RSatSession& aSession );

        /**
         * Closes the sub-session.
         * 
         * @since 5.1
         */
        IMPORT_C void Close();
        
        /**
         * Requests notification when idle mode text or image are
         * updated.
         *
         * Idle client needs to monitor for setup idle mode content updates.
         * When a request is completed, the Idle client 
         * can read new content using GetSetupIdleModeTextL and must 
         * report idle mode content displaying result using
         * SetIdleModeTextResponse().
         *
         * @since 5.1
         * @param aStatus Request to be completed on update.
         * @return KErrNone or a system-wide error code. 
         * On an error request is not accepted so don't set requester active.      
         */
        IMPORT_C TInt NotifySetupIdleModeTextChange( 
            TRequestStatus& aStatus );
            
        /**
         * Cancels notification request for setup idle mode 
         * text or image change.
         *        
         * @since 5.1
         */
        IMPORT_C void NotifySetupIdleModeTextChangeCancel();
            
        /**
         * Get idle mode text and icon.
         *
         * Icon and text should be displayed depending on how aIconQualifier
         * specifies icon role. Result must be reported using  
         * RSatService::SetIdleModeTextResponse().
         * Notice: result needs to be reported also if content is available 
         * already after startup when client requests notification for the 
         * first time.
         * If unable to display icon, then only text can be displayed (adjust
         * reported result value).
         *
         * @since 5.1
         * @param aText New text, if available. Ownership transferred.
         * NULL if no text to display.
         * @param aIconQualifier Specifies if icon, text or both are preferred.
         * @param aRecordNumber Record id for icon.
         * @return Reserved for future use.
         * @leave KErrNotFound ESatIconNotSelfExplanatory icon without text.
         */
        IMPORT_C TBool GetSetupIdleModeTextL( 
            HBufC*& aText, 
            TSatIconQualifier& aIconQualifier,
            TUint8& aRecordNumber );        

        /**
         * Gets the information of icons from the SIM.
         *
         * @since 5.1
         * @param aRecordNumber Record number from proactive command.
         * @param aIconEf Contains the icon information.
         * @leave Error value returned by RSubSessionBase::SendReceive.
         * @leave KErrCorrupt if the info is invalid.
         */
        IMPORT_C void GetIconInfoL( TUint8 aRecordNumber, 
                                    RIconEf& aIconEf ) const;

        /**
         * Gets the bitmap of selected icon from SIM. Client should determine
         * the suitable icon using GetIconInfoL.
         *
         * @since 5.1
         * @param aIconInfo Information of the selected icon.
         * @leave Error value returned by RSubSessionBase::SendReceive.
         * @leave Error value returned by CFbsBitmap::Duplicate.
         * @leave Error value returned by RSubSessionBase::Send.
         * @return Bitmap of the icon. The client is responsible for deleting
         * the pointer.
         */
        IMPORT_C CFbsBitmap* GetIconL( const TSatIconInfo& aIconInfo ) const;

        /**
         * Sets SetUpIdleModeText response. 
         *
         * This must be called by the Idle client application after 
         * IdleModeText has been modified by SAT, meaning
         * when RSatService::NotifySetupIdleModeTextChange() is completed.
         * Also needs to be called if GetSetupIdleModeTextL() returned 
         * content and RSatService::NotifySetupIdleModeTextChange() 
         * notification was not requested yet.
         * 
         * @since 5.1
         * @param aResult Result of displaying idle mode text and icon.
         */
        IMPORT_C void SetIdleModeTextResponse( 
            const TSATIdleResult& aResult ) const;


    private: // New functions

        /**
         * Checks that the data from SIM is valid.
         * 
         * @since 5.1
         * @param aData Icon info to be checked.
         * @leave KErrCorrupt if the info is invalid.
         */
        void CheckInfoValidityL( const TDesC8& aData ) const;
 
        /**
         * Set ETSI specified bitmap data to TSatIconInfo.
         * 
         * @since 5.1
         * @param aContainer Client side Icon info.
         * @param aIconSatData RSat::TIcon data from server.
         */    
        void SetIconInfoL( TSatIconInfo& aContainer, 
                          const TDesC8& aIconSatData ) const;

        /**
         * Convert TSatIconinfo to TSatInternalIconInfo.
         * 
         * @since 5.1
         * @param aIconInfo user side icon info.
         * @param aInternalIconINfo sat side icon info.
         */                           
        void FillInternalIconInfo( const TSatIconInfo& aIconInfo, 
                 TSatInternalIconInfo& aInternalIconInfo ) const;                          

    private: 

        /** 
         * Prohibit copy constructor if not deriving from CBase. 
         */
        RSatService( const RSatService& );

        /**  
         * Prohibit assigment operator if not deriving from CBase. 
         */
        RSatService& operator=( const RSatService& );
        
    private: // Data  
        
        /**
         * Arguments for asynchronous message.
         */
        TIpcArgs iArgs;

        /**
         * Maximum support icon size.
         */        
        TInt iMaxIconDataSize;
        
        /**
         * SetupIdleMode content monitoring.
         * Own
         */   
        CSatPluginMonitor* iSatPluginMonitor;

    };

#endif // RSATSERVICE_H

// End of File
