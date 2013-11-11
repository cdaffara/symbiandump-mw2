/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observer for refresh events.
*
*/



#ifndef MSATREFRESHOBSERVER_H
#define MSATREFRESHOBSERVER_H

//  INCLUDES
#include <e32std.h>
#include <etelsat.h>

// DATA TYPES
enum TSatElementaryFiles
    {
    KUnknownEf          = RSat::KUnknownEf,     // Unknown EF type
    KDirEf              = RSat::KDirEf,         // Application directory EF
    KElpEf              = RSat::KElpEf,         // Extended Language Preference
                                                // EF
    KArrEf              = RSat::KArrEf,         // Access Rule Reference EF at
                                                // the MF level.
    KImgEf              = RSat::KImgEf,         // Image EF
    KPscEf              = RSat::KPscEf,         // Phonebook synchronisation
                                                // counter EF
    KCcEf               = RSat::KCcEf,          // Change counter EF
    KPuidEf             = RSat::KPuidEf,        // Previous unique identifier
                                                // EF
    KPbr                = RSat::KPbr,           // Phonebook reference file EF
    KSllEf              = RSat::KSllEf,         // SoLSA LSA List EF
    KInvScan            = RSat::KInvScan,       // Investigation File EF
    KImpiEf             = RSat::KImpiEf,        // IMS private user identity EF
    KImpuEf             = RSat::KImpuEf,        // IMS public user identity EF
    KDomainEf           = RSat::KDomainEf,      // Home Network Domain Name EF
    KLpEf               = RSat::KLpEf,          // Language Preference EF
    KArradEf            = RSat::KArradEf,       // Access rule reference (under
                                                // ADFUSIM and DFTELECOM) EF
    KImsiEf             = RSat::KImsiEf,        // IMSI EF
    KCsp1Ef             = RSat::KCsp1Ef,        // CSP 1 EF (7F20/21 6F15)
    KDckEf              = RSat::KDckEf,         // De-personalization Control
                                                // Keys EF
    KPlmnSelEf          = RSat::KPlmnSelEf,     // PLMN Selection EF
    KHplmnEf            = RSat::KHplmnEf,       // Home PLMN EF
    KCnlEf              = RSat::KCnlEf,         // Co-operative Network List EF
    KAcmMaxEf           = RSat::KAcmMaxEf,      // ACM max value EF
    KSstEf              = RSat::KSstEf,         // SIM Service Table EF
    KAcmEf              = RSat::KAcmEf,         // Accumulated Call Meter EF
    KAdnEf              = RSat::KAdnEf,         // Abbreviated Dialling Numbers
                                                // EF
    KFdnEf              = RSat::KFdnEf,         // Fixed Diallind Numbers EF
    KSmsEf              = RSat::KSmsEf,         // Short Messages EF
    KCcpEf              = RSat::KCcpEf,         // Capability Configuration
                                                // Parameters EF
    KGid1Ef             = RSat::KGid1Ef,        // Group Identifier Level 1 EF
    KGid2Ef             = RSat::KGid2Ef,        // Group Identifier Level 2 EF
    KMsisdnEf           = RSat::KMsisdnEf,      // MS ISDN EF
    KPuctEf             = RSat::KPuctEf,        // Price per Unit and Currency
                                                // Table EF
    KSmspEf             = RSat::KSmspEf,        // SMS Parameters EF
    KSmssEf             = RSat::KSmssEf,        // SMS Status EF
    KLndEf              = RSat::KLndEf,         // Last Number Dialled EF
    KCbmiEf             = RSat::KCbmiEf,        // Cell Broadcast Message
                                                // Identifier
    KSpnEf              = RSat::KSpnEf,         // Service Provider Name EF
    KSmsrEf             = RSat::KSmsrEf,        // SMS reports EF
    KCbmidEf            = RSat::KCbmidEf,       // CBMI for Data Download EF
    KSdnEf              = RSat::KSdnEf,         // Service Dialling Numbers EF
    KExt1Ef             = RSat::KExt1Ef,        // Extension 1 EF
    KExt2Ef             = RSat::KExt2Ef,        // Extension 2 EF
    KExt3Ef             = RSat::KExt3Ef,        // Extension 3 EF
    KBdnEf              = RSat::KBdnEf,         // Barred Dialling Numbers EF
    KExt5Ef             = RSat::KExt5Ef,        // Extension 5 EF
    KCcp2Ef             = RSat::KCcp2Ef,        // Capability configuration
                                                // parameters 2 EF
    KCbmirEf            = RSat::KCbmirEf,       // CBMI Range Selection EF
    KNiaEf              = RSat::KNiaEf,         // Network's Indication of
                                                // Alerting EF
    KLociGprsEf         = RSat::KLociGprsEf,    // GPRS Location Information EF
    KSumeEf             = RSat::KSumeEf,        // SetUpMenu Elements EF
    KExt4Ef             = RSat::KExt4Ef,        // Extension 4 EF
    KEstEf              = RSat::KEstEf,         // Enabled services table EF
    KAclEf              = RSat::KAclEf,         // Access point name control
                                                // list EF
    KCmiEf              = RSat::KCmiEf,         // Comparison method
                                                // information EF
    KStartHfnEf         = RSat::KStartHfnEf,    // Initialisation value for
                                                // Hyperframe number EF
    KThresholdEf        = RSat::KThresholdEf,   // Maximum value of START EF
    KOPlmnwAcTEf        = RSat::KOPlmnwAcTEf,   // Operator controlled PLMN
                                                // selector with Access
                                                // Technology EF
    KHPlmnwAcTEf        = RSat::KHPlmnwAcTEf,   // HPLMN selector with Access
                                                // Technology EF
    KRplmnActEf         = RSat::KRplmnActEf,    // RPLMN Last used Access
                                                // Technology EF
    KPsLociEf           = RSat::KPsLociEf,      // Packet switched location
                                                // information EF
    KAccEf              = RSat::KAccEf,         // Access Control Class EF
    KFPlmnEf            = RSat::KFPlmnEf,       // Forbidden PLMN EF
    KIciEf              = RSat::KIciEf,         // Incoming call information EF
    KOciEf              = RSat::KOciEf,         // Outgoing call information EF
    KIctEf              = RSat::KIctEf,         // Incoming call timer EF
    KOctEf              = RSat::KOctEf,         // Outgoing call timer EF
    KCsp2Ef             = RSat::KCsp2Ef,        // CSP 2 EF (7F40 6F98)
    KAdEf               = RSat::KAdEf,          // Administrative Data EF
    KPhaseEf            = RSat::KPhaseEf,       // Phase Identification EF
    KVcgsEf             = RSat::KVcgsEf,        // Voice Group Call Service EF
    KVgcssEf            = RSat::KVgcssEf,       // Voice Group Call Service
                                                // Status EF
    KVbsEf              = RSat::KVbsEf,         // Voice Broadcast Service EF
    KVbssEf             = RSat::KVbssEf,        // Voice Broadcast Service
                                                // Status EF
    KeMlppEf            = RSat::KeMlppEf,       // enhanced Multi Level
                                                // Pre-emption and Priority EF
    KAaemef             = RSat::KAaemef,        // Automatic Answer for eMLLP
                                                // Service EF
    KEccEf              = RSat::KEccEf,         // Emergency Call Codes EF
    KGmsi               = RSat::KGmsi,          // Group Identity EF
    KHiddenKeyEf        = RSat::KHiddenKeyEf,   // Key for hidden phone book
                                                // entries EF
    KPnnEf              = RSat::KPnnEf,         // PLMN Network Name EF
    KOplEf              = RSat::KOplEf,         // Operator Network List EF
    KMbdnEf             = RSat::KMbdnEf,        // Mailbox Dialling Numbers EF
    KExt6Ef             = RSat::KExt6Ef,        // Extension 6 EF
    KMbiEf              = RSat::KMbiEf,         // Mailbox Identifier EF
    KMwisEf             = RSat::KMwisEf,        // Message Waiting Indication
                                                // Status EF
    KCfisEf             = RSat::KCfisEf,        // Call Forwarding Indication
                                                // Status EF
    KExt7Ef             = RSat::KExt7Ef,        // Extension 7 EF
    KSpdiEf             = RSat::KSpdiEf,        // Service provider display
                                                // information EF
    KMmsnEF             = RSat::KMmsnEF,        // MMS notification EF
    KExt8Ef             = RSat::KExt8Ef,        // Extension 8 EF
    KMmsicpEf           = RSat::KMmsicpEf,      // MMS issuer connectivity
                                                // parameters EF
    KMmsupEf            = RSat::KMmsupEf,       // MMS user preferences EF
    KMmsucpEf           = RSat::KMmsucpEf,      // MMS user connectivity
    KNiaRel5Ef          = RSat::KNiaRel5Ef,     // Network's Indication of Alerting EF (from Rel 5 onwards)
                                                // parameters EF
    KCspEf              = 0xffff                // CSP (Not yet defined in ETel
                                                // SAT API)
    };

