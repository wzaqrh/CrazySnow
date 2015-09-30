//
//  GraphDataStorage.cpp
//  crazysnow_maptool
//
//  Created by qrh on 15-9-27.
//
//

#include "GraphDataStorage.h"
#include <cocos2d.h>
#include "external/tinyxml2/tinyxml2.h"
#include "graph/GraphCommDef.h"
#include "data/StageData.h"
using namespace cocos2d;
using namespace tinyxml2;

/*
 int m_score;
 Point2i m_offset;
 int m_cols;
 int m_rows;
 enColor map[EDIT_GRAPH_ROW][EDIT_GRAPH_COL];
 */
void GraphDataStorageXML::save(const char* path, const GraphData& data, const StageBaseData* baseData) {
    cocos2d::log("save_crazysnow map path=%s", path);
    try {
        XMLDocument doc;
        XMLDocument *myDocument = &doc;
        
        {
            XMLElement* property = myDocument->NewElement("property");
            myDocument->LinkEndChild(property);
            
            //property->SetAttribute("score", data.score);
            property->SetAttribute("cols", data.cols);
            property->SetAttribute("rows", data.rows);
        }
        {
            XMLElement* map = myDocument->NewElement("map");
            myDocument->LinkEndChild(map);
            
            for (size_t row = 0; row < data.rows; ++row)
                for (size_t col = 0; col < data.cols; ++col) {
                    char buf[200]; sprintf(buf, "%ld_%ld", row, col);
                    map->SetAttribute(buf, data.map[row][col].catgy.color);
                }
        }
        
        myDocument->SaveFile(path);
    } catch (std::string& e) {
        cocos2d::log("%s\n", e.c_str());
    }
}

bool GraphDataStorageXML::load(const char* fullpath, GraphData& data, StageBaseData* baseData) {
    try {
        XMLDocument doc;
        XMLDocument *myDocument = &doc;
        
        if (! FileUtils::getInstance()->isFileExist(fullpath)) return false;
        
        auto Data = FileUtils::getInstance()->getDataFromFile(fullpath);
        if (myDocument->Parse((const char*)Data.getBytes(), Data.getSize())) return false;
        //if (myDocument->LoadFile(fullpath)) return false;
        
        static GraphData s_defGraphData = {
            10/*row*/,
            10/*column*/,
            5/*vertex_num*/,
            64/*box_heights*/,
            64/*box_widths*/,
            0/*diff_row_off_x*/,
            32/*offsert_x*/,
            32/*offsert_y*/
        };
        data = s_defGraphData;
        
        for (XMLElement* property = myDocument->FirstChildElement("property"); property; property = property->NextSiblingElement("property")) {
            data.cols  = property->IntAttribute("cols");
            data.rows  = property->IntAttribute("rows");
            
            if (baseData) {
                baseData->m_bonusNeedStar = 12;
                baseData->m_lbNeedScore = property->IntAttribute("score") * 5;
            }
        }
        for (XMLElement* property = myDocument->FirstChildElement("map"); property; property = property->NextSiblingElement()) {
            for (auto attribute = property->FirstAttribute(); attribute; attribute = attribute->Next()) {
                const char* name = attribute->Name();
                int row = 0, col = 0;
                sscanf(name, "a%d_%d", &row, &col);
                data.map[row][col] = GraphAttrib::make(NodeCategory::make(static_cast<enNodeColor>(attribute->IntValue()), CHESS_CATEGORY_NORM, 0));
            }
        }
    } catch (std::string& e) {
        cocos2d::log("%s\n", e.c_str());
        return false;
    }
    return true;
}