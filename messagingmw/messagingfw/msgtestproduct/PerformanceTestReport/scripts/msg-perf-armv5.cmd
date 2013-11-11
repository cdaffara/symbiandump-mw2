# Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:
#


mkdir \performance-logs\
mkdir \performance-logs\armv5\
copy \utils\findandcopy.pl \ /R /Y
perl findandcopy.pl \testdriver\results\*.htm \\performance-logs\armv5\
perl write-to-excell.pl armv5
