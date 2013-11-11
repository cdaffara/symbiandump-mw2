#
# ============================================================================
#  Name        : platformthemes.pro
#  Part of     : platformthemes
#  Description : Project definition file for project platformthemes
#  Version     : %version: %
#
#  Copyright (c) 2008-2010 Nokia.  All rights reserved.
#  This material, including documentation and any related computer
#  programs, is protected by copyright controlled by Nokia.  All
#  rights are reserved.  Copying, including reproducing, storing,
#  adapting or translating, any or all of this material requires the
#  prior written consent of Nokia.  This material also contains
#  confidential information which may not be disclosed to others
#  without the prior written consent of Nokia.
# ============================================================================
#

NAME = platformthemes
TEMPLATE = subdirs
EXCLUDE += --exclude \"*.orig\"
EXCLUDE += --exclude \"*/distribution.policy.s60\"

# ============================================================================
# determine HB_THEMES_DIR
# ============================================================================
!symbian {
    HB_THEMES_DIR = $$(HB_THEMES_DIR)
    isEmpty(HB_THEMES_DIR) {
        win32:ENV_HELP = eg. set HB_THEMES_DIR=C:\hb\themes
        else:ENV_HELP = eg. export HB_THEMES_DIR=/usr/local/hb/themes
        error(HB_THEMES_DIR environment variable is not set. ($$ENV_HELP))
    }
} else {
    HB_THEMES_DIR = $${EPOCROOT}epoc32/data/z/resource/hb
}
HB_THEMES_DIR = $$HB_THEMES_DIR/themes

# ============================================================================
# determine dir separators & /dev/null
# ============================================================================
win32:!win32-g++ {
    unixstyle = false
} else:symbian:isEmpty(QMAKE_SH) {
    unixstyle = false
} else:win32-g++:isEmpty(QMAKE_SH) {
    unixstyle = false
} else {
    unixstyle = true
}

# ============================================================================
# extract archives
# ============================================================================
ARGS = --input src --output $$OUT_PWD/tmp/src $$EXCLUDE --clean
!system(python bin/extract.py $$ARGS) {
    error(*** bin/extract.py reported an error. Stop.)
}

# ============================================================================
# convert svg->nvg
# ============================================================================
symbian {
    nvg|!no_nvg {
        ARGS = --input $$OUT_PWD/tmp/src/icons
        !system(python bin/svg2nvg.py $$ARGS) {
            error(*** bin/svg2nvg.py reported an error. Stop.)
        }
    }
}

# ============================================================================
# theme indexing target
# ============================================================================
THEMEINDEXER = hbthemeindexer
!symbian {
    $$unixstyle {
        DEVNULL = /dev/null
    } else {
        DEVNULL = nul
    }
    !system($$THEMEINDEXER > $$DEVNULL 2>&1) {
        error(\'$$THEMEINDEXER\' must be in PATH.)
    }
}
ARGS = -s $$OUT_PWD/tmp/src -t $$OUT_PWD/tmp/src
!system($$THEMEINDEXER $$ARGS) {
    error(*** $$THEMEINDEXER reported an error. Stop.)
}

index.path = $$(HB_THEMES_DIR)/themes
index.files = $$OUT_PWD/tmp/src/*.themeindex
INSTALLS += index
QMAKE_CLEAN += $$OUT_PWD/tmp/src/*.themeindex

# ============================================================================
# generate installs.pri
# ============================================================================
ARGS = --input $$OUT_PWD/tmp/src --output $$OUT_PWD/tmp $$EXCLUDE
!system(python bin/installs.py $$ARGS) {
    error(*** bin/installs.py reported an error. Stop.)
}
isEmpty(QMAKE_UNZIP):QMAKE_UNZIP = unzip -u -o
QMAKE_CLEAN += $$OUT_PWD/tmp/installs.pri

# ============================================================================
# generate rom files
# ============================================================================
symbian {
    ARGS = --input $$OUT_PWD/tmp/src --output $$OUT_PWD/tmp $$EXCLUDE
    !system(python bin/rom.py $$ARGS) {
        error(*** bin/rom.py reported an error. Stop.)
    }
    QMAKE_CLEAN += $$OUT_PWD/tmp/*.iby
    QMAKE_CLEAN += $$OUT_PWD/tmp/*.thx
}

# ============================================================================
# installs/exports
# ============================================================================
symbian {

    # theme exports
    exists(src/theme.theme) {
        BLD_INF_RULES.prj_exports += "src/theme.theme $${EPOCROOT}epoc32/data/z/resource/hb/themes/"
        BLD_INF_RULES.prj_exports += "src/theme.theme $${EPOCROOT}epoc32/winscw/c/resource/hb/themes/"
    }
    exists(rom/theme.theme.iby) {
        BLD_INF_RULES.prj_exports += "rom/theme.theme.iby $$CORE_MW_LAYER_IBY_EXPORT_PATH(theme.theme.iby)"
    }

    # params: <files> <target>
    defineTest(exportThemeFiles) {
        files = $$1
        target = $$2
        for(file, files) {
            # strip possible drive letter
            file = $$split(file, :)
            file = $$last(file)
            BLD_INF_RULES.prj_exports += "$$file $$target"
        }
        export(BLD_INF_RULES.prj_exports)
        return(true)
    }
    exportThemeFiles($$files($$OUT_PWD/tmp/*.iby), $$CORE_MW_LAYER_IBY_EXPORT_PATH())
    exportThemeFiles($$files($$OUT_PWD/tmp/*.thx), $${EPOCROOT}epoc32/data/z/resource/hb/themes/)
    exportThemeFiles($$files($$OUT_PWD/tmp/src/*.themeindex), $${EPOCROOT}epoc32/winscw/c/resource/hb/themes/)

    # configuration files
    BLD_INF_RULES.prj_exports += "$$section(PWD, ":", 1)/confml/platformthemes.confml            MW_LAYER_CONFML(platformthemes.confml)

} else {
    exists(src/theme.theme) {
        theme.theme.path = $$(HB_THEMES_DIR)/themes
        theme.theme.files += src/theme.theme
        INSTALLS += theme.theme
    }
}

# ============================================================================
# NOTE: qmake/s60 does not support INSTALLS
# ============================================================================
symbian {
    install.depends += export
    uninstall.depends = cleanexport
    QMAKE_EXTRA_TARGETS += install uninstall
}

message(Run \'make install\')
include($$OUT_PWD/tmp/installs.pri)
