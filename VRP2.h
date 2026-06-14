#pragma once
#include<vector>
#include<string>
#include "ExperimentConfig.h"
using namespace std;

#ifndef VRP2_H
#define VRP2_H

const int MAX = 601;
const int FUNC_NUM = 4;

const int Kk = 8;
double a_time[MAX];		         	//到达订单时间数组
double l_time[MAX];			        //离开订单时间数组
double start_time[MAX];             //开始行程时间数组
double end_time[MAX];				//结束行程时间数组
double w_time[MAX];			        //等待时间数组
double d_time[MAX];			        //延迟时间数组
double total_wait[MAX];             //i点之前的总等待时间数组
double total_delay[MAX];            //i点之后的总延迟时间数组
double b_demand[MAX];               //在客户点i之前（包括i）的已有乘客人数
double a_demand[MAX];               //在客户点i之后（包括i）的已有乘车人数
double b_distance[MAX];             //在客户点i之前（包括i）的已行驶路径长度
double a_distance[MAX];             //在客户点i之后（包括i）的已行驶路径长度
double b__time[MAX];                //在客户点i之前（包括i）的已行驶时间
double a__time[MAX];                //在客户点i之后（包括i）的已行驶时间
int max_wait[MAX];

int cust_num;                       //客户点数目
int order_num = 600;                //订单数目
int trip_num = 600;				    //行程数目
int car_num = 600;                   //总车辆数目
int car_num1;                       //五座车车辆数目
int car_num2;                       //七座车车辆数目
double max_capacity_1 = 4;          //五座车最大载客数
double max_capacity_2 = 6;          //七座车最大载客数
extern double max_delay_time;         //最大延迟时间

#endif // VRP2_H
