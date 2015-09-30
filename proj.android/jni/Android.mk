LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
../../Classes//AppDelegate.cpp \
../../Classes//AppDelegate.h \
../../Classes//AppMacros.h \
../../Classes//common/CommonDef.h \
../../Classes//common/DialogComponent.cpp \
../../Classes//common/DialogComponent.h \
../../Classes//common/LabelNode.cpp \
../../Classes//common/LabelNode.h \
../../Classes//common/LabelNodeImpl.cpp \
../../Classes//common/LabelNodeImpl.h \
../../Classes//common/PreCompile.h \
../../Classes//common/ResourceDef.h \
../../Classes//common/ResourceUtility.cpp \
../../Classes//common/ResourceUtility.h \
../../Classes//common/SoundPool.cpp \
../../Classes//common/SoundPool.h \
../../Classes//common/TableViewHelper.cpp \
../../Classes//common/TableViewHelper.h \
../../Classes//common/TmplHelp.h \
../../Classes//common/TouchSwallowLayer.cpp \
../../Classes//common/TouchSwallowLayer.h \
../../Classes//common/WindowAdaptUtility.cpp \
../../Classes//common/WindowAdaptUtility.h \
../../Classes//data/DataStorage.cpp \
../../Classes//data/DataStorage.h \
../../Classes//data/GameEntity.cpp \
../../Classes//data/GameEntity.h \
../../Classes//data/GraphDataStorage.cpp \
../../Classes//data/GraphDataStorage.h \
../../Classes//data/StageData.cpp \
../../Classes//data/StageData.h \
../../Classes//data/UserInfo.cpp \
../../Classes//data/UserInfo.h \
../../Classes//entity/ShopEntity.cpp \
../../Classes//entity/ShopEntity.h \
../../Classes//entity/StageEntity.cpp \
../../Classes//entity/StageEntity.h \
../../Classes//entity/WindowStageEntity.cpp \
../../Classes//entity/WindowStageEntity.h \
../../Classes//entity/WindowStageEntityType.h \
../../Classes//event/EventPopstar.cpp \
../../Classes//event/EventPopstar.h \
../../Classes//event/EventSpecial.cpp \
../../Classes//event/EventSpecial.h \
../../Classes//event/TilemapEvtHandler.cpp \
../../Classes//event/TilemapEvtHandler.h \
../../Classes//event/TilmapEvtDispatcher.cpp \
../../Classes//event/TilmapEvtDispatcher.h \
../../Classes//graph/GNodeIter1.h \
../../Classes//graph/GrapgIterator.hpp \
../../Classes//graph/Graph.h \
../../Classes//graph/GraphCommDebug.h \
../../Classes//graph/GraphCommDef.h \
../../Classes//graph/GraphIterator.h \
../../Classes//graph/GraphMatrix.cpp \
../../Classes//graph/GraphMatrix.h \
../../Classes//graph/GraphNode.cpp \
../../Classes//graph/GraphNode.h \
../../Classes//graph/GraphTool.h \
../../Classes//graph/GraphTool.hpp \
../../Classes//graph/IElemPaddingStragy.cpp \
../../Classes//graph/IElemPaddingStragy.h \
../../Classes//graph/RollCardUtinity.cpp \
../../Classes//graph/RollCardUtinity.h \
../../Classes//scene/BeginLayer.cpp \
../../Classes//scene/BeginLayer.h \
../../Classes//scene/board/PlygonDrawer.cpp \
../../Classes//scene/board/PlygonDrawer.h \
../../Classes//scene/board/TileMap.cpp \
../../Classes//scene/board/TileMap.h \
../../Classes//scene/board/ZZTopLayer.cpp \
../../Classes//scene/board/ZZTopLayer.h \
../../Classes//scene/game/GameMainNode.cpp \
../../Classes//scene/game/GameMainNode.h \
../../Classes//scene/game/GameTitleNode.cpp \
../../Classes//scene/game/GameTitleNode.h \
../../Classes//scene/game/LoseBoard.cpp \
../../Classes//scene/game/LoseBoard.h \
../../Classes//scene/game/PauseBoard.cpp \
../../Classes//scene/game/PauseBoard.h \
../../Classes//scene/game/StringUpdater.h \
../../Classes//scene/game/WinBoard.cpp \
../../Classes//scene/game/WinBoard.h \
../../Classes//scene/help/HelpLayer.cpp \
../../Classes//scene/help/HelpLayer.h \
../../Classes//scene/shop/ShopLayer.cpp \
../../Classes//scene/shop/ShopLayer.h

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
../../Classes/common \
../../Classes/data \
../../Classes/entity \
../../Classes/event \
../../Classes/graph \
../../Classes/scene \
../../Classes/scene/board \
../../Classes/scene/game \
../../Classes/scene/help \
../../Classes/scene/shop

LOCAL_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
