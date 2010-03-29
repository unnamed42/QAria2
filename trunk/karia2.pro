# karia2.pro --- 
# 
# Created: 2010-03-29 13:13:36 +0800
# Version: $Id$
# 
HEADERS     = karia2.h settings.h
SOURCES     = karia2.cpp main.cpp
FORMS       = karia2.ui
TRANSLATIONS = karia2_en.ts \
			  karia2_it.ts

# install
target.path = karia2
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS *.pro
sources.path = .
INSTALLS += target sources
RESOURCES = karia2.qrc
