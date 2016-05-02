/* 
 * File:   strategy.h
 * Author: Jack
 *
 * Created on April 24, 2016, 8:51 PM
 */

#ifndef STRATEGY_H
#define	STRATEGY_H
#include "trade.h"

struct strategy{
    int buy;
    int sell;
    int numberOfTrades;
    trade* additionalTrades;
    struct strategy* next;
    struct strategy* beg;
    struct strategy* end;
};


#endif	/* STRATEGY_H */

