//
//  CommonDef.h
//  crazy_snow
//
//  Created by macos1 on 14-12-25.
//
//
#pragma once
#include "PreCompile.h"

#include "ResourceDef.h"
#include "LabelNode.h"
#include "WindowAdaptUtility.h"
#include "TouchSwallowLayer.h"
#include "ResourceUtility.h"
#include "DialogComponent.h"

typedef enum tagSceneGlobalZorder {
    SCENE_GLOBAL_ZORDER_LOWEST = -1000,
    SCENE_GLOBAL_ZORDER_DIALOG = 10000
}SceneGlobalZorder;


#define DESIGN_RESOLUTION_WIDTH  (640)
#define DESIGN_RESOLUTION_WIDTH_HALF  (DESIGN_RESOLUTION_WIDTH/2)
#define DESIGN_RESOLUTION_HEIGHT (960)
#define DESIGN_RESOLUTION_HEIGHT_HALF (DESIGN_RESOLUTION_HEIGHT/2)

