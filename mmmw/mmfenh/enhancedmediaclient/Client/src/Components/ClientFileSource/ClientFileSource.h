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
* Description:  Definition of the ClientFileSource class.
*
*/


#ifndef C_CCLIENTFILESOURCE_H
#define C_CCLIENTFILESOURCE_H

#include <e32base.h>
#include <FileSource.h>
#include <mmfcontrollerframework.h>

#include "SourceBase.h"
#include "AsyncAO.h"
#include "FileDataSourceCommon.h"

class MCustomCommand;

namespace multimedia
    {
    class CEventNotifier;

    
    /**
    *  Implements the ClientSide Source for the File Source.
    *  Provides functionality to the Clients to query attributes
    *  from the ServerSide source
    *  @lib EnhancedMediaClient.lib
    *  @since S60 v3.2
    */    
    class CClientFileSource : public CSourceBase,
                                public MFileSource,
                                public MAsyncAOObserver
        {
        public:
            /**
            * Function to Create the Object.
            */
            CClientFileSource();
            /**
            * Destructor.
            */
            ~CClientFileSource();

            /**
            * Post Contructor. This is to Support the new type of Construction
            * of the Object. Notice there is no Two phased Constructor in this class
            */
            TInt PostConstructor();
            
            // From MControl begins
            /**
            * From MControl.
            * Adds the Observer to this Object
            * @since S60 v3.2
            * @param aObserver Client which set the Observer.             
            */            
            TInt AddObserver( MControlObserver& aObserver );
            /**
            * From MControl.
            * Removes the Observer to this Object
            * @since S60 v3.2
            * @param aObserver Client which set the Observer.             
            */            
            TInt RemoveObserver( MControlObserver& aObserver );
            /**
            * From MControl.
            * Returns the Type of this Object.
            * request and that it is now outstanding 
            * @since S60 v3.2
            */            
            TUid Type();
            /**
            * From MControl.
            * Returns the Control Type of this Object.
            *
            * @since S60 v3.2
            */            
            TControlType ControlType();
            // From MControl ends
            
            // From MSourceControl begins
            /**
            * From MSourceControl.
            * Returns the Size in Bytes of the Source.
            * Since this is File Source, the size represents
            * the file size which will be passed by the Client
            *  to the Source
            * @since S60 v3.2
            * @param aSize Size returned by the Source.             
            */            
            TInt GetSize( TUint& aSize );
            /**
            * From MSourceControl.
            * Returns the Mime Type of the Data for which
            * the Source is initialized.
            * @since S60 v3.2
            * @param aMimeType MimeType returned by the Source.             
            */            
            TInt GetMimeType( TDes8& aMimeType );
            /**
            * From MSourceControl.
            * Closes the Source.
            *
            * @since S60 v3.2
            */            
            TInt Close();
            // From MSourceControl ends
            
            // From MFileSource begins
            /**
            * From MFileSource.
            * Opens the Source with the mimetype and HeaderData
            *
            * @since S60 v3.2
            * @param aFileName      FileName to be Opened by the Source
            * @param aMimeType      MimeType used to Configure the Source.
            */            
            TInt Open(TDesC& aFileName, TDesC8& aMimeType );
            /**
            * From MFileSource.
            * Returns the BitRate of the Source
            * the Source is initialized.
            *
            * @since S60 v3.2
            * @param aRate   Returns the BitRate of the Source.             
            */            
            TInt GetBitRate( TUint& aRate );
            /**
            * From MFileSource.
            * Returns the FileName associated with the Source
            * This value is set by the Client when it calls Open().
            *
            * @since S60 v3.2
            * @param aFileName  FileName to be Opened by the Source
            */            
            TInt GetFileName(TPtr& aFileName);
            // From MFileSource ends
            
            // From CSourceBase begins
            /**
            * From CSourceBase.
            * Callback from the StreamControl when the Source is Created
            *
            * @since S60 v3.2
            * @param aCustomCommand    Custom Command handle to send message to the Source             
            * @param aSourceHandle     Handle Identifying the Source             
            */            
            void ServerSourceCreated( MCustomCommand& aCustomCommand,
                                    TMMFMessageDestination& aSourceHandle );
            /**
            * From CSourceBase.
            * Callback from the StreamControl when the ServerSource is Deleted
            *
            * @since S60 v3.2
            */            
            void ServerSourceDeleted();
            /**
            * From CSourceBase.
            * Returns the Content Protection information to the Stream Control
            * Currently returns KErrNotSupported 
            * @since S60 v3.2
            */            
            TBool IsEncrypted();
            /**
            * From CSourceBase.
            * Returns the SourceUID
            *
            * @since S60 v3.2
            */            
            TUid GetSourceUid();
            /**
            * From CSourceBase.
            * Returns the Header Data passed when the Client calls Open
            *  Currently return KErrNotSupported
            * @since S60 v3.2
            */            
            TInt GetHeaderData(TPtr& aPtr);
            // From CSourceBase ends

            // From MAsyncAOObserver begins
            /**
            * From MAsyncAOObserver.
            * Callback from the Source for the Events from the Source Side
            * @since S60 v3.2
            * @param aError Error Code from the ServerSide Source.             
            */            
            void Event( TInt aError );
            // From MAsyncAOObserver ends
        private:
            void DoBitRateChanged( TUint aBitRate );           
        private:
            // Variable to Hold the Mime Type sent by the Client
            HBufC8* iMimeType;
            // Variable to Hold the FileName sent by the Client
            HBufC* iFileName;
            // BitRate from the Source Side
            TUint iBitRate;
            // FileSize from the Source Side
            TUint iFileSize;
            // Variable to determine if the ServerSource exists
            TBool iServerSourceExists;
            // Handle to send messages to the Server Side Source
            MCustomCommand* iCustomCommand;
            // Handle Identifying the Server Side Source
            TMMFMessageDestination iSourceHandle;
            // Server Side Source State
            CMultimediaDataSource::TState iState;
            // Event Notifier that notifies the Client
            CEventNotifier* iEventNotifier;
            // Active Object to get Events from the Server Side Source
            CAsyncAO* iAsyncEventAO;
            // Event Package sent to the Server Side.
            TFileDataSourceEventPckgBuf iEventPckg;
        };
    } // namespace multimedia

#endif // C_CCLIENTFILESOURCE_H

// End of file