typedef TBuf16<242> TSatRefreshFiles;

// Refresh types.
enum TSatRefreshType
    {
    ERefreshTypeNotSet,
    ESimInitFullFileChangeNotification,
    EFileChangeNotification,
    ESimInitFileChangeNotification,
    ESimInit,
    ESimReset,
    EUsimApplicationReset,
    E3GSessionReset
    };

// CLASS DECLARATION

/**
*  Observer for refresh events. Client must not call any RSatRefresh
*  methods from the observer methods.
*
*  @lib SatClient
*  @since 2.6
*/
class MSatRefreshObserver
    {
    protected:

        /**
        * C++ constructor.
        */
        MSatRefreshObserver() {};

        /**
        * Destructor.
        */
        virtual ~MSatRefreshObserver() {};

    public: // New functions

        /**
        * Refresh query. Client should determine whether it allow the
        * refresh to happen.
        * @since 2.6
        * @param aType Refresh type.
        * @param aFiles Elementary files which are to be changed.
        * May be zero length.
        * @return ETrue to allow refresh, EFalse to decline refresh.
        */
        virtual TBool AllowRefresh(
            TSatRefreshType aType, const TSatRefreshFiles& aFiles ) = 0;

         /**
        * Notification of refresh.
        * @since 2.6
        * @param aType Type of refresh which has happened.
        * @param aFiles List of elementary files which has been changed.
        * May be zero length.
        */
        virtual void Refresh(
            TSatRefreshType aType, const TSatRefreshFiles& aFiles ) = 0;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatRefreshObserver( const MSatRefreshObserver& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatRefreshObserver& operator=( const MSatRefreshObserver& );

    };

#endif      // MSATREFRESHOBSERVER_H

// End of File
