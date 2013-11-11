/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:
 *
 */

#ifndef PSETCLIWRAPPER_H_
#define PSETCLIWRAPPER_H_


#include <QObject>
#include <psetwrappertypes.h>

// Forward declarations 
class CPsetContainer;
class CPsetCli;
class MPsetRequestObserver;
class PSetCliWrapperPrivate;

#ifdef BUILD_PSETWRAPPER
#define PSETWRAPPER_EXPORT Q_DECL_EXPORT
#else
#define PSETWRAPPER_EXPORT Q_DECL_IMPORT
#endif

class PSETWRAPPER_EXPORT PSetCliWrapper: 
    public QObject
{
    Q_OBJECT

public:
    
    explicit PSetCliWrapper( CPsetContainer &psetContainer, 
            QObject *parent = NULL);
    
    ~PSetCliWrapper();
    
    
public: // Functions (adaptees):  
    
    /**
    * Shows COLP (Connected Line Identification Presentation) mode.
    */
    void getColpMode();

    /**
    * Shows CLIP (Calling Line Identification Presentation) mode.
    */
    void getClipMode();
    
    /**
    * Shows CLIR (Calling Line Identification Restriction) mode.
    */
    void getClirMode();

    /**
    * Shows COLR (Connected Line Identification Restriction) mode.
    */
    void getColrMode();

    /**
    * Shows CNAP (Calling Number Presentation) mode.
    * 
    */
    void getCnap();

    /**
    * Cancels any (and all) request(s).
    */
    void cancelAll();
    
signals: // Notify via signals     

    void handleCliRequesting( bool ongoing, bool interrupted ); 

    void cliInformation( const PsCallLineIdentity& type );

    void handleCliStatus(
            unsigned char *bsc, int numberOfBsc, const PsCallLineIdentity& aMode );

    void handleCnapStatus( int aStatus );

    void handleCliError( int aError );

private: // Data: 
    // Phone setting handlers 
    CPsetCli* m_psetCli; 
    
    // Own
    QScopedPointer<PSetCliWrapperPrivate> m_privateImpl;
    friend class PSetCliWrapperPrivate;
};


#endif /* PSETCLIWRAPPER_H_ */
