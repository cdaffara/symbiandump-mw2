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


#include <f32pluginutils.h>
#include "mdsfileserverplugin.h"
#include "harvesterlog.h"
#include "mdsutils.h"

_LIT( KMdsFileServerPlugin, "MdsFileServerPlugin" );
_LIT( KExludedThumbPath, "\\_PAlbTN\\");
_LIT( KExludedMediaArtPath, "\\.mediaartlocal\\");

/* Server name */
_LIT( KHarvesterServerName, "HarvesterServer" );

const TInt KCleanQueueTreshold( 1000 );

//-----------------------------------------------------------------------------
// CMdsFileServerPlugin implementation
//-----------------------------------------------------------------------------
//
CMdsFileServerPlugin::CMdsFileServerPlugin()
: iFormatOldMediaId( 0 ), iFormatDriveNumber( -1 ), iConnectionCount( 0 )
	{
	WRITELOG( "CMdsFileServerPlugin::CMdsFileServerPlugin()" );
	}
	
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
//
CMdsFileServerPlugin::~CMdsFileServerPlugin()
    {
    WRITELOG( "CMdsFileServerPlugin::~CMdsFileServerPlugin()" );
    
#ifdef _DEBUG
    TRAPD( error, DisableL() );
    _LIT( KPanicCategory,"CMdsFileServerPlugin" );
    __ASSERT_DEBUG( error == KErrNone, User::Panic( KPanicCategory,  error ) );
#else
    TRAP_IGNORE( DisableL() );    
#endif
    
    iCreatedFiles.ResetAndDestroy();
    iCreatedFiles.Close();
    
    iModifiedFiles.ResetAndDestroy();
    iModifiedFiles.Close();
  
    iPaths.ResetAndDestroy();
    iPaths.Close();
    
    iIgnorePaths.ResetAndDestroy();
    iIgnorePaths.Close();
        
    iQueue.ResetAndDestroy();
    iQueue.Close();
    }
    
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
//
CMdsFileServerPlugin* CMdsFileServerPlugin::NewL()
    {
    WRITELOG( "CMdsFileServerPlugin::NewL()" );
    return new (ELeave) CMdsFileServerPlugin;
    }

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
//
void CMdsFileServerPlugin::InitialiseL()
    {
    WRITELOG( "CMdsFileServerPlugin::InitializeL()" );
    }
    
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
//
void CMdsFileServerPlugin::EnableL()
    {
    WRITELOG( "CMdsFileServerPlugin::EnableL()" );
    User::LeaveIfError( RegisterIntercept(EFsFileCreate, EPostIntercept) );
    User::LeaveIfError( RegisterIntercept(EFsFileRename, EPostIntercept) );
    User::LeaveIfError( RegisterIntercept(EFsRename, EPostIntercept) );
    User::LeaveIfError( RegisterIntercept(EFsDelete, EPostIntercept) );
    User::LeaveIfError( RegisterIntercept(EFsFileReplace, EPostIntercept) );
    User::LeaveIfError( RegisterIntercept(EFsReplace, EPostIntercept) );
    User::LeaveIfError( RegisterIntercept(EFsFileSetModified, EPostIntercept) );
    User::LeaveIfError( RegisterIntercept(EFsFileSubClose, EPostIntercept) );
    User::LeaveIfError( RegisterIntercept(EFsSetEntry, EPostIntercept) );
	// format events
	User::LeaveIfError( RegisterIntercept(EFsFormatSubClose, EPostIntercept) );
	User::LeaveIfError( RegisterIntercept(EFsFormatOpen, EPreIntercept) );

#ifdef _DEBUG_EVENTS
    RegisterDebugEventsL();
#endif
    }
    
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
//
void CMdsFileServerPlugin::DisableL()
    {
    WRITELOG( "CMdsFileServerPlugin::DisableL()" );
    User::LeaveIfError( UnregisterIntercept(EFsFileCreate, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept(EFsFileRename, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept(EFsRename, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept(EFsDelete, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept(EFsFileReplace, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept(EFsReplace, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept(EFsFileSetModified, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept(EFsFileSubClose, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept(EFsSetEntry, EPrePostIntercept) );
	// format events
	User::LeaveIfError( UnregisterIntercept(EFsFormatSubClose, EPostIntercept) );
	User::LeaveIfError( UnregisterIntercept(EFsFormatOpen, EPreIntercept) );

#ifdef _DEBUG_EVENTS
    UnregisterDebugEventsL();
#endif
    }
    
//-----------------------------------------------------------------------------
// AddConnection
//-----------------------------------------------------------------------------
//
void CMdsFileServerPlugin::AddConnection()
    {
    WRITELOG( "CMdsFileServerPlugin::AddConnection()" );
    
    ++iConnectionCount;
    iPendingEvents = 0;
    }

//-----------------------------------------------------------------------------
// RemoveConnection
//-----------------------------------------------------------------------------
//
void CMdsFileServerPlugin::RemoveConnection()
    {
    WRITELOG( "CMdsFileServerPlugin::RemoveConnection()" );

    --iConnectionCount;

    // remove notification request if this was last connection
    if( iConnectionCount <= 0 )
    	{
    	WRITELOG( "CMdsFileServerPlugin::RemoveConnection() last connection" );
    	
    	iNotification = NULL;
    	iPendingEvents = 0;
    	iQueue.ResetAndDestroy();
    	iQueue.Compress();
    	}
    }

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
//
TInt CMdsFileServerPlugin::DoRequestL( TFsPluginRequest& aRequest )
	{
	// ignore event if there are not any clients listening
	if( iConnectionCount <= 0 )
		{
		WRITELOG( "CMdsFileServerPlugin::DoRequestL() - no clients -> ignore event" );
		return KErrNone;
		}
	
	TInt function = aRequest.Function();
	
	iFileName.Zero();
    iNewFileName.Zero();
	
#ifdef _DEBUG_EVENTS
    PrintDebugEvents( function );
#endif
    
    if ( function == EFsFileSubClose && iCreatedFiles.Count() == 0 )
        {
#ifdef _DEBUG_EVENTS
        if( GetName( &aRequest, iFileName ) == KErrNone && iFileName.Length() <= KMaxFileName )
            {
            WRITELOG1("Dropping subclose %S",&iFileName);
            }
#endif
        return KErrNone;
        }
    
    const TBool formatFunction( function == EFsFormatOpen || function == EFsFormatSubClose );
        
    WRITELOG1( "----- CMdsFileServerPlugin::DoRequestL() - plugin function: %d -----", function );

	if ( !formatFunction )
		{
		if( GetName( &aRequest, iFileName ) != KErrNone || iFileName.Length() > KMaxFileName )
		    {
		    return KErrNone;
		    }
		else
		    {
		    WRITELOG1( "CMdsFileServerPlugin::DoRequestL() - fileName: %S", &iFileName );
		    }
		}

    // get process id
	TUid processId = aRequest.Message().SecureId();

	TBool isDirectory = EFalse;
	
    // if rename, check destination path
    if ( function == EFsRename  || function == EFsFileRename || 
    	 function == EFsReplace || function == EFsFileReplace  ||
    	 function == EFsSetEntry )
        {
        const TInt newNameErr = GetNewName( &aRequest, iNewFileName );
        if ( iNewFileName.Length() > KMaxFileName )
        	{
        	return KErrNone;
        	}
        WRITELOG2( "CMdsFileServerPlugin::DoRequestL() - newFileName: '%S' %d", &iNewFileName, newNameErr );
        TBool setToBeDeleted( EFalse );
        if ( newNameErr == KErrNone )
            {
            if ( !CheckPath(iNewFileName) )
                {
                WRITELOG( "CMdsFileServerPlugin::DoRequestL() - new path not supported" );
                if( !CheckPath(iFileName) )
                    {
                    WRITELOG( "CMdsFileServerPlugin::DoRequestL() - old path not supported" );
                    WRITELOG( "CMdsFileServerPlugin::DoRequestL() - ignore file" );
                    return KErrNone;
                    }
                // file moved to ignored path, delete from db
                function = EFsDelete;
                setToBeDeleted = ETrue;
                }
            
            if( !setToBeDeleted )
                {
                RFsPlugin fsPlugin( aRequest ); 
                const TInt rfsPluginErr = fsPlugin.Connect();
            
                if ( ( rfsPluginErr == KErrNone ) && !CheckAttribs( iNewFileName, isDirectory, fsPlugin, EFalse ) )
                    {
                    WRITELOG( "CMdsFileServerPlugin::DoRequestL() - new path attribute check failed" );
                    if( !CheckAttribs(iFileName, isDirectory, fsPlugin, EFalse) )
                        {
                        WRITELOG( "CMdsFileServerPlugin::DoRequestL() - old path attribute check failed" );
                        WRITELOG( "CMdsFileServerPlugin::DoRequestL() - ignore file" );
                        fsPlugin.Close();
                        return KErrNone;
                        }
                    // file set to hidden, delete from db
                    function = EFsDelete;
                    }
                fsPlugin.Close();
                }
            }
        else
        	{
            if( !CheckIfValidFile(iFileName, isDirectory, aRequest, EFalse)  )
                {
                return KErrNone;
                }
        	}
        }
    else if ( !formatFunction )
        {
        const TBool deleteFunction( function == EFsDelete );
        if( !CheckIfValidFile(iFileName, isDirectory, aRequest, deleteFunction)  )
            {
            return KErrNone;
            }
        }

    TInt fileEventType = EMdsFileUnknown;
    TInt drvNumber = aRequest.DriveNumber();
    TVolumeInfo volInfo;

    WRITELOG1( "CMdsFileServerPlugin::DoRequestL() - drive number: %d", drvNumber );

    TInt err( KErrNone );
    
    switch( function )
        {
        case EFsFileCreate:
            {
#ifdef _DEBUG            
            WRITELOG( "CMdsFileServerPlugin::DoRequestL() - EFsFileCreate" );
#endif            
            
            HBufC* createdFileName = iFileName.Alloc();
            if( createdFileName )
                {
                // Ownership transferred if succeeded
                const TInt appendError( iCreatedFiles.Append( createdFileName ) );
                if( appendError != KErrNone )
                    {
                    delete createdFileName;
                    createdFileName = NULL;
                    }
                }
            return KErrNone;
            }

        case EFsFileSubClose:
            {
            WRITELOG( "CMdsFileServerPlugin::DoRequestL() - EFsFileSubClose" );

            TBool found = EFalse;

           	for ( TInt i = iCreatedFiles.Count(); --i >= 0; )
            		{
           			if ( MdsUtils::Compare( iFileName, *(iCreatedFiles[i]) ) == 0 )
            		  {
            			fileEventType = EMdsFileCreated;
            			delete iCreatedFiles[i];
            			iCreatedFiles[i] = NULL;
            			iCreatedFiles.Remove( i );
            			
						//Have to check whether file has been hidden          			
            	        RFsPlugin fsPlugin( aRequest ); 
            	        const TInt rfsPluginError( fsPlugin.Connect() );
            	        if( (rfsPluginError == KErrNone) && CheckAttribs( iFileName, isDirectory, fsPlugin, EFalse ) )
            	            {
            	            found = ETrue;
            	            }
            	        else if( rfsPluginError != KErrNone )
            	            {
            	            found = ETrue;
            	            }
            	        fsPlugin.Close();
            			}
            		}
           	
           	if( iCreatedFiles.Count() == 0 )
				{
				iCreatedFiles.GranularCompress();
				}
            
            if ( !found )
                {
                return KErrNone;
                }
            }
            break;

        case EFsRename:
            {
            WRITELOG1( "CMdsFileServerPlugin::DoRequestL() - EFsRename, new file: %S", &iNewFileName );
            
            if ( isDirectory )
            	{
				WRITELOG( "CMdsFileServerPlugin::DoRequestL() - it is directory!" );			
				fileEventType = EMdsDirRenamed;
				AddBackslashIfNeeded( iNewFileName ); // new file name 
				AddBackslashIfNeeded( iFileName ); // old file name
            	}
            else
            	{
            	fileEventType = EMdsFileRenamed;
            	}
            }
            break;

        case EFsFileRename:
            WRITELOG1( "CMdsFileServerPlugin::DoRequestL() - EFsFileRename, new file: %S", &iNewFileName );
            fileEventType = EMdsFileRenamed;
            break;

        case EFsFileSetModified:
            {
            WRITELOG( "CMdsFileServerPlugin::DoRequestL() - EFsFileSetModified" );
            
            for ( TInt i = iCreatedFiles.Count(); --i >= 0; )
                {
                // If file has been created but not closed, do not handle modified events
                // for that file before the application that has created the file is done with writing 
                // it to the disk.
                if ( MdsUtils::Compare( iFileName, *(iCreatedFiles[i]) ) == 0 )
                    {
                    return KErrNone;
                    }
                } 

            HBufC* modifiedFileName = iFileName.Alloc();
            if( modifiedFileName )
                {
                // Ownership transferred if succeeded
                const TInt appendError( iModifiedFiles.Append( modifiedFileName ) );
                if( appendError != KErrNone )
                    {
                    delete modifiedFileName;
                    modifiedFileName = NULL;
                    }
                }
            
            fileEventType = EMdsFileModified;
            }
            break;

        case EFsSetEntry:
            {
            WRITELOG( "CMdsFileServerPlugin::DoRequestL() - EFsSetEntry" );

            TBool found = EFalse;

            for ( TInt i = iModifiedFiles.Count(); --i >= 0; )
                    {
                    if ( MdsUtils::Compare( iFileName, *(iModifiedFiles[i]) ) == 0 )
                      {
                        delete iModifiedFiles[i];
                        iModifiedFiles[i] = NULL;
                        iModifiedFiles.Remove( i );
                        found = ETrue;
                        }
                    }
            
            if( iModifiedFiles.Count() == 0 )
                {
                iModifiedFiles.GranularCompress();
                }
            
            if ( found )
                {
                return KErrNone;
                }
            
            fileEventType = EMdsFileAttribsChanged;
            }
            break;            
            
        case EFsFileReplace:
        case EFsReplace:
            WRITELOG1( "CMdsFileServerPlugin::DoRequestL() - EFsReplace/EFsFileReplace, new file: %S", &iNewFileName );
            fileEventType = EMdsFileReplaced;
            break;

        case EFsDelete:
            WRITELOG( "CMdsFileServerPlugin::DoRequestL() - EFsDelete" );
            fileEventType = EMdsFileDeleted;
            break;

		case EFsFormatOpen:
		    {
			WRITELOG( "CMdsFileServerPlugin::DoRequestL() - EFsFormatOpen" );
			// get the drive letter
		    RFsPlugin fsPlugin( aRequest ); 
		    err = fsPlugin.Connect();
	        if( err == KErrNone )
	            {
	            err = fsPlugin.Volume( volInfo, drvNumber );
	            }
	        fsPlugin.Close();
	        
			if( KErrNone == err )
				{
				iFormatOldMediaId = volInfo.iUniqueID;
				iFormatDriveNumber = drvNumber;
				fileEventType = EMdsDriveFormatted;
				processId.iUid = 0;
				}
			else
				{
				iFormatDriveNumber = -1;
				}
			return KErrNone;
		    }

		case EFsFormatSubClose:
			WRITELOG( "CMdsFileServerPlugin::DoRequestL() - EFsFormatSubClose" );
			if ( iFormatDriveNumber < 0 )
				{
				return KErrNone;
				}
			drvNumber = iFormatDriveNumber;
			volInfo.iUniqueID = iFormatOldMediaId;
			iFormatOldMediaId = 0;
			iFormatDriveNumber = -1;
			fileEventType = EMdsDriveFormatted;
			if ( processId.iUid == 0 )
				{
				processId.iUid = 1;
				}
			break;

        default:
            WRITELOG( "CMdsFileServerPlugin::DoRequestL() - Unknown function" );
            return KErrNone;
        }

    if ( iNotification )
        {
        WRITELOG( "CMdsFileServerPlugin::DoRequestL() - iNotification found" );
        TMdsFSPStatusPckg clientStatusBuf;
        TMdsFSPStatus& clientStatus = clientStatusBuf();
        clientStatus.iDriveNumber = drvNumber;
        clientStatus.iDriveMediaId = volInfo.iUniqueID;
        clientStatus.iFileName.Copy( iFileName );
        clientStatus.iNewFileName.Copy( iNewFileName );
        clientStatus.iFileEventType = fileEventType;
        clientStatus.iProcessId = processId;

        TRAP( err, iNotification->WriteParam1L(clientStatusBuf) );
        iNotification->Complete( err );
        iNotification = NULL;
        WRITELOG( "CMdsFileServerPlugin::DoRequestL() - iNotification complete" );
        }
    else // no notification ready, put in the queue
        {
        WRITELOG( "CMdsFileServerPlugin::DoRequestL() - iNotification not found. Put in the queue" );

        // Check if harvester is still alive
        if( CheckHarvesterStatus() )
            {
            CMdsFSPQueueItem* queueItem = CMdsFSPQueueItem::NewL();
            CleanupStack::PushL( queueItem );
            queueItem->iDriveNumber = drvNumber;
            queueItem->iDriveMediaId = volInfo.iUniqueID;
            queueItem->iFileName = iFileName.AllocL();
            queueItem->iNewFileName = iNewFileName.AllocL();
            queueItem->iFileEventType = fileEventType;
            queueItem->iProcessId = processId;

            iQueue.AppendL( queueItem ); // ownership is transferred
            CleanupStack::Pop( queueItem );
            err = KErrNone;
            iPendingEvents++;
            WRITELOG( "CMdsFileServerPlugin::DoRequestL() - added to queue" );
            }
        else
            {
            WRITELOG( "CMdsFileServerPlugin::DoRequestL() - Harvester process not detected" );
            iConnectionCount = 0;
            iPendingEvents = 0;
            iQueue.ResetAndDestroy();
            iQueue.Compress();
            }
        }
    
    WRITELOG( "CMdsFileServerPlugin::DoRequestL() - return" );
    return err;
    }

//-----------------------------------------------------------------------------
// CMdsFileServerPluginConn implementation
//-----------------------------------------------------------------------------
class CMdsFileServerPluginConn : public CFsPluginConn
    {
    public:
        static CMdsFileServerPluginConn* NewL( CMdsFileServerPlugin& aPlugin );
        virtual ~CMdsFileServerPluginConn();

        virtual TInt DoControl( CFsPluginConnRequest& aRequest );
        virtual void DoRequest( CFsPluginConnRequest& aRequest );
        virtual void DoCancel( TInt aReqMask );

    private:
    	CMdsFileServerPluginConn( CMdsFileServerPlugin& aPlugin );

    	CMdsFileServerPlugin& iPlugin;

        RMessage2* iMessage;
    };
    
/**
* Leaving New function for the plugin
* @internalComponent
*/
CMdsFileServerPluginConn* CMdsFileServerPluginConn::NewL( 
		CMdsFileServerPlugin& aPlugin )
    {
    WRITELOG( "CMdsFileServerPluginConn::NewL()" );
    return new (ELeave) CMdsFileServerPluginConn( aPlugin );
    }


/**
* Constructor for the plugin
* @internalComponent
*/
CMdsFileServerPluginConn::CMdsFileServerPluginConn( 
		CMdsFileServerPlugin& aPlugin ) : 
		iPlugin( aPlugin )
    {
    WRITELOG( "CMdsFileServerPluginConn::CMdsFileServerPluginConn()" );
    
    iPlugin.AddConnection();
    }


/**
* The destructor.
* @internalComponent
*/
CMdsFileServerPluginConn::~CMdsFileServerPluginConn()
    {
    WRITELOG( "CMdsFileServerPluginConn::~CMdsFileServerPluginConn()" );
    
    iPlugin.RemoveConnection();
    }

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
//
TInt CMdsFileServerPluginConn::DoControl( CFsPluginConnRequest& aRequest )
    {
    WRITELOG( "CMdsFileServerPluginConn::DoControl()" );
    TInt err = KErrNotSupported;
    
    CMdsFileServerPlugin& myPlugin = *(CMdsFileServerPlugin*)Plugin();
    
    switch( aRequest.Function() )
        {
        case EMdsFSPOpEnable:
            {
            WRITELOG( "CMdsFileServerPluginConn::DoControl() - EMdsFSPOpEnable" );
            TRAP( err, myPlugin.EnableL() );
            break;
            }
        
        case EMdsFSPOpDisable:
            {
            WRITELOG( "CMdsFileServerPluginConn::DoControl() - EMdsFSPOpDisable" );
            TRAP( err, myPlugin.DisableL() );
            break;
            }
        
        default:
            {
            WRITELOG( "CMdsFileServerPluginConn::DoControl() - Unknown Control" );
            break;            
            }
        }

    return err;
    }
    

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
//
void CMdsFileServerPluginConn::DoRequest( CFsPluginConnRequest& aRequest )
    {
    WRITELOG( "CMdsFileServerPluginConn::DoRequest()" );
    CMdsFileServerPlugin& myPlugin = *(CMdsFileServerPlugin*)Plugin();
    
    switch( aRequest.Function() )
        {
        case EMdsFSPOpRegisterNotification:
            {
            WRITELOG( "CMdsFileServerPluginConn::DoControl() - EMdsFSPOpRegisterNotification" );
            const TInt err = myPlugin.RegisterNotification( aRequest );
            
            if ( err != KErrNone )
                {
                aRequest.Complete( err );
                }
            break;
            }
            
        case EMdsFSPOpAddNotificationPath:
            {
            WRITELOG( "CMdsFileServerPluginConn::DoControl() - EMdsFSPOpAddNotificationPath" );
            const TInt err = myPlugin.AddNotificationPath( aRequest );
            aRequest.Complete( err );
            break;
            }
            
        case EMdsFSPOpRemoveNotificationPath:
            {
            WRITELOG( "CMdsFileServerPluginConn::DoControl() - EMdsFSPOpRemoveNotificationPath" );
            const TInt err = myPlugin.RemoveNotificationPath( aRequest );
            aRequest.Complete( err );
            break;
            }
            
        case EMdsFSPOpAddIgnorePath:
            {
            WRITELOG( "CMdsFileServerPluginConn::DoControl() - EMdsFSPOpAddIgnorePath" );
            const TInt err = myPlugin.AddIgnorePath( aRequest );
            aRequest.Complete( err );
            break;
            }
            
        case EMdsFSPOpRemoveIgnorePath:
            {
            WRITELOG( "CMdsFileServerPluginConn::DoControl() - EMdsFSPOpRemoveIgnorePath" );
            const TInt err = myPlugin.RemoveIgnorePath( aRequest );
            aRequest.Complete( err );
            break;
            }
        default:
            break;
        }
    }

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
//
void CMdsFileServerPluginConn::DoCancel( TInt /*aReqMask*/ )
    {
    WRITELOG( "CMdsFileServerPluginConn::DoCancel()" );
    iRequestQue.DoCancelAll( KErrCancel );
    }
    
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
//
CFsPluginConn* CMdsFileServerPlugin::NewPluginConnL()
    {
    WRITELOG( "CMdsFileServerPluginConn::NewPluginConnL()" );
    return CMdsFileServerPluginConn::NewL( *this );
    }
    
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
//
TInt CMdsFileServerPlugin::RegisterNotification( CFsPluginConnRequest& aRequest )
    {
    WRITELOG( "CMdsFileServerPlugin::RegisterNotification()" );
    
    if ( iNotification )
        {
        return KErrInUse;
        }
    
    iNotification = &aRequest;
    
    if ( iQueue.Count() > 0 )
        {
        WRITELOG( "CMdsFileServerPlugin::RegisterNotification() - item in queue" );
        
        CMdsFSPQueueItem* queueItem = iQueue[0];
        
        TMdsFSPStatusPckg pckg;
        TMdsFSPStatus& status = pckg();
        
        queueItem->GetAsFspStatus( status );

        TRAPD( err, iNotification->WriteParam1L(pckg) );
        iNotification->Complete( err );
        iNotification = NULL;
        
        delete queueItem;
        queueItem = NULL;
        iQueue.Remove( 0 );
        }
    else
    	{
    	iQueue.GranularCompress();
    	}
    
    return KErrNone;
    }
    
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
//
TInt CMdsFileServerPlugin::AddNotificationPath( const CFsPluginConnRequest& aRequest )
    {
    WRITELOG( "CMdsFileServerPlugin::AddNotificationPath()" );
    TInt err( KErrNone );
    
    TMdsFSPStatusPckg pckg;
    TRAP( err, aRequest.ReadParam1L(pckg) );
    
    if ( err != KErrNone )
        {
        return err;
        }
    
    TMdsFSPStatus& status = pckg();
    
    if ( status.iFileName.Length() > 0 )
        {
        // check if already exist
        const TInt ret = iPaths.FindInOrder(&status.iFileName,
                                     TLinearOrder<TDesC>(CMdsFileServerPlugin::Compare));

        if( ret >= 0 )
            {
            return KErrNone;
            }     

        WRITELOG1( "CMdsFileServerPlugin::AddNotificationPath() - add path: %S", &status.iFileName );
        HBufC* fn = status.iFileName.Alloc();
        if ( fn )
            {
            const TInt insertError( iPaths.InsertInOrder(fn, TLinearOrder<TDesC>(CMdsFileServerPlugin::Compare)));
            if( insertError != KErrNone )
                {
                delete fn;
                fn = NULL;
                }
            }
        else
            {
            err = KErrNoMemory;
            }
        }
    else
        {
        err = KErrNotFound;
        }
        
    return err;
    }
    
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
//
TInt CMdsFileServerPlugin::RemoveNotificationPath( const CFsPluginConnRequest& aRequest )
    {
    WRITELOG( "CMdsFileServerPlugin::RemoveNotificationPath()" );
    TInt err( KErrNone );
    
    TMdsFSPStatusPckg pckg;
    TRAP( err, aRequest.ReadParam1L(pckg) );
    
    if ( err != KErrNone )
        {
        return err;
        }
    
    TMdsFSPStatus& status = pckg();
    
    if ( status.iFileName.Length() > 0 )
        {
        const TInt ret = iPaths.FindInOrder(&status.iFileName,
                                     TLinearOrder<TDesC>(CMdsFileServerPlugin::Compare));

        if( ret >= 0 )
            {
            WRITELOG1( "CMdsFileServerPlugin::RemoveNotificationPath() - remove path: %S", &status.iFileName );
            delete iPaths[ret];
            iPaths[ret] = NULL;
            iPaths.Remove( ret );
            }    
        iPaths.Compress();
        }
    else
        {
        err = KErrNotFound;
        }
        
    return err;
    }
    
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
//
TInt CMdsFileServerPlugin::AddIgnorePath( const CFsPluginConnRequest& aRequest )
    {
    WRITELOG( "CMdsFileServerPlugin::AddIgnorePath()" );
    TInt err( KErrNone );
    
    TMdsFSPStatusPckg pckg;
    TRAP( err, aRequest.ReadParam1L(pckg) );
    
    if ( err != KErrNone )
        {
        return err;
        }
    
    TMdsFSPStatus& status = pckg();
    
    if ( status.iFileName.Length() > 0 )
        {
        // check if already exist
        const TInt ret = iIgnorePaths.FindInOrder(&status.iFileName,
                                     TLinearOrder<TDesC>(CMdsFileServerPlugin::Compare));
    
        if( ret >= 0 )
            {
            return KErrNone;
            }
            
        WRITELOG1( "CMdsFileServerPlugin::AddIgnorePath() - add path: %S", &status.iFileName );
        HBufC* fn = status.iFileName.Alloc();
        if ( fn )
            {
            const TInt insertError( iIgnorePaths.InsertInOrder(fn, TLinearOrder<TDesC>(CMdsFileServerPlugin::Compare))); 
            if( insertError != KErrNone )
                {
                delete fn;
                fn = NULL;
                }
            }
        else
            {
            err = KErrNoMemory;
            }
        }
    else
        {
        err = KErrNotFound;
        }
        
    return err;
    }
    
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
//
TInt CMdsFileServerPlugin::RemoveIgnorePath( const CFsPluginConnRequest& aRequest )
    {
    WRITELOG( "CMdsFileServerPlugin::RemoveIgnorePath()" );
    TInt err( KErrNone );
    
    TMdsFSPStatusPckg pckg;
    TRAP( err, aRequest.ReadParam1L(pckg) );
    
    if ( err != KErrNone )
        {
        return err;
        }
    
    TMdsFSPStatus& status = pckg();
    
    if ( status.iFileName.Length() > 0 )
        {
        const TInt ret = iIgnorePaths.FindInOrder(&status.iFileName,
                                     TLinearOrder<TDesC>(CMdsFileServerPlugin::Compare));

        if( ret >= 0 )
            {
            WRITELOG1( "CMdsFileServerPlugin::RemoveIgnorePath() - remove path: %S", &status.iFileName );
            delete iIgnorePaths[ret];
            iIgnorePaths[ret] = NULL;
            iIgnorePaths.Remove( ret );
            }    
        iIgnorePaths.Compress();
        }
    else
        {
        err = KErrNotFound;
        }
        
    return err;
    }
    
//-----------------------------------------------------------------------------
// CheckPath
//-----------------------------------------------------------------------------
//
TBool CMdsFileServerPlugin::CheckPath( const TDesC& aFilename ) const
    {
    // check if ignored pathlist   
#ifdef __WINSCW__
    // start checking from beginning of list as if the path is ignored, there
    // is higher possibility that it is located on beginning side of the list
    const TInt count( iIgnorePaths.Count() );
    for ( TInt i = 0; i < count; i++ )   
#else    
    for ( TInt i = iIgnorePaths.Count(); --i >= 0; )    
#endif
        {    
        TDesC* pathName = iIgnorePaths[i];
        if ( MdsUtils::Find( aFilename, *pathName ) != KErrNotFound )
            {
            return EFalse;
            }
        else if( MdsUtils::Find( aFilename, KExludedThumbPath ) != KErrNotFound )
            {
            return EFalse;
            }
        else if( MdsUtils::Find( aFilename, KExludedMediaArtPath ) != KErrNotFound )
            {
            return EFalse;
            }
        }

    // check if notification path
    if ( iPaths.Count() > 0 )
        {
#ifdef __WINSCW__
        // start checking from beginning of list as if the path is supported, there
        // is higher possibility that it is located on beginning side of the list
        const TInt count( iPaths.Count() );
        for ( TInt i = 0; i < count; i++ )   
#else            
        for ( TInt i = iPaths.Count(); --i >= 0; )
#endif
            {
            TDesC* pathName = iPaths[i];
            if ( MdsUtils::Find( aFilename, *pathName ) != KErrNotFound )
                {
                return ETrue;
                }
            }
        }
    else
        {
        return ETrue;
        }

    return EFalse;
    }

//-----------------------------------------------------------------------------
// CheckAttribs
//-----------------------------------------------------------------------------
//
TBool CMdsFileServerPlugin::CheckAttribs( const TDesC& aFilename, 
		TBool& aIsDirectory, RFsPlugin& aFsPlugin, TBool aDelete ) const
	{
	// find last backslash from filename and 
    // take drive and path from filename including last backslash
    const TChar KBackslashChar( '\\' );
    TInt pos = aFilename.LocateReverse( KBackslashChar );
    if ( KErrNotFound == pos )
    	{
    	return ETrue;
    	}
    TPtrC path( aFilename.Left( pos + 1 ) );

    TEntry entry;

    // check if path is hidden 
    TInt err = aFsPlugin.Entry( path, entry );
    if ( err == KErrNone )
        {      
        if ( entry.iAtt & KEntryAttHidden )
            {
            return EFalse;
            }
        }

    if( !aDelete )
        {
        // check if the file hidden or system file
        err = aFsPlugin.Entry( aFilename, entry );
        if ( err == KErrNone )
            {
            if ( entry.iAtt & KEntryAttHidden || entry.iAtt & KEntryAttSystem )
                {
                return EFalse;
                }
        
            aIsDirectory = entry.iAtt & KEntryAttDir ? ETrue : EFalse;
            }    
        }

    return ETrue;
    }

//-----------------------------------------------------------------------------
// AddBackslashIfNeeded
//-----------------------------------------------------------------------------
//
void CMdsFileServerPlugin::AddBackslashIfNeeded( TDes& aDirName ) const
	{
	WRITELOG1( "CMdsFileServerPlugin::AddBackslashIfNeeded() - begin, dirName: %S", &aDirName );

	const TChar KBackslashChar( '\\' );
	
	const TInt lastChar = aDirName.Length() - 1;
	
	// If it is directory which not contain backslash. 
	if ( KBackslashChar != aDirName[lastChar] )
		{		
		aDirName.Append( KBackslashChar );
		}

	WRITELOG1( "CMdsFileServerPlugin::AddBackslashIfNeeded() - end, dirName: %S", &aDirName );
	}

//-----------------------------------------------------------------------------
// CheckHarvesterStatus
//-----------------------------------------------------------------------------
//
TBool CMdsFileServerPlugin::CheckHarvesterStatus()
    {
    WRITELOG( "CMdsFileServerPlugin::CheckHarvesterStatus() - begin" );

    if( iPendingEvents <= KCleanQueueTreshold )
        {
        WRITELOG( "CMdsFileServerPlugin::CheckHarvesterStatus() - end" );
        return ETrue;
        }
    
    TFindServer findHarvesterServer( KHarvesterServerName );
    TFullName name;

    TInt result = findHarvesterServer.Next( name );
    if ( result == KErrNone )
        {
        WRITELOG( "CMdsFileServerPlugin::CheckHarvesterStatus() - HarvesterServer running" );
        WRITELOG( "CMdsFileServerPlugin::CheckHarvesterStatus() - end" );
        
        iPendingEvents = 0;
        return ETrue;
        }
    
    WRITELOG( "CMdsFileServerPlugin::CheckHarvesterStatus() - end" );
    return EFalse;
    }

//-----------------------------------------------------------------------------
// CheckIfValidFile
//-----------------------------------------------------------------------------
//
TBool CMdsFileServerPlugin::CheckIfValidFile( const TDesC& aFilename, 
        TBool& aIsDirectory, TFsPluginRequest& aRequest, TBool aDelete )
    {
    if ( !CheckPath(aFilename) )
        {
        WRITELOG( "CMdsFileServerPlugin::CheckIfValidFile() - path not supported" );
        return EFalse;
        }

    RFsPlugin fsPlugin( aRequest ); 
    const TInt rfsPluginErr = fsPlugin.Connect();
    
    if( rfsPluginErr == KErrNone )
        {
        if ( !CheckAttribs(aFilename, aIsDirectory, fsPlugin, aDelete) )
            {
            WRITELOG( "CMdsFileServerPlugin::CheckIfValidFile() - attribute check failed" );
            return EFalse;
            }
        }
    fsPlugin.Close(); 
    
    return ETrue;
    }

//-----------------------------------------------------------------------------
// CMdsFileServerPluginFactory implementation
//-----------------------------------------------------------------------------
//
class CMdsFileServerPluginFactory : public CFsPluginFactory
    {
    public:
        CMdsFileServerPluginFactory();
        virtual TInt Install();            
        virtual CFsPlugin* NewPluginL();
        virtual TInt UniquePosition();
    };
    
// Constructor for the plugin factory
// @internalComponent
CMdsFileServerPluginFactory::CMdsFileServerPluginFactory()
    {
    WRITELOG( "CMdsFileServerPluginFactory::CMdsFileServerPluginFactory()" );
    }

// Install function for the plugin factory
// @internalComponent
TInt CMdsFileServerPluginFactory::Install()
    {
    WRITELOG( "CMdsFileServerPluginFactory::Install()" );
    iSupportedDrives = KPluginAutoAttach;
    
    return( SetName(&KMdsFileServerPlugin) );
    }

// @internalComponent
TInt CMdsFileServerPluginFactory::UniquePosition()
    {
    WRITELOG( "CMdsFileServerPluginFactory::UniquePosition()" );
    return( KMdsFSPluginPosition );
    }

// Plugin factory function
// @internalComponent
CFsPlugin* CMdsFileServerPluginFactory::NewPluginL()
    {
    WRITELOG( "CMdsFileServerPluginFactory::NewPluginL()" );
    return CMdsFileServerPlugin::NewL();
    }

// Create a new Plugin
// @internalComponent
extern "C"
    {
    EXPORT_C CFsPluginFactory* CreateFileSystem()
        {
        WRITELOG( "CMdsFileServerPluginFactory::CreateFileSystem" );
        return( new CMdsFileServerPluginFactory() );
        }
    }

#ifdef _DEBUG_EVENTS

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
//
void CMdsFileServerPlugin::RegisterDebugEventsL()
    {
    User::LeaveIfError( RegisterIntercept( EFsFileDuplicate, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsFileCreate, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsFileWrite, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsFileFlush, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsFileSetAtt, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsFileChangeMode, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsDelete, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsFileAdopt, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsFileLock, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsFileSize, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsFileRename, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsRename, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsFileOpen, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsFileTemp,    EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsFileUnLock, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsFileSetSize, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsFileDrive, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsReplace, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsFileSubClose, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsFileRead, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsFileSeek, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsFileAtt, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsFileSet, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsFileName, EPreIntercept) );
    User::LeaveIfError( RegisterIntercept( EFsDirOpen, EPreIntercept) );
    }
    
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
//
void CMdsFileServerPlugin::UnregisterDebugEventsL()
    {
    User::LeaveIfError( UnregisterIntercept( EFsFileDuplicate, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsFileCreate, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsFileWrite, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsFileFlush, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsFileSetAtt, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsFileChangeMode, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsDelete, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsFileAdopt, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsFileLock, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsFileSize, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsFileRename, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsRename, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsFileOpen, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsFileTemp, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsFileUnLock, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsFileSetSize, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsFileDrive, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsReplace, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsFileSubClose, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsFileRead, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsFileSeek, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsFileAtt, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsFileSet, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsFileName, EPrePostIntercept) );
    User::LeaveIfError( UnregisterIntercept( EFsDirOpen, EPrePostIntercept) );
    }
    
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
//
void CMdsFileServerPlugin::PrintDebugEvents( TInt aFunction )
    {
    switch ( aFunction )
        {
        case EFsFileDuplicate:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileDuplicate" );
            break;
            
        case EFsFileCreate:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileCreate" );
            break;
        case EFsFileWrite:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileWrite" );
            break;
        case EFsFileFlush:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileFlush" );
            break;
        case EFsFileSetAtt:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileSetAtt" );
            break;
        case EFsFileChangeMode:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileChangeMode" );
            break;    
        case EFsDelete:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsDelete" );
            break;    
        case EFsFileAdopt:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileAdopt" );
            break;    
        case EFsFileReplace:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileReplace" );
            break;    
        case EFsFileLock:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileLock" );
            break;    
        case EFsFileSize:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileSize" );
            break;    
        case EFsFileModified:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileModified" );
            break;    
        case EFsFileRename:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileRename" );
            break;    
        case EFsRename:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsRename" );
            break;    
        case EFsFileOpen:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileOpen" );
            break;    
        case EFsFileTemp:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileTemp" );
            break;    
        case EFsFileUnLock:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileUnLock" );
            break;    
        case EFsFileSetSize:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileSetSize" );
            break;    
        case EFsFileSetModified:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileSetModified" );
            break;
        case EFsFileDrive:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileDrive" );
            break;    
        case EFsReplace:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsReplace" );
            break;    
        case EFsFileSubClose:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileSubClose" );
            break;    
        case EFsFileRead:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileRead" );
            break;
        case EFsFileSeek:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileSeek" );
            break;    
        case EFsFileAtt:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileAtt" );
            break;    
        case EFsFileSet:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileSet" );
            break;    
        case EFsFileName:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsFileName" );
            break;    
        case EFsDirOpen:
            WRITELOG( "CMdsFileServerPlugin::PrintDebugEvents() - EFsDirOpen" );
            break;    
        default:
        break;
        }
    }
#endif

TInt CMdsFileServerPlugin::Compare(const TDesC& aFirst, const TDesC& aSecond)
    {
    return aFirst.CompareF( aSecond );
    }

