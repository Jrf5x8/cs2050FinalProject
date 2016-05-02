/* 
 * File:   trade.h
 * Author: Jack
 *
 * Created on May 1, 2016, 6:46 PM
 */

#ifndef TRADE_H
#define	TRADE_H

struct trade{
    int buy;
    int sell;
    struct trade* next;
};


#endif	/* TRADE_H */

