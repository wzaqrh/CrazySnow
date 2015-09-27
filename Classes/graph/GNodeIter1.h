//
//  GNodeIter.cpp
//  proj.ios
//
//  Created by root911 on 14-5-22.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//

//#pragma mark GraphNodeVisitorNeib1
template<int __neib_num>
void cast_direct(typename GraphNodeVisitorLine::en_direct dir_in, typename GraphNodeVisitorRay::en_direct* dir_out)
{
    switch (dir_in) {
        case GraphNodeVisitorLine::e_dir_ang_0:
            dir_out[0] = GraphNodeVisitorRay::e_dir_ang2_0;
            dir_out[1] = GraphNodeVisitorRay::e_dir_ang2_180;
            break;
        case GraphNodeVisitorLine::e_dir_ang_60:
            dir_out[0] = GraphNodeVisitorRay::e_dir_ang2_60;
            dir_out[1] = GraphNodeVisitorRay::e_dir_ang2_240;
            break;
        case GraphNodeVisitorLine::e_dir_ang_120:
            dir_out[0] = GraphNodeVisitorRay::e_dir_ang2_120;
            dir_out[1] = GraphNodeVisitorRay::e_dir_ang2_300;
            break;
        default:
            assert(0);
            break;
    }
}
template<int __neib_num>
void GraphNodeVisitorLine::begin(Point2i pt, en_direct dir, int rows, int cols)
{
    this->m_cnt = 0;
    GraphNodeVisitorRay::en_direct dir_out[2];
    cast_direct(dir, dir_out);
    
    m_ray[0].begin(pt, dir_out[0], rows, cols);
    m_ray[1].begin(pt, dir_out[1], rows, cols);
    
    this->m_ptCur = m_ray[m_cnt].m_ptCur;
}
template<int __neib_num>
bool GraphNodeVisitorLine::end() const
{
    return m_cnt>=1 && m_ray[1].end();
}
template<int __neib_num>
void GraphNodeVisitorLine::operator++()
{
    ++m_ray[this->m_cnt];
    if( this->m_cnt==0 && m_ray[this->m_cnt].end() )
        this->m_cnt++;
    
    this->m_ptCur = m_ray[this->m_cnt].m_ptCur;
}