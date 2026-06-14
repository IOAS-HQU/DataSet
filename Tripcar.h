#pragma once
#include<vector>
#include<string>
using namespace std;

#ifndef TRIPCAR_H
#define TRIPCAR_H

const int MAX1 = 601;
const int FUNC_NUM1 = 4;

//订单
struct Order
{
	int peoples;                        //订单乘客人数
	double order_id;                       //订单编号
	string src_coordinate;                 //订单出发点经纬度
	string dest_coordinate;                //订单目的地经纬度
	double s_time;                        //订单服务时间
	double b_time;                        //订单最早服务时间
	double e_time;                        //订单最晚服务时间
	double location_type;                 //订单种类：0:a->b 1:b->a
	//vector<double>order_customers;       //订单的两个接送点
	double wait_otime = 0;                 //该订单目前所在行程的等待时间
	double travel_otime = 0;               //该订单目前所在行程的行驶时间
};

//行程
struct Trip
{
	int trip_id;                        //行程编号
	int trip_set;                       //行程集合属于trip1、trip2
	//vector<int>trip_customers;        //行程中含有的客户点标号
	vector<int>trip_orders;             //行程中含有的订单编号
	double travel_dist;                 //这趟行程的行驶距离
	double travel_time;                 //这趟行程的行驶时间
	double wait_time;                   //这趟行程的等待时间
	double delay_time;		            //这趟行程的延迟时间
	double tb_time;                     //这趟行程的最早服务时间
	double te_time;                     //这趟行程的最晚服务时间
	double capacity;                    //乘车人数
	double trip_car = 4;                //行程对应车辆规模
	double f[FUNC_NUM1];                 //4个目标值：f[0]:最小化车辆数目 f[1]:最小化乘客等待时间、f[2]:最小化总行驶距离、f[3]:最大化上座率
	vector<int>orders_off;              //下客顺序
};

//车辆
struct Car
{
	int car_id;                        //车辆编号
	int load = 5;				       //车辆载客量
	vector<int>car_trips;              //车辆分配的行程编号
	vector<int>relax;                  //记录该行程结束时是否需要休息，0-不需要 1-需要
	double car_dist;                   //该车辆的行驶总距离
	double car_delay;                  //该车辆的总延迟时间
	double customers;				   //客户数量
	double attendance;				   //上座率
	double f[4];
};

struct Chromosome                   //路径规划方案
{
	vector<Trip>trips;              //行程集合
	vector<Trip>trip1;              //行程1集合：a->b
	vector<Trip>trip2;              //行程2集合：b->a
	vector<Car>cars;                //车辆集合
	double f[4];					//4个目标值：f[0]最小化车辆数 f[1]:最小化乘客等待时间、f[2]:最小化总行驶距离、f[3]:最大化上座率
	int box_f[4];                   //非占有解的关联矩阵代表解在<-解空间上的位置
	int count;
	double cub_len;
	int ranking;

	bool operator < (const Chromosome& chrome)
	{
		return f[0] < chrome.f[0] || (f[0] == chrome.f[0] && f[1] < chrome.f[1]) || (f[0] == chrome.f[0] && f[1] == chrome.f[1] && f[2] > chrome.f[2]);
	}

	double similarity;            //类似；相像性; 相仿性; 类似性，相似物;
	double crowding;              //拥挤现象；晕线加密
};

#endif // TRIPCAR_H
