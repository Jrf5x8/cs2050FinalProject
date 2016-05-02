/* 
 * File:   node.h
 * Author: Jack
 *
 * Created on April 24, 2016, 1:44 PM
 */

#ifndef NODE_H
#define	NODE_H
#include "strategy.h"

struct node{
    int profit;
    struct strategy *strategy;
    
};

#endif	/* NODE_H */

