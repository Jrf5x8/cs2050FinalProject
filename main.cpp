/* 
 * File:   main.cpp
 * Author: Jack
 *
 * Created on April 24, 2016, 1:31 PM
 */

#include <cstdlib>
#include "node.h"
#include <iostream>
#include <fstream>


using namespace std;

/*
 * 
 */

//int max(int a, int b, int c){
//    if(a > b){
//        if(a > c){
//            return a;
//        }else{
//            return c;
//        }
//    }else if(b > c){
//        return b;
//    }else{
//        return c;
//    }
//}

node* max(node* a, node* b){
    if(a->profit >= b->profit){
        return a;
    }else{
        return b;
    }
}

int max(int a, int b){
    if(a >= b){
        return a;
    }else{
        return b;
    }
}

void printStrategy(strategy* s){
    strategy* tmp = s;
    while(tmp){
        cout << "Buy: " << tmp->buy << "Sell: " << tmp->sell << " ";
        tmp = tmp->next;
    }
    cout << endl;
}

strategy* addTradeToStrategy(strategy* s, int b, int se){
    strategy* trade = new strategy;
    trade->beg = s;
    trade->end = trade;
    trade->numberOfTrades = s->numberOfTrades + 1;
    trade->buy = b;
    trade->sell = se;
    trade->next = NULL;
    
    s->next = trade;
    s->end = trade;
    
    
    return s;
    
    
}



strategy* betterStrategy(strategy* s1, strategy* s2, int* prices){
    int s1Profit = prices[s1->buy] - prices[s1->sell];
    int s2Profit = prices[s2->buy] - prices[s2->sell];
    
    
    
    
    if(s1Profit > s2Profit){
        return s1;
    }else{
        return s2;
    }
    
    
}

int calculateProfitFromStrategy(strategy* s, int* prices){
    strategy* tmp = s;
    int totalProfit = 0;
    while(tmp){
        totalProfit += (prices[tmp->sell] - prices[tmp->buy]);
        tmp = tmp->next;
    }
    
    return totalProfit;
}

int main(int argc, char** argv) {

    ifstream input(argv[1]);
    int numberOfDays;
    cout << "Please enter the number of days\n";
    cin >> numberOfDays;
    cout << "Please enter r\n";
    int inputr;
    cin >> inputr;
    
    string line;
    int prices[numberOfDays];
    
    for(int i = 0; i < numberOfDays; i++){
        getline(input, line);
        int num = stoi(line);
        prices[i] = num;
    }
    
    cout << "The prices array is\n";
    for(int i = 0; i < numberOfDays; i++){
        cout << prices[i] << " ";
    }
    cout << endl;
    
    //initializing the chart
    node *chart[inputr][numberOfDays];
    
    int firstBuy = 0;
    int crawl = 0;
    while(crawl < numberOfDays && prices[crawl] > prices[crawl+1]){
        crawl++;
        firstBuy = crawl;
    }
    
    
    
    for(int i = 0; i < inputr; i++){
        for(int j = 0; j < numberOfDays; j++){
            //initial buy/sell strategy for all nodes
            strategy* initialStrategy = new strategy;
            initialStrategy->buy = firstBuy;
            initialStrategy->sell = firstBuy;
            initialStrategy->numberOfTrades = 0;
            initialStrategy->next = NULL;
            initialStrategy->beg = initialStrategy;
            initialStrategy->end = initialStrategy;
            //initial value for all nodes in chart
            node* initialValue = new node;
            initialValue->strategy = initialStrategy;
            initialValue->profit = 0;
            chart[i][j] = initialValue;
        }
    }
    
//    int chart[inputr][numberOfDays];
//    for(int i = 0; i < inputr; i++){
//        for(int j = 0; j < numberOfDays; j++){
//            chart[i][j] = 0;
//        }
//    }
    bool buyingOpportunity = false;
    int numberOfTrades = 1;
    for(int r = 0; r < inputr; r++){
        for(int today = firstBuy > 0 ? firstBuy : 1; today < numberOfDays; today++){
            //if today's price is greater than yesterdays, there is a better selling opportunity
            if(prices[today] > prices[today-1]){
                
                //prev r days profit + (todays buy - prev r days last sell)
                int todaysValue = max(chart[r][today-1]->profit, (chart[r][today-1]->profit + (prices[today] - prices[chart[r][today-1]->strategy->end->sell])));
                
                if(buyingOpportunity){
                    if(chart[r][today-1]->strategy->numberOfTrades >= r){ //cannot add a trade
                        if(prices[chart[r][today-1]->strategy->end->buy] < prices[today]){
                            chart[r][today]->strategy->end->buy = today-1;
                        }
                        strategy* todaysStrategy = chart[r][today]->strategy->end;
                        todaysStrategy->sell = today;
                        //pick whichever strategy is better: selling on a previous day or selling today
                        //if todays strategy is better than yesterdays strategy
                        if(calculateProfitFromStrategy(todaysStrategy, prices) > calculateProfitFromStrategy(chart[r][today-1]->strategy, prices)){
                            chart[r][today]->strategy = todaysStrategy;
                            chart[r][today]->profit = calculateProfitFromStrategy(todaysStrategy, prices);
                        }else{
                            chart[r][today]->profit = chart[r][today-1]->profit;
                            chart[r][today]->strategy = chart[r][today-1]->strategy;
                        }
                        
                    }else{ //can add trade
                        strategy* todaysStrategy = chart[r][today]->strategy;
                        todaysStrategy->beg = chart[r][today-1]->strategy;
                        todaysStrategy->end = todaysStrategy;
                        todaysStrategy->buy = chart[r][today-1]->strategy->buy;
                        todaysStrategy->sell = chart[r][today-1]->strategy->sell;
                        todaysStrategy->numberOfTrades = chart[r][today-1]->strategy->numberOfTrades;
                        todaysStrategy->next = chart[r][today-1]->strategy->end;
                        todaysStrategy = addTradeToStrategy(todaysStrategy, today-1, today);
                        if(calculateProfitFromStrategy(todaysStrategy, prices) > calculateProfitFromStrategy(chart[r][today-1]->strategy, prices)){
                            chart[r][today]->strategy = todaysStrategy;
                            chart[r][today]->profit = calculateProfitFromStrategy(todaysStrategy, prices);
                        }else{
                            chart[r][today]->profit = chart[r][today-1]->profit;
                            chart[r][today]->strategy = chart[r][today-1]->strategy;
                        }
                    }
                }else{ //what to do if there isn't a buying opportunity
                    chart[r][today]->strategy->end->sell = today;
                
                    if(r > 0){
                        chart[r][today]->profit = max(chart[r-1][today]->profit, todaysValue);
                    }else{
                        chart[r][today]->profit = todaysValue;
                    }
                }
                
                
                
            }else{ //today's price is less  than yesterdays = buying opportunity
                if(today != firstBuy){
                    buyingOpportunity = true;
                }
                
                chart[r][today] = chart[r][today-1];
                
                
            }
        }
        buyingOpportunity = false;
    }
    
    for(int i = 0; i < inputr; i++){
        for(int j = 0; j < numberOfDays; j++){
            cout << chart[i][j]->profit << " ";
            //cout << chart[i][j] << " ";
        }
        cout << endl;
    }
        
    for(int i = 0; i < inputr; i++){
        for(int j = 0; j < numberOfDays; j++){
            printStrategy(chart[i][j]->strategy);
            cout << endl;
        }
        cout << endl;
    }
    
    return 0;
}

