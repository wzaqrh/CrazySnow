//
//  GraphDataStorage.h
//  crazysnow_maptool
//
//  Created by qrh on 15-9-27.
//
//

#ifndef __crazysnow_maptool__GraphDataStorage__
#define __crazysnow_maptool__GraphDataStorage__

class GraphData;
class StageBaseData;
class GraphDataStorage {
public:
    virtual ~GraphDataStorage() {}
    virtual bool load(const char* fullpath, GraphData& data, StageBaseData* baseData) = 0;
    virtual void save(const char* fullpath, const GraphData& data, const StageBaseData* baseData) = 0;
};

class GraphDataStorageXML : public GraphDataStorage {
public:
    virtual bool load(const char* fullpath, GraphData& data, StageBaseData* baseData);
    virtual void save(const char* fullpath, const GraphData& data, const StageBaseData* baseData);
};

#endif /* defined(__crazysnow_maptool__GraphDataStorage__) */
