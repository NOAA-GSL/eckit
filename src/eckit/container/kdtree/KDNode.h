/*
 * (C) Copyright 1996-2013 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef KDNode_H
#define KDNode_H

#include "eckit/container/sptree/SPNode.h"

namespace eckit {


template<class Traits>
class KDNode : public SPNode<Traits> {
public:

    typedef SPNode<Traits>      SPNode;
    typedef typename SPNode::Value     Value;
    typedef typename SPNode::Alloc     Alloc;
    typedef typename SPNode::Point     Point;
    typedef typename SPNode::NodeList  NodeList;
    typedef typename SPNode::NodeQueue NodeQueue;
    typedef typename SPNode::NodeInfo  NodeInfo;

private:

    size_t    axis_;

public:
    KDNode(const Value& value, size_t axis);


    template<typename ITER>
    static KDNode* build(Alloc& a,const ITER& begin, const ITER& end, int depth = 0);


private:
    virtual void nearestNeighbourX(Alloc& a,const Point& p, SPNode*& best, double& max, int depth);
    virtual void findInSphereX(Alloc& a,const Point& p ,double radius, NodeList& result, int depth) ;
    virtual void kNearestNeighboursX(Alloc& a,const Point& p ,size_t k, NodeQueue& result, int depth) ;

    //==========================


};


} // Name space

#include "KDNode.cc"

#endif