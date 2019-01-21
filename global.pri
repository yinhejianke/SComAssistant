# 通用的pri

rootPath=/home/zhaiyu/Documents/qt/SComAssistant
binPath=$${rootPath}/bin
uiPath=$${rootPath}/uih
tmpPath=$${rootPath}/tmp

INCLUDEPATH += $${PWD}
INCLUDEPATH += $${uiPath}
INCLUDEPATH += $${tmpPath}

DESTDIR = $${binPath}
UI_DIR += $${uiPath}
RCC_DIR += $${tmpPath}
MOC_DIR += $${tmpPath}
OBJECTS_DIR += $${tmpPath}

