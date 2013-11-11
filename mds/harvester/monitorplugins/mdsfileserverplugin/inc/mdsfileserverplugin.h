/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Monitors file creations, modifications and deletions.*
*/


#ifndef __MDSFILESERVERPLUGIN_H__
#define __MDSFILESERVERPLUGIN_H__

#include <f32plugin.h>
#include "mdsfileserverpluginclient.h"
#include "mdsfspqueue.h"

const TInt KDoubleMaxFileName = 2 * KMaxFileName;
/**
* File server plug-in.
*/
class CMdsFileServerPlugin : public CFsPlugin
	{
    public:

        /**
         * Standard NewL()
         * From CActive.
         */
        static CMdsFileServerPlugin* NewL();

        /**
         * Virtual destructor.
         */
        virtual ~CMdsFileServerPlugin();

        /**
         * Initialize plugin.
         */
        virtual void InitialiseL();

        virtual TInt DoRequestL( TFsPluginRequest& aRequest );

        virtual CFsPluginConn* NewPluginConnL();

        TInt RegisterNotification( CFsPluginConnRequest& aRequest );

        /**
         * Add a new notification path.
         */
        TInt AddNotificationPath( const CFsPluginConnRequest& aRequest );

        /**
         * Remove notification path.
         */
        TInt RemoveNotificationPath( const CFsPluginConnRequest& aRequest );

        /**
         * Add a new path to ignore.
         */
        TInt AddIgnorePath( const CFsPluginConnRequest& aRequest );

        /**
         * Remove a path from the ignore list.
         */
        TInt RemoveIgnorePath( const CFsPluginConnRequest& aRequest );

        void EnableL();

        void DisableL();

        /**
         * Add CMdsFileServerPluginConn connection to this file server plugin.
         */
        void AddConnection();
        
        /**
         * Remove CMdsFileServerPluginConn connection from this file server 
         * plugin.
         */
        void RemoveConnection();
        
    private:

        /**
         * Private constructor.
         */
        CMdsFileServerPlugin();

        /**
         * Check if a path is listed.
         * 
         * @param aFilename  Path to check.
         * 
         * @return EFalse, if path is ignored.
         *         ETrue, if the path is on the notification path list or
         *         if there is no notification paths set.
         *         Else EFalse.
         */
        TBool CheckPath( const TDesC& aFilename ) const;

        /**
         * Check if path/file hass hidden or system attribute.
         * 
         * @param aFilename  Filename to check.
         * @param aIsDirectory ETrue if filename points to directory.
         * @param aFsPlugin Reference to Fileserver Plugin.
         * @param aDelete Indicates if file deletion is in question.
         * 
         * @return EFalse, if path or file has a hidden or system attribute set.
         */
        TBool CheckAttribs( const TDesC& aFilename, 
                                        TBool& aIsDirectory, 
                                        RFsPlugin& aFsPlugin,
                                        TBool aDelete) const;

        /**
         * Check if directory is named correctly and ends with backslash.
         * 
         * @param aDirName  Directory name to check.
         */
		void AddBackslashIfNeeded ( TDes& aDirName ) const;
		
        /**
         * Check if harvester (main observer) is alive
         */
        TBool CheckHarvesterStatus();

        /**
         * Checks file entry path and attributes for validity
         * 
         * @param aFilename  Filename to check.
         * @param aIsDirectory ETrue if filename points to directory.
         * @param aRequest Reference to request from file server.
         * @param aDelete Indicates if file deletion is in question.
         * 
         * @return EFalse if file is not to be handled
         */
        TBool CheckIfValidFile( const TDesC& aFilename, 
                TBool& aIsDirectory, TFsPluginRequest& aRequest, TBool aDelete );
        
#ifdef _DEBUG_EVENTS
        void RegisterDebugEventsL();
        void UnregisterDebugEventsL();
        void PrintDebugEvents( TInt aFunction );
#endif
    	
        static TInt Compare(const TDesC& aFirst, const TDesC& aSecond);
        
    private:

        CFsPluginConnRequest* iNotification;

        /**
         * An array of notification paths.
         */
        RPointerArray<TDesC> iPaths;

        /**
         * A queue of file server events.
         */
        RPointerArray<CMdsFSPQueueItem> iQueue;

        RPointerArray<TDesC> iIgnorePaths;
        
        RPointerArray<TDesC> iCreatedFiles;
        RPointerArray<TDesC> iModifiedFiles;
        
        TUint32 iFormatOldMediaId;
        TInt iFormatDriveNumber;
        
        TInt iConnectionCount;
        
        TBuf<KDoubleMaxFileName> iFileName;
        TBuf<KDoubleMaxFileName> iNewFileName;
        
        TInt iPendingEvents;
        
    };

#endif // __MDSFILESERVERPLUGIN_H__
