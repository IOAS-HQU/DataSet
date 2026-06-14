#include "pch.h"
#include "Individual.h"
#include"VRP1.h"
#include"VRP2.h"
#include"GeneticAlgorithm.h"
#include "ExperimentConfig.h"
#include<assert.h>
#define pai 3.1415926

double max_delay_time = 0;

void clear_trip(Trip& trip)
{
	trip.capacity = 0;
	trip.delay_time = 0;
	trip.travel_dist = 0;
	trip.travel_time = 0;
	trip.wait_time = 0;
	trip.trip_orders.clear();
}

void change_delay(int delay_time)
{
	max_delay_time = delay_time;
}

bool comparator_car_0(Car a, Car b)
{
	return a.f[0] > b.f[0];
}

bool comparator_car_1(Car a, Car b)
{
	return a.f[1] < b.f[1];
}

bool comparator_car_2(Car a, Car b)
{
	return a.f[2] < b.f[2];
}

bool comparator_car_3(Car a, Car b)
{
	return a.f[3] < b.f[3];
}

//删除字符串中空格，制表符等tab等无效字符
string Trim(string& str)
{
	str.erase(0, str.find_first_not_of(" \t\r\n"));
	str.erase(str.find_last_not_of(" \t\r\n") + 1);
	return str;
}

int test(Trip te[]) {
	int res = 0;
	for(int i = 1; i <= MAX1; i++) {
		if (res != 0 && te[i].trip_orders.size() == 0)
			break;
		res = te[i].trip_id;
	}
	return res;
}

int test_check(int t[]) {
	int count = 0;
	for (int i = 1; i < order_num; i++) {
		if (t[i] == 1)
			count++;
	}
	return count;
}

//分解字符串为一组字符串，s为要分解的字符串，substr为分隔字符串
vector<string> splitStr(string s, string substr)
{
	char buf[100];
	vector<string> strlist;
	string str(s);
	strcpy(buf, str.c_str());
	char* ptr;
	ptr = strtok(buf, substr.c_str());
	while (ptr != NULL)
	{
		strlist.push_back(ptr);
		ptr = strtok(NULL, substr.c_str());
	}
	return strlist;
}

//以分钟为单位
double converted_to_number(string strtime)
{
	vector<string> arr = splitStr(strtime, ":");
	double time = 0;
	time = (atof(arr[0].c_str())) * 60 + atof(arr[1].c_str());
	return time;
}

//以分钟为单位
//只转换"2018-09-01 01:00:00"以后到"2018-09-02 01:00:00"之前的24h的订单，其他时间为-100000
double time_to_number(string string_time)
{
	double NO_COMPUTER = -100000;
	double tmptime = 0;
	vector<string> date_time = splitStr(string_time, " ");
	if (date_time[0] == "2023/4/17")
		tmptime = converted_to_number(date_time[1]);
	/*else if (date_time[0] == "2018/9/2")
	{
		tmptime = double(24 * 60);
		tmptime += converted_to_number(date_time[1]);
	}*/
	else
		tmptime = NO_COMPUTER++;
	return tmptime;
}

void getData(string spec)
{
	ifstream fin(spec);                                                 //打开文件流操作
	string line;
	int count = 1;
	getline(fin, line);
	//spec : 种群，id, demand, arrival_time
	while (getline(fin, line))
		//整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取
	{
		istringstream sin(line);
		vector<string> fields;
		string field;
		while (getline(sin, field, ','))
		{
			fields.push_back(field);
		}
		Order ord;
		string order_id, peoples, src_coordinate, dest_coordinate, s_time, b_time, e_time, location_type;
		order_id = Trim(fields[0]);
		peoples = Trim(fields[1]);
		src_coordinate = Trim(fields[2]);
		dest_coordinate = Trim(fields[3]);
		s_time = Trim(fields[4]);
		b_time = Trim(fields[5]);
		e_time = Trim(fields[6]);
		location_type = Trim(fields[7]);

		vector<string>date_time = splitStr(src_coordinate, ";");
		string str1 = date_time[1] + "," + date_time[0];
		date_time = splitStr(dest_coordinate, ";");
		string str2 = date_time[1] + "," + date_time[0];

		ord.order_id = atof(order_id.c_str());
		ord.peoples = atof(peoples.c_str());
		ord.s_time = atof(s_time.c_str());
		ord.b_time = atof(b_time.c_str());
		ord.e_time = atof(e_time.c_str());
		/*ord.b_time = time_to_number(b_time);
		ord.e_time = time_to_number(e_time);*/
		ord.location_type = atof(location_type.c_str());
		//ord.order_customers.push_back(atof(src_coordinate.c_str()));
		//ord.order_customers.push_back(atof(dest_coordinate.c_str()));
		ord.src_coordinate = str1;
		ord.dest_coordinate = str2;
		order[count++] = ord;
	}
	order_num = count;
	fin.close();
}

void get_peer_time(string spec)
{
	ifstream fin(spec);
	string line;
	int i = 1;
	while (getline(fin, line))
	{
		istringstream sin(line);
		vector<string> fields;
		string field;
		while (getline(sin, field, ','))
		{
			fields.push_back(field);
		}
		for (int j = 1; j < order_num; j++)
		{
			peer_time[i][j] = atof(fields[j - 1].c_str());
		}
		i = i + 1;
	}
	fin.close();
}

void get_peer_time2(string spec)
{
	ifstream fin(spec);
	string line;
	int i = 1;
	while (getline(fin, line))
	{
		istringstream sin(line);
		vector<string> fields;
		string field;
		while (getline(sin, field, ','))
		{
			fields.push_back(field);
		}
		for (int j = 1; j < order_num; j++)
		{
			peer_time_2[i][j] = atof(fields[j - 1].c_str());
		}
		i = i + 1;
	}
	fin.close();
}

void get_peer_time3(string spec)
{
	ifstream fin(spec);
	string line;
	int i = 1;
	while (getline(fin, line))
	{
		istringstream sin(line);
		vector<string> fields;
		string field;
		while (getline(sin, field, ','))
		{
			fields.push_back(field);
		}
		for (int j = 1; j < order_num; j++)
		{
			peer_time_3[i][j] = atof(fields[j - 1].c_str());
		}
		i = i + 1;
	}
	fin.close();
}

void get_peer_time4(string spec)
{
	ifstream fin(spec);
	string line;
	int i = 1;
	while (getline(fin, line))
	{
		istringstream sin(line);
		vector<string> fields;
		string field;
		while (getline(sin, field, ','))
		{
			fields.push_back(field);
		}
		for (int j = 1; j < order_num; j++)
		{
			peer_time_4[i][j] = atof(fields[j - 1].c_str());
		}
		i = i + 1;
	}
	fin.close();
}

void get_peer_srcdistance(string spec)
{
	ifstream fin(spec);
	string line;
	int i = 1;
	while (getline(fin, line))
	{
		istringstream sin(line);
		vector<string> fields;
		string field;
		while (getline(sin, field, ','))
		{
			fields.push_back(field);
		}
		for (int j = 1; j < order_num; j++)
		{
			peer_srcdistance[i][j] = atof(fields[j - 1].c_str());
		}
		i = i + 1;
	}
	fin.close();
}

void get_peer_desdistance(string spec)
{
	ifstream fin(spec);
	string line;
	int i = 1;
	while (getline(fin, line))
	{
		istringstream sin(line);
		vector<string> fields;
		string field;
		while (getline(sin, field, ','))
		{
			fields.push_back(field);
		}
		for (int j = 1; j < order_num; j++)
		{
			peer_desdistance[i][j] = atof(fields[j - 1].c_str());
		}
		i = i + 1;
	}
	fin.close();
}

void get_peer_sd_distance(string spec)
{
	ifstream fin(spec);
	string line;
	int i = 1;
	while (getline(fin, line))
	{
		istringstream sin(line);
		vector<string> fields;
		string field;
		while (getline(sin, field, ','))
		{
			fields.push_back(field);
		}
		for (int j = 1; j < order_num; j++)
		{
			peer_sd_distance[i][j] = atof(fields[j - 1].c_str());
		}
		i = i + 1;
	}
	fin.close();
}

void get_peer_sd2_distance(string spec)
{
	ifstream fin(spec);
	string line;
	int i = 1;
	while (getline(fin, line))
	{
		istringstream sin(line);
		vector<string> fields;
		string field;
		while (getline(sin, field, ','))
		{
			fields.push_back(field);
		}
		for (int j = 1; j < order_num; j++)
		{
			peer_sd2_distance[i][j] = atof(fields[j - 1].c_str());
		}
		i = i + 1;
	}
	fin.close();
}

bool comp_angle7(int a, int b)
{
	return arcValues[a] < arcValues[b];
}

bool comp_early(int a, int b)
{
	return order[a].b_time < order[b].b_time;
}

bool comp_trip_early(int a, int b)
{
	return trip[a].tb_time < trip[b].tb_time;
}

bool is_operator_enabled(int index);
int random_enabled_operator();

int initialization_offset = 0;

// Initialization diversity used for ablation: rotate the time-sorted request
// sequence so the initial population is not made of identical greedy solutions.
void set_initialization_offset(int offset)
{
	initialization_offset = offset < 0 ? 0 : offset;
}

void rotate_time_sequence(vector<int>& ids)
{
	if (!EXPERIMENT_INIT_ROTATION_DIVERSITY || ids.size() <= 1) return;
	int shift = initialization_offset % (int)ids.size();
	if (shift > 0) rotate(ids.begin(), ids.begin() + shift, ids.end());
}

double calculateDis(string src, string desc)
{
	vector<double> a, b;
	vector<string> date_time = splitStr(src, ",");
	a.push_back(atof(date_time[0].c_str()));
	a.push_back(atof(date_time[1].c_str()));
	date_time = splitStr(desc, ",");
	b.push_back(atof(date_time[0].c_str()));
	b.push_back(atof(date_time[1].c_str()));
	return sqrt((a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]));
}

void init_trip1(Chromosome& chrome)
{
	//请求所在的集合，生成服务请求集合
	vector <int> order1, temp_order1;
	//该函数将一个新的元素加到vector的最后面
	for (int i = 1; i < order_num; ++i)
	{
		if (order[i].location_type == 0)
		{
			order1.push_back(order[i].order_id);
		}
	}
	sort(order1.begin(), order1.end(), comp_early);
	rotate_time_sequence(order1);
	//生成一个行程
	Trip new_trip1;
	int count = 1;
	while (true)
	{
		clear_trip(new_trip1);                                  //清空新的行程
		double w_time = 0;										//临时变量等待时间初始化为0
		double t_time = 0;										//临时变量行驶时间初始化为0
		double capacity = 0;									//临时变量容量初始化为0

		int o_ind = 0;	                //记录上一个客户点编号
		int last = order1[o_ind];
		new_trip1.trip_orders.push_back(last);
		capacity = order[last].peoples;
		t_time = order[last].b_time;
		vector<int>::iterator iter = order1.begin();
		order1.erase(iter + o_ind);
		for (iter = order1.begin(); iter != order1.end(); ++iter)
			temp_order1.push_back(*iter);
		order1 = temp_order1;
		temp_order1.clear();

		if (order1.size() == 0)
		{
			new_trip1.trip_id = count;
			new_trip1.trip_set = 1;
			trip[count++] = new_trip1;
			chrome.trips.push_back(new_trip1);
			chrome.trip1.push_back(new_trip1);
			break;
		}
		else
			while (true)
			{
				iter = order1.begin();
				int r_ind = 0;                                   //按时间序列扫描候选请求
				t_time += peer_time[last][order1[r_ind]];				              //路径时间等于路径上行驶的时间
				w_time = t_time > order[order1[r_ind]].b_time ? 0 : order[order1[r_ind]].b_time - t_time;	  //根据公式求等待时间
				//根据容量约束或时间窗约束来判断，是否可以加入当前行程的情况
				//情况① 不可以加入当前路径，加入后会导致以下三种情况
				//1.载客人数大于行程最大载客量（4）
				//2.迟到，行驶时间大于客户点r_ind的最迟时间 + 最大延迟时间
				if (capacity + order[order1[r_ind]].peoples > max_capacity_1 || t_time > order[order1[r_ind]].e_time + max_delay_time)
				{
					t_time -= peer_time[last][order1[r_ind]];              //将行驶时间归于上一个行驶时间
					temp_order1.push_back(order1[r_ind]);                  //将这个客户点请求放入临时客户点点集
					order1.erase(iter + r_ind);                            //不能用的客户点从cust客户点集中删除
					if (order1.size() == 0)
						break;                                           //客户点数为0时，跳出循环
					continue;                                            //否则，继续判断
				}
				//情况② 满足时间窗约束和车辆容量约束的情况：
				else
				{
					t_time += w_time + order[order1[r_ind]].s_time;                  //行驶时间 = 上一个行驶时间 + 在r_ind客户点的等待时间 + 在r_ind客户点的服务时间
					capacity += order[order1[r_ind]].peoples;						 //第r_ind客户点的容量 = 前一个客户点的容量 + 第r_ind客户点的容量
					new_trip1.trip_orders.push_back(order1[r_ind]);      //将客户点插入到路径中
					last = order1[r_ind];
					order1.erase(iter + r_ind);							 //从集合cust中删除第r_ind个客户点
					for (iter = order1.begin(); iter != order1.end(); iter++)//添加成功后，将剩余的客户点放入到temp_cust客户点集中
						temp_order1.push_back(*iter);						 //即将cust集合中的元素copy到temp_cust集合中
					order1 = temp_order1;                                    //将剩余的服务请求重新加入到cust集合中
					temp_order1.clear();									 //临时集合清空
				}
				//如果客户点数为0时，跳出循环；
				//当cust集合中的元素为0是因为cust中的客户点都不适合添加到此路径,可以开始新路径的创建
				if (order1.size() == 0)
					break;
			}
		order1 = temp_order1;
		temp_order1.clear();
		new_trip1.trip_id = count;
		new_trip1.trip_set = 1;
		trip[count++] = new_trip1;
		chrome.trips.push_back(new_trip1);
		chrome.trip1.push_back(new_trip1);
		if (order1.size() == 0)
			break;
	}
}

void init_trip2(Chromosome& chrome)
{
	//请求所在的集合，生成服务请求集合
	vector <int> order2, temp_order2;
	//该函数将一个新的元素加到vector的最后面
	for (int i = 1; i < order_num; ++i)
	{
		if (order[i].location_type == 1)
		{
			order2.push_back(order[i].order_id);
		}
	}
	sort(order2.begin(), order2.end(), comp_early);
	rotate_time_sequence(order2);
	//生成一个行程
	Trip new_trip2;
	int count = chrome.trip1.size() + 1;
	while (true)
	{
		clear_trip(new_trip2);                                  //清空新的行程
		double w_time = 0;										//临时变量等待时间初始化为0
		double t_time = 0;										//临时变量行驶时间初始化为0
		double capacity = 0;									//临时变量容量初始化为0

		int o_ind = 0;	                    //记录上一个客户点编号
		int last = order2[o_ind];
		new_trip2.trip_orders.push_back(last);
		capacity = order[last].peoples;
		t_time = order[last].b_time;
		vector<int>::iterator iter = order2.begin();
		order2.erase(iter + o_ind);
		for (iter = order2.begin(); iter != order2.end(); ++iter)
			temp_order2.push_back(*iter);
		order2 = temp_order2;
		temp_order2.clear();

		if (order2.size() == 0)
		{
			new_trip2.trip_id = count;
			new_trip2.trip_set = 2;
			trip[count++] = new_trip2;
			chrome.trips.push_back(new_trip2);
			chrome.trip2.push_back(new_trip2);
			break;
		}
		else
			while (true)
			{
				iter = order2.begin();
				int r_ind = 0;                                   //按时间序列扫描候选请求
				t_time += peer_time[last][order2[r_ind]];				              //路径时间等于路径上行驶的时间
				w_time = t_time > order[order2[r_ind]].b_time ? 0 : order[order2[r_ind]].b_time - t_time;	  //根据公式求等待时间
				//根据容量约束或时间窗约束来判断，是否可以加入当前行程的情况
				//情况① 不可以加入当前路径，加入后会导致以下三种情况
				//1.载客人数大于行程最大载客量（4）
				//2.迟到，行驶时间大于客户点r_ind的最迟时间 + 最大延迟时间
				if (capacity + order[order2[r_ind]].peoples > max_capacity_1 || t_time > order[order2[r_ind]].e_time + max_delay_time)
				{
					t_time -= peer_time[last][order2[r_ind]];              //将行驶时间归于上一个行驶时间
					temp_order2.push_back(order2[r_ind]);                    //将这个客户点请求放入临时客户点点集
					order2.erase(iter + r_ind);                            //不能用的客户点从cust客户点集中删除
					if (order2.size() == 0)
						break;                                           //客户点数为0时，跳出循环
					continue;                                            //否则，继续判断
				}
				//情况② 满足时间窗约束和车辆容量约束的情况：
				else
				{
					t_time += w_time + order[order2[r_ind]].s_time;                  //行驶时间 = 上一个行驶时间 + 在r_ind客户点的等待时间 + 在r_ind客户点的服务时间
					capacity += order[order2[r_ind]].peoples;						 //第r_ind客户点的容量 = 前一个客户点的容量 + 第r_ind客户点的容量
					new_trip2.trip_orders.push_back(order2[r_ind]);      //将客户点插入到路径中
					last = order2[r_ind];
					order2.erase(iter + r_ind);							 //从集合cust中删除第r_ind个客户点
					for (iter = order2.begin(); iter != order2.end(); iter++)//添加成功后，将剩余的客户点放入到temp_cust客户点集中
						temp_order2.push_back(*iter);						 //即将cust集合中的元素copy到temp_cust集合中
					order2 = temp_order2;                                    //将剩余的服务请求重新加入到cust集合中
					temp_order2.clear();									 //临时集合清空
				}
				//如果客户点数为0时，跳出循环；
				//当cust集合中的元素为0是因为cust中的客户点都不适合添加到此路径,可以开始新路径的创建
				if (order2.size() == 0)
					break;
			}
		order2 = temp_order2;
		temp_order2.clear();
		new_trip2.trip_id = count;
		new_trip2.trip_set = 2;
		trip[count++] = new_trip2;
		chrome.trips.push_back(new_trip2);
		chrome.trip2.push_back(new_trip2);
		if (order2.size() == 0)
			break;
	}
}

void compute_car_specialtime(Car& car, Trip trip[])
{
	if (car.car_trips.size() == 0)
		return;
	//计算各行程的开始行程时间、结束行程时间、行程结束时是否需要休息
	double accu_time = 0;                      //累计时间数组
	double curr_time = 0;                      //当前时间数组
	double dis_time = 0;                       //行程之间的行驶时间
	vector<int>::iterator t_iter;
	for (t_iter = car.car_trips.begin(); t_iter != car.car_trips.end(); ++t_iter)
	{
		if (t_iter == car.car_trips.begin())
		{
			start_time[*t_iter] = trip[*t_iter].tb_time;
			curr_time = trip[*t_iter].travel_time;
			accu_time = trip[*t_iter].travel_time - order[trip[*t_iter].trip_orders[0]].b_time;
			end_time[*t_iter] = curr_time;
		}
		else
		{
			int len = trip[*(t_iter - 1)].trip_orders.size();
			dis_time = peer_time_3[trip[*(t_iter - 1)].trip_orders[len - 1]][trip[*t_iter].trip_orders[0]];
			accu_time += dis_time;
			curr_time += dis_time;
			//如果早到
			if (curr_time < trip[*t_iter].tb_time)
			{
				accu_time += trip[*t_iter].tb_time - curr_time;
				curr_time = trip[*t_iter].tb_time;
			}
			accu_time += trip[*t_iter].travel_time - order[trip[*t_iter].trip_orders[0]].b_time;
			start_time[*t_iter] = curr_time;
			if (accu_time > 240)
			{
				accu_time = 0;
				curr_time += 20;
				car.relax.push_back(-1);
			}
			else
				car.relax.push_back(0);
			start_time[*t_iter] = curr_time;
			//正常到达
			curr_time += trip[*t_iter].travel_time - order[trip[*t_iter].trip_orders[0]].b_time;
			end_time[*t_iter] = curr_time;
		}
	}
	car.relax.push_back(0);
}

void compute_car_specialtime(Car& car)
{
	if (car.car_trips.size() == 0)
		return;
	//计算各行程的开始行程时间、结束行程时间、行程结束时是否需要休息
	double accu_time = 0;                      //累计时间数组
	double curr_time = 0;                      //当前时间数组
	double dis_time = 0;                       //行程之间的行驶时间
	vector<int>::iterator t_iter;
	for (t_iter = car.car_trips.begin(); t_iter != car.car_trips.end(); ++t_iter)
	{
		if (trip[*t_iter].trip_id == 0)
			return;
		if (t_iter == car.car_trips.begin())
		{
			start_time[*t_iter] = trip[*t_iter].tb_time;
			curr_time = trip[*t_iter].travel_time;
			accu_time = trip[*t_iter].travel_time - order[trip[*t_iter].trip_orders[0]].b_time;
			end_time[*t_iter] = curr_time;
		}
		else
		{
			int len = trip[*(t_iter - 1)].trip_orders.size();
			dis_time = peer_time_3[trip[*(t_iter - 1)].orders_off[len - 1]][trip[*t_iter].trip_orders[0]];
			accu_time += dis_time;
			curr_time += dis_time;
			//如果早到
			if (curr_time < trip[*t_iter].tb_time)
			{
				accu_time += trip[*t_iter].tb_time - curr_time;
				curr_time = trip[*t_iter].tb_time;
			}
			accu_time += trip[*t_iter].travel_time - order[trip[*t_iter].trip_orders[0]].b_time;
			start_time[*t_iter] = curr_time;
			if (accu_time > 240)
			{
				accu_time = 0;
				curr_time += 20;
				car.relax.push_back(-1);
			}
			else
				car.relax.push_back(0);
			start_time[*t_iter] = curr_time;
			//正常到达
			curr_time += trip[*t_iter].travel_time - order[trip[*t_iter].trip_orders[0]].b_time;
			end_time[*t_iter] = curr_time;
		}
	}
	car.relax.push_back(0);
}

//判断该车辆行程是否满足约束
bool check_feasible_car(Car& car)
{
	compute_car_specialtime(car);
	vector<int>::iterator iter;
	for (iter = car.car_trips.begin(); iter != car.car_trips.end(); ++iter)
	{
		if (start_time[*iter] > trip[*iter].te_time + max_delay_time)
			return false;
	}
	return true;
}

//判断该车辆行程是否满足约束
bool check_feasible_car(Car& car, Trip trip[])
{
	compute_car_specialtime(car, trip);
	vector<int>::iterator iter;
	for (iter = car.car_trips.begin(); iter != car.car_trips.end(); ++iter)
	{
		if (start_time[*iter] > trip[*iter].te_time + max_delay_time)
			return false;
	}
	return true;
}

void clean_chromosome(Chromosome& chrome)
{
	chrome.trips.clear();
	chrome.trip1.clear();
	chrome.trip2.clear();
	chrome.cars.clear();
	for (int i = 0; i < FUNC_NUM; ++i)
		chrome.f[i] = 0;
}

void arrange_customer_off(Trip& r)
{
	vector<double> longitudes, latitudes;
	vector<int> custs = r.trip_orders;
	int len = custs.size();
	double lo, la, sum_lo = 0, sum_la = 0;
	vector<string>date_time;

	for (int i = 0; i < len; i++)
	{
		date_time = splitStr(order[custs[i]].dest_coordinate, ",");
		lo = atof(date_time[0].c_str());
		la = atof(date_time[1].c_str());
		longitudes.push_back(lo);
		latitudes.push_back(la);
		sum_lo += lo;
		sum_la += la;
	}

	sum_lo /= len;
	sum_la /= len;

	inds.clear();
	for (int x = 0; x < 5; x++) {
		arcValues[x] = 0;
	}

	for (int i = 0; i < len; i++)
	{
		//下车点排序，计算几个点的极角
		lo = longitudes[i] - sum_lo;
		la = latitudes[i] - sum_la;
		double t = la * 1.0 / lo;
		double arc = atan(t) * (double)180 / pai;
		inds.push_back(i);
		arcValues[i] = arc;
	}

	//根据极角，对下客点进行排序
	sort(inds.begin(), inds.end(), comp_angle7);
	r.orders_off.clear();
	for (int i = 0; i < len; i++)
		r.orders_off.push_back(custs[inds[i]]);
}

//计算行程中各个客户点的行驶时间、等待时间、延迟时间、最早服务时间、最晚服务时间
void compute_trip_time(Trip& trip)
{
	if (trip.trip_orders.size() == 0)
		return;
	vector<int>::iterator iter;
	iter = trip.trip_orders.begin();
	trip.travel_time = order[*iter].b_time;
	l_time[*iter] = trip.travel_time;
	order[*iter].travel_otime = order[*iter].b_time;
	trip.delay_time = 0;
	trip.wait_time = 0;
	order[*iter].wait_otime = 0;
	total_wait[*iter] = 0;
	total_delay[*iter] = 0;
	for (; iter != trip.trip_orders.end() - 1; ++iter)
	{
		double wait = 0, delay = 0;
		trip.travel_time += peer_time[*iter][*(iter + 1)];
		a_time[*(iter + 1)] = trip.travel_time;
		if (trip.travel_time > order[*(iter + 1)].e_time)
		{
			delay = trip.travel_time - order[*(iter + 1)].e_time;
			order[*(iter + 1)].wait_otime = delay;
		}
		trip.delay_time += delay;
		trip.f[1] = trip.delay_time;
		total_delay[*(iter + 1)] = total_delay[*iter] + delay;
		//总的下一个点的延迟时间 = 本客户点已累计的延迟时间 + 下一个点的延迟时间
		//求等待时间
		if (trip.travel_time < order[*(iter + 1)].b_time)
			wait = order[*(iter + 1)].b_time - trip.travel_time;
		w_time[*(iter + 1)] = wait;
		total_wait[*(iter + 1)] = total_wait[*iter] + wait;
		trip.wait_time += wait;
		//路径的总行驶时间 = [k + 1]的行驶时间 + [k + 1]的等待时间 + [k + 1]的服务时间
		trip.travel_time += (wait + order[*(iter + 1)].s_time);
		//第k+1的离开时间 = 第k+1的行驶时间
		l_time[*(iter + 1)] = trip.travel_time;
	}
	//计算各个客户点的最大合法延迟时间
	iter = trip.trip_orders.begin();
	max_wait[*iter] = order[*iter].e_time + max_delay_time;
	iter = trip.trip_orders.end() - 1;
	max_wait[*iter] = order[*iter].e_time + max_delay_time;
	for (iter = trip.trip_orders.end() - 1; iter != trip.trip_orders.begin(); --iter)
	{
		if (iter == trip.trip_orders.end() - 1)
			max_wait[*iter] = order[*iter].e_time + max_delay_time;
		else
			max_wait[*iter] = min(order[*iter].e_time + max_delay_time - a_time[*iter], w_time[*iter] + max_wait[*(iter + 1)]);
	}
	//计算后半段行驶时间加上去
	vector<int> custs = trip.trip_orders;
	vector<string> res;
	size_t len = trip.trip_orders.size();
	for (int i = 1; i < len; i++)
	{
		trip.travel_time += peer_time_2[custs[i - 1]][custs[i]];
	}
	trip.travel_time += peer_time_4[custs[len - 1]][custs[0]];
	//计算最早服务时间
	iter = trip.trip_orders.begin();
	trip.tb_time = order[*iter].b_time;
	//计算最晚服务时间
	trip.te_time = order[*iter].e_time;
}

void compute_car_time(Car& car)
{
	if (car.car_trips.size() == 0)
		return;
	car.car_delay = 0;
	int count = 0;
	vector<int>::iterator iter = car.car_trips.begin();
	vector<int> orders;
	double tra_time = order[trip[*iter].trip_orders[0]].b_time;

	//1.计算车辆行程特殊时间
	compute_car_specialtime(car);

	//2.生成车辆所有行程的所有订单按访问顺序排好
	vector<int>::iterator iter1, iter2;
	for (iter1 = car.car_trips.begin(); iter1 != car.car_trips.end(); iter1++)
	{
		for (iter2 = trip[*iter1].trip_orders.begin(); iter2 != trip[*iter1].trip_orders.end(); iter2++)
		{
			orders.push_back(*iter2);
		}
		//0和-1用来区分行程且表示行程结束是否需要休息 0:不需要 -1:需要
		orders.push_back(car.relax[count]);
		count += 1;
	}

	//3.计算各订单之间的延迟时间
	//5 9 -1 15 12 20 0
	vector<int>::iterator iter3 = orders.begin();
	order[*iter3].travel_otime = tra_time;
	for (iter3 = orders.begin(); iter3 != orders.end() - 2; ++iter3)
	{
		double delay = 0;
		if (*(iter3) > 0)
		{
			if (*(iter3 + 1) > 0)
			{
				tra_time += order[*iter3].s_time + peer_time[*iter3][*(iter3 + 1)];
				order[*(iter3 + 1)].travel_otime = tra_time;
				if (tra_time > order[*(iter3 + 1)].e_time)
				{
					delay = tra_time - order[*(iter3 + 1)].e_time;
					order[*(iter3 + 1)].wait_otime = delay;
				}
				car.car_delay += delay;
			}
			else
			{
				if (*(iter3 + 1) == 0)
					continue;
				else
					tra_time += 20;
			}
		}
		else
		{
			tra_time += order[*(iter3 - 1)].s_time + peer_time_3[*(iter3 - 1)][*(iter3 + 1)];
			order[*(iter3 + 1)].travel_otime = tra_time;
			if (tra_time > order[*(iter3 + 1)].e_time)
			{
				delay = tra_time - order[*(iter3 + 1)].e_time;
				order[*(iter3 + 1)].wait_otime = delay;
			}
			car.car_delay += delay;
		}
	}
}

//计算生成车辆行程后的总延迟时间
void compute_car_time(Car& car, Trip trip[])
{
	if (car.car_trips.size() == 0)
		return;
	car.car_delay = 0;
	int count = 0;
	vector<int>::iterator iter = car.car_trips.begin();
	vector<int> orders;
	double tra_time = order[trip[*iter].trip_orders[0]].b_time;

	//1.计算车辆行程特殊时间
	compute_car_specialtime(car, trip);

	//2.生成车辆所有行程的所有订单按访问顺序排好
	vector<int>::iterator iter1, iter2;
	for (iter1 = car.car_trips.begin(); iter1 != car.car_trips.end(); iter1++)
	{
		for (iter2 = trip[*iter1].trip_orders.begin(); iter2 != trip[*iter1].trip_orders.end(); iter2++)
		{
			orders.push_back(*iter2);
		}
		//0和-1用来区分行程且表示行程结束是否需要休息 0:不需要 -1:需要
		orders.push_back(car.relax[count]);
		count += 1;
	}

	//3.计算各订单之间的延迟时间
	//5 9 -1 15 12 20 0
	vector<int>::iterator iter3 = orders.begin();
	order[*iter3].travel_otime = tra_time;
	for (iter3 = orders.begin(); iter3 != orders.end() - 2; ++iter3)
	{
		double delay = 0;
		if (*(iter3) > 0)
		{
			if (*(iter3 + 1) > 0)
			{
				tra_time += order[*iter3].s_time + peer_time[*iter3][*(iter3 + 1)];
				order[*(iter3 + 1)].travel_otime = tra_time;
				if (tra_time > order[*(iter3 + 1)].e_time)
				{
					delay = tra_time - order[*(iter3 + 1)].e_time;
					order[*(iter3 + 1)].wait_otime = delay;
				}
				car.car_delay += delay;
			}
			else
			{
				if (*(iter3 + 1) == 0)
					continue;
				else
					tra_time += 20;
			}
		}
		else
		{
			tra_time += order[*(iter3 - 1)].s_time + peer_time_3[*(iter3 - 1)][*(iter3 + 1)];
			order[*(iter3 + 1)].travel_otime = tra_time;
			if (tra_time > order[*(iter3 + 1)].e_time)
			{
				delay = tra_time - order[*(iter3 + 1)].e_time;
				order[*(iter3 + 1)].wait_otime = delay;
			}
			car.car_delay += delay;
		}
	}
}

void adjust_tripid(Chromosome& chrome) {
	vector<Trip>::iterator t_iter = chrome.trips.begin();
	vector<Trip>::iterator t1, t2;
	vector<Car>::iterator c_iter;
	vector<int>::iterator iter;
	int temp_id = 0;
	int real_id = 0;
	int ox = 0;
	for (; t_iter != chrome.trips.end(); t_iter++) {
		temp_id = t_iter->trip_id;
		real_id = real_id + 1;
		//①自己chrome.trips的id修正
		t_iter->trip_id = real_id;
		//②通过temp_id修正trip1、trip2
		if (t_iter->trip_set == 1) {
			for (t1 = chrome.trip1.begin(); t1 != chrome.trip1.end(); t1++) {
				if (t1->trip_id == temp_id) {
					t1->trip_id = real_id;
					break;
				}
			}
		}
		else {
			for (t2 = chrome.trip2.begin(); t2 != chrome.trip2.end(); t2++) {
				if (t2->trip_id == temp_id) {
					t2->trip_id = real_id;
					break;
				}
			}
		}
		//③通过temp_id遍历搜寻car_trip里的id进行修正
		for (c_iter = chrome.cars.begin(); c_iter != chrome.cars.end(); c_iter++) {
			ox = 0;
			for (iter = c_iter->car_trips.begin(); iter != c_iter->car_trips.end(); iter++) {
				if ((*iter) == temp_id) {
					(*iter) = real_id;
					ox = 1;
					break;
				}
			}
			if (ox == 1)
				break;
		}
	}
}

void sync_Info(Chromosome& chrome, Car car, Trip t[]) {
	//①将car信息同步到trips、trip1、trip2
	vector<int>::iterator iter;
	for (iter = car.car_trips.begin(); iter != car.car_trips.end(); iter++) {
		chrome.trips.push_back(t[*iter]);
		if (t[*iter].trip_set == 1)
			chrome.trip1.push_back(t[*iter]);
		else
			chrome.trip2.push_back(t[*iter]);
	}
}

////计算行程trip的容量
//void compute_trip_capacity(Trip& trip)
//{
//	if (trip.trip_orders.size() == 0)
//		return;
//	//计算行程的载客数即容量
//	trip.capacity = 0;
//	vector<int>::iterator iter;
//	for (iter = trip.trip_orders.begin(); iter != trip.trip_orders.end(); ++iter)
//	{
//		trip.capacity += order[*iter].peoples;
//		b_demand[*iter] = 0;
//		a_demand[*iter] = 0;
//	}
//	for (iter = trip.trip_orders.begin(); iter != trip.trip_orders.end(); ++iter)
//	{
//		if (iter == trip.trip_orders.begin())
//			b_demand[*iter] = order[*iter].peoples;
//		else
//			b_demand[*iter] += (b_demand[*(iter - 1)] + order[*iter].peoples);
//	}
//	for (iter = trip.trip_orders.end() - 1; iter != trip.trip_orders.begin(); --iter)
//	{
//		if (iter == trip.trip_orders.end() - 1)
//			a_demand[*iter] = order[*iter].peoples;
//		else
//			a_demand[*iter] += (a_demand[*(iter + 1)] + order[*iter].peoples);
//	}
//}

//计算行程trip的容量
void compute_trip_capacity(Trip& trip)
{
	if (trip.trip_orders.size() == 0)
		return;
	if (trip.trip_id == 0)
		return;
	//计算行程的载客数即容量
	trip.capacity = 0;
	vector<int>::iterator iter;
	for (iter = trip.trip_orders.begin(); iter != trip.trip_orders.end(); ++iter)
	{
		trip.capacity += order[*iter].peoples;
	}
	for (iter = trip.trip_orders.begin(); iter != trip.trip_orders.end(); ++iter)
	{
		b_demand[*iter] = 0;
		a_demand[*iter] = 0;
	}
	for (iter = trip.trip_orders.begin(); iter != trip.trip_orders.end(); ++iter)
	{
		if (iter == trip.trip_orders.begin())
			b_demand[*iter] = order[*iter].peoples;
		else
			b_demand[*iter] += (b_demand[*(iter - 1)] + order[*iter].peoples);
	}
	vector<int>::reverse_iterator back;
	for (back = trip.trip_orders.rbegin(); back != trip.trip_orders.rend(); ++back)
	{
		if (back == trip.trip_orders.rbegin())
			a_demand[*back] = order[*back].peoples;
		else
			a_demand[*back] += (a_demand[*(back - 1)] + order[*back].peoples);
	}

}

//计算行程的长度
void compute_trip_distance(Trip& trip)
{
	if (trip.trip_orders.size() == 0)
		return;
	vector<int> custon = trip.trip_orders;
	vector<int> custoff = trip.orders_off;
	if (trip.orders_off.size() != trip.trip_orders.size())
	{
		arrange_customer_off(trip);
		custoff = trip.orders_off;
	}

	trip.travel_dist = 0;
	size_t len = trip.trip_orders.size();
	vector<int>::iterator iter;
	for (int i = 1; i < len; i++)
	{
		trip.travel_dist += peer_srcdistance[custon[i - 1]][custon[i]];
		trip.travel_dist += peer_desdistance[custoff[i - 1]][custoff[i]];
	}
	trip.travel_dist += peer_sd2_distance[custon[len - 1]][custoff[0]];
	trip.f[2] = trip.travel_dist;
}

//计算生成车辆的行驶长度(加上行程之间的行驶长度）
void compute_car_distance_only(Car& car)
{
	if (car.car_trips.size() == 0)
		return;
	vector<int> trips = car.car_trips;
	double res;
	car.car_dist = 0;
	size_t len = car.car_trips.size();
	vector<int>::iterator iter1 = car.car_trips.begin();

	//1.将车辆包括的各行程行驶长度求和
	for (; iter1 != car.car_trips.end(); iter1++)
	{
		compute_trip_distance(trip[*iter1]);
		car.car_dist += trip[*iter1].travel_dist;
	}

	//2.将车辆中各行程之间的行驶长度加上：即上一行程的最后一个订单目的地加上下一行程的第一个订单出发点之间的行驶距离
	for (int i = 1; i < len; i++)
	{
		int j = trip[trips[i - 1]].trip_orders.size();
		res = peer_sd_distance[trip[trips[i - 1]].orders_off[j - 1]][trip[trips[i]].trip_orders[0]];
		car.car_dist += res;
	}
}

//计算生成车辆行程后的总延迟时间
void compute_car_time_only(Car& car)
{
	if (car.car_trips.size() == 0)
		return;
	car.car_delay = 0;
	int count = 0;
	vector<int>::iterator iter = car.car_trips.begin();
	vector<int> orders;
	double tra_time = order[trip[*iter].trip_orders[0]].b_time;

	//1.计算车辆行程特殊时间
	compute_car_specialtime(car);

	//2.生成车辆所有行程的所有订单按访问顺序排好
	vector<int>::iterator iter1, iter2;
	for (iter1 = car.car_trips.begin(); iter1 != car.car_trips.end(); iter1++)
	{
		for (iter2 = trip[*iter1].trip_orders.begin(); iter2 != trip[*iter1].trip_orders.end(); iter2++)
		{
			orders.push_back(*iter2);
		}
		//0和-1用来区分行程且表示行程结束是否需要休息 0:不需要 -1:需要
		orders.push_back(car.relax[count]);
		count += 1;
	}

	//3.计算各订单之间的延迟时间
	//5 9 -1 15 12 20 0
	vector<int>::iterator iter3 = orders.begin();
	order[*iter3].travel_otime = tra_time;
	for (iter3 = orders.begin(); iter3 != orders.end() - 2; ++iter3)
	{
		double delay = 0;
		if (*(iter3) > 0)
		{
			if (*(iter3 + 1) > 0)
			{
				tra_time += order[*iter3].s_time + peer_time[*iter3][*(iter3 + 1)];
				order[*(iter3 + 1)].travel_otime = tra_time;
				if (tra_time > order[*(iter3 + 1)].e_time)
				{
					delay = tra_time - order[*(iter3 + 1)].e_time;
					order[*(iter3 + 1)].wait_otime = delay;
				}
				car.car_delay += delay;
			}
			else
			{
				if (*(iter3 + 1) == 0)
					continue;
				else
					tra_time += 20;
			}
		}
		else
		{
			tra_time += order[*(iter3 - 1)].s_time + peer_time_3[*(iter3 - 1)][*(iter3 + 1)];
			order[*(iter3 + 1)].travel_otime = tra_time;
			if (tra_time > order[*(iter3 + 1)].e_time)
			{
				delay = tra_time - order[*(iter3 + 1)].e_time;
				order[*(iter3 + 1)].wait_otime = delay;
			}
			car.car_delay += delay;
		}
	}
}

void compute_car_distance(Car& car)
{
	if (car.car_trips.size() == 0)
		return;
	vector<int> trips = car.car_trips;
	double res;
	car.car_dist = 0;
	size_t len = car.car_trips.size();
	vector<int>::iterator iter1 = car.car_trips.begin();

	//1.将车辆包括的各行程行驶长度求和
	for (; iter1 != car.car_trips.end(); iter1++)
	{
		compute_trip_distance(trip[*iter1]);
		car.car_dist += trip[*iter1].travel_dist;
	}

	//2.将车辆中各行程之间的行驶长度加上：即上一行程的最后一个订单目的地加上下一行程的第一个订单出发点之间的行驶距离
	for (int i = 1; i < len; i++)
	{
		int j = trip[trips[i - 1]].trip_orders.size();
		res = peer_sd_distance[trip[trips[i - 1]].orders_off[j - 1]][trip[trips[i]].trip_orders[0]];
		car.car_dist += res;
	}
}

//计算生成车辆的行驶长度(加上行程之间的行驶长度）
void compute_car_distance(Car& car, Trip trip[])
{
	if (car.car_trips.size() == 0)
		return;
	vector<int> trips = car.car_trips;
	double res;
	car.car_dist = 0;
	size_t len = car.car_trips.size();
	vector<int>::iterator iter1 = car.car_trips.begin();

	//1.将车辆包括的各行程行驶长度求和
	for (; iter1 != car.car_trips.end(); iter1++)
	{
		compute_trip_distance(trip[*iter1]);
		car.car_dist += trip[*iter1].travel_dist;
	}

	//2.将车辆中各行程之间的行驶长度加上：即上一行程的最后一个订单目的地加上下一行程的第一个订单出发点之间的行驶距离
	for (int i = 1; i < len; i++)
	{
		int j = trip[trips[i - 1]].trip_orders.size();
		res = peer_sd_distance[trip[trips[i - 1]].orders_off[j - 1]][trip[trips[i]].trip_orders[0]];
		car.car_dist += res;
	}
}

//清空车辆中所有的行程
void clear_car(Car& car)
{
	car.car_id = 0;
	car.car_trips.clear();
	car.car_delay = 0;
	car.car_dist = 0;
	car.relax.clear();
}

void compute_all(Trip& trip)
{
	if (trip.trip_orders.size() == 0)
		return;
	compute_trip_capacity(trip);
	compute_trip_distance(trip);
	compute_trip_time(trip);
}

//将路径按随机生成的方式构建car
//* 有延迟时间版本，软约束
void init_car(Chromosome& chrome)
{
	//请求所在的集合,生成服务请求集合
	vector<int> trip1, trip2;
	vector<int> temp_trip1, temp_trip2;

	//该函数将一个新的元素加到vector的最后面,分别生成a->b和b->a
	for (int i = 1; i <= chrome.trip1.size(); i++)
	{
		trip1.push_back(trip[i].trip_id);
	}

	for (int i = chrome.trip1.size() + 1; i <= chrome.trip1.size() + chrome.trip2.size(); i++)
	{
		trip2.push_back(trip[i].trip_id);
	}
	sort(trip1.begin(), trip1.end(), comp_trip_early);
	sort(trip2.begin(), trip2.end(), comp_trip_early);
	rotate_time_sequence(trip1);
	rotate_time_sequence(trip2);

	//生成一个车辆
	Car new_car;
	int count = 1;
	for (int i = 1; i < car_num; i++)
	{
		cout << "trip1 : " << trip1.size() << endl;
		cout << "trip2 : " << trip2.size() << endl;
		if (trip1.size() == 0 && trip2.size() == 0)
		{
			break;
		}

		if (trip1.size() == 0)
		{
			vector<int>::iterator iter10;
			for (iter10 = trip2.begin(); iter10 != trip2.end(); )
			{
				Car new_car1;
				new_car1.car_trips.push_back(trip[*iter10].trip_id);
				new_car1.car_id = count;
				car[count++] = new_car1;
				chrome.cars.push_back(new_car1);
				clear_car(new_car1);
				iter10 = trip2.erase(iter10);
			}
			break;
		}

		if (trip2.size() == 0)
		{
			vector<int>::iterator iter20;
			for (iter20 = trip1.begin(); iter20 != trip1.end(); )
			{
				Car new_car2;
				new_car2.car_trips.push_back(trip[*iter20].trip_id);
				new_car2.car_id = count;
				car[count++] = new_car2;
				chrome.cars.push_back(new_car2);
				clear_car(new_car2);
				iter20 = trip1.erase(iter20);
			}
			break;
		}

		clear_car(new_car);
		double w_time = 0;
		double t_time = 0;
		double accu_time = 0;
		double curr_time = 0;

		//从trip1中选择一个行程
		int t_ind = 0;
		int last = trip1[t_ind];
		new_car.car_trips.push_back(last);
		t_time = trip[last].tb_time;
		accu_time = trip[last].travel_time - order[trip[last].trip_orders[0]].b_time;
		curr_time = trip[last].travel_time;
		vector<int>::iterator iter1 = trip1.begin();
		trip1.erase(iter1 + t_ind);

		for (iter1 = trip1.begin(); iter1 != trip1.end(); ++iter1)
			temp_trip1.push_back(*iter1);
		trip1 = temp_trip1;
		temp_trip1.clear();

	Loop111:
		while (true)
		{
			vector<int>::iterator iter2 = trip2.begin();
			int s = trip1.size();
			int t_ind2 = 0;
			int len = trip[last].trip_orders.size();
			double dis_time = peer_time_3[trip[last].trip_orders[len - 1]][trip[trip2[t_ind2]].trip_orders[0]];
			curr_time += dis_time;
			w_time = curr_time > trip[trip2[t_ind2]].tb_time ? 0 : trip[trip2[t_ind2]].tb_time - curr_time;
			//根据连续行驶休息时间约束或时间窗约束来判断是否可以加入当前car的情况：
			//第一种情况：①加入后满足司机不需要休息且符合时间窗约束
			if ((accu_time + trip[trip2[t_ind2]].travel_time - order[trip[trip2[t_ind2]].trip_orders[0]].b_time <= 240)
				&& ((curr_time <= trip[trip2[t_ind2]].tb_time) || (curr_time > trip[trip2[t_ind2]].tb_time && curr_time <= trip[trip2[t_ind2]].te_time + max_delay_time)))
			{
				new_car.car_trips.push_back(trip[trip2[t_ind2]].trip_id);
				accu_time += (trip[trip2[t_ind2]].travel_time - order[trip[trip2[t_ind2]].trip_orders[0]].b_time);
				curr_time += (w_time + trip[trip2[t_ind2]].travel_time - order[trip[trip2[t_ind2]].trip_orders[0]].b_time);
				last = trip2[t_ind2];
				trip2.erase(iter2 + t_ind2);
				for (iter2 = trip2.begin(); iter2 != trip2.end(); iter2++)
					temp_trip2.push_back(*iter2);
				trip2 = temp_trip2;
				temp_trip2.clear();
				break;
			}
			//第二种情况：②加入后满足司机需要休息但+20可满足时间窗约束
			if ((accu_time + trip[trip2[t_ind2]].travel_time - order[trip[trip2[t_ind2]].trip_orders[0]].b_time > 240)
				&& ((curr_time + 20 <= trip[trip2[t_ind2]].tb_time) || (curr_time + 20 > trip[trip2[t_ind2]].tb_time && curr_time + 20 <= trip[trip2[t_ind2]].te_time + max_delay_time)))
			{
				new_car.car_trips.push_back(trip[trip2[t_ind2]].trip_id);
				accu_time = 0;
				curr_time += (w_time + trip[trip2[t_ind2]].travel_time - order[trip[trip2[t_ind2]].trip_orders[0]].b_time + 20);
				last = trip2[t_ind2];
				trip2.erase(iter2 + t_ind2);
				for (iter2 = trip2.begin(); iter2 != trip2.end(); iter2++)
					temp_trip2.push_back(*iter2);
				trip2 = temp_trip2;
				temp_trip2.clear();
				break;
			}
			else
			{
				curr_time -= dis_time;
				temp_trip2.push_back(trip2[t_ind2]);
				trip2.erase(iter2 + t_ind2);
				if (trip2.size() == 0)
				{
					trip2 = temp_trip2;
					temp_trip2.clear();
					goto Loop222;
				}
				continue;
			}
		}

		while (true)
		{
			if (trip1.size() == 0)
				break;
			iter1 = trip1.begin();
			int s = trip2.size();
			int t_ind1 = 0;
			int len = trip[last].trip_orders.size();
			double dis_time = peer_time_3[trip[last].trip_orders[len - 1]][trip[trip1[t_ind1]].trip_orders[0]];
			curr_time += dis_time;
			w_time = curr_time > trip[trip1[t_ind1]].tb_time ? 0 : trip[trip1[t_ind1]].tb_time - curr_time;
			//根据连续行驶休息时间约束或时间窗约束来判断是否可以加入当前car的情况：
			//第一种情况：①加入后满足司机不需要休息且符合时间窗约束
			if ((accu_time + trip[trip1[t_ind1]].travel_time - order[trip[trip1[t_ind1]].trip_orders[0]].b_time <= 240)
				&& ((curr_time <= trip[trip1[t_ind1]].tb_time) || (curr_time > trip[trip1[t_ind1]].tb_time && curr_time <= trip[trip1[t_ind1]].te_time + max_delay_time)))
			{
				new_car.car_trips.push_back(trip[trip1[t_ind1]].trip_id);
				accu_time += (trip[trip1[t_ind1]].travel_time - order[trip[trip1[t_ind1]].trip_orders[0]].b_time);
				curr_time += (w_time + trip[trip1[t_ind1]].travel_time - order[trip[trip1[t_ind1]].trip_orders[0]].b_time);
				last = trip1[t_ind1];
				trip1.erase(iter1 + t_ind1);
				for (iter1 = trip1.begin(); iter1 != trip1.end(); iter1++)
					temp_trip1.push_back(*iter1);
				trip1 = temp_trip1;
				temp_trip1.clear();
				goto Loop111;
			}
			//第二种情况：②加入后满足司机需要休息但+20可满足时间窗约束
			if ((accu_time + trip[trip1[t_ind1]].travel_time - order[trip[trip1[t_ind1]].trip_orders[0]].b_time > 240)
				&& ((curr_time + 20 <= trip[trip1[t_ind1]].tb_time) || (curr_time + 20 > trip[trip1[t_ind1]].tb_time && curr_time + 20 <= trip[trip1[t_ind1]].te_time + max_delay_time)))
			{
				new_car.car_trips.push_back(trip[trip1[t_ind1]].trip_id);
				accu_time = 0;
				curr_time += (w_time + trip[trip1[t_ind1]].travel_time - order[trip[trip1[t_ind1]].trip_orders[0]].b_time + 20);
				last = trip1[t_ind1];
				trip1.erase(iter1 + t_ind1);
				for (iter1 = trip1.begin(); iter1 != trip1.end(); iter1++)
					temp_trip1.push_back(*iter1);
				trip1 = temp_trip1;
				temp_trip1.clear();
				goto Loop111;
			}
			else
			{
				curr_time -= dis_time;
				temp_trip1.push_back(trip1[t_ind1]);
				trip1.erase(iter1 + t_ind1);
				if (trip1.size() == 0)
				{
					trip1 = temp_trip1;
					temp_trip1.clear();
					break;
				}
				continue;
			}
		}
	Loop222:
		new_car.car_id = count;
		car[count++] = new_car;
		chrome.cars.push_back(new_car);
		clear_car(new_car);
		continue;
	}
}

bool find_is_same(int order, vector<int> orderss) {
	vector<int>::iterator it = orderss.begin();
	for (; it != orderss.end(); it++) {
		if (*it == order) {
			orderss.erase(it);
			return true;
		}
	}
	return false;
}

void update_trip12(Chromosome& chrome, Trip& trip)
{
	vector<Trip>::iterator t_iter;
	//更新trip1
	if (trip.trip_set == 1)
	{
		for (t_iter = chrome.trip1.begin(); t_iter != chrome.trip1.end(); )
		{
			if (t_iter->trip_id == trip.trip_id)
				t_iter = chrome.trip1.erase(t_iter);
			else
				++t_iter;
		}
	}
	//更新trip2
	else
	{
		for (t_iter = chrome.trip2.begin(); t_iter != chrome.trip2.end(); )
		{
			if (t_iter->trip_id == trip.trip_id)
				t_iter = chrome.trip2.erase(t_iter);
			else
				++t_iter;
		}
	}
	//更新car_trip
	vector<Car>::iterator c_iter;
	vector<int>::iterator iter;
	for (c_iter = chrome.cars.begin(); c_iter != chrome.cars.end(); ++c_iter)
	{
		for (iter = c_iter->car_trips.begin(); iter != c_iter->car_trips.end(); )
		{
			if (*iter == trip.trip_id)
				iter = c_iter->car_trips.erase(iter);
			else
				++iter;
		}
	}
}

//一条路径的trip[]、trip1、trip2更新版本
void update_onetrip(Chromosome& chrome, Trip& trip_one)
{
	/*if (trip_one.trip_orders.size() == 0)
		return;*/
	vector<Trip>::iterator t_iter;
	////更新trip[]
	//trip[trip_one.trip_id] = trip_one;
	//compute_all(trip[trip_one.trip_id]);
	//更新trip_one
	//更新trip1
	if (trip_one.trip_set == 1)
	{
		for (t_iter = chrome.trip1.begin(); t_iter != chrome.trip1.end(); ++t_iter)
		{
			if (t_iter->trip_id == trip_one.trip_id)
			{
				*t_iter = trip_one;
				compute_all(*t_iter);
			}
		}
	}
	//更新trip2
	else
	{
		for (t_iter = chrome.trip2.begin(); t_iter != chrome.trip2.end(); ++t_iter)
		{
			if (t_iter->trip_id == trip_one.trip_id)
			{
				*t_iter = trip_one;
				compute_all(*t_iter);
			}
		}
	}
}

//一条路径的trip1、trip2更新版本
void update_onetrip(Chromosome& chrome, Trip& trip_one, Trip trip[])
{
	if (trip_one.trip_orders.size() == 0)
		return;
	vector<Trip>::iterator t_iter;
	trip[trip_one.trip_id] = trip_one;
	compute_all(trip[trip_one.trip_id]);
	//更新trip_one
	//更新trip1
	if (trip_one.trip_set == 1)
	{
		for (t_iter = chrome.trip1.begin(); t_iter != chrome.trip1.end(); ++t_iter)
		{
			if (t_iter->trip_id == trip_one.trip_id)
			{
				*t_iter = trip_one;
				compute_all(*t_iter);
			}
		}
	}
	//更新trip2
	else
	{
		for (t_iter = chrome.trip2.begin(); t_iter != chrome.trip2.end(); ++t_iter)
		{
			if (t_iter->trip_id == trip_one.trip_id)
			{
				*t_iter = trip_one;
				compute_all(*t_iter);
			}
		}
	}
}

//两条路径的trip[]、trip1、trip2更新版本
void update_twotrips(Chromosome& chrome, Trip& trip_one, Trip& trip_two)
{
	/*if (trip_one.trip_orders.size() == 0)
		return;*/
	vector<Trip>::iterator t_iter;
	//更新trip_one
	//更新trip[]
	trip[trip_one.trip_id] = trip_one;
	compute_all(trip[trip_one.trip_id]);
	//更新trip1
	if (trip_one.trip_set == 1)
	{
		for (t_iter = chrome.trip1.begin(); t_iter != chrome.trip1.end(); ++t_iter)
		{
			if (t_iter->trip_id == trip_one.trip_id)
			{
				*t_iter = trip_one;
				compute_all(*t_iter);
			}
		}
	}
	//更新trip2
	else
	{
		for (t_iter = chrome.trip2.begin(); t_iter != chrome.trip2.end(); ++t_iter)
		{
			if (t_iter->trip_id == trip_one.trip_id)
			{
				*t_iter = trip_one;
				compute_all(*t_iter);
			}
		}
	}
	if (trip_two.trip_orders.size() == 0)
		return;
	//更新trip_two
	trip[trip_two.trip_id] = trip_two;
	compute_all(trip[trip_two.trip_id]);
	//更新trip1
	if (trip_two.trip_set == 1)
	{
		for (t_iter = chrome.trip1.begin(); t_iter != chrome.trip1.end(); ++t_iter)
		{
			if (t_iter->trip_id == trip_two.trip_id)
			{
				*t_iter = trip_two;
				compute_all(*t_iter);
			}
		}
	}
	//更新trip2
	else
	{
		for (t_iter = chrome.trip2.begin(); t_iter != chrome.trip2.end(); ++t_iter)
		{
			if (t_iter->trip_id == trip_two.trip_id)
			{
				*t_iter = trip_two;
				compute_all(*t_iter);
			}
		}
	}
}

// Paper mechanism: AOS reward update.
// After one local-search operator is applied, compare the old objective vector
// f[] with the new chromosome objectives. Positive minimization improvements
// are accumulated as operator rewards for PM/AP/MAB.
void update_Matrix(Chromosome& chrome, double f[], int object)
{
	for (int i = 0; i < FUNC_NUM; ++i) {
		if (EXPERIMENT_ENABLE_LOG) {
			cout << "[AOSReward] operator=M" << object + 1 << " objective=" << i + 1
				 << " old=" << f[i] << " new=" << chrome.f[i] << endl;
		}
		if (f[i] - chrome.f[i] > EPS) {
			double denominator = fabs(f[i]);
			if (denominator < EPS) denominator = 1.0;
			double reward = (EXPERIMENT_REWARD_MODE == REWARD_FREQUENCY)
				? 1.0
				: (f[i] - chrome.f[i]) / denominator;
			UF_Matrix[object][i] += reward;
			UFC_Matrix[object][i] += reward;
		}
	}
}

double compute_Object_nums(double Matrix[8][4], int index)
{
	double total = 0;
	for (int i = 0; i < 8; i++) {
		total += Matrix[i][index];
	}
	return total;
}

int compute_ObjectC_nums(int index)
{
	int total = 0;
	for (int i = 0; i < 8; i++) {
		total += LSC_Matrix[i][index];
	}
	return total;
}

void init_Matrix()
{
	for (int i = 0; i < 8; i++) {
		memset(LS_Matrix[i], 0, 4 * sizeof(double));
		memset(SF_Matrix[i], 0, 4 * sizeof(double));
		memset(UF_Matrix[i], 0, 4 * sizeof(double));
		memset(LSC_Matrix[i], 0, 4 * sizeof(double));
		memset(AP_Matrix[i], 0, 4 * sizeof(double));
		memset(PM_Matrix[i], 0, 4 * sizeof(double));
		memset(MAB_Matrix[i], 0, 4 * sizeof(double));
	}
}

void init_MatrixC()
{
	for (int i = 0; i < 8; i++) {
		memset(SFC_Matrix[i], 0, 4 * sizeof(double));
		memset(UFC_Matrix[i], 0, 4 * sizeof(double));
	}
}

// Legacy objective-improvement trackers. They are kept for compatibility with
// the original IoI-style logic; the complete version uses ObjectiveSelectionState
// in NSGA2.cpp for the paper IoP/SPO objective-selection mechanism.
void init_global_f()
{
	for (int i = 0; i < 4; i++) {
		global_f[i] = 1.0e100;
	}
}

void init_curr_f()
{
	for (int i = 0; i < 4; i++) {
		curr_f[i] = 1.0e100;
	}
}

void compute_w()
{
	for (int i = 0; i < 4; i++) {
		w[i] = (abs(curr_f[i] - global_f[i])) / (max(curr_f[i], global_f[i]) + 1);
	}
}

void compute_IoI()
{
	int sum = 0;
	for (int i = 0; i < 4; i++) {
		sum += exp(w[i]);
	}
	for (int i = 0; i < 4; i++) {
		IoI[i] = exp(w[i]) / sum;
	}
}

int select_aim()
{
	double p = rand() % 1000 / 1000.0;
	int res = -1;
	if (p < IoI[0]) res = 0;
	else if (p < (IoI[0] + IoI[1])) res = 1;
	else if (p < (IoI[0] + IoI[1] + IoI[2])) res = 2;
	else res = 3;
	return res;
}

void compute_f(Chromosome& chrome)
{
	//更新trip[]:直接全更新
	vector<Trip>::iterator t_iter;
	for (t_iter = chrome.trips.begin(); t_iter != chrome.trips.end(); ++t_iter) {
		trip[t_iter->trip_id] = *t_iter;
	}
	vector<Car>::iterator c_iter;
	for (c_iter = chrome.cars.begin(); c_iter != chrome.cars.end(); ++c_iter)
	{
		compute_car_distance(*c_iter);
		compute_car_time(*c_iter);
	}
	//解的目标函数全清零
	for (int i = 0; i < FUNC_NUM; ++i)
		chrome.f[i] = 0;
	//计算解的目标1大小
	chrome.f[0] = chrome.cars.size();
	//计算解的目标2大小
	for (int i = 0; i < chrome.cars.size(); ++i)
		chrome.f[1] += chrome.cars[i].car_delay;
	//计算解的目标3大小
	for (int i = 0; i < chrome.cars.size(); ++i)
		chrome.f[2] += chrome.cars[i].car_dist;
	//计算解的各行程上车人数大小
	for (int i = 0; i < chrome.trips.size(); ++i)
		compute_trip_capacity(chrome.trips[i]);
	//计算解的目标4大小
	double a = 0;
	for (int i = 0; i < chrome.trips.size(); ++i) {
		assert(chrome.trips[i].capacity <= 4);
		a += chrome.trips[i].capacity / max_capacity_1;
	}
	chrome.f[3] = float(chrome.trips.size() / a);
}

//计算解的目标函数：f[0]:最小化车辆数目 f[1]:最小化乘客等待时间、f[2]:最小化总行驶距离、f[3]:最大化上座率
void compute_f(Chromosome& chrome, Trip trip[])
{
	vector<Car>::iterator c_iter;
	for (c_iter = chrome.cars.begin(); c_iter != chrome.cars.end(); ++c_iter)
	{
		compute_car_distance(*c_iter, trip);
		compute_car_time(*c_iter, trip);
	}
	//解的目标函数全清零
	for (int i = 0; i < FUNC_NUM; ++i)
		chrome.f[i] = 0;
	//计算解的目标1大小
	chrome.f[0] = chrome.cars.size();
	//计算解的目标2大小
	for (int i = 0; i < chrome.cars.size(); ++i)
		chrome.f[1] += chrome.cars[i].car_delay;
	//计算解的目标3大小
	for (int i = 0; i < chrome.cars.size(); ++i)
		chrome.f[2] += chrome.cars[i].car_dist;
	//计算解的各行程上车人数大小
	for (int i = 0; i < chrome.trips.size(); ++i)
		compute_trip_capacity(chrome.trips[i]);
	//计算解的目标4大小
	double a = 0;
	for (int i = 0; i < chrome.trips.size(); ++i)
		a += chrome.trips[i].capacity / max_capacity_1;
	chrome.f[3] = float(chrome.trips.size() / a);
}

//计算车辆的目标函数：
//f[0]:最小化车辆数目[最多客户]|f[1]:最小化乘客等待时间|f[2]:最小化总行驶距离|f[3]:最大化上座率
void compute_car_f(Car& car, Trip trip[])
{
	compute_car_distance(car, trip);
	compute_car_time(car, trip);
	//解的目标函数全清零
	for (int i = 0; i < FUNC_NUM; ++i)
		car.f[i] = 0;
	//计算解的各行程上车人数大小
	for (int i = 0; i < car.car_trips.size(); ++i)
		compute_trip_capacity(trip[car.car_trips[i]]);
	//计算解的目标1大小
	int sum = 0;
	for (int i = 0; i < car.car_trips.size(); i++) {
		sum += trip[car.car_trips[i]].capacity;
	}
	car.f[0] = sum;
	//计算解的目标2大小
	car.f[1] = car.car_delay;
	//计算解的目标3大小
	car.f[2] = car.car_dist;
	//计算解的目标4大小
	double a = 0;
	for (int i = 0; i < car.car_trips.size(); ++i)
		a += trip[car.car_trips[i]].capacity / max_capacity_1;
	car.f[3] = float(car.car_trips.size() / a);
}

void init_pop(Chromosome& chrome)
{
	clean_chromosome(chrome);
	init_trip1(chrome);
	init_trip2(chrome);
	vector<Trip>::iterator iter;
	for (iter = chrome.trips.begin(); iter != chrome.trips.end(); iter++)
		compute_all(*iter);
	for (iter = chrome.trip1.begin(); iter != chrome.trip1.end(); iter++)
		compute_all(*iter);
	for (iter = chrome.trip2.begin(); iter != chrome.trip2.end(); iter++)
		compute_all(*iter);

	for (int j = 1; j <= trip_num; j++)
	{
		if (trip[j].trip_id != 0 && trip[j].trip_orders.size() != 0)
			compute_all(trip[j]);
		else
			break;
	}

	init_car(chrome);

	for (int k = 0; k <= chrome.trips.size() - 1; k++)
		compute_all(chrome.trips[k]);

	vector<Car>::iterator car_iter;
	for (car_iter = chrome.cars.begin(); car_iter != chrome.cars.end(); ++car_iter)
	{
		compute_car_time(*car_iter, trip);
		compute_car_distance(*car_iter, trip);
	}
	compute_f(chrome, trip);
}

int check_repeat_delete(int x, vector<int> id) {
	for (int i = 0; i < id.size(); i++) {
		if (x == id[i])
			return i;
	}
	return -1;
}

bool compare_is_equal(vector<int> num1, vector<int> num2) {
	if (num1.size() != num2.size())
		return false;
	for (int i = 0; i < num1.size(); i++) {
		if (num1[i] != num2[i])
			return false;
	}
	return true;
}

int findMax_car_id(vector<Car> vec) {
	int max = -999;
	for (auto v : vec) {
		if (max < v.car_id)
			max = v.car_id;
	}
	return max;
}

int findMax_trip_id(vector<Trip> vec) {
	int max = -999;
	for (auto v : vec) {
		if (max < v.trip_id)
			max = v.trip_id;
	}
	return max;
}

int find_corresponding(vector<Trip> vec, int x) {
	int result = 0;
	vector<Trip>::iterator iter = vec.begin();
	for (; iter != vec.end(); iter++) {
		if (iter->trip_id == x)
			return result;
		else
			result++;
	}
	return -1;
}

bool check_capacity(Trip trip)
{
	bool truth = true;
	double capacity = 0;
	vector<int>::iterator c_iter;
	for (c_iter = trip.trip_orders.begin(); c_iter != trip.trip_orders.end(); ++c_iter)
	{
		capacity += order[*c_iter].peoples;
		if (capacity > max_capacity_1)
			truth = false;
	}
	return truth;
}

bool is_repeat(int check[], Trip t[], Car car)
{
	//check_is_repeat
	vector<int>::iterator iter;
	vector<int>::iterator oter;
	for (iter = car.car_trips.begin(); iter != car.car_trips.end(); iter++) {
		for (oter = t[*iter].trip_orders.begin(); oter != t[*iter].trip_orders.end(); oter++) {
			if (check[*oter] == 1)
				return false;
		}
	}
	//update
	for (iter = car.car_trips.begin(); iter != car.car_trips.end(); iter++) {
		for (oter = t[*iter].trip_orders.begin(); oter != t[*iter].trip_orders.end(); oter++) {
			check[*oter] = 1;
		}
	}
	return true;
}

//用于计算特殊行程生成后的生成车辆行程后的总延迟时间、总行驶距离
double spe_cardelay(vector<Trip> trip)
{
	double car_delay = 0;
	int count = 0;
	vector<Trip>::iterator iter;
	vector<int> orders;
	iter = trip.begin();
	double tra_time = iter->tb_time;

	//计算各行程的开始行程时间、结束行程时间、行程结束时是否需要休息
	double accu_time = 0;                      //累计时间数组
	double curr_time = 0;                      //当前时间数组
	double dis_time = 0;                       //行程之间的行驶时间
	vector<int>relax;
	vector<Trip>::iterator t_iter;
	for (t_iter = trip.begin(); t_iter != trip.end(); ++t_iter)
	{
		if (t_iter == trip.begin())
		{
			start_time[t_iter->trip_id] = t_iter->tb_time;
			curr_time = t_iter->travel_time;
			accu_time = t_iter->travel_time - order[t_iter->trip_orders[0]].b_time;
			end_time[t_iter->trip_id] = curr_time;
		}
		else
		{
			int len = (t_iter - 1)->trip_orders.size();
			dis_time = peer_time_3[(t_iter - 1)->trip_orders[len - 1]][t_iter->trip_orders[0]];
			accu_time += dis_time;
			curr_time += dis_time;
			//如果早到
			if (curr_time < t_iter->tb_time)
			{
				accu_time += t_iter->tb_time - curr_time;
				curr_time = t_iter->tb_time;
			}
			accu_time += t_iter->travel_time - order[t_iter->trip_orders[0]].b_time;
			start_time[t_iter->trip_id] = curr_time;
			if (accu_time > 240)
			{
				accu_time = 0;
				curr_time += 20;
				relax.push_back(-1);
			}
			else
				relax.push_back(0);
			start_time[t_iter->trip_id] = curr_time;
			//正常到达
			curr_time += t_iter->travel_time - order[t_iter->trip_orders[0]].b_time;
			end_time[t_iter->trip_id] = curr_time;
		}
	}
	relax.push_back(0);

	//2.生成车辆所有行程的所有订单按访问顺序排好
	vector<Trip>::iterator iter1;
	vector<int>::iterator iter2;
	for (iter1 = trip.begin(); iter1 != trip.end(); iter1++)
	{
		for (iter2 = iter1->trip_orders.begin(); iter2 != iter1->trip_orders.end(); iter2++)
		{
			orders.push_back(*iter2);
		}
		//0用于区分订单点是否同属于一个行程
		orders.push_back(relax[count]);
		count += 1;
	}

	//3.计算各订单之间的延迟时间
	//5 9 -1 15 12 20 0
	vector<int>::iterator iter3 = orders.begin();
	order[*iter3].travel_otime = tra_time;
	for (iter3 = orders.begin(); iter3 != orders.end() - 2; ++iter3)
	{
		double delay = 0;
		if (*(iter3) > 0)
		{
			if (*(iter3 + 1) > 0)
			{
				tra_time += order[*iter3].s_time + peer_time[*iter3][*(iter3 + 1)];
				order[*(iter3 + 1)].travel_otime = tra_time;
				if (tra_time > order[*(iter3 + 1)].e_time)
				{
					delay = tra_time - order[*(iter3 + 1)].e_time;
					order[*(iter3 + 1)].wait_otime = delay;
				}
				car_delay += delay;
			}
			else
			{
				if (*(iter3 + 1) == 0)
					continue;
				else
					tra_time += 20;
			}
		}
		else
		{
			tra_time += order[*(iter3 - 1)].s_time + peer_time_3[*(iter3 - 1)][*(iter3 + 1)];
			order[*(iter3 + 1)].travel_otime = tra_time;
			if (tra_time > order[*(iter3 + 1)].e_time)
			{
				delay = tra_time - order[*(iter3 + 1)].e_time;
				order[*(iter3 + 1)].wait_otime = delay;
			}
			car_delay += delay;
		}
	}
	return car_delay;
}

double spe_cardist(vector<Trip> trip)
{
	double res;
	double car_dist = 0;
	vector<Trip>::iterator iter1;

	//1.将车辆包括的各行程行驶长度求和
	for (iter1 = trip.begin(); iter1 != trip.end(); iter1++)
	{
		compute_trip_distance(*iter1);
		car_dist += iter1->travel_dist;
	}
	//2.将车辆中各行程之间的行驶长度加上：即上一行程的最后一个订单目的地加上下一行程的第一个订单出发点之间的行驶距离
	for (iter1 = trip.begin(); iter1 != trip.end() - 1; iter1++)
	{
		int j = iter1->trip_orders.size();
		vector<Trip>::iterator iter2 = iter1 + 1;
		res = peer_sd_distance[iter1->orders_off[j - 1]][iter2->trip_orders[0]];
		car_dist += res;
	}
	return car_dist;
}

double spe_carseat(vector<Trip> trip)
{
	double car_seat = 0;
	vector<Trip>::iterator t_iter;
	for (t_iter = trip.begin(); t_iter != trip.end(); ++t_iter)
	{
		compute_trip_capacity(*t_iter);
		car_seat += t_iter->capacity;
	}
	return car_seat;
}

void update_global_trip(Chromosome& chrome)
{
	//更新trip[]:直接全更新
	for (int i = 1; i < trip_num; i++) {
		trip[i].trip_id = 0;
	}
	adjust_tripid(chrome);
	vector<Trip>::iterator t_iter;
	for (t_iter = chrome.trips.begin(); t_iter != chrome.trips.end(); ++t_iter) {
		trip[t_iter->trip_id] = *t_iter;
	}
}

//针对总行驶距离和总等待时间，④交换单个行程
//exchange two trips, 距离变化将车辆方案r1中的行程t1和车辆方案r2中的行程t2进行交换
void ls_exchange_trip(Chromosome& s, int r1, int obj, int& count)
{
	update_global_trip(s);
	count++;
	int r2;
	int V = s.cars.size();
	r2 = rand() % V;
	int nv1 = s.cars[r1].car_trips.size(), nv2 = s.cars[r2].car_trips.size();
	if (nv1 > 1 && nv2 > 1)
	{
		int t1 = rand() % nv1;
		int t2 = rand() % nv2;
		int x = s.cars[r1].car_trips[t1];
		int y = s.cars[r2].car_trips[t2];
		string strx1, strx2;
		string stry1, stry2;
		//随便设两个车库经纬度，不影响delta
		string depot1 = { "24.499423,118.114758" };
		string depot2 = { "24.516634,117.651252" };

		if (t1 == 0)
			strx1 = depot1;
		else
		{
			int size1 = trip[s.cars[r1].car_trips[t1 - 1]].trip_orders.size();
			strx1 = order[trip[s.cars[r1].car_trips[t1 - 1]].orders_off[size1 - 1]].src_coordinate;
		}
		if (t1 + 1 >= nv1)
			strx2 = depot2;
		else
			strx2 = order[trip[s.cars[r1].car_trips[t1 + 1]].trip_orders[0]].src_coordinate;

		if (t2 == 0)
			stry1 = depot1;
		else
		{
			int size2 = trip[s.cars[r2].car_trips[t2 - 1]].trip_orders.size();
			stry1 = order[trip[s.cars[r2].car_trips[t2 - 1]].orders_off[size2 - 1]].src_coordinate;
		}

		if (t2 + 1 >= nv2)
			stry2 = depot2;
		else
			stry2 = order[trip[s.cars[r2].car_trips[t2 + 1]].trip_orders[0]].src_coordinate;

		if (x == y)
			return;

		//与交换order不同，不需要检查是否满足容量约束， 司机休息约束？路径形式 0->客户点  0->行程
		double dt1, dt2;
		int sizex = trip[x].trip_orders.size();
		int sizey = trip[y].trip_orders.size();
		//不考虑r1 == r2，仅考虑行程处于路径首尾，分三种情况考虑
		//第一种情况：①t1和t2位于r1和r2的首尾位置，且分两个子情况：决定使用该行程首订单还是尾订单                    2
		if (trip[s.cars[r1].car_trips[t1]].trip_set == trip[s.cars[r2].car_trips[t2]].trip_set)
		{
			Chromosome ss = s;
			//针对总延迟时间
			if (obj == 1)
			{
				ss.cars[r1].car_trips[t1] = y;
				ss.cars[r2].car_trips[t2] = x;
				compute_car_time(ss.cars[r1]);
				compute_car_time(ss.cars[r2]);
				dt1 = ss.cars[r1].car_delay - s.cars[r1].car_delay;
				dt2 = ss.cars[r2].car_delay - s.cars[r1].car_delay;
			}
			//针对总行驶距离
			if (obj == 2)
			{
				dt1 = calculateDis(strx1, order[trip[y].trip_orders[0]].src_coordinate)
					+ calculateDis(order[trip[y].orders_off[sizey - 1]].src_coordinate, strx2)
					- calculateDis(strx1, order[trip[x].trip_orders[0]].src_coordinate)
					- calculateDis(order[trip[x].orders_off[sizex - 1]].src_coordinate, strx2);

				dt2 = calculateDis(stry1, order[trip[x].trip_orders[0]].src_coordinate)
					+ calculateDis(order[trip[x].orders_off[sizex - 1]].src_coordinate, stry2)
					- calculateDis(stry1, order[trip[y].trip_orders[0]].src_coordinate)
					- calculateDis(order[trip[y].orders_off[sizey - 1]].src_coordinate, stry2);

				ss.cars[r1].car_trips[t1] = y;
				ss.cars[r2].car_trips[t2] = x;
			}
			if ((dt1 + dt2) < 0 && check_feasible_car(ss.cars[r1]) && check_feasible_car(ss.cars[r2]))
			{
				cout << "[RouteOpt] swap route" << endl;
				s.cars[r1].car_trips[t1] = y;
				s.cars[r2].car_trips[t2] = x;
			}
		}
		else
			return;
	}
	else
		return;
}

//针对总行驶距离和总等待时间，④交换单个行程
//exchange two trips, 距离变化将车辆方案r1中的行程t1和车辆方案r2中的行程t2进行交换
void ls_exchange_mutitrip(Chromosome& s, int r1, int obj, int& count)
{
	update_global_trip(s);
	count++;
	int r2;
	int V = s.cars.size();
	r2 = rand() % V;
	int nv1 = s.cars[r1].car_trips.size(), nv2 = s.cars[r2].car_trips.size();
	if (nv1 >= 2 && nv2 >= 2)
	{
		int t1 = rand() % (nv1 - 1);
		int t2 = rand() % (nv2 - 1);
		int x = s.cars[r1].car_trips[t1];
		int y = s.cars[r2].car_trips[t2];
		int x2 = s.cars[r1].car_trips[t1 + 1];
		int y2 = s.cars[r2].car_trips[t2 + 1];
		string strx1, strx2;
		string stry1, stry2;
		//随便设两个车库经纬度，不影响delta
		string depot1 = { "24.499423,118.114758" };
		string depot2 = { "24.516634,117.651252" };

		if (t1 == 0)
			strx1 = depot1;
		else
		{
			int size1 = trip[s.cars[r1].car_trips[t1 - 1]].trip_orders.size();
			strx1 = order[trip[s.cars[r1].car_trips[t1 - 1]].orders_off[size1 - 1]].src_coordinate;
		}
		if (t1 + 2 >= nv1)
			strx2 = depot2;
		else
			strx2 = order[trip[s.cars[r1].car_trips[t1 + 2]].trip_orders[0]].src_coordinate;

		if (t2 == 0)
			stry1 = depot1;
		else
		{
			int size2 = trip[s.cars[r2].car_trips[t2 - 1]].trip_orders.size();
			stry1 = order[trip[s.cars[r2].car_trips[t2 - 1]].orders_off[size2 - 1]].src_coordinate;
		}

		if (t2 + 2 >= nv2)
			stry2 = depot2;
		else
			stry2 = order[trip[s.cars[r2].car_trips[t2 + 2]].trip_orders[0]].src_coordinate;

		if (x == y)
			return;

		//与交换order不同，不需要检查是否满足容量约束， 司机休息约束？路径形式 0->客户点  0->行程
		double dt1, dt2;
		int sizex = trip[x2].trip_orders.size();
		int sizey = trip[y2].trip_orders.size();
		//不考虑r1 == r2，仅考虑行程处于路径首尾，分三种情况考虑
		//第一种情况：①t1和t2位于r1和r2的首尾位置，且分两个子情况：决定使用该行程首订单还是尾订单
		if (trip[s.cars[r1].car_trips[t1]].trip_set == trip[s.cars[r2].car_trips[t2]].trip_set)
		{
			Chromosome ss = s;
			//针对总延迟时间
			if (obj == 1)
			{
				ss.cars[r1].car_trips[t1] = y;
				ss.cars[r2].car_trips[t2] = x;
				ss.cars[r1].car_trips[t1 + 1] = y2;
				ss.cars[r2].car_trips[t2 + 1] = x2;
				compute_car_time(ss.cars[r1]);
				compute_car_time(ss.cars[r2]);
				dt1 = ss.cars[r1].car_delay - s.cars[r1].car_delay;
				dt2 = ss.cars[r2].car_delay - s.cars[r1].car_delay;
			}
			//针对总行驶距离
			if (obj == 2)
			{
				dt1 = calculateDis(strx1, order[trip[y].trip_orders[0]].src_coordinate)
					+ calculateDis(order[trip[y2].orders_off[sizey - 1]].src_coordinate, strx2)
					- calculateDis(strx1, order[trip[x].trip_orders[0]].src_coordinate)
					- calculateDis(order[trip[x2].orders_off[sizex - 1]].src_coordinate, strx2);

				dt2 = calculateDis(stry1, order[trip[x].trip_orders[0]].src_coordinate)
					+ calculateDis(order[trip[x2].orders_off[sizex - 1]].src_coordinate, stry2)
					- calculateDis(stry1, order[trip[y].trip_orders[0]].src_coordinate)
					- calculateDis(order[trip[y2].orders_off[sizey - 1]].src_coordinate, stry2);

				ss.cars[r1].car_trips[t1] = y;
				ss.cars[r2].car_trips[t2] = x;
				ss.cars[r1].car_trips[t1 + 1] = y2;
				ss.cars[r2].car_trips[t2 + 1] = x2;
			}
			if ((dt1 + dt2) < 0 && check_feasible_car(ss.cars[r1]) && check_feasible_car(ss.cars[r2]))
			{
				cout << "[RouteOpt] swap route" << endl;
				s.cars[r1].car_trips[t1] = y;
				s.cars[r2].car_trips[t2] = x;
				s.cars[r1].car_trips[t1 + 1] = y2;
				s.cars[r2].car_trips[t2 + 1] = x2;
			}
		}
		else
			return;
	}
	else
		return;
}

//局部搜索1：判断行程中的订单是否可以插入到其他行程中，并且实现插入
bool check_feasible_lsone(Trip& trip, int ordernum)
{
	//1.初始化数据准备
	double capacity = 0;
	int s = trip.trip_orders.size();

	if (trip.trip_set == order[ordernum].location_type + 1)
	{
		//2.检查容量约束
		capacity = trip.capacity + order[ordernum].peoples;
		if (capacity > max_capacity_1)
			return false;

		//3.检查时间约束
		vector<int>::iterator c_iter;
		double time = 0;
		double tmp_time = 0;

		for (c_iter = trip.trip_orders.begin(); c_iter != trip.trip_orders.end(); ++c_iter)
		{
			//第一种情况：ordernum插入首位,只检查插入后之后的客户点是否有问题
			if (c_iter == trip.trip_orders.begin())
			{
				tmp_time = order[ordernum].b_time + order[ordernum].s_time + peer_time[ordernum][*c_iter];
				if (tmp_time <= order[*c_iter].b_time)
				{
					trip.trip_orders.insert(c_iter, ordernum);
					compute_all(trip);
					return true;
				}
			}
			//第二种情况：ordernum插入路径客户点之间，检查插入后之前和之后的客户点是否有问题
			else
			{
				tmp_time = a_time[*(c_iter - 1)] + order[*(c_iter - 1)].s_time + peer_time[*(c_iter - 1)][ordernum];
				//插入后，c_iter之前的客户点有问题，不能插入，更新时间后继续往后搜
				if (tmp_time > order[ordernum].e_time + max_delay_time)
				{
					time += peer_time[*(c_iter - 1)][*c_iter];
					if (time < order[*c_iter].b_time)
						time = order[*c_iter].b_time;
					time += order[*c_iter].s_time;
					continue;
				}
				//可以插入，c_iter之前的客户点没问题
				if (tmp_time < order[ordernum].b_time)
					tmp_time = order[ordernum].b_time;
				//判断c_iter之后的客户点有无问题
				tmp_time += order[ordernum].s_time + peer_time[ordernum][*c_iter];
				//插入后c_iter之后的客户点没有问题
				//这里选择插入的是第一个合理的位置，将客户点ordernum插入到c_iter所在的位置，c_iter客户点后移
				if (tmp_time <= a_time[*c_iter] || tmp_time <= order[*c_iter].b_time)
				{
					trip.trip_orders.insert(c_iter, ordernum);
					compute_all(trip);
					return true;
				}
				time += peer_time[*(c_iter - 1)][*c_iter];
				if (time < order[*c_iter].b_time)
					time = order[*c_iter].b_time;
				time += order[*c_iter].s_time;
			}
		}
		//第三种情况：for循环遍历完插入行程到行程trip后面，只检查插入后之前的客户点有无问题
		c_iter = trip.trip_orders.end() - 1;
		tmp_time = a_time[*c_iter] + order[*c_iter].s_time + peer_time[*c_iter][ordernum];
		if (tmp_time > order[ordernum].e_time + max_delay_time)
			return false;
		else
		{
			trip.trip_orders.push_back(ordernum);
			compute_all(trip);
			return true;
		}
	}
	//三种情况均不满足，则不能插入
	return false;
}

//检测将ordernum插入trip路径中是否是最优
//1.利用check_feasible_lsone检查ordernum能否插入trip
//2.计算插入后所在车辆的行驶距离、延迟时间、1/上座率
//3.delta比较检查最优

bool check_best_pos1(int ordernum, Car& car, Trip& trip, vector<Trip>temp_trip, int r_num, int& c_pos, double& min_delta, int r_lamda[], double now_dist, double now_time, double now_delay, double now_seat)
{
	bool feasible = false;
	min_delta = 99999999;
	double delta = 0;
	Trip spe_trip = trip;
	cout << "[Debug1] spe_id: " << spe_trip.trip_id << " spe_size: " << spe_trip.trip_orders.size() << endl;
	for (int i = 0; i < spe_trip.trip_orders.size(); i++)
		cout << "orders: " << spe_trip.trip_orders[i] << " " << endl;
	//error C2678 : 二进制“ == ” : 没有找到接受“Trip”类型的左操作数的运算符(或没有可接受的转换)
	cout << "[Debug5] " << delta << "   " << min_delta << endl;
	vector<Trip>::iterator iter;
	if (check_feasible_lsone(spe_trip, ordernum))
	{
		for (iter = temp_trip.begin(); iter != temp_trip.end(); ++iter)
		{
			if (iter->trip_id == trip.trip_id)
				*iter = spe_trip;
		}
	}
	//注意迭代器指针输出元素的方式和distance用法
	else
		return false;

	cout << "[Debug2] spe_id: " << spe_trip.trip_id << " spe_size: " << spe_trip.trip_orders.size() << endl;
	for (int i = 0; i < spe_trip.trip_orders.size(); i++)
		cout << "orders: " << spe_trip.trip_orders[i] << " " << endl;

	//计算三大项
	double delta_delay = spe_cardelay(temp_trip);
	double delta_dist = spe_cardist(temp_trip);
	double delta_seat = spe_carseat(temp_trip);

	if (r_lamda[1] == 1)
		delta = now_delay + delta_delay;
	else if (r_lamda[2] == 1)
		delta = now_dist + delta_dist;
	else
		delta = r_num * max_capacity_1 / (now_seat + delta_seat);

	cout << "[Debug6] " << delta << "   " << min_delta << endl;

	if (min_delta > delta)
	{
		vector<int>::iterator c_iter = std::find(spe_trip.trip_orders.begin(), spe_trip.trip_orders.end(), ordernum);
		min_delta = delta;
		c_pos = c_iter - spe_trip.trip_orders.begin();
		feasible = true;
		cout << "[Debug7] " << delta << "   " << min_delta << " c_pos :  " << c_pos << endl;
		cout << "[Debug3] spe_id: " << spe_trip.trip_id << " spe_size: " << spe_trip.trip_orders.size() << endl;
		for (int i = 0; i < spe_trip.trip_orders.size(); i++)
			cout << "orders: " << spe_trip.trip_orders[i] << " " << endl;
	}
	return feasible;
}

bool check_best_pos1(int ordernum, Car& car, Trip& trip, vector<Trip>temp_trip, int r_num, int& c_pos, double& min_delta, int r_lamda[], double now_dist, double now_time, double now_delay, double now_seat, double now_carnum)
{
	bool feasible = false;
	min_delta = 99999999;
	double delta = 0;
	Trip spe_trip = trip;
	vector<Trip>::iterator iter;
	if (check_feasible_lsone(spe_trip, ordernum))
	{
		for (iter = temp_trip.begin(); iter != temp_trip.end(); ++iter)
		{
			if (iter->trip_id == trip.trip_id)
				*iter = spe_trip;
		}
	}
	//注意迭代器指针输出元素的方式和distance用法
	else
		return false;

	//计算三大项
	double delta_delay = spe_cardelay(temp_trip);
	double delta_dist = spe_cardist(temp_trip);
	double delta_seat = spe_carseat(temp_trip);

	if (r_lamda[0] == 1)
		delta = 0;               //=0说明能在不新增车辆的情况下插入，否则return false之后会create_car_trip
	else if (r_lamda[1] == 1)
		delta = now_delay + delta_delay;
	else if (r_lamda[2] == 1)
		delta = now_dist + delta_dist;
	else
		delta = r_num * max_capacity_1 / (now_seat + delta_seat);

	if (min_delta > delta)
	{
		vector<int>::iterator c_iter = std::find(spe_trip.trip_orders.begin(), spe_trip.trip_orders.end(), ordernum);
		min_delta = delta;
		c_pos = c_iter - spe_trip.trip_orders.begin();
		feasible = true;
	}
	return feasible;
}

int judge_tripiscarend(Chromosome& chrome, int tripnum)
{
	int tripnext = 0;
	vector<Car>::iterator icar;
	for (icar = chrome.cars.begin(); icar != chrome.cars.end(); icar++)
	{
		int size = icar->car_trips.size() - 1;
		auto iter = std::find(icar->car_trips.begin(), icar->car_trips.end(), tripnum);
		if (iter != std::end(icar->car_trips))
			//找到了，判断是不是最后一个行程
		{
			if (iter - icar->car_trips.begin() == size) //是最后一个行程
				return tripnext;
			else
			{
				tripnext = icar->car_trips[iter - icar->car_trips.begin() + 1];
				return tripnext;
			}
		}
		//没找到，遍历循环继续找
		else
			continue;
	}
	//全部找到没找，不应该出现这种情况，以防万一
	return tripnext;
}

//删除路径trip_ind，将路径上每个客户点尝试插入其他路径，删除成功返回true
bool opt_lsone(Chromosome& chrome, int trip_ind, int count, int before_id)
{
	count = count + 1;
	if (count > 2)
		return false;

	vector<int>::iterator c_iter;
	vector<Trip>::iterator t_iter;
	Chromosome temp_chrome = chrome;
	//对trip_ind路径中的客户点进行遍历
	for (c_iter = chrome.trips[trip_ind].trip_orders.begin(); c_iter != chrome.trips[trip_ind].trip_orders.end(); )
	{
		compute_trip_time(chrome.trips[trip_ind]);
		//路径trip_ind中可以删除的客户点要满足的条件(暂时不需要考虑0仓库和客户点,但需考虑指针泄露，故分首和非首考虑
		//1.当删除点是第一个点时，可直接删除
		//2.当删除点不是第一个点时，删除后,到达下一个客户点的时间小于下个客户点的最迟时间+最大允许迟到时间
		if ((c_iter == chrome.trips[trip_ind].trip_orders.begin()) || ((c_iter != chrome.trips[trip_ind].trip_orders.begin()) && (l_time[*(c_iter - 1)] + peer_time[*(c_iter - 1)][*(c_iter + 1)] < order[*(c_iter + 1)].e_time + max_delay_time)))
		{
			for (t_iter = chrome.trips.begin(); t_iter != chrome.trips.end(); ++t_iter)
			{
				if ((t_iter - chrome.trips.begin() != trip_ind) && (t_iter->trip_id != before_id) && (t_iter->trip_set == order[*(c_iter)].location_type + 1) && check_feasible_lsone(*t_iter, *c_iter))
					//c_iter客户点可以插入到*t_iter路径
				{
					//先更新check_feasible_lsone里的insert改动
					update_onetrip(chrome, *t_iter);
					//update_global_trip(chrome);
					//再更新以下的erase改动
					c_iter = chrome.trips[trip_ind].trip_orders.erase(c_iter);
					update_onetrip(chrome, chrome.trips[trip_ind]);
					//update_global_trip(chrome);
					break;
				}
			}
			if (t_iter == chrome.trips.end())
				return false;
		}
		else
			return false;
		//到最后一条路径，c_iter都没有插入，直接遍历检查插入下一个
		if (t_iter == chrome.trips.end())
			++c_iter;
	}
	if (chrome.trips[trip_ind].trip_orders.size() == 0)
		//出现空路径，则删除trip_ind路径：判断该路径是否是该车辆最后一条路径
		//①若是则可直接删去
		//②若不是则需检查后一条路径是否也可删去
		//note: 前路径为空，后路径中的客户点除了不能插入自身外也不能插入前路径
	{
		//1 是，直接删去
		if (judge_tripiscarend(chrome, chrome.trips[trip_ind].trip_id) == 0)
		{
			update_trip12(chrome, chrome.trips[trip_ind]);
			chrome.trips.erase(chrome.trips.begin() + trip_ind);
			//update_global_trip(chrome);
			return true;
		}
		else
		{
			//2-1 不是，但后一条路径也可删去,仅检查一次，不然无底洞
			int tripnext = judge_tripiscarend(chrome, chrome.trips[trip_ind].trip_id);
			//因为trip_id前面（序号在前面，但是下一条路径）行程若被删，会影响位置f_iter2->trip_id == chrome.trips[trip_ind].trip_id,故要用新变量记录真实位置即id
			int tru_id = chrome.trips[trip_ind].trip_id;
			//tripnext只是id，不是chrometrips的位置，所以需要寻找到真正的trip_ind格式导入
			int next_ind;
			vector<Trip>::iterator f_iter;
			for (f_iter = chrome.trips.begin(); f_iter != chrome.trips.end(); ++f_iter)
				if (f_iter->trip_id == tripnext)
					next_ind = f_iter - chrome.trips.begin();
			if (opt_lsone(chrome, next_ind, count, tru_id))
			{
				//tripnext已在opt_lsone中删去，则下面应继续删去tripind
				////注意这里不能以+trip_ind，因删回程，此时删掉一个会影响下一个总行程数量从而影响对应位置，故应该以id判断
				int real_ind;
				vector<Trip>::iterator f_iter2;
				for (f_iter2 = chrome.trips.begin(); f_iter2 != chrome.trips.end(); ++f_iter2)
					if (f_iter2->trip_id == tru_id)
						real_ind = f_iter2 - chrome.trips.begin();
				update_trip12(chrome, chrome.trips[real_ind]);
				chrome.trips.erase(chrome.trips.begin() + real_ind);
				//update_global_trip(chrome);
				return true;
			}
			//2-2 不是，且后一条路径不可删去，需还原
			else
			{
				chrome = temp_chrome;
				return false;
			}
		}
	}
	return false;
}

//局部搜索2：判断车辆中的回程是否可以插入到其他车辆中，并且实现插入
bool check_feasible_lstwo(Chromosome& chrome, int car_ind, Car& car, int tripnum)
{
	//1.初始化数据准备
	//定义变量记录车辆回程数
	int s = chrome.cars[car_ind].car_trips.size() / 2;
	if (s < 1)
		return false;
	int ss = car.car_trips.size() / 2;
	if (ss < 1)
		return false;
	int tripnext = 0;
	//定义变量记录tripnum的下一个行程
	vector<Car>::iterator icar;
	for (icar = chrome.cars.begin(); icar != chrome.cars.end(); icar++)
	{
		int size = icar->car_trips.size() - 1;
		auto iter = std::find(icar->car_trips.begin(), icar->car_trips.end(), tripnum);
		if (iter != std::end(icar->car_trips))
			//找到了，判断是不是最后一个行程
		{
			if (iter - icar->car_trips.begin() != size) //不是最后一个行程
				tripnext = icar->car_trips[iter - icar->car_trips.begin() + 1];
			else
				return false;
		}
		//没找到，遍历循环继续找
		else
			continue;
	}

	//2.检查时间约束,与trip不同注意判断疲劳驾驶
	vector<int>::iterator c_iter;
	for (int count = 0; count <= ss; count++)
	{
		cout << "[Action] execute once" << endl;
		Car temp_car = car;
		int real = 2 * count;
		c_iter = temp_car.car_trips.begin() + real;
		temp_car.car_trips.insert(c_iter, { tripnum, tripnext });
		if (!check_feasible_car(temp_car))
		{
			cout << "[Constraint] vehicle time constraint violated!" << endl;
			continue;
		}
		else
		{
			c_iter = car.car_trips.begin() + real;
			car.car_trips.insert(c_iter, { tripnum, tripnext });
			compute_car_time(car);
			compute_car_distance(car);
			return true;
		}
	}
	return false;
}

bool opt_lstwo(Chromosome& chrome, int car_ind)
{
	vector<int>::iterator c_iter;
	vector<Car>::iterator t_iter;
	update_global_trip(chrome);
	for (c_iter = chrome.cars[car_ind].car_trips.begin(); c_iter != chrome.cars[car_ind].car_trips.end();)
	{
		compute_car_time(chrome.cars[car_ind]);
		for (t_iter = chrome.cars.begin(); t_iter != chrome.cars.end(); ++t_iter)
		{
			if (t_iter - chrome.cars.begin() != car_ind && check_feasible_lstwo(chrome, car_ind, *t_iter, *c_iter))
				//回程(c_iter 和 c_iter + 1)可以插入到车辆
			{
				cout << "[RouteOpt] success 1" << endl;
				c_iter = chrome.cars[car_ind].car_trips.erase(c_iter);
				c_iter = chrome.cars[car_ind].car_trips.erase(c_iter);
				break;
			}
		}
		if (t_iter == chrome.cars.end())
			return false;
		//到最后一辆车，回程(c_iter 和 c_iter + 1)都没有插入，直接遍历检查插入下一个
		if (t_iter == chrome.cars.end())
			std::advance(c_iter, 2);
	}
	if (chrome.cars[car_ind].car_trips.size() == 0)
		//出现空路径，则删除car_ind车辆
	{
		cout << "[Vehicle] remove empty vehicle success" << endl;
		cout << chrome.cars.size() << endl;
		chrome.cars.erase(chrome.cars.begin() + car_ind);
		return true;
	}
	return false;
}

//从trip中随机删除count个客户点，检查解中此路径是否有效
//有效返回 true 无效返回 false
bool check_feasible_trip(Trip& trip)
{
	double time = trip.tb_time;
	for (vector<int>::iterator c_iter = trip.trip_orders.begin(); c_iter != trip.trip_orders.end() - 1; ++c_iter)
	{
		time += peer_time[*c_iter][*(c_iter + 1)];
		time += order[*c_iter].s_time;
		////车库0
		//if (*(c_iter + 1) == 0 && time > order[*(c_iter + 1)].e_time)
		//	return false;
		if (*(c_iter + 1) != 0 && time > order[*(c_iter + 1)].e_time + max_delay_time)
			return false;
		if (time < order[*(c_iter + 1)].b_time)
			time = order[*(c_iter + 1)].b_time;
		time += order[*(c_iter + 1)].s_time;
	}
	return true;
}

//拆分解chrome的第r_ind条路径，产生新的路径添加到解的路径集合中
void split_trip(Chromosome& chrome, int r_ind)
{
	Trip temp_trip;
	//获得要拆分的路径
	vector<Trip>::iterator r_iter = chrome.trips.begin() + r_ind;
	vector<int>::iterator c_iter = r_iter->trip_orders.begin();
	double time = order[*c_iter].b_time;
	int previous_cust = *c_iter;
	int now_cust;
	for (c_iter = r_iter->trip_orders.begin() + 1; c_iter != r_iter->trip_orders.end(); ++c_iter)
	{
		now_cust = *c_iter;
		time += peer_time[previous_cust][now_cust];
		double return_time = time;
		if (return_time < order[now_cust].b_time)
			return_time = order[now_cust].b_time;
		return_time += order[now_cust].s_time + peer_time[now_cust][0];

		if (time > order[now_cust].e_time + max_delay_time || return_time > order[0].e_time)
		{
			if (temp_trip.trip_orders.size() != 0)
			{
				compute_trip_time(temp_trip);
				compute_trip_distance(temp_trip);
				compute_trip_capacity(temp_trip);
				chrome.trips.push_back(temp_trip);
			}
			temp_trip.trip_orders.clear();
			previous_cust = 0;
			time = 0;
			--c_iter;
		}
		else
		{
			temp_trip.trip_orders.push_back(now_cust);
			if (time < order[now_cust].b_time)
				time = order[now_cust].b_time;
			time += order[now_cust].s_time;
			previous_cust = now_cust;
		}
	}
	compute_trip_time(temp_trip);
	compute_trip_distance(temp_trip);
	compute_trip_capacity(temp_trip);
	chrome.trips.push_back(temp_trip);
	chrome.trips.erase(chrome.trips.begin() + r_ind);
}

/*
功能：用LS6,检查最优的插入位置,没有进行两路径序列的交换：结果：
用min_dalta记录LS6领域操作后对应目标的值，其中对于f[2]，min_delta记录的是r_ind和i路径中最长路径行驶时间
即：找到最优插入位置后，f[0]最小化车辆数、f[1]:最小化乘客等待时间、f[2]:最小化总行驶距离、f[3]:最大化上座率
用c_pos记录最优的插入位置的索引    feasible记录是否找到插入位置
*/
bool check_best_pos2(Trip& org_trip, int ordernum, Trip& trip_one, int& c_pos, double& min_delta, int r_lamda[], int r_num, double now_dist, double now_time, double now_seat, double now_delay)
{
	bool feasible = false;
	min_delta = 999999;

	//LS6操作后的
	double delta_dist = 0;
	double delta_time_org = 0;
	double delta_time = 0;
	double delta = 0;
	double dist_org;
	double dist_mod;
	double time_org;
	double time_mod;
	double seat_1 = 0;
	double seat_2 = 0;
	double Aux_a = 0;
	double Aux_b = 0;

	Trip temp_trip = trip_one;
	cout << " temp_trip info:  ";
	cout << temp_trip.trip_id << endl;
	for (int x2 = 0; x2 < temp_trip.trip_orders.size(); x2++) {
		cout << temp_trip.trip_orders[x2] << " ---- " << "b:" << b_demand[temp_trip.trip_orders[x2]] << " a: " << a_demand[temp_trip.trip_orders[x2]];
	}
	compute_trip_capacity(temp_trip);
	compute_trip_time(temp_trip);
	compute_trip_distance(temp_trip);

	//容量约束检查：r_ind路径中的ordernum客户点， i路径的客户点迭代器c_iter
	vector<int>::iterator org_c_iter = org_trip.trip_orders.begin() + ordernum, c_iter;
	//将r_ind路径ordernum客户点后的序列与i路径适当c_iter客户点后的序列进行交换
	for (c_iter = temp_trip.trip_orders.begin(); c_iter != temp_trip.trip_orders.end(); ++c_iter)
	{
		Aux_a = 0;
		Aux_b = 0;
		//两种情况不行，指针向下一位置移动
		//对于r_ind路径：如果ordernum客户点之前容量 + c_iter之后容量 > max-capacity, ++c_iter
		cout << " check1:" << *org_c_iter << " " << *c_iter << endl;
		cout << " org_trip - orders:" << endl;
		for (int x1 = 0; x1 < org_trip.trip_orders.size(); x1++) {
			cout << org_trip.trip_orders[x1] << " ";
		}
		cout << endl;
		cout << " temp_trip - orders:" << endl;
		for (int x2 = 0; x2 < temp_trip.trip_orders.size(); x2++) {
			cout << temp_trip.trip_orders[x2] << " ";
		}
		cout << endl;
		if (b_demand[*org_c_iter] + a_demand[*c_iter] > max_capacity_1)
			continue;
		cout << " check2:" << b_demand[*org_c_iter] << " " << a_demand[*c_iter] << endl;
		cout << " org_trip - orders:" << endl;
		for (int x1 = 0; x1 < org_trip.trip_orders.size(); x1++) {
			cout << org_trip.trip_orders[x1] << " ";
		}
		cout << endl;
		cout << " temp_trip - orders:" << endl;
		for (int x2 = 0; x2 < temp_trip.trip_orders.size(); x2++) {
			cout << temp_trip.trip_orders[x2] << " ";
		}
		cout << endl;
		seat_1 = b_demand[*org_c_iter] + a_demand[*c_iter];
		//否则，r_ind路径交换容量检查通过，检查i路径
		//对于i路径，路径i只剩c_iter - 1之前部分，r_ind路径只剩ordernum + 1之后部分，连接检查容量约束
		//特殊情况考虑，若前后为空，引入辅助变量
		if (c_iter != temp_trip.trip_orders.begin())
			Aux_a = b_demand[*(c_iter - 1)];
		if (org_c_iter != org_trip.trip_orders.end() - 1)
			Aux_b = a_demand[*(org_c_iter + 1)];
		if (Aux_a + Aux_b > max_capacity_1)
			continue;
		seat_2 = Aux_a + Aux_b;

		//容量约束通过，进行时间检查
		//特殊情况考虑，若前后为空，引入辅助变量
		//第一种时间检查 判断客户点是不是0，该模型不需要
		if (l_time[*org_c_iter] + peer_time[*org_c_iter][*c_iter] > order[*c_iter].e_time + max_delay_time)
			continue;
		if (c_iter != temp_trip.trip_orders.begin() && org_c_iter != org_trip.trip_orders.end() - 1)
			if (l_time[*(c_iter - 1)] + peer_time[*(c_iter - 1)][*(org_c_iter + 1)] > order[*(org_c_iter + 1)].e_time + max_delay_time)
				continue;
		//第二种时间检查 看i路径的c_iter(含)后序列插入r_ind路径ordernum(不含)后 和 将r_ind路径ordernum + 1(含)后序列插入i路径c_iter - 1后
		//将c_iter后序列插入r_ind路径,到c_iter时的时间增量
		delta_time_org = l_time[*org_c_iter] + peer_time[*org_c_iter][*c_iter] - a_time[*c_iter];
		if (delta_time_org > max_wait[*c_iter])
			continue;
		//c_iter后序列插入r_ind路径可以插入
		//将ordernum+1后序列插入i路径，到org_c_iter+1时的时间增量
		if (c_iter != temp_trip.trip_orders.begin() && org_c_iter != org_trip.trip_orders.end() - 1)
		{
			delta_time = l_time[*(c_iter - 1)] + peer_time[*(c_iter - 1)][*(org_c_iter + 1)] - a_time[*(org_c_iter + 1)];
			if (delta_time > max_wait[*(org_c_iter + 1)])
				continue;
		}
		//两种时间检查后
		//求交换后，r_ind路径距离之和dist_org
		dist_org = b_distance[*org_c_iter] + peer_srcdistance[*org_c_iter][*c_iter] + a_distance[*c_iter];
		//求交换后，i路径距离之和dist_mod
		if (c_iter == temp_trip.trip_orders.begin() && org_c_iter != org_trip.trip_orders.end() - 1)
			dist_mod = calculateDis(order[*(org_c_iter + 1)].src_coordinate, order[*(org_c_iter + 1)].dest_coordinate);
		if (c_iter != temp_trip.trip_orders.begin() && org_c_iter == org_trip.trip_orders.end() - 1)
			dist_mod = calculateDis(order[*(c_iter - 1)].src_coordinate, order[*(c_iter - 1)].dest_coordinate);
		if (c_iter != temp_trip.trip_orders.begin() && org_c_iter != org_trip.trip_orders.end() - 1)
			dist_mod = b_distance[*(c_iter - 1)] + peer_srcdistance[*(c_iter - 1)][*(org_c_iter + 1)] + a_distance[*(org_c_iter + 1)];
		if (c_iter == temp_trip.trip_orders.begin() && org_c_iter == org_trip.trip_orders.end() - 1)
			dist_mod = 0;

		if (dist_org == 0)
			--r_num;
		if (dist_mod == 0)
			--r_num;

		double time_1 = 0;
		double time_2 = 0;
		double delay_1 = 0;
		double delay_2 = 0;

		//计算行驶时间time、延迟时间delay、上座率seat
		time_1 = l_time[org_trip.trip_orders[ordernum]];
		delay_1 = total_delay[org_trip.trip_orders[ordernum]];
		time_1 += peer_time[org_trip.trip_orders[ordernum]][*c_iter];
		if (time_1 < order[*c_iter].b_time)				        //小于最早时间
			time_1 = order[*c_iter].b_time;
		else if (time_1 > order[*c_iter].e_time)				//大于最迟时间
			delay_1 += time_1 - order[*c_iter].e_time;
		//在时间窗之内
		time_1 += order[*c_iter].s_time;
		for (vector<int>::iterator c_ind = c_iter + 1; c_ind != temp_trip.trip_orders.end(); ++c_ind)
		{
			time_1 += peer_time[*(c_ind - 1)][*c_ind];
			if (time_1 < order[*c_ind].b_time)
				time_1 = order[*c_ind].b_time;
			else if (time_1 > order[*c_ind].e_time)
				delay_1 += time_1 - order[*c_ind].e_time;
			time_1 += order[*c_ind].s_time;
		}
		if (c_iter != temp_trip.trip_orders.begin() && org_c_iter != org_trip.trip_orders.end() - 1)
		{
			time_2 = l_time[*(c_iter - 1)];
			delay_2 = total_delay[*(c_iter - 1)];
			time_2 += peer_time[*(c_iter - 1)][org_trip.trip_orders[ordernum + 1]];
			if (time_2 < order[org_trip.trip_orders[ordernum + 1]].b_time)
				time_2 = order[org_trip.trip_orders[ordernum + 1]].b_time;
			else if (time_2 > order[org_trip.trip_orders[ordernum + 1]].e_time)
				delay_2 += time_2 - order[org_trip.trip_orders[ordernum + 1]].e_time;
			time_2 += order[org_trip.trip_orders[ordernum + 1]].s_time;
			for (int i = ordernum + 2; i < org_trip.trip_orders.size(); ++i)
			{
				time_2 += peer_time[org_trip.trip_orders[i - 1]][org_trip.trip_orders[i]];
				if (time_2 < order[org_trip.trip_orders[i]].b_time)
					time_2 = order[org_trip.trip_orders[i]].b_time;
				else if (time_2 > order[org_trip.trip_orders[i]].e_time)
					delay_2 += order[org_trip.trip_orders[i]].e_time - time_2;
				time_2 += order[org_trip.trip_orders[i]].s_time;
			}
		}
		else
			delay_2 = 0;

		if (r_lamda[1] == 1)
			delta = now_delay + delay_1 + delay_2;
		else if (r_lamda[2] == 1)
			delta = now_dist + dist_org + dist_mod;
		else
			delta = r_num * max_capacity_1 / (now_seat + seat_1 + seat_2);            //上座率的倒数 最大化->最小化

		if (min_delta > delta)
		{
			min_delta = delta;
			c_pos = c_iter - temp_trip.trip_orders.begin();
			feasible = true;
		cout << " true: " << *org_c_iter << "--" << *c_iter << "  min_delta: " << min_delta << " c_pos:  " << c_pos << endl;;
		}
	}
	return feasible;
}

/*
功能：用LS6,检查最优的插入位置,没有进行两客户点的交换：结果：
用min_dalta记录LS6领域操作后对应目标的值，其中对于f[2]，min_delta记录的是r_ind和i路径中最长路径行驶时间
即：找到最优插入位置后，f[0]最小化车辆数、f[1]:最小化乘客等待时间、f[2]:最小化总行驶距离、f[3]:最大化上座率
用c_pos记录最优的交换客户点位置的索引    feasible记录是否找到插入位置
*/
bool check_best_pos3(Chromosome chrome, Car car1, Car car2, Trip& org_trip, int ordernum, Trip& trip_one, int& c_pos, double& min_delta, int r_lamda[], int r_num, double now_dist, double now_time, double now_seat, double now_delay)
{
	bool feasible = false;
	min_delta = 999999;

	//LS6操作后的
	double delta_dist = 0;
	double delta_time_org = 0;
	double delta_time = 0;
	double delta = 0;
	double dist_org;
	double dist_mod;
	double time_org;
	double time_mod;
	double seat_1 = 0;
	double seat_2 = 0;
	int temp_all = 0;
	int temp_ord = 0;
	int temp_que = 0;

	//容量约束检查：先记录变量保留orgtrip的总乘客人数 - ordernum的乘客人数
	compute_trip_capacity(org_trip);
	int org_all = org_trip.capacity;
	int org_ord = order[org_trip.trip_orders[ordernum]].peoples;
	int org_que = org_all - org_ord;

	Trip temp_trip = trip_one;
	compute_trip_capacity(temp_trip);
	compute_trip_time(temp_trip);
	compute_trip_distance(temp_trip);
	temp_all = temp_trip.capacity;

	//容量约束检查：r_ind路径中的ordernum客户点， i路径的客户点迭代器c_iter
	vector<int>::iterator org_c_iter = org_trip.trip_orders.begin() + ordernum, c_iter;
	//①：将org_trip路径去除ordernum客户点后的序列加上temp_trip的c_iter客户点
	for (c_iter = temp_trip.trip_orders.begin(); c_iter != temp_trip.trip_orders.end(); ++c_iter)
	{
		//两种情况不行，指针向下一位置移动
		//容量约束检查①
		if (org_que + order[*c_iter].peoples > max_capacity_1)
			continue;
		seat_1 = org_que + order[*c_iter].peoples;
		temp_ord = order[*c_iter].peoples;
		temp_que = temp_all - temp_ord;
		//容量约束检查②
		if (temp_que + org_ord > max_capacity_1)
			continue;
		seat_2 = temp_que + org_ord;

		//容量约束通过，进行时间检查
		//①生成Trip[]记录
		Trip t[MAX];
		vector<Trip>::iterator t_iter;
		for (t_iter = chrome.trips.begin(); t_iter != chrome.trips.end(); t_iter++) {
			t[t_iter->trip_id] = *t_iter;
		}
		t[org_trip.trip_id].trip_orders[ordernum] = *c_iter;
		t[temp_trip.trip_id].trip_orders[c_iter - temp_trip.trip_orders.begin()] = org_trip.trip_orders[ordernum];
		//时间检查
		if (!check_feasible_car(car1, t) || !check_feasible_car(car2, t))
			continue;
		//两种时间检查后
		//计算客户点交换后的两条路径行驶距离
		compute_trip_distance(t[org_trip.trip_id]);
		compute_trip_distance(t[temp_trip.trip_id]);
		//求交换后，r_ind路径距离之和dist_org
		//求交换后，i路径距离之和dist_mod
		dist_org = t[org_trip.trip_id].travel_dist;
		dist_mod = t[temp_trip.trip_id].travel_dist;

		if (dist_org == 0)
			--r_num;
		if (dist_mod == 0)
			--r_num;

		double time_1 = 0;
		double time_2 = 0;
		double delay_1 = 0;
		double delay_2 = 0;

		//计算行驶时间time、延迟时间delay、上座率seat
		time_1 = t[org_trip.trip_id].travel_time;
		time_2 = t[temp_trip.trip_id].travel_time;
		delay_1 = t[org_trip.trip_id].delay_time;
		delay_2 = t[temp_trip.trip_id].delay_time;

		if (r_lamda[1] == 1)
			delta = now_delay + delay_1 + delay_2;
		else if (r_lamda[2] == 1)
			delta = now_dist + dist_org + dist_mod;
		else
			delta = r_num * max_capacity_1 / (now_seat + seat_1 + seat_2);            //上座率的倒数 最大化->最小化

		if (min_delta > delta)
		{
			min_delta = delta;
			c_pos = c_iter - temp_trip.trip_orders.begin();
			feasible = true;
		}
	}
	return feasible;
}

Car find_car_id(Chromosome chrome, Trip trip)
{
	vector<Car>::iterator c_iter;
	vector<int>::iterator t_iter;
	for (c_iter = chrome.cars.begin(); c_iter != chrome.cars.end(); c_iter++) {
		for (t_iter = c_iter->car_trips.begin(); t_iter != c_iter->car_trips.end(); t_iter++) {
			if (*t_iter == trip.trip_id)
				return *c_iter;
		}
	}
	Car empty_car;
	clear_car(empty_car);
	return empty_car;
}

void ls_exchange_order(Chromosome& chrome, int r_lamda[], int r_ind, double now_time)
{
	//1.怎么选择客户点，随机 or 时间接近程度
	//按时间对客户点进行排序，对其范围上下内的客户点进行交换
	int c_ind = rand() % (chrome.trips[r_ind].trip_orders.size());
	compute_all(chrome.trips[r_ind]);
	double dist = 0, now_cap, now_delay;
	int c_pos = 0, min_c_pos = 0;
	double min_delta = 99999999, delta = 0;
	bool feasible = false;
	int r_pos;
	//建立一个函数，找到trip-car-id
	for (int i = 0; i < chrome.trips.size(); ++i)
	{
		//求除r_ind路径和i路径的路径长度之和、延迟时间之和、上座率
		if (i != r_ind)
		{
			//再加上一个判断，该客户点需在该行程最早开始时间-最晚时间之间才有意义需要检查
			dist = 0; now_time = 0; now_cap = 0; now_delay = 0;
			for (int j = 0; j < chrome.trips.size(); ++j)
			{
				if (j != r_ind && j != i)
				{
					dist += chrome.trips[j].travel_dist;            //除r_ind路径和i路径的路径长度之和
					now_cap += chrome.trips[j].capacity;			//除r_ind路径和i路径的上座人数
					now_delay += chrome.trips[j].delay_time;        //除r_ind路径和i路径的延迟时间之和
				}
			}
			Car car_one = find_car_id(chrome, chrome.trips[r_ind]);
			Car car_two = find_car_id(chrome, chrome.trips[i]);
			//用LS6，查找最优的插入位置:找到最优位置后，用delta记录对应目标的值
			if ((chrome.trips[r_ind].trip_set == chrome.trips[i].trip_set) && check_best_pos3(chrome, car_one, car_two, chrome.trips[r_ind], c_ind, chrome.trips[i], c_pos, delta, r_lamda, chrome.trips.size(), dist, now_time, now_cap, now_delay))
			{
				feasible = true;
				if (min_delta > delta)
				{
					min_delta = delta;
					min_c_pos = c_pos;
					r_pos = i;
				}
			}
		}
	}
	//本模型不讨论: 没找到最佳插入 || (对最长路径行驶时间f[2]进行优化 || LS6处理的f[2] > 原来的f[2] 拆分路径
	if (feasible)
	{
		int x1 = chrome.trips[r_ind].trip_orders[c_ind];
		int x2 = chrome.trips[r_pos].trip_orders[min_c_pos];
		chrome.trips[r_ind].trip_orders[c_ind] = x2;
		chrome.trips[r_pos].trip_orders[min_c_pos] = x1;
		//分别计算索引为r_ind和r_pos路径的行驶时间、行驶距离等
		compute_all(chrome.trips[r_ind]);
		compute_all(chrome.trips[r_pos]);
		update_twotrips(chrome, chrome.trips[r_ind], chrome.trips[r_pos]);
		update_global_trip(chrome);
	}
}


void two_opt(Chromosome& chrome, int r_lamda[], int r_ind, double now_time)
{
	int c_ind = rand() % (chrome.trips[r_ind].trip_orders.size());
	compute_all(chrome.trips[r_ind]);
	double dist = 0, now_cap, now_delay;
	int c_pos = 0, min_c_pos = 0;
	double min_delta = 99999999, delta = 0;
	bool feasible = false;
	int r_pos;
	for (int i = 0; i < chrome.trips.size(); ++i)
	{
		//求除r_ind路径和i路径的路径长度之和、延迟时间之和、上座率
		if (i != r_ind)
		{
			dist = 0; now_time = 0; now_cap = 0; now_delay = 0;
			for (int j = 0; j < chrome.trips.size(); ++j)
			{
				if (j != r_ind && j != i)
				{
					dist += chrome.trips[j].travel_dist;            //除r_ind路径和i路径的路径长度之和
					now_cap += chrome.trips[j].capacity;			//除r_ind路径和i路径的上座人数
					now_delay += chrome.trips[j].delay_time;        //除r_ind路径和i路径的延迟时间之和
				}
			}
			//用LS6，查找最优的插入位置:找到最优位置后，用delta记录对应目标的值
			if ((chrome.trips[r_ind].trip_set == chrome.trips[i].trip_set) && check_best_pos2(chrome.trips[r_ind], c_ind, chrome.trips[i], c_pos, delta, r_lamda, chrome.trips.size(), dist, now_time, now_cap, now_delay))
			{
				feasible = true;
				if (min_delta > delta)
				{
					min_delta = delta;
					min_c_pos = c_pos;
					r_pos = i;
				}
			}
		}
	}
	//本模型不讨论: 没找到最佳插入 || (对最长路径行驶时间f[2]进行优化 || LS6处理的f[2] > 原来的f[2] 拆分路径
	if (feasible)
	{
		cout << "[Exchange] before" << endl;
		cout << "route1 info:" << "id: " << chrome.trips[r_ind].trip_id << endl;
		cout << "orders: ";
		for (int x1 = 0; x1 < chrome.trips[r_ind].trip_orders.size(); x1++) {
			cout << chrome.trips[r_ind].trip_orders[x1] << "->";
		}
		cout << endl;
		cout << "route2 info:" << "id: " << chrome.trips[r_pos].trip_id << endl;
		cout << "orders: ";
		for (int x2 = 0; x2 < chrome.trips[r_pos].trip_orders.size(); x2++) {
			cout << chrome.trips[r_pos].trip_orders[x2] << "->";
		}
		cout << endl;
		vector<int>r1, r2;
		for (int i = 0; i <= c_ind; ++i)
			r1.push_back(chrome.trips[r_ind].trip_orders[i]);
		for (int i = min_c_pos; i < chrome.trips[r_pos].trip_orders.size(); ++i)
			r1.push_back(chrome.trips[r_pos].trip_orders[i]);
		for (int i = 0; i < min_c_pos; ++i)
			r2.push_back(chrome.trips[r_pos].trip_orders[i]);
		for (int i = c_ind + 1; i < chrome.trips[r_ind].trip_orders.size(); ++i)
			r2.push_back(chrome.trips[r_ind].trip_orders[i]);
		//分别将客户点容器赋给对应路径的客户点容器变量
		chrome.trips[r_ind].trip_orders = r1;
		chrome.trips[r_pos].trip_orders = r2;
		cout << "[Exchange] after" << endl;
		cout << "route1 info:" << "id: " << chrome.trips[r_ind].trip_id << endl;
		cout << "orders: ";
		for (int x1 = 0; x1 < chrome.trips[r_ind].trip_orders.size(); x1++) {
			cout << chrome.trips[r_ind].trip_orders[x1] << "->";
		}
		cout << endl;
		cout << "route2 info:" << "id: " << chrome.trips[r_pos].trip_id << endl;
		cout << "orders: ";
		for (int x2 = 0; x2 < chrome.trips[r_pos].trip_orders.size(); x2++) {
			cout << chrome.trips[r_pos].trip_orders[x2] << "->";
		}
		cout << endl;
		//分别计算索引为r_ind和r_pos路径的行驶时间、行驶距离等
		compute_all(chrome.trips[r_ind]);
		compute_all(chrome.trips[r_pos]);
		update_twotrips(chrome, chrome.trips[r_ind], chrome.trips[r_pos]);
		update_global_trip(chrome);
		//如果路径变为无效路径，从解中删除此路径
		if (r1.size() == 0)
		{
			update_trip12(chrome, chrome.trips[r_ind]);
			chrome.trips.erase(chrome.trips.begin() + r_ind);
			update_global_trip(chrome);
		}

		if (r2.size() == 0)
		{
			update_trip12(chrome, chrome.trips[r_pos]);
			chrome.trips.erase(chrome.trips.begin() + r_pos);
			update_global_trip(chrome);
		}
	}
}

//基于订单ordernum新建与更新路径(车辆)
//1.新建一条路径判断能否插入现有车辆
//2.若不可插入，则新建一辆车辆

void create_car_trip(Chromosome& chrome, int ordernum)
{
	cout << "[Create] before trips: " << chrome.trips.size() << " cars: " << chrome.cars.size() << endl;
	//新建一个路径、更新路径信息
	Trip temp_trip;
	clear_trip(temp_trip);
	//找当前最大trip_id
	vector<int>find_maxid;
	vector<Trip>::iterator t_iter;
	for (t_iter = chrome.trips.begin(); t_iter != chrome.trips.end(); ++t_iter)
		find_maxid.push_back(t_iter->trip_id);
	sort(find_maxid.begin(), find_maxid.end());
	vector<int>::iterator iter = find_maxid.end() - 1;
	temp_trip.trip_id = (*iter) + 1;
	temp_trip.trip_set = order[ordernum].location_type + 1;
	temp_trip.trip_orders.push_back(ordernum);
	compute_all(temp_trip);
	chrome.trips.push_back(temp_trip);
	trip[temp_trip.trip_id] = temp_trip;
	if (temp_trip.trip_set == 1)
		chrome.trip1.push_back(temp_trip);
	else
		chrome.trip2.push_back(temp_trip);
	//重大bug，车辆里的行程数有重复，定义一个变量检测
	int bug = 0;
	//检查能否插入现有车辆，分析可得新建路径只能插入车辆最后一个位置，若车辆均从a->b，则trip1插双数，trip2插单数
	vector<Car>::iterator car_iter;
	for (car_iter = chrome.cars.begin(); car_iter != chrome.cars.end(); ++car_iter)
	{
		int len = car_iter->car_trips.size();
		if ((temp_trip.trip_set + len) % 2 != 0)
		{
			car_iter->car_trips.push_back(temp_trip.trip_id);
			//当前车辆可插入，则完成插入break
			if (check_feasible_car(*car_iter)) {
				bug = 1;  //说明已经插入了，下面不要再重复插入
				break;
			}
			//当前车辆不可插入，退回操作
			else
				car_iter->car_trips.pop_back();
		}
	}
	if (bug == 1)
		return;
	//所有车辆检查完毕均不可插入，则新建一辆车辆
	Car temp_car;
	clear_car(temp_car);
	//找当前最大car_id
	vector<int>find_maxid2;
	for (car_iter = chrome.cars.begin(); car_iter != chrome.cars.end(); ++car_iter)
		find_maxid2.push_back(car_iter->car_id);
	sort(find_maxid2.begin(), find_maxid2.end());
	vector<int>::iterator iter2 = find_maxid2.end() - 1;
	temp_car.car_id = (*iter2) + 1;
	temp_car.car_trips.push_back(temp_trip.trip_id);
	compute_car_time(temp_car);
	compute_car_distance(temp_car);
	chrome.cars.push_back(temp_car);
	cout << "[Create] after trips: " << chrome.trips.size() << " cars: " << chrome.cars.size() << endl;
	//check_is_change(chrome);
}

void create_car_trip(Chromosome& chrome, int ordernum, Trip ttt[])
{
	//新建一个路径、更新路径信息
	Trip temp_trip;
	clear_trip(temp_trip);
	//找当前最大trip_id
	vector<int>find_maxid;
	vector<Trip>::iterator t_iter;
	for (t_iter = chrome.trips.begin(); t_iter != chrome.trips.end(); ++t_iter)
		find_maxid.push_back(t_iter->trip_id);
	sort(find_maxid.begin(), find_maxid.end());
	vector<int>::iterator iter = find_maxid.end() - 1;
	temp_trip.trip_id = (*iter) + 1;
	temp_trip.trip_set = order[ordernum].location_type + 1;
	temp_trip.trip_orders.push_back(ordernum);
	compute_all(temp_trip);
	chrome.trips.push_back(temp_trip);
	ttt[temp_trip.trip_id] = temp_trip;
	if (temp_trip.trip_set == 1)
		chrome.trip1.push_back(temp_trip);
	else
		chrome.trip2.push_back(temp_trip);
	//重大bug，车辆里的行程数有重复，定义一个变量检测
	int bug = 0;
	//检查能否插入现有车辆，分析可得新建路径只能插入车辆最后一个位置，若车辆均从a->b，则trip1插双数，trip2插单数
	vector<Car>::iterator car_iter;
	for (car_iter = chrome.cars.begin(); car_iter != chrome.cars.end(); ++car_iter)
	{
		int len = car_iter->car_trips.size();
		if ((temp_trip.trip_set + len) % 2 != 0)
		{
			car_iter->car_trips.push_back(temp_trip.trip_id);
			//当前车辆可插入，则完成插入break
			if (check_feasible_car(*car_iter, ttt)) {
				bug = 1;  //说明已经插入了，下面不要再重复插入
				break;
			}
			//当前车辆不可插入，退回操作
			else
				car_iter->car_trips.pop_back();
		}
	}
	if (bug == 1)
		return;
	//所有车辆检查完毕均不可插入，则新建一辆车辆
	Car temp_car;
	clear_car(temp_car);
	//找当前最大car_id
	vector<int>find_maxid2;
	for (car_iter = chrome.cars.begin(); car_iter != chrome.cars.end(); ++car_iter)
		find_maxid2.push_back(car_iter->car_id);
	sort(find_maxid2.begin(), find_maxid2.end());
	vector<int>::iterator iter2 = find_maxid2.end() - 1;
	temp_car.car_id = (*iter2) + 1;
	temp_car.car_trips.push_back(temp_trip.trip_id);
	compute_car_time(temp_car, ttt);
	compute_car_distance(temp_car, ttt);
	chrome.cars.push_back(temp_car);
}

void update_ydmd(Chromosome chrome)
{
	for (vector<Trip>::iterator iter = chrome.trips.begin(); iter != chrome.trips.end(); ++iter)
	{
		compute_trip_distance(*iter);
		compute_trip_capacity(*iter);
		compute_trip_time(*iter);
	}
}

void insert_order(Chromosome& chrome, int r_lamda[], int ordernum, double now_time = 9999999)
{
	//没有插入order[ordernum]时，解的总行驶距离\解的总延迟时间\解的上座人数
	double now_dist, now_delay, now_seat, now_carnum;
	Trip temp_trip;
	Car temp_car;
	vector<Car>::iterator car_iter, car_pos;
	vector<Trip>::iterator r_pos;

	int num = chrome.cars.size();
	int c_pos = 0;
	int min_c_pos = 0;
	double min_delta = 99999999;
	double delta = 0;
	bool feasible = false;

	for (car_iter = chrome.cars.begin(); car_iter != chrome.cars.end(); ++car_iter)
	{
		now_dist = 0;
		now_delay = 0;
		now_seat = 0;
		now_carnum = chrome.cars.size();
		for (vector<Car>::iterator c_index = chrome.cars.begin(); c_index != chrome.cars.end(); ++c_index)
		{
			if (c_index->car_id != car_iter->car_id)
			{
				now_dist += c_index->car_dist;                  //除car_iter以外其他车辆的行驶距离
				now_delay += c_index->car_delay;			     //除car_iter以外其他车辆的延迟时间
				vector<int>::iterator s_iter;
				for (s_iter = c_index->car_trips.begin(); s_iter != c_index->car_trips.end(); s_iter++)
					now_seat += trip[*s_iter].capacity;
			}
		}
		vector<Trip>temps;
		for (vector<int>::iterator temp_iter = car_iter->car_trips.begin(); temp_iter != car_iter->car_trips.end(); ++temp_iter)
		{
			temps.push_back(trip[*temp_iter]);
		}
		for (vector<int>::iterator r_iter = car_iter->car_trips.begin(); r_iter != car_iter->car_trips.end(); ++r_iter)
		{
			if (check_best_pos1(ordernum, *car_iter, trip[*r_iter], temps, num, c_pos, delta, r_lamda, now_dist, now_time, now_delay, now_seat))
			{
				feasible = true;
				cout << " jin lai le  " << trip[*r_iter].trip_orders.size() << endl;
				if (min_delta > delta)
				{
					cout << " jin lai le111   " <<  *r_iter << endl;
					min_delta = delta;
					min_c_pos = c_pos;
					for (vector<Trip>::iterator t_iter = chrome.trips.begin(); t_iter != chrome.trips.end(); ++t_iter)
						if (t_iter->trip_id == *r_iter) {
							cout << " jin lai le222" << endl;
							cout << t_iter->trip_id << endl;
							r_pos = t_iter;
							cout << r_pos->trip_orders.size() << endl;
						}
				}
			}
		}
	}
	//没有找到最佳插入位置
	if (!feasible) {
		create_car_trip(chrome, ordernum);
		//update_global_trip(chrome);
	}
	//找到最佳插入位置，把cust插入到最佳路径r_pos的最佳位置min_c_pos中
	else
	{
		r_pos->trip_orders.insert(r_pos->trip_orders.begin() + min_c_pos, ordernum);
		compute_all(*r_pos);
		update_onetrip(chrome, *r_pos);
		update_global_trip(chrome);
	}
}

void insert_order(Chromosome& chrome, int r_lamda[], int ordernum, Trip tt[], double now_time = 9999999)
{
	//没有插入order[ordernum]时，解的总行驶距离\解的总延迟时间\解的上座人数
	double now_dist, now_delay, now_seat, now_carnum;
	Trip temp_trip;
	Car temp_car;
	vector<Car>::iterator car_iter, car_pos;
	vector<Trip>::iterator r_pos;

	int num = chrome.cars.size();
	int c_pos = 0;
	int min_c_pos = 0;
	double min_delta = 99999999;
	double delta = 0;
	bool feasible = false;

	for (car_iter = chrome.cars.begin(); car_iter != chrome.cars.end(); ++car_iter)
	{
		now_dist = 0;
		now_delay = 0;
		now_seat = 0;
		now_carnum = chrome.cars.size();
		for (vector<Car>::iterator c_index = chrome.cars.begin(); c_index != chrome.cars.end(); ++c_index)
		{
			if (c_index->car_id != car_iter->car_id)
			{
				now_dist += c_index->car_dist;                  //除car_iter以外其他车辆的行驶距离
				now_delay += c_index->car_delay;			     //除car_iter以外其他车辆的延迟时间
				vector<int>::iterator s_iter;
				for (s_iter = c_index->car_trips.begin(); s_iter != c_index->car_trips.end(); s_iter++)
					now_seat += tt[*s_iter].capacity;
			}
		}
		vector<Trip>temps;
		for (vector<int>::iterator temp_iter = car_iter->car_trips.begin(); temp_iter != car_iter->car_trips.end(); ++temp_iter)
		{
			temps.push_back(tt[*temp_iter]);
		}
		for (vector<int>::iterator r_iter = car_iter->car_trips.begin(); r_iter != car_iter->car_trips.end(); ++r_iter)
		{
			if (check_best_pos1(ordernum, *car_iter, tt[*r_iter], temps, num, c_pos, delta, r_lamda, now_dist, now_time, now_delay, now_seat, now_carnum))
			{
				feasible = true;
				if (min_delta > delta)
				{
					min_delta = delta;
					min_c_pos = c_pos;
					for (vector<Trip>::iterator t_iter = chrome.trips.begin(); t_iter != chrome.trips.end(); ++t_iter)
						if (t_iter->trip_id == *r_iter)
							r_pos = t_iter;
				}
			}
		}
	}
	//没有找到最佳插入位置
	if (!feasible) {
		create_car_trip(chrome, ordernum, tt);
		update_global_trip(chrome);
	}
	//找到最佳插入位置，把cust插入到最佳路径r_pos的最佳位置min_c_pos中
	else
	{
		r_pos->trip_orders.insert(r_pos->trip_orders.begin() + min_c_pos, ordernum);
		compute_all(*r_pos);
		update_onetrip(chrome, *r_pos, tt);
		update_global_trip(chrome);
	}
}

bool is_better(Chromosome& chrome_1, Chromosome& chrome_2)
{
	bool better = false;
	for (int i = 0; i < FUNC_NUM; ++i)
	{
		if (chrome_2.f[i] - chrome_1.f[i] > 0.000001)
			better = true;
		if (chrome_1.f[i] - chrome_2.f[i] > 0.000001)
			return false;
	}
	return better;
}

bool is_better_box(Chromosome& chrome_1, Chromosome& chrome_2)
{
	bool better = false;
	for (int i = 0; i < FUNC_NUM; ++i)
	{
		if (chrome_2.box_f[i] - chrome_1.box_f[i] > 0.000001)
			better = true;
		if (chrome_1.box_f[i] - chrome_2.box_f[i] > 0.000001)
			return false;
	}
	return better;
}

bool is_equal(Chromosome& chrome_1, Chromosome& chrome_2)
{
	for (int i = 0; i < FUNC_NUM; ++i)
	{
		if (fabs(chrome_1.f[i] - chrome_2.f[i]) > 0.000001)
			return false;
	}
	return true;
}

bool is_equal_box(Chromosome& chrome_1, Chromosome& chrome_2)
{
	for (int i = 0; i < FUNC_NUM; ++i)
	{
		if (abs(chrome_1.box_f[i] - chrome_2.box_f[i]) > 0.000001)
			return false;
	}
	return true;
}

// Paper mechanism: epsilon-dominance archive grid.
// box_f records the epsilon box index of each objective and is used by update_EP
// to control archive density while preserving nondominated solutions.
void get_box(Chromosome& chrome)
{
	for (int i = 0; i < FUNC_NUM; ++i)
	{
		if (chrome.f[i] <= 0)
			chrome.box_f[i] = 0;
		else
			chrome.box_f[i] = (int)(log(chrome.f[i]) / log(1 + EXPERIMENT_EPSILON_BOX));
	}
}

bool is_extreme(Chromosome& chrome)
{
	for (int index = 0; index < FUNC_NUM; ++index)
	{
		if (extreme[index].f[index] - chrome.f[index] > 0.00001)
			return true;
	}
	return false;
}

void preserve_extreme(Chromosome& chrome, int index)
{
	if (extreme[index].f[index] - chrome.f[index] > 0.00001)
	{
		extreme[index] = chrome;
	}
}

void update_extreme(Chromosome& new_chromosome)
{
	for (int i = 0; i < FUNC_NUM; ++i)
	{
		preserve_extreme(new_chromosome, i);
	}
}

//更新EP里加入更新car
// Paper mechanism: external archive update with Pareto dominance,
// epsilon-box pruning, and extreme-solution preservation.
void update_EP(Chromosome& new_chromosome, vector<Chromosome>& EPP)
{
	bool flag = false;
	//update_Car(new_chromosome);
	get_box(new_chromosome);
	if (EPP.size() == 0)
	{
		update_extreme(new_chromosome);
		EPP.push_back(new_chromosome);
		for (int i = 0; i < 4; i++) {
			if (new_chromosome.f[i] < curr_f[i])
				curr_f[i] = new_chromosome.f[i];
			if (new_chromosome.f[i] < global_f[i])
				global_f[i] = new_chromosome.f[i];
		}
	}
	else
	{
		vector<Chromosome>::iterator iter = EPP.begin();
		for (; iter != EPP.end(); ++iter)
		{
			if (is_better(*iter, new_chromosome) || is_equal(*iter, new_chromosome))
				return;
		}
		iter = EPP.begin();
		if (is_extreme(new_chromosome))
		{
			update_extreme(new_chromosome);
			iter = EPP.begin();
			while (iter != EPP.end())
			{
				if (is_better(new_chromosome, *iter))
					iter = EPP.erase(iter);
				else
					++iter;
			}
			EPP.push_back(new_chromosome);
			for (int i = 0; i < 4; i++) {
				if (new_chromosome.f[i] < curr_f[i])
					curr_f[i] = new_chromosome.f[i];
				if (new_chromosome.f[i] < global_f[i])
					global_f[i] = new_chromosome.f[i];
			}
		}
		else
		{
			iter = EPP.begin();
			while (iter != EPP.end())
			{
				if (is_better(new_chromosome, *iter))
				{
					iter = EPP.erase(iter);
					flag = true;
				}
				else
					++iter;
			}
			if (flag)
			{
				EPP.push_back(new_chromosome);
				for (int i = 0; i < 4; i++) {
					if (new_chromosome.f[i] < curr_f[i])
						curr_f[i] = new_chromosome.f[i];
					if (new_chromosome.f[i] < global_f[i])
						global_f[i] = new_chromosome.f[i];
				}
			}
			else
			{
				iter = EPP.begin();
				for (; iter != EPP.end(); ++iter)
				{
					if (is_equal_box(new_chromosome, *iter) || is_better_box(*iter, new_chromosome))
						break;
				}
				if (iter == EPP.end())
				{
					EPP.push_back(new_chromosome);
					for (int i = 0; i < 4; i++) {
						if (new_chromosome.f[i] < curr_f[i])
							curr_f[i] = new_chromosome.f[i];
						if (new_chromosome.f[i] < global_f[i])
							global_f[i] = new_chromosome.f[i];
					}
				}
			}
		}
	}
}

//采用局部搜索方法对解进行优化，将新解进行执行更新存档操作；针对234目标
//针对②③，换路径  针对④，删路径
void localsearch_fir(Chromosome& chrome, int& count, int r_lamda[], int index, vector<Chromosome>& EPxx)
{
	//针对②③，换路径
	if (r_lamda[1] == 1)
	{
		cout << "[LocalSearch] M2" << endl;
		int V = chrome.cars.size();
		int r1 = rand() % V;
		int obj = rand() % 2 + 1;
		ls_exchange_trip(chrome, r1, obj, count);
		for (vector<Trip>::iterator iter = chrome.trips.begin(); iter != chrome.trips.end(); ++iter)
		{
			compute_trip_distance(*iter);
			compute_trip_capacity(*iter);
			compute_trip_time(*iter);
		}
		compute_f(chrome);
		update_Matrix(chrome, init_data, 1);
		update_EP(chrome, EPxx);
	}
	//针对④，删路径
	else
	{
		cout << "[LocalSearch] M3" << endl;
		vector<int>::iterator c_iter;
		vector<Trip>::iterator r_iter;
		int trip_ind; int min_num = 999999;
		for (r_iter = chrome.trips.begin(); r_iter != chrome.trips.end(); ++r_iter)
		{
			if (min_num > r_iter->capacity)
			{
				min_num = r_iter->capacity;
				trip_ind = r_iter - chrome.trips.begin();
			}
		}
		int com = 0;    //定义执行次数，最多判断两个行程即一回程
		if (opt_lsone(chrome, trip_ind, com, 0)) {
			for (vector<Car>::iterator car_iter = chrome.cars.begin(); car_iter != chrome.cars.end(); )
			{
				if (car_iter->car_trips.size() == 0)
					car_iter = chrome.cars.erase(car_iter);
				else
					++car_iter;
			}
			for (vector<Trip>::iterator iter = chrome.trips.begin(); iter != chrome.trips.end(); ++iter)
			{
				compute_trip_distance(*iter);
				compute_trip_capacity(*iter);
				compute_trip_time(*iter);
			}
			compute_f(chrome);
			update_Matrix(chrome, init_data, 2);
			update_EP(chrome, EPxx);
		}
	}
}

void check_is_change(Chromosome& chrome) {
	/*int x = 0;
	for (int i = 1; i < 100; i++) {
		x += trip[i].trip_orders.size();
		if (trip[i].trip_id == 0 && trip[i].trip_set == 0)
			break;
	}
	if (x != Invariant)
		Invariant = x;*/
	int check[MAX] = {};
	vector<Car>::iterator c_iter;
	vector<int>::iterator iter;
	for (c_iter = chrome.cars.begin(); c_iter != chrome.cars.end(); c_iter++) {
		for (iter = c_iter->car_trips.begin(); iter != c_iter->car_trips.end(); iter++) {
			check[*iter]++;
			assert(check[*iter] <= 1);
		}
	}
}

//采用局部搜索方法对解进行优化，将新解进行执行更新存档操作；针对234目标
//customers->route  orders->trips->car
//trip分trip1和trip2
void localsearch_k(Chromosome& chrome, int r_lamda[], int index, vector<Chromosome>& EPxx)
{
	vector<int>orders;
	Trip temp_trip;
	double now_time = 0;
	orders.clear();
	int r_ind = 0;
	r_ind = rand() % chrome.trips.size();

	int r_lam[4];
	int p = (rand() % 3) + 1;
	r_lam[p] = 1;

	//删除一个客户点
	if (r_lamda[3] == 1)
	{
		cout << "[LocalSearch] M4" << endl;
		index = 3;
		int cust_ind = rand() % (chrome.trips[r_ind].trip_orders.size());  //考虑车库-2，而本模型暂不考虑车库
		int cust = chrome.trips[r_ind].trip_orders[cust_ind];
		orders.push_back(cust);
		chrome.trips[r_ind].trip_orders.erase(chrome.trips[r_ind].trip_orders.begin() + cust_ind);
		update_onetrip(chrome, chrome.trips[r_ind]);
		update_global_trip(chrome);
		if (chrome.trips[r_ind].trip_orders.size() == 0)
		{
			update_trip12(chrome, chrome.trips[r_ind]);
			chrome.trips.erase(chrome.trips.begin() + r_ind);
			update_global_trip(chrome);
		}
		else
		{
			//检查删除chrome.trips[r_ind]路径的cust_ind后，该路径是否有效  路径本可行，删去应该也可行
			if (check_feasible_trip(chrome.trips[r_ind]))
			{
				compute_trip_time(chrome.trips[r_ind]);
				compute_trip_distance(chrome.trips[r_ind]);
			}
		}
		cout << chrome.cars.size() << " ---qiu--- " << chrome.trips.size() << endl;
		check_is_change(chrome);
		//将删除容器中的客户点插入到解中产生新解
		for (int iter = 0; iter < orders.size(); ++iter)
			insert_order(chrome, r_lam, orders[iter], now_time);
		//更新trip[]:直接全更新
		vector<Trip>::iterator t_iter;
		for (t_iter = chrome.trips.begin(); t_iter != chrome.trips.end(); ++t_iter) {
			trip[t_iter->trip_id] = *t_iter;
		}
		compute_f(chrome);
	}
	//删除n个客户点
	else if (r_lamda[4] == 1)
	{
		cout << "[LocalSearch] M5" << endl;
		index = 4;
		int count = rand() % (chrome.trips[r_ind].trip_orders.size());
		for (int i = 0; i < count; ++i)
		{
			int cust_ind = rand() % (chrome.trips[r_ind].trip_orders.size());
			orders.push_back(chrome.trips[r_ind].trip_orders[cust_ind]);
			chrome.trips[r_ind].trip_orders.erase(chrome.trips[r_ind].trip_orders.begin() + cust_ind);
			update_onetrip(chrome, chrome.trips[r_ind]);
			update_global_trip(chrome);
		}
		if (chrome.trips[r_ind].trip_orders.size() == 0)
		{
			update_trip12(chrome, chrome.trips[r_ind]);
			chrome.trips.erase(chrome.trips.begin() + r_ind);
			update_global_trip(chrome);
		}
		else
		{
			if (check_feasible_trip(chrome.trips[r_ind]))
			{
				compute_trip_time(chrome.trips[r_ind]);
				compute_trip_distance(chrome.trips[r_ind]);
			}
		}
		//将删除容器中的客户点插入到解中产生新解
		for (int iter = 0; iter < orders.size(); ++iter)
			insert_order(chrome, r_lam, orders[iter], now_time);
		//更新trip[]:直接全更新
		vector<Trip>::iterator t_iter;
		for (t_iter = chrome.trips.begin(); t_iter != chrome.trips.end(); ++t_iter) {
			trip[t_iter->trip_id] = *t_iter;
		}
		compute_f(chrome);
	}
	//two-opt操作
	else
	{
		index = 5;
		cout << "[LocalSearch] M6" << endl;
		two_opt(chrome, r_lamda, r_ind, now_time);
		update_global_trip(chrome);
	}
	//更新阶段
	for (vector<Trip>::iterator iter = chrome.trips.begin(); iter != chrome.trips.end(); ++iter)
	{
		compute_trip_distance(*iter);
		compute_trip_capacity(*iter);
		compute_trip_time(*iter);
	}
	compute_f(chrome);
	update_Matrix(chrome, init_data, index);
	update_EP(chrome, EPxx);
	for (vector<Car>::iterator car_iter = chrome.cars.begin(); car_iter != chrome.cars.end(); )
	{
		if (car_iter->car_trips.size() == 0)
			car_iter = chrome.cars.erase(car_iter);
		else
			++car_iter;
	}
}

void local_search_lunliu(Chromosome& chrome, int r_lamda[], int index, vector<Chromosome>& EPxx)
{
	//初始阶段，计算局部搜索前chrome的各目标值
	update_ydmd(chrome);
	compute_f(chrome);
	for (int i = 0; i < FUNC_NUM; i++) {
		init_data[i] = chrome.f[i];
	}
	//开始进入轮流局部搜索阶段
	if (r_lamda[0] == 1)
	{
		cout << "[LocalSearch] M1" << endl;
		vector<int>::iterator c_iter;
		vector<Car>::iterator r_iter;
		int car_ind; int min_num = 999999;
		for (r_iter = chrome.cars.begin(); r_iter != chrome.cars.end(); ++r_iter)
		{
			if (min_num > r_iter->car_trips.size())
			{
				min_num = r_iter->car_trips.size();
				car_ind = r_iter - chrome.cars.begin();
			}
		}
		if (opt_lstwo(chrome, car_ind))
		{
			update_ydmd(chrome);
			compute_f(chrome);
			update_Matrix(chrome, init_data, 0);
			update_EP(chrome, EPxx);
		}
	}
	else if (r_lamda[1] == 1 || r_lamda[2] == 1)
	{
		int count = 0;
		Chromosome best = chrome;
		localsearch_fir(chrome, count, r_lamda, -1, EPxx);
	}
	else if (r_lamda[3] == 1 || r_lamda[4] == 1 || r_lamda[5] == 1)
	{
		Chromosome best = chrome;
		localsearch_k(chrome, r_lamda, -1, EPxx);
	}
	else if (r_lamda[6] == 1)
	{
		cout << "[LocalSearch] M7" << endl;
		double now_time = 0;
		int r_ind = 0;
		r_ind = rand() % chrome.trips.size();
		ls_exchange_order(chrome, r_lamda, r_ind, now_time);
		update_ydmd(chrome);
		compute_f(chrome);
		update_Matrix(chrome, init_data, 6);
		update_EP(chrome, EPxx);
		update_global_trip(chrome);
	}
	else
	{
		cout << "[LocalSearch] M8" << endl;
		int count = 0;
		int V = chrome.cars.size();
		int r1 = rand() % V;
		int obj = rand() % 2 + 1;
		ls_exchange_mutitrip(chrome, r1, obj, count);
		update_ydmd(chrome);
		compute_f(chrome);
		update_Matrix(chrome, init_data, 7);
		update_EP(chrome, EPxx);
	}
}

void init_pop(vector<Chromosome>& EPr)
{
	for (int i = 0; i < FUNC_NUM; ++i)
		extreme[i].f[i] = 999999;
	Chromosome chrome;
	int r_lamda[8];
	for (int i = 0; i < 8; ++i)
	{
		if (!is_operator_enabled(i)) continue;
		clean_chromosome(chrome);
		init_pop(chrome);
		for (int j = 0; j < 8; ++j)
			r_lamda[j] = 0;
		r_lamda[i] = 1;
		local_search_lunliu(chrome, r_lamda, -1, EPr);
	}
}

// Preprocessing variant: exhaustively cycles enabled local-search operators
// for a short learning period to initialize operator-quality statistics.
void init_Study(vector<Chromosome>& EP_study)
{
	//初始化学习阶段，只更新学习矩阵数据
	clock_t pro_begin, pro_end;
	pro_begin = clock();
	init_pop(EP_study);
	init_Matrix();
	Chromosome chrome, temp_chrome;
	int r_lamda[8];
	while (true) {
		clean_chromosome(chrome);
		int ind = rand() % EP_study.size();
		temp_chrome = EP_study[ind];
		for (int i = 0; i < 8; ++i)
		{
			if (!is_operator_enabled(i)) continue;
			for (int j = 0; j < 8; ++j)
				r_lamda[j] = 0;
			r_lamda[i] = 1;
			chrome = temp_chrome;
			local_search_lunliu(chrome, r_lamda, -1, EP_study);
			cout << "*************************************" << endl;
			cout << "[Preprocess] operator " << i << "   ********    " << endl;
			cout << "*************end****************" << endl;
		}
		pro_end = clock();
		if ((double)((pro_end - pro_begin) / CLOCKS_PER_SEC) > EXPERIMENT_PREPROCESS_SECONDS)
			break;
	}
}

// Paper mechanism: learning/preprocessing stage.
// A random objective and an enabled local-search operator are sampled, then the
// resulting improvement is recorded in the same AOS reward matrices used later
// during online evolution.
void learning_Stage(Chromosome& chrome, vector<Chromosome>& EP_study)
{
	//初始阶段，计算局部搜索前chrome的各目标值
	update_ydmd(chrome);
	compute_f(chrome);
	for (int i = 0; i < FUNC_NUM; i++) {
		init_data[i] = chrome.f[i];
	}
	//保持不变，加入lamda
	int lamda[FUNC_NUM] = { 0, 0, 0, 0 };
	int x = rand() % 4;
	lamda[x] = 1;
	int LS_num = random_enabled_operator();
	if (LS_num == 1) {
		SF_Matrix[0][x]++;
		SFC_Matrix[0][x]++;
		cout << "[LearningStage] M1" << endl;
		vector<int>::iterator c_iter;
		vector<Car>::iterator r_iter;
		int car_ind; int min_num = 999999;
		for (r_iter = chrome.cars.begin(); r_iter != chrome.cars.end(); ++r_iter)
		{
			if (min_num > r_iter->car_trips.size())
			{
				min_num = r_iter->car_trips.size();
				car_ind = r_iter - chrome.cars.begin();
			}
		}
		if (opt_lstwo(chrome, car_ind))
		{
			update_ydmd(chrome);
			compute_f(chrome);
			update_EP(chrome, EP_study);
		}
		update_Matrix(chrome, init_data, 0);
	}
	else if (LS_num == 2) {
		SF_Matrix[1][x]++;
		SFC_Matrix[1][x]++;
		int count = 0;
		cout << "[LearningStage] M2" << endl;
		int V = chrome.cars.size();
		int r1 = rand() % V;
		int obj = rand() % 2 + 1;
		ls_exchange_trip(chrome, r1, obj, count);
		update_ydmd(chrome);
		compute_f(chrome);
		update_EP(chrome, EP_study);
		update_Matrix(chrome, init_data, 1);
	}
	else if (LS_num == 3) {
		SF_Matrix[2][x]++;
		SFC_Matrix[2][x]++;
		int count = 0;
		cout << "[LearningStage] M3" << endl;
		vector<int>::iterator c_iter;
		vector<Trip>::iterator r_iter;
		int trip_ind; int min_num = 999999;
		for (r_iter = chrome.trips.begin(); r_iter != chrome.trips.end(); ++r_iter)
		{
			if (min_num > r_iter->capacity)
			{
				min_num = r_iter->capacity;
				trip_ind = r_iter - chrome.trips.begin();
			}
		}
		int com = 0;    //定义执行次数，最多判断两个行程即一回程
		if (opt_lsone(chrome, trip_ind, com, 0)) {
			for (vector<Car>::iterator car_iter = chrome.cars.begin(); car_iter != chrome.cars.end(); )
			{
				if (car_iter->car_trips.size() == 0)
					car_iter = chrome.cars.erase(car_iter);
				else
					++car_iter;
			}
			update_ydmd(chrome);
			compute_f(chrome);
			update_EP(chrome, EP_study);
		}
		update_Matrix(chrome, init_data, 2);
	}
	else if (LS_num == 4) {
		SF_Matrix[3][x]++;
		SFC_Matrix[3][x]++;
		vector<int>orders;
		double now_time = 0;
		orders.clear();
		int r_ind = 0;
		r_ind = rand() % chrome.trips.size();
		cout << "[LearningStage] M4" << endl;
		int cust_ind = rand() % (chrome.trips[r_ind].trip_orders.size());  //考虑车库-2，而本模型暂不考虑车库
		int cust = chrome.trips[r_ind].trip_orders[cust_ind];
		orders.push_back(cust);
		chrome.trips[r_ind].trip_orders.erase(chrome.trips[r_ind].trip_orders.begin() + cust_ind);
		update_onetrip(chrome, chrome.trips[r_ind]);
		update_global_trip(chrome);
		if (chrome.trips[r_ind].trip_orders.size() == 0)
		{
			update_trip12(chrome, chrome.trips[r_ind]);
			chrome.trips.erase(chrome.trips.begin() + r_ind);
			update_global_trip(chrome);
		}
		else
		{
			//检查删除chrome.trips[r_ind]路径的cust_ind后，该路径是否有效  路径本可行，删去应该也可行
			if (check_feasible_trip(chrome.trips[r_ind]))
			{
				compute_trip_time(chrome.trips[r_ind]);
				compute_trip_distance(chrome.trips[r_ind]);
			}
		}
		//将删除容器中的客户点插入到解中产生新解
		for (int iter = 0; iter < orders.size(); ++iter)
			insert_order(chrome, lamda, orders[iter], now_time);
		//更新trip[]:直接全更新
		vector<Trip>::iterator t_iter;
		for (t_iter = chrome.trips.begin(); t_iter != chrome.trips.end(); ++t_iter) {
			trip[t_iter->trip_id] = *t_iter;
		}
		//更新阶段
		update_ydmd(chrome);
		compute_f(chrome);
		update_EP(chrome, EP_study);
		update_Matrix(chrome, init_data, 3);
		for (vector<Car>::iterator car_iter = chrome.cars.begin(); car_iter != chrome.cars.end(); )
		{
			if (car_iter->car_trips.size() == 0)
				car_iter = chrome.cars.erase(car_iter);
			else
				++car_iter;
		}
	}
	else if (LS_num == 5) {
		SF_Matrix[4][x]++;
		SFC_Matrix[4][x]++;
		vector<int>orders;
		double now_time = 0;
		orders.clear();
		int r_ind = 0;
		r_ind = rand() % chrome.trips.size();
		cout << "[LearningStage] M5" << endl;
		int count = rand() % (chrome.trips[r_ind].trip_orders.size());
		for (int i = 0; i < count; ++i)
		{
			int cust_ind = rand() % (chrome.trips[r_ind].trip_orders.size());
			orders.push_back(chrome.trips[r_ind].trip_orders[cust_ind]);
			chrome.trips[r_ind].trip_orders.erase(chrome.trips[r_ind].trip_orders.begin() + cust_ind);
			update_onetrip(chrome, chrome.trips[r_ind]);
			update_global_trip(chrome);
		}
		if (chrome.trips[r_ind].trip_orders.size() == 0)
		{
			update_trip12(chrome, chrome.trips[r_ind]);
			chrome.trips.erase(chrome.trips.begin() + r_ind);
			update_global_trip(chrome);
		}
		else
		{
			if (check_feasible_trip(chrome.trips[r_ind]))
			{
				compute_trip_time(chrome.trips[r_ind]);
				compute_trip_distance(chrome.trips[r_ind]);
			}
		}
		//将删除容器中的客户点插入到解中产生新解
		for (int iter = 0; iter < orders.size(); ++iter)
			insert_order(chrome, lamda, orders[iter], now_time);
		//更新trip[]:直接全更新
		vector<Trip>::iterator t_iter;
		for (t_iter = chrome.trips.begin(); t_iter != chrome.trips.end(); ++t_iter) {
			trip[t_iter->trip_id] = *t_iter;
		}
		//更新阶段
		update_ydmd(chrome);
		compute_f(chrome);
		update_EP(chrome, EP_study);
		update_Matrix(chrome, init_data, 4);
		for (vector<Car>::iterator car_iter = chrome.cars.begin(); car_iter != chrome.cars.end(); )
		{
			if (car_iter->car_trips.size() == 0)
				car_iter = chrome.cars.erase(car_iter);
			else
				++car_iter;
		}
	}
	else if (LS_num == 6) {
		SF_Matrix[5][x]++;
		SFC_Matrix[5][x]++;
		double now_time = 0;
		int r_ind = 0;
		r_ind = rand() % chrome.trips.size();
		cout << "[LearningStage] M6" << endl;
		two_opt(chrome, lamda, r_ind, now_time);
		update_global_trip(chrome);
		//更新阶段
		update_ydmd(chrome);
		compute_f(chrome);
		update_EP(chrome, EP_study);
		update_Matrix(chrome, init_data, 5);
		for (vector<Car>::iterator car_iter = chrome.cars.begin(); car_iter != chrome.cars.end(); )
		{
			if (car_iter->car_trips.size() == 0)
				car_iter = chrome.cars.erase(car_iter);
			else
				++car_iter;
		}
	}
	else if (LS_num == 7) {
		SF_Matrix[6][x]++;
		SFC_Matrix[6][x]++;
		cout << "[LearningStage] M7" << endl;
		double now_time = 0;
		int r_ind = 0;
		r_ind = rand() % chrome.trips.size();
		ls_exchange_order(chrome, lamda, r_ind, now_time);
		update_ydmd(chrome);
		compute_f(chrome);
		update_EP(chrome, EP_study);
		update_Matrix(chrome, init_data, 6);
		update_global_trip(chrome);
	}
	else {
		SF_Matrix[7][x]++;
		SFC_Matrix[7][x]++;
		cout << "[LearningStage] M8" << endl;
		int count = 0;
		int V = chrome.cars.size();
		int r1 = rand() % V;
		int obj = rand() % 2 + 1;
		ls_exchange_mutitrip(chrome, r1, obj, count);
		update_ydmd(chrome);
		compute_f(chrome);
		update_Matrix(chrome, init_data, 7);
		update_EP(chrome, EP_study);
	}
}

// Wrapper for the learning-based preprocessing stage controlled by
// EXPERIMENT_ENABLE_PREPROCESSING and EXPERIMENT_PREPROCESS_SECONDS.
void preProcessing(vector<Chromosome>& EP_study)
{
	//初始化学习阶段，只更新学习矩阵数据
	clock_t pro_begin, pro_end;
	pro_begin = clock();
	init_pop(EP_study);
	Chromosome chrome;
	cout << "[Preprocessing] random local search" << endl;
	while (true) {
		clean_chromosome(chrome);
		int ind = rand() % EP_study.size();
		chrome = EP_study[ind];
		learning_Stage(chrome, EP_study);
		pro_end = clock();
		if ((double)((pro_end - pro_begin) / CLOCKS_PER_SEC) > EXPERIMENT_PREPROCESS_SECONDS)
			break;
	}
}

// Convert operator rewards and selection frequencies into LS quality values.
// EXPERIMENT_ALPHA smooths the current quality with the previous-generation
// quality, matching the adaptive operator-selection mechanism.
void update_Quality()
{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 4; j++) {
			//cout << "1-> " << LS_Matrix[i][j] << " - " << UF_Matrix[i][j] << " - " << SF_Matrix[i][j] << " - " << LSC_Matrix[i][j] << " - " << UFC_Matrix[i][j] << " - " << SFC_Matrix[i][j] << endl;
			if (SF_Matrix[i][j] == 0)
				LS_Matrix[i][j] = 0;
			else
				LS_Matrix[i][j] = UF_Matrix[i][j] / SF_Matrix[i][j];
			//cout << "2-> " << LS_Matrix[i][j] << " - " << UF_Matrix[i][j] << " - " << SF_Matrix[i][j] << endl;
			//cout << "3-> " << LSC_Matrix[i][j] << " - " << UFC_Matrix[i][j] << " - " << SFC_Matrix[i][j] << endl;
			LS_Matrix[i][j] = ((1 - EXPERIMENT_ALPHA) * LSC_Matrix[i][j]) + EXPERIMENT_ALPHA * LS_Matrix[i][j];
			//cout << "4-> " << LS_Matrix[i][j] << " - " << ((1 - EXPERIMENT_ALPHA) * LSC_Matrix[i][j]) << " - " << EXPERIMENT_ALPHA * LS_Matrix[i][j] << endl;
		}
	}
}

//记录上一代结束时更新，两个版本:①学习阶段后
// Store quality estimates after preprocessing.
void update_Last_End1()
{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 4; j++) {
			if (SF_Matrix[i][j] == 0)
				LSC_Matrix[i][j] = 0;
			else
				LSC_Matrix[i][j] = UF_Matrix[i][j] / SF_Matrix[i][j];
		}
	}
}

//记录上一代结束时更新，两个版本:②版本自适应阶段
// Store quality estimates after each evolutionary generation.
void update_Last_End2()
{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 4; j++) {
			if (SFC_Matrix[i][j] == 0)
				LSC_Matrix[i][j] = 0;
			else
				LSC_Matrix[i][j] = UFC_Matrix[i][j] / SFC_Matrix[i][j];
		}
	}
}

bool is_operator_enabled(int index)
{
	return index >= 0 && index < Kk && EXPERIMENT_MUTATION_OPERATOR_ENABLED[index];
}

int enabled_operator_count()
{
	int count = 0;
	for (int i = 0; i < Kk; ++i) {
		if (is_operator_enabled(i)) count++;
	}
	return count;
}

int first_enabled_operator()
{
	for (int i = 0; i < Kk; ++i) {
		if (is_operator_enabled(i)) return i + 1;
	}
	return 1;
}

int random_enabled_operator()
{
	int count = enabled_operator_count();
	if (count <= 0) return 1;
	int target = rand() % count;
	for (int i = 0; i < Kk; ++i) {
		if (!is_operator_enabled(i)) continue;
		if (target == 0) return i + 1;
		target--;
	}
	return first_enabled_operator();
}

double compute_enabled_object_nums(double Matrix[8][4], int object)
{
	double total = 0.0;
	for (int i = 0; i < Kk; ++i) {
		if (is_operator_enabled(i)) total += Matrix[i][object];
	}
	return total;
}

// AOS variant used in the paper configuration: Probability Matching (PM).
// Better-quality operators receive larger probabilities, while pmin keeps each
// enabled operator available for exploration.
void compute_PM(int object)
{
	double total = compute_enabled_object_nums(LS_Matrix, object);
	int enabled_count = enabled_operator_count();
	if (total <= EPS) {
		for (int i = 0; i < 8; i++) {
			PM_Matrix[i][object] = is_operator_enabled(i) && enabled_count > 0 ? 1.0 / enabled_count : 0.0;
		}
		return;
	}
	for (int i = 0; i < 8; i++) {
		if (!is_operator_enabled(i)) {
			PM_Matrix[i][object] = 0.0;
			continue;
		}
		double q = LS_Matrix[i][object];
		if (q < 0) q = 0;
		PM_Matrix[i][object] = EXPERIMENT_PM_PMIN + (1 - enabled_count * EXPERIMENT_PM_PMIN) * (q / total);
	}
}

// AOS variant for ablation: Adaptive Pursuit (AP).
// The current best operator is pulled toward pmax and the others toward pmin.
void compute_AP(int object)
{
	double temp = -1.0;
	int winner = first_enabled_operator() - 1;
	for (int i = 0; i < 8; i++) {
		if (!is_operator_enabled(i)) continue;
		if (LS_Matrix[i][object] > temp) {
			temp = LS_Matrix[i][object];
			winner = i;
		}
	}
	double current_total = compute_enabled_object_nums(AP_Matrix, object);
	int enabled_count = enabled_operator_count();
	if (current_total <= EPS) {
		for (int i = 0; i < 8; i++) {
			AP_Matrix[i][object] = is_operator_enabled(i) && enabled_count > 0 ? 1.0 / enabled_count : 0.0;
		}
	}
	for (int i = 0; i < 8; i++) {
		if (!is_operator_enabled(i)) {
			AP_Matrix[i][object] = 0.0;
			continue;
		}
		if (i == winner) {
			AP_Matrix[i][object] = AP_Matrix[i][object] + EXPERIMENT_AP_BETA * (EXPERIMENT_AP_PMAX - AP_Matrix[i][object]);
		}
		else
			AP_Matrix[i][object] = AP_Matrix[i][object] + EXPERIMENT_AP_BETA * (EXPERIMENT_PM_PMIN - AP_Matrix[i][object]);
	}
}

// AOS variant for ablation: Multi-Armed Bandit (MAB/UCB style).
// The score combines exploitation (LS quality) and exploration (selection count).
int compute_MAB(int object)
{
	double total = 0.0;
	for (int i = 0; i < 8; i++) {
		if (!is_operator_enabled(i)) continue;
		total += SF_Matrix[i][object];
		if (SF_Matrix[i][object] <= 0) return i + 1;
	}
	if (total <= 1.0) return random_enabled_operator();

	double best_value = -1.0e100;
	int winner = first_enabled_operator();
	for (int i = 0; i < 8; i++) {
		if (!is_operator_enabled(i)) continue;
		double exploration = EXPERIMENT_MAB_C * sqrt((2.0 * log(total)) / SF_Matrix[i][object]);
		MAB_Matrix[i][object] = LS_Matrix[i][object] + exploration;
		if (MAB_Matrix[i][object] > best_value) {
			best_value = MAB_Matrix[i][object];
			winner = i + 1;
		}
	}
	return winner;
}

int get_Probability(double Matrix[8][4], int object)
{
	double total = compute_enabled_object_nums(Matrix, object);
	if (total <= EPS) return random_enabled_operator();

	double p = rand() % 10000 / 10000.0;
	double cumulative = 0.0;
	for (int i = 0; i < 8; i++) {
		if (!is_operator_enabled(i)) continue;
		cumulative += Matrix[i][object] / total;
		if (p <= cumulative) return i + 1;
	}
	return first_enabled_operator();
}

const char* aos_mode_name()
{
	if (EXPERIMENT_AOS_MODE == AOS_AP) return "AP";
	if (EXPERIMENT_AOS_MODE == AOS_MAB) return "MAB";
	return "PM";
}

// Central AOS operator-selection entry point. It selects one of the enabled
// M1-M8 mutation/local-search operators for the current target objective.
int select_operator_by_aos(int object)
{
	update_Quality();
	int selected = 1;
	if (EXPERIMENT_AOS_MODE == AOS_AP) {
		compute_AP(object);
		selected = get_Probability(AP_Matrix, object);
	}
	else if (EXPERIMENT_AOS_MODE == AOS_MAB) {
		selected = compute_MAB(object);
	}
	else {
		compute_PM(object);
		selected = get_Probability(PM_Matrix, object);
	}
	if (EXPERIMENT_ENABLE_LOG) {
		cout << "[AOS] objective=" << object + 1 << " mode=" << aos_mode_name()
			 << " selected_operator=M" << selected << endl;
	}
	return selected;
}

// Paper mechanism: dynamic local-search mutation pool.
// The selected M1-M8 operator modifies the route-sharing solution at different
// granularities, then updates the archive and AOS reward matrices.
void Dynamic_LS(Chromosome& chrome, int r_lamda[], int object, vector<Chromosome>& EPE)
{
	//初始阶段，计算局部搜索前chrome的各目标值
	update_ydmd(chrome);
	compute_f(chrome);
	for (int i = 0; i < FUNC_NUM; i++) {
		init_data[i] = chrome.f[i];
	}
	//开始进入动态局部搜索池阶段
	int LS_num = select_operator_by_aos(object);
	if (LS_num == 1) {
		SF_Matrix[0][object]++;
		SFC_Matrix[0][object]++;
		cout << "[Stage2LocalSearch] M1" << endl;
		vector<int>::iterator c_iter;
		vector<Car>::iterator r_iter;
		int car_ind; int min_num = 999999;
		for (r_iter = chrome.cars.begin(); r_iter != chrome.cars.end(); ++r_iter)
		{
			if (min_num > r_iter->car_trips.size())
			{
				min_num = r_iter->car_trips.size();
				car_ind = r_iter - chrome.cars.begin();
			}
		}
		if (opt_lstwo(chrome, car_ind))
		{
			update_ydmd(chrome);
			compute_f(chrome);
			update_Matrix(chrome, init_data, 0);
			update_EP(chrome, EPE);
		}
	}
	else if (LS_num == 2) {
		SF_Matrix[1][object]++;
		SFC_Matrix[1][object]++;
		int count = 0;
		cout << "[Stage2LocalSearch] M2" << endl;
		int V = chrome.cars.size();
		int r1 = rand() % V;
		int obj = rand() % 2 + 1;
		ls_exchange_trip(chrome, r1, obj, count);
		update_ydmd(chrome);
		compute_f(chrome);
		update_Matrix(chrome, init_data, 1);
		update_EP(chrome, EPE);
	}
	else if (LS_num == 3) {
		SF_Matrix[2][object]++;
		SFC_Matrix[2][object]++;
		int count = 0;
		cout << "[LocalSearch] M3" << endl;
		vector<int>::iterator c_iter;
		vector<Trip>::iterator r_iter;
		int trip_ind; int min_num = 999999;
		for (r_iter = chrome.trips.begin(); r_iter != chrome.trips.end(); ++r_iter)
		{
			if (min_num > r_iter->capacity)
			{
				min_num = r_iter->capacity;
				trip_ind = r_iter - chrome.trips.begin();
			}
		}
		int com = 0;    //定义执行次数，最多判断两个行程即一回程
		if (opt_lsone(chrome, trip_ind, com, 0)) {
			for (vector<Car>::iterator car_iter = chrome.cars.begin(); car_iter != chrome.cars.end(); )
			{
				if (car_iter->car_trips.size() == 0)
					car_iter = chrome.cars.erase(car_iter);
				else
					++car_iter;
			}
			update_ydmd(chrome);
			compute_f(chrome);
			update_Matrix(chrome, init_data, 2);
			update_EP(chrome, EPE);
		}
	}
	else if (LS_num == 4) {
		SF_Matrix[3][object]++;
		SFC_Matrix[3][object]++;
		vector<int>orders;
		double now_time = 0;
		orders.clear();
		int r_ind = 0;
		r_ind = rand() % chrome.trips.size();
		cout << "[Stage2LocalSearch] M4" << endl;
		int cust_ind = rand() % (chrome.trips[r_ind].trip_orders.size());  //考虑车库-2，而本模型暂不考虑车库
		int cust = chrome.trips[r_ind].trip_orders[cust_ind];
		orders.push_back(cust);
		chrome.trips[r_ind].trip_orders.erase(chrome.trips[r_ind].trip_orders.begin() + cust_ind);
		update_onetrip(chrome, chrome.trips[r_ind]);
		update_global_trip(chrome);
		if (chrome.trips[r_ind].trip_orders.size() == 0)
		{
			update_trip12(chrome, chrome.trips[r_ind]);
			chrome.trips.erase(chrome.trips.begin() + r_ind);
			update_global_trip(chrome);
		}
		else
		{
			//检查删除chrome.trips[r_ind]路径的cust_ind后，该路径是否有效  路径本可行，删去应该也可行
			if (check_feasible_trip(chrome.trips[r_ind]))
			{
				compute_trip_time(chrome.trips[r_ind]);
				compute_trip_distance(chrome.trips[r_ind]);
			}
		}
		//将删除容器中的客户点插入到解中产生新解
		for (int iter = 0; iter < orders.size(); ++iter)
			insert_order(chrome, r_lamda, orders[iter], now_time);
		//更新trip[]:直接全更新
		vector<Trip>::iterator t_iter;
		for (t_iter = chrome.trips.begin(); t_iter != chrome.trips.end(); ++t_iter) {
			trip[t_iter->trip_id] = *t_iter;
		}
		//更新阶段
		update_ydmd(chrome);
		compute_f(chrome);
		update_Matrix(chrome, init_data, 3);
		update_EP(chrome, EPE);
		for (vector<Car>::iterator car_iter = chrome.cars.begin(); car_iter != chrome.cars.end(); )
		{
			if (car_iter->car_trips.size() == 0)
				car_iter = chrome.cars.erase(car_iter);
			else
				++car_iter;
		}
	}
	else if (LS_num == 5) {
		SF_Matrix[4][object]++;
		SFC_Matrix[4][object]++;
		vector<int>orders;
		double now_time = 0;
		orders.clear();
		int r_ind = 0;
		r_ind = rand() % chrome.trips.size();
		cout << "[Stage2LocalSearch] M5" << endl;
		int count = rand() % (chrome.trips[r_ind].trip_orders.size());
		for (int i = 0; i < count; ++i)
		{
			int cust_ind = rand() % (chrome.trips[r_ind].trip_orders.size());
			orders.push_back(chrome.trips[r_ind].trip_orders[cust_ind]);
			chrome.trips[r_ind].trip_orders.erase(chrome.trips[r_ind].trip_orders.begin() + cust_ind);
			update_onetrip(chrome, chrome.trips[r_ind]);
			update_global_trip(chrome);
		}
		if (chrome.trips[r_ind].trip_orders.size() == 0)
		{
			update_trip12(chrome, chrome.trips[r_ind]);
			chrome.trips.erase(chrome.trips.begin() + r_ind);
			update_global_trip(chrome);
		}
		else
		{
			if (check_feasible_trip(chrome.trips[r_ind]))
			{
				compute_trip_time(chrome.trips[r_ind]);
				compute_trip_distance(chrome.trips[r_ind]);
			}
		}
		//将删除容器中的客户点插入到解中产生新解
		for (int iter = 0; iter < orders.size(); ++iter)
			insert_order(chrome, r_lamda, orders[iter], now_time);
		//更新trip[]:直接全更新
		vector<Trip>::iterator t_iter;
		for (t_iter = chrome.trips.begin(); t_iter != chrome.trips.end(); ++t_iter) {
			trip[t_iter->trip_id] = *t_iter;
		}
		//更新阶段
		update_ydmd(chrome);
		compute_f(chrome);
		update_Matrix(chrome, init_data, 4);
		update_EP(chrome, EPE);
		for (vector<Car>::iterator car_iter = chrome.cars.begin(); car_iter != chrome.cars.end(); )
		{
			if (car_iter->car_trips.size() == 0)
				car_iter = chrome.cars.erase(car_iter);
			else
				++car_iter;
		}
	}
	else if (LS_num == 6) {
		SF_Matrix[5][object]++;
		SFC_Matrix[5][object]++;
		double now_time = 0;
		int r_ind = 0;
		r_ind = rand() % chrome.trips.size();
		cout << "[Stage2LocalSearch] M6" << endl;
		two_opt(chrome, r_lamda, r_ind, now_time);
		update_global_trip(chrome);
		//更新阶段
		update_ydmd(chrome);
		compute_f(chrome);
		update_Matrix(chrome, init_data, 5);
		update_EP(chrome, EPE);
		for (vector<Car>::iterator car_iter = chrome.cars.begin(); car_iter != chrome.cars.end(); )
		{
			if (car_iter->car_trips.size() == 0)
				car_iter = chrome.cars.erase(car_iter);
			else
				++car_iter;
		}
	}
	else if (LS_num == 7) {
		SF_Matrix[6][object]++;
		SFC_Matrix[6][object]++;
		cout << "[LocalSearch] M7" << endl;
		double now_time = 0;
		int r_ind = 0;
		r_ind = rand() % chrome.trips.size();
		ls_exchange_order(chrome, r_lamda, r_ind, now_time);
		update_ydmd(chrome);
		compute_f(chrome);
		update_Matrix(chrome, init_data, 6);
		update_EP(chrome, EPE);
		update_global_trip(chrome);
	}
	else {
		SF_Matrix[7][object]++;
		SFC_Matrix[7][object]++;
		cout << "[LocalSearch] M8" << endl;
		int count = 0;
		int V = chrome.cars.size();
		int r1 = rand() % V;
		int obj = rand() % 2 + 1;
		ls_exchange_mutitrip(chrome, r1, obj, count);
		update_ydmd(chrome);
		compute_f(chrome);
		update_Matrix(chrome, init_data, 7);
		update_EP(chrome, EPE);
	}
}

// Target-objective dispatcher for dynamic MDLS. Mutation_Greedy sets exactly
// one r_lamda entry according to the IoP/SPO-selected objective, then this
// function routes the solution into the AOS local-search pool.
void Dynamic_mdls(Chromosome& chrome, int r_lamda[], int index, vector<Chromosome>& EPx)
{
	if (r_lamda[0] == 1)
	{
		cout << "[DynamicSearch] objective 1" << endl;
		index = 0;
		Dynamic_LS(chrome, r_lamda, index, EPx);
	}
	else if (r_lamda[1] == 1)
	{
		cout << "[DynamicSearch] objective 2" << endl;
		index = 1;
		Dynamic_LS(chrome, r_lamda, index, EPx);
	}
	else if (r_lamda[2] == 1)
	{
		cout << "[DynamicSearch] objective 3" << endl;
		index = 2;
		Dynamic_LS(chrome, r_lamda, index, EPx);
	}
	else
	{
		cout << "[DynamicSearch] objective 4" << endl;
		index = 3;
		Dynamic_LS(chrome, r_lamda, index, EPx);
	}
}

Individual::Individual() {}

Individual::Individual(Chromosome chrome, int geneSize)
{
	clean_chromosome(chrome);
	init_pop(chrome);
	for (int i = 0; i < geneSize; i++)
	{
		genes.push_back(chrome);
	}
}

void Individual::evaluation()
{
	objectiveSet.resize(4);
	for (int i = 0; i < genes.size(); i++)
	{
		objectiveSet[0] = genes[i].f[0];
		objectiveSet[1] = genes[i].f[1];
		objectiveSet[2] = genes[i].f[2];
		objectiveSet[3] = genes[i].f[3];
	}
}

void Individual::maxevaluation()
{
	objectiveSet.resize(4);
	for (int i = 0; i < genes.size(); i++)
	{
		objectiveSet[0] = genes[i].f[0] * 2;
		objectiveSet[1] = genes[i].f[1] * 5;
		objectiveSet[2] = genes[i].f[2] * 5;
		objectiveSet[3] = genes[i].f[3] * 2;
	}
}

void Individual::minevaluation()
{
	objectiveSet.resize(4);
	for (int i = 0; i < genes.size(); i++)
	{
		objectiveSet[0] = genes[i].f[0] / 2;
		objectiveSet[1] = genes[i].f[1] / 5;
		objectiveSet[2] = genes[i].f[2] / 5;
		objectiveSet[3] = genes[i].f[3] / 2;
	}
}

bool Individual::dominate(Individual q)
{
	for (int i = 0; i < objectiveSet.size(); i++)
	{
		if (objectiveSet[i] > q.objectiveSet[i])
		{
			return false;
		}
	}
	return true;
}

void GeneticAlgorithm::wheelSelection(Population& p)
{

}

void GeneticAlgorithm::crossover(Population& p)
{
	//1.对群体中的个体进行随机配对
	int rand_1 = rand() % p.individualSet.size();
	int rand_2 = rand() % p.individualSet.size();
	if (rand_1 == rand_2) return;
	//2.在配对个体中，随机设定交叉处，使配对个体交换部分信息[随机选择交叉个体的car]
	int rand_c1 = rand() % p.individualSet[rand_1].genes[0].cars.size();
	int rand_c2 = rand() % p.individualSet[rand_2].genes[0].cars.size();
	//3.产生两个需要交叉的车辆，在对方的父代中消去交叉部分(车辆)中的元素值(车辆),然后将交叉部分(车辆)补在已经删去部分元素的对方父代之后。
	//3.1 创建Trip[]记录chrome.trips
	Trip t1[MAX1], t2[MAX1];
	vector<Trip>::iterator trip_one, trip_two;
	for (trip_one = p.individualSet[rand_1].genes[0].trips.begin(); trip_one != p.individualSet[rand_1].genes[0].trips.end(); trip_one++) {
		t1[trip_one->trip_id] = *trip_one;
	}
	for (trip_two = p.individualSet[rand_2].genes[0].trips.begin(); trip_two != p.individualSet[rand_2].genes[0].trips.end(); trip_two++) {
		t2[trip_two->trip_id] = *trip_two;
	}
	//3.2 删去交叉部分订单数，若行程中的订单数或车辆中的行程数为空，注意删除
	//①记录要删除的交叉部分订单数
	vector<int> car1_nums, car2_nums;
	vector<int>::iterator iter1, iter2;
	vector<int>::iterator iter11, iter22;
	for (iter1 = p.individualSet[rand_1].genes[0].cars[rand_c1].car_trips.begin(); iter1 != p.individualSet[rand_1].genes[0].cars[rand_c1].car_trips.end(); iter1++) {
		for (iter11 = t1[*iter1].trip_orders.begin(); iter11 != t1[*iter1].trip_orders.end(); iter11++)
			car1_nums.push_back(*iter11);
	}
	for (iter2 = p.individualSet[rand_2].genes[0].cars[rand_c2].car_trips.begin(); iter2 != p.individualSet[rand_2].genes[0].cars[rand_c2].car_trips.end(); iter2++) {
		for (iter22 = t2[*iter2].trip_orders.begin(); iter22 != t2[*iter2].trip_orders.end(); iter22++)
			car2_nums.push_back(*iter22);
	}
	if (compare_is_equal(car1_nums, car2_nums))
		return;
	//②开始清除交叉部分订单数，并检查空行程
	vector<Trip>::iterator it1, it2;
	//生成临时变量，便于插入，此外，生成专属t[]，以免重复删除： 65 89 93/ 85 93 70
	Car temp_car1 = p.individualSet[rand_1].genes[0].cars[rand_c1];
	Car temp_car2 = p.individualSet[rand_2].genes[0].cars[rand_c2];
	//这些id用来记录，后续使用
	vector<int> id1, id2;
	vector<Trip> temo1, temo2;
	for (int i1 = 0; i1 < temp_car1.car_trips.size(); i1++) {
		id1.push_back(temp_car1.car_trips[i1]);
		temo1.push_back(t1[temp_car1.car_trips[i1]]);
	}
	for (int i2 = 0; i2 < temp_car2.car_trips.size(); i2++) {
		id2.push_back(temp_car2.car_trips[i2]);
		temo2.push_back(t2[temp_car2.car_trips[i2]]);
	}
	for (it2 = p.individualSet[rand_2].genes[0].trips.begin(); it2 != p.individualSet[rand_2].genes[0].trips.end(); it2++) {
		for (iter2 = it2->trip_orders.begin(); iter2 != it2->trip_orders.end(); ) {
			if (find_is_same(*iter2, car1_nums)) {
				int x2 = it2->trip_id;
				int temp1 = iter2 - it2->trip_orders.begin();
				iter11 = t2[x2].trip_orders.begin();
				while (temp1--) {
					iter11++;
				}
				t2[x2].trip_orders.erase(iter11);
				iter2 = it2->trip_orders.erase(iter2);
			}
			else
				iter2++;
		}
	}
	for (it1 = p.individualSet[rand_1].genes[0].trips.begin(); it1 != p.individualSet[rand_1].genes[0].trips.end(); it1++) {
		for (iter1 = it1->trip_orders.begin(); iter1 != it1->trip_orders.end(); ) {
			if (find_is_same(*iter1, car2_nums)) {
				int x1 = it1->trip_id;
				int temp2 = iter1 - it1->trip_orders.begin();
				iter22 = t1[x1].trip_orders.begin();
				while (temp2--) {
					iter22++;
				}
				t1[x1].trip_orders.erase(iter22);
				iter1 = it1->trip_orders.erase(iter1);
			}
			else
				iter1++;
		}
	}
	//③将交叉部分(车辆)补在已经删去部分元素的对方父代之后
	int s1 = findMax_car_id(p.individualSet[rand_2].genes[0].cars);
	int s2 = findMax_car_id(p.individualSet[rand_1].genes[0].cars);
	//改动：不能直接car，push进去，将trip也同步，t[]记录全局，chrome.trips记录局部。
	p.individualSet[rand_1].genes[0].cars[rand_c1].car_id = s2 + 1;
	p.individualSet[rand_2].genes[0].cars[rand_c2].car_id = s1 + 1;
	//保存trip，做相应trip_id更改
	vector<Trip> temp_trips1, temp_trips2;
	int a = -1, b = -1;
	int num_one = findMax_trip_id(p.individualSet[rand_2].genes[0].trips) + 1;
	int num_two = findMax_trip_id(p.individualSet[rand_1].genes[0].trips) + 1;
	for (iter1 = temp_car1.car_trips.begin(); iter1 != temp_car1.car_trips.end(); iter1++) {
		a = a + 1;
		if (check_repeat_delete(*iter1, id1) >= 0) {
			temp_trips1.push_back(temo1[check_repeat_delete(*iter1, id1)]);
		}
		else
			temp_trips1.push_back(t1[*iter1]);
		temp_trips1[a].trip_id = num_one;
		*iter1 = num_one;
		num_one++;
	}
	for (iter2 = temp_car2.car_trips.begin(); iter2 != temp_car2.car_trips.end(); iter2++) {
		b = b + 1;
		if (check_repeat_delete(*iter2, id2) >= 0) {
			temp_trips2.push_back(temo2[check_repeat_delete(*iter2, id2)]);
		}
		else
			temp_trips2.push_back(t2[*iter2]);
		temp_trips2[b].trip_id = num_two;
		*iter2 = num_two;
		num_two++;
	}
	//对两个解的chrometrips和t[]进行同步
	vector<Trip>::iterator t_iter1, t_iter2;
	for (t_iter1 = temp_trips1.begin(); t_iter1 != temp_trips1.end(); t_iter1++) {
		p.individualSet[rand_2].genes[0].trips.push_back(*t_iter1);
		if (t_iter1->trip_set == 1) {
			p.individualSet[rand_2].genes[0].trip1.push_back(*t_iter1);
		}
		else {
			p.individualSet[rand_2].genes[0].trip2.push_back(*t_iter1);
		}
		t2[t_iter1->trip_id] = *t_iter1;
	}
	for (t_iter2 = temp_trips2.begin(); t_iter2 != temp_trips2.end(); t_iter2++) {
		p.individualSet[rand_1].genes[0].trips.push_back(*t_iter2);
		if (t_iter2->trip_set == 1) {
			p.individualSet[rand_1].genes[0].trip1.push_back(*t_iter2);
		}
		else {
			p.individualSet[rand_1].genes[0].trip2.push_back(*t_iter2);
		}
		t1[t_iter2->trip_id] = *t_iter2;
	}
	p.individualSet[rand_1].genes[0].cars.push_back(temp_car2);
	p.individualSet[rand_2].genes[0].cars.push_back(temp_car1);
	//自己的car需要删除
	vector<Car>::iterator c_iter1, c_iter2;
	/*int z1 = rand_c1, z2 = rand_c2;
	c_iter1 = p.individualSet[rand_1].genes[0].cars.begin();
	c_iter2 = p.individualSet[rand_2].genes[0].cars.begin();
	while (z1--)
		c_iter1++;
	while (z2--)
		c_iter2++;
	p.individualSet[rand_1].genes[0].cars.erase(c_iter1);
	p.individualSet[rand_2].genes[0].cars.erase(c_iter2);*/

	//④清除订单数为0的行程和行程数为0的车辆
	for (t_iter1 = p.individualSet[rand_1].genes[0].trips.begin(); t_iter1 != p.individualSet[rand_1].genes[0].trips.end(); ) {
		if (t_iter1->trip_orders.size() == 0) {
			int ind1 = t_iter1 - p.individualSet[rand_1].genes[0].trips.begin();
			update_trip12(p.individualSet[rand_1].genes[0], p.individualSet[rand_1].genes[0].trips[ind1]);
			t_iter1 = p.individualSet[rand_1].genes[0].trips.erase(t_iter1);
		}
		else
			t_iter1++;
	}
	for (t_iter2 = p.individualSet[rand_2].genes[0].trips.begin(); t_iter2 != p.individualSet[rand_2].genes[0].trips.end(); ) {
		if (t_iter2->trip_orders.size() == 0) {
			int ind2 = t_iter2 - p.individualSet[rand_2].genes[0].trips.begin();
			update_trip12(p.individualSet[rand_2].genes[0], p.individualSet[rand_2].genes[0].trips[ind2]);
			t_iter2 = p.individualSet[rand_2].genes[0].trips.erase(t_iter2);
		}
		else
			t_iter2++;
	}
	for (c_iter1 = p.individualSet[rand_1].genes[0].cars.begin(); c_iter1 != p.individualSet[rand_1].genes[0].cars.end(); ) {
		if (c_iter1->car_trips.size() == 0) {
			c_iter1 = p.individualSet[rand_1].genes[0].cars.erase(c_iter1);
		}
		else
			c_iter1++;
	}
	for (c_iter2 = p.individualSet[rand_2].genes[0].cars.begin(); c_iter2 != p.individualSet[rand_2].genes[0].cars.end(); ) {
		if (c_iter2->car_trips.size() == 0) {
			c_iter2 = p.individualSet[rand_2].genes[0].cars.erase(c_iter2);
		}
		else
			c_iter2++;
	}
	//4.利用罚函数对不可行的解进行目标函数×多倍的处理
	//4.1 对每辆车进行检查行程设置合不合理-是否还有回程
	int change1 = 0;
	int change2 = 0;
	for (c_iter1 = p.individualSet[rand_1].genes[0].cars.begin(); c_iter1 != p.individualSet[rand_1].genes[0].cars.end(); c_iter1++) {
		for (iter1 = c_iter1->car_trips.begin(); iter1 != c_iter1->car_trips.end(); iter1++) {
			int truth1 = find_corresponding(p.individualSet[rand_1].genes[0].trips, *iter1);
			if ((iter1 - c_iter1->car_trips.begin()) % 2 != p.individualSet[rand_1].genes[0].trips[truth1].trip_set - 1) {
				//目标函数加倍
				p.individualSet[rand_1].objectiveSet[0] *= 10;
				p.individualSet[rand_1].objectiveSet[1] *= 10;
				p.individualSet[rand_1].objectiveSet[2] *= 10;
				p.individualSet[rand_1].objectiveSet[3] *= 10;
				change1 = 1;
				break;
			}
		}
		if (change1 == 1) break;
	}
	for (c_iter2 = p.individualSet[rand_2].genes[0].cars.begin(); c_iter2 != p.individualSet[rand_2].genes[0].cars.end(); c_iter2++) {
		for (iter2 = c_iter2->car_trips.begin(); iter2 != c_iter2->car_trips.end(); iter2++) {
			int truth2 = find_corresponding(p.individualSet[rand_2].genes[0].trips, *iter2);
			if (((iter2 - c_iter2->car_trips.begin()) % 2) != (p.individualSet[rand_2].genes[0].trips[truth2].trip_set - 1)) {
				//目标函数加倍
				p.individualSet[rand_2].objectiveSet[0] *= 10;
				p.individualSet[rand_2].objectiveSet[1] *= 10;
				p.individualSet[rand_2].objectiveSet[2] *= 10;
				p.individualSet[rand_2].objectiveSet[3] *= 10;
				change2 = 1;
				break;
			}
		}
		if (change2 == 1) break;
	}
	//4.2 检查时间顺序的约束条件合不合理
	//记录是否改变即是否合理
	for (c_iter1 = p.individualSet[rand_1].genes[0].cars.begin(); c_iter1 != p.individualSet[rand_1].genes[0].cars.end(); c_iter1++) {
		if (!check_feasible_car(*c_iter1, t1)) {
			p.individualSet[rand_1].objectiveSet[0] *= 10;
			p.individualSet[rand_1].objectiveSet[1] *= 10;
			p.individualSet[rand_1].objectiveSet[2] *= 10;
			p.individualSet[rand_1].objectiveSet[3] *= 10;
			change1 = 1;
			break;
		}
	}
	for (c_iter2 = p.individualSet[rand_2].genes[0].cars.begin(); c_iter2 != p.individualSet[rand_2].genes[0].cars.end(); c_iter2++) {
		if (!check_feasible_car(*c_iter2, t2)) {
			p.individualSet[rand_2].objectiveSet[0] *= 10;
			p.individualSet[rand_2].objectiveSet[1] *= 10;
			p.individualSet[rand_2].objectiveSet[2] *= 10;
			p.individualSet[rand_2].objectiveSet[3] *= 10;
			change2 = 1;
			break;
		}
	}
	if (change1 == 0) {
		compute_f(p.individualSet[rand_1].genes[0], t1);
	}
	if (change2 == 0) {
		compute_f(p.individualSet[rand_2].genes[0], t2);
	}
	adjust_tripid(p.individualSet[rand_1].genes[0]);
	adjust_tripid(p.individualSet[rand_2].genes[0]);
}

void GeneticAlgorithm::mutation(Population& p)
{
	//1.随机选择群体中的个体，生成两个随机数为变异的位置
	int rand_mut = rand() % p.individualSet.size();
	int rand_c1 = rand() % p.individualSet[rand_mut].genes[0].cars.size();
	int rand_c2 = rand() % p.individualSet[rand_mut].genes[0].cars.size();
	int rand_t1 = rand() % p.individualSet[rand_mut].genes[0].cars[rand_c1].car_trips.size();
	int rand_t2 = rand() % p.individualSet[rand_mut].genes[0].cars[rand_c2].car_trips.size();
	int x1 = p.individualSet[rand_mut].genes[0].cars[rand_c1].car_trips[rand_t1];
	int x2 = p.individualSet[rand_mut].genes[0].cars[rand_c2].car_trips[rand_t2];
	int re1 = find_corresponding(p.individualSet[rand_mut].genes[0].trips, x1);
	int re2 = find_corresponding(p.individualSet[rand_mut].genes[0].trips, x2);
	int rand_o1 = rand() % p.individualSet[rand_mut].genes[0].trips[re1].trip_orders.size();
	int rand_o2 = rand() % p.individualSet[rand_mut].genes[0].trips[re2].trip_orders.size();
	int o1 = p.individualSet[rand_mut].genes[0].trips[re1].trip_orders[rand_o1];
	int o2 = p.individualSet[rand_mut].genes[0].trips[re2].trip_orders[rand_o2];
	if (o1 == o2) return;
	//2.互换元素并进行约束判断，更新trip；
	//①生成Trip[]记录
	Trip t[MAX1];
	vector<Trip>::iterator t_iter;
	for (t_iter = p.individualSet[rand_mut].genes[0].trips.begin(); t_iter != p.individualSet[rand_mut].genes[0].trips.end(); t_iter++) {
		t[t_iter->trip_id] = *t_iter;
	}
	//②开始互换元素
	p.individualSet[rand_mut].genes[0].trips[re1].trip_orders[rand_o1] = o2;
	p.individualSet[rand_mut].genes[0].trips[re2].trip_orders[rand_o2] = o1;
	//③增添调整功能：行程中交换后并调整服务次序
	sort(p.individualSet[rand_mut].genes[0].trips[re1].trip_orders.begin(), p.individualSet[rand_mut].genes[0].trips[re1].trip_orders.end(), comp_early);
	sort(p.individualSet[rand_mut].genes[0].trips[re2].trip_orders.begin(), p.individualSet[rand_mut].genes[0].trips[re2].trip_orders.end(), comp_early);
	//④并检查对应变化车辆的约束满足
	if (!check_feasible_car(p.individualSet[rand_mut].genes[0].cars[rand_c1], t) || !check_feasible_car(p.individualSet[rand_mut].genes[0].cars[rand_c2], t)) {
		p.individualSet[rand_mut].objectiveSet[0] *= 10;
		p.individualSet[rand_mut].objectiveSet[1] *= 10;
		p.individualSet[rand_mut].objectiveSet[2] *= 10;
		p.individualSet[rand_mut].objectiveSet[3] *= 10;
	}
	else if (!check_capacity(p.individualSet[rand_mut].genes[0].trips[re1]) || !check_capacity(p.individualSet[rand_mut].genes[0].trips[re2])) {
		p.individualSet[rand_mut].objectiveSet[0] *= 10;
		p.individualSet[rand_mut].objectiveSet[1] *= 10;
		p.individualSet[rand_mut].objectiveSet[2] *= 10;
		p.individualSet[rand_mut].objectiveSet[3] *= 10;
	}
	else {
		update_onetrip(p.individualSet[rand_mut].genes[0], p.individualSet[rand_mut].genes[0].trips[re1], t);
		update_onetrip(p.individualSet[rand_mut].genes[0], p.individualSet[rand_mut].genes[0].trips[re2], t);
		compute_f(p.individualSet[rand_mut].genes[0], t);
	}
}

void sort_aim(Chromosome& ch, int m) {
	switch (m)
	{
	case 0:
		sort(ch.cars.begin(), ch.cars.end(), comparator_car_0);
		break;
	case 1:
		sort(ch.cars.begin(), ch.cars.end(), comparator_car_1);
		break;
	case 2:
		sort(ch.cars.begin(), ch.cars.end(), comparator_car_2);
		break;
	case 3:
		sort(ch.cars.begin(), ch.cars.end(), comparator_car_3);
		break;
	default:
		break;
	}
}

//m代表优化第几个目标
Chromosome GeneticAlgorithm::crossover_greedy(Population& parent, int m)
{
	cout << "*****************************************" << endl;
	/*
	策略取父代精华		二元概率锦标赛选择
	从一个空的解决方案开始，并迭代构建子解决方案。在每次迭代中，它随机选择一个父解决方案，
	并在该父解决方案中找到最有前途的可行路径。最有前途的路径被复制到子解决方案，子解决方案中的路径数量增加一。
	此路径将从其所采用的父解决方案中删除。并相应地更新与受影响的路线相关的各种数量，
	将属于最有希望的路线的客户从其各自的路线中删除。
	*/
	//1.复制种群[父解决方案],①计算每个解决方案的车辆的目标值|②计算设置k|③每个解决方案的车辆按目标值排序
	Population tmp = parent;
	vector<Individual>::iterator it = tmp.individualSet.begin();
	vector<Car>::iterator c_iter;
	vector<Trip>::iterator t_iter;
	int MINR = INT_MAX;
	for (; it != tmp.individualSet.end(); it++) {
		update_global_trip(it->genes[0]);
		if(it->genes[0].cars.size() < MINR)
			MINR = it->genes[0].cars.size();
		for (c_iter = it->genes[0].cars.begin(); c_iter != it->genes[0].cars.end(); c_iter++) {
			compute_car_f(*c_iter, trip);
		}
	}
	int k = MINR - 1;
	int car_id = 0;
	Individual child;
	Chromosome child_chrome;
	child.genes.push_back(child_chrome);
	int *check = new int[order_num + 1];
	memset(check, 0, (order_num + 1) * sizeof(int));
	//2.随机选择一个父解决方案,并选择最有前途的可行车辆,过程重复k次，k的值取决于MINR和MAXR
	for (int i = 0; i < k; i++) {
		int ran = rand() % tmp.individualSet.size();
		Trip t1[MAX1];
		vector<Trip>::iterator trip_one;
		for (trip_one = tmp.individualSet[ran].genes[0].trips.begin(); trip_one != tmp.individualSet[ran].genes[0].trips.end(); trip_one++) {
			t1[trip_one->trip_id] = *trip_one;
		}
		switch(m)
		{
		case 0:
			sort(tmp.individualSet[ran].genes[0].cars.begin(), tmp.individualSet[ran].genes[0].cars.end(), comparator_car_0);
			break;
		case 1:
			sort(tmp.individualSet[ran].genes[0].cars.begin(), tmp.individualSet[ran].genes[0].cars.end(), comparator_car_1);
			break;
		case 2:
			sort(tmp.individualSet[ran].genes[0].cars.begin(), tmp.individualSet[ran].genes[0].cars.end(), comparator_car_2);
			break;
		case 3:
			sort(tmp.individualSet[ran].genes[0].cars.begin(), tmp.individualSet[ran].genes[0].cars.end(), comparator_car_3);
			break;
		default:
			break;
		}
		//判断是否满足有重复
		if (is_repeat(check, t1, tmp.individualSet[ran].genes[0].cars[0])) {
			Car tmp_car = tmp.individualSet[ran].genes[0].cars[0];
			tmp_car.car_id = car_id;
			car_id++;
			child_chrome.cars.push_back(tmp_car);
			sync_Info(child_chrome, tmp.individualSet[ran].genes[0].cars[0], t1);
			tmp.individualSet[ran].genes[0].cars.erase(tmp.individualSet[ran].genes[0].cars.begin());
		}
		else
			continue;
	}
	//3.更新编排car、trip信息
	adjust_tripid(child_chrome);
	/*
	4.针对性策略补齐子代: 以迭代的方式将未分配的客户插入最佳位置，并创建一个完整的子解决方案。
	在每次迭代过程中，它随机选择一个未分配的客户，并将其插入根据所考虑的目标定义的最佳位置。
	*/
	Trip t[MAX1];
	vector<Trip>::iterator trip_one;
	for (trip_one = child_chrome.trips.begin(); trip_one != child_chrome.trips.end(); trip_one++) {
		t[trip_one->trip_id] = *trip_one;
	}
	int lamda[FUNC_NUM] = { 0, 0, 0, 0 };
	lamda[m] = 1;
	for (int i = 1; i < order_num; i++) {
		if (check[i] == 1) {
			continue;
		}
		else {
			insert_order(child_chrome, lamda, i, t);
			check[i] = 1;
		}
	}
	delete []check;
	check = nullptr;
	return child_chrome;
}

Chromosome GeneticAlgorithm::Crossover_Greedy(Chromosome p1, Chromosome p2, int m)
{
	//p1是较优解、p2是较差解
	cout << "*****************************************" << endl;
	/*
	策略取父代精华		二元概率锦标赛选择
	从一个空的解决方案开始，并迭代构建子解决方案。在每次迭代中，它随机选择一个父解决方案，
	并在该父解决方案中找到最有前途的可行路径。最有前途的路径被复制到子解决方案，子解决方案中的路径数量增加一。
	此路径将从其所采用的父解决方案中删除。并相应地更新与受影响的路线相关的各种数量，
	将属于最有希望的路线的客户从其各自的路线中删除。
	*/
	//1.①计算两个父代的车辆的目标值|②计算设置k|③该父代车辆按目标值排序|④按概率ptb随机选择一个父代
	vector<Individual> Set;
	Individual tmp1;
	Individual tmp2;
	tmp1.genes.push_back(p1);
	tmp2.genes.push_back(p2);
	Set.push_back(tmp1);
	Set.push_back(tmp2);
	vector<Individual>::iterator it = Set.begin();
	vector<Car>::iterator c_iter;
	vector<Trip>::iterator t_iter;
	int MINR = (p1.cars.size() < p2.cars.size()) ? p1.cars.size() : p2.cars.size();
	for (; it != Set.end(); it++) {
		update_global_trip(it->genes[0]);
		for (c_iter = it->genes[0].cars.begin(); c_iter != it->genes[0].cars.end(); c_iter++) {
			compute_car_f(*c_iter, trip);
		}
	}
	int k = MINR - 1;
	int car_id = 0;
	Chromosome child_chrome;
	int* check = new int[order_num + 1];
	memset(check, 0, (order_num + 1) * sizeof(int));
	//2.随机选择一个父解决方案,并选择最有前途的可行车辆,过程重复k次，k的值取决于MINR和MAXR
	int ran = -1;
	for (int i = 0; i < k; i++) {
		double pbt = rand() % 1000 / 1000.0;
		if (pbt < 0.7)
			ran = 0;
		else
			ran = 1;
		Trip t1[MAX1];
		vector<Trip>::iterator trip_one;
		for (trip_one = Set[ran].genes[0].trips.begin(); trip_one != Set[ran].genes[0].trips.end(); trip_one++) {
			t1[trip_one->trip_id] = *trip_one;
		}
		switch (m)
		{
		case 0:
			sort(Set[ran].genes[0].cars.begin(), Set[ran].genes[0].cars.end(), comparator_car_0);
			break;
		case 1:
			sort(Set[ran].genes[0].cars.begin(), Set[ran].genes[0].cars.end(), comparator_car_1);
			break;
		case 2:
			sort(Set[ran].genes[0].cars.begin(), Set[ran].genes[0].cars.end(), comparator_car_2);
			break;
		case 3:
			sort(Set[ran].genes[0].cars.begin(), Set[ran].genes[0].cars.end(), comparator_car_3);
			break;
		default:
			break;
		}
		//判断是否满足有重复
		if (is_repeat(check, t1, Set[ran].genes[0].cars[0])) {
			Car tmp_car = Set[ran].genes[0].cars[0];
			tmp_car.car_id = car_id;
			car_id++;
			child_chrome.cars.push_back(tmp_car);
			sync_Info(child_chrome, Set[ran].genes[0].cars[0], t1);
			Set[ran].genes[0].cars.erase(Set[ran].genes[0].cars.begin());
		}
		else
			continue;
	}
	//3.更新编排car、trip信息
	adjust_tripid(child_chrome);
	/*
	4.针对性策略补齐子代: 以迭代的方式将未分配的客户插入最佳位置，并创建一个完整的子解决方案。
	在每次迭代过程中，它随机选择一个未分配的客户，并将其插入根据所考虑的目标定义的最佳位置。
	*/
	Trip t[MAX1];
	vector<Trip>::iterator trip_one;
	for (trip_one = child_chrome.trips.begin(); trip_one != child_chrome.trips.end(); trip_one++) {
		t[trip_one->trip_id] = *trip_one;
	}
	int lamda[FUNC_NUM] = { 0, 0, 0, 0 };
	lamda[m] = 1;
	for (int i = 1; i < order_num; i++) {
		if (check[i] == 1) {
			continue;
		}
		else {
			insert_order(child_chrome, lamda, i, t);
			check[i] = 1;
		}
	}
	delete[]check;
	check = nullptr;
	return child_chrome;
}


Chromosome GeneticAlgorithm::mutation_target(Population& entity, int m)
{
	cout << "----------------------------------------------" << endl;
	/*
	Mutation 分为二加一阶段，解决方案被部分破坏，然后通过使用特定于目标的方法再次重建。
	阶段一: 破坏阶段，分为两种破坏策略: f1的突变运算符仅使用Select_route过程的第一个版本[一条路径删部分或所有客户]，
	而f2、f3和f4的突变运算符以互斥的方式使用Select_route过程的两个版本。
	阶段二: 重建阶段，以迭代的方式创建的。在每次迭代中，从未分配的客户池中随机选择一个客户[随机选择一个客户]，
	并将其插入某个现有路线的最佳位置。如果由于违反限制而没有找到最佳位置，
	则通过在新创建的路线的第一个位置插入选定的未分配客户来创建新路线。
	*/
	int rand_mut = rand() % entity.individualSet.size();
	int lamda[FUNC_NUM] = { 0, 0, 0, 0 };
	lamda[m] = 1;
	double p = rand() % 1000 / 1000.0;
	vector<int>orders;
	double now_time = 0;
	orders.clear();
	int r_ind = 0;
	r_ind = rand() % entity.individualSet[rand_mut].genes[0].trips.size();
	#define solution entity.individualSet[rand_mut].genes[0]
	// 第一种破坏重建策略: f1的突变运算符仅使用Select_route过程的第一个版本[一条路径删部分或所有客户]
	if (m == 0 || (m == 1 && p < 0.6) || (m == 2 && p < 0.7) || (m == 3 && p < 0.5)) {
		int count = rand() % (solution.trips[r_ind].trip_orders.size());
		for (int i = 0; i < count; ++i)
		{
			int cust_ind = rand() % (solution.trips[r_ind].trip_orders.size());
			orders.push_back(solution.trips[r_ind].trip_orders[cust_ind]);
			solution.trips[r_ind].trip_orders.erase(solution.trips[r_ind].trip_orders.begin() + cust_ind);
			update_global_trip(solution);
			update_onetrip(solution, solution.trips[r_ind], trip);
		}
		if (solution.trips[r_ind].trip_orders.size() == 0)
		{
			update_trip12(solution, solution.trips[r_ind]);
			solution.trips.erase(solution.trips.begin() + r_ind);
			update_global_trip(solution);
		}
		else
		{
			if (check_feasible_trip(solution.trips[r_ind]))
			{
				compute_trip_time(solution.trips[r_ind]);
				compute_trip_distance(solution.trips[r_ind]);
			}
		}
		//将删除容器中的客户点插入到解中产生新解
		for (int iter = 0; iter < orders.size(); ++iter)
			insert_order(solution, lamda, orders[iter], trip, now_time);
		//更新trip[]:直接全更新
		vector<Trip>::iterator t_iter;
		for (t_iter = solution.trips.begin(); t_iter != solution.trips.end(); ++t_iter) {
			trip[t_iter->trip_id] = *t_iter;
		}
		//更新阶段
		update_ydmd(solution);
		compute_f(solution, trip);
		for (vector<Car>::iterator car_iter = solution.cars.begin(); car_iter != solution.cars.end(); )
		{
			if (car_iter->car_trips.size() == 0)
				car_iter = solution.cars.erase(car_iter);
			else
				++car_iter;
		}
	}
	//第二种破坏重建策略: 从未分配的客户池中随机选择一个客户[随机选择一个客户]
	else {
		int cust_ind = rand() % (solution.trips[r_ind].trip_orders.size());  //考虑车库-2，而本模型暂不考虑车库
		int cust = solution.trips[r_ind].trip_orders[cust_ind];
		orders.push_back(cust);
		solution.trips[r_ind].trip_orders.erase(solution.trips[r_ind].trip_orders.begin() + cust_ind);
		update_global_trip(solution);
		update_onetrip(solution, solution.trips[r_ind], trip);
		if (solution.trips[r_ind].trip_orders.size() == 0)
		{
			update_trip12(solution, solution.trips[r_ind]);
			solution.trips.erase(solution.trips.begin() + r_ind);
			update_global_trip(solution);
		}
		else
		{
			//检查删除chrome.trips[r_ind]路径的cust_ind后，该路径是否有效  路径本可行，删去应该也可行
			if (check_feasible_trip(solution.trips[r_ind]))
			{
				compute_trip_time(solution.trips[r_ind]);
				compute_trip_distance(solution.trips[r_ind]);
			}
		}
		//将删除容器中的客户点插入到解中产生新解
		insert_order(solution, lamda, orders[0], trip, now_time);
		//更新trip[]:直接全更新
		vector<Trip>::iterator t_iter;
		for (t_iter = solution.trips.begin(); t_iter != solution.trips.end(); ++t_iter) {
			trip[t_iter->trip_id] = *t_iter;
		}
		//更新阶段
		update_ydmd(solution);
		compute_f(solution, trip);
		for (vector<Car>::iterator car_iter = solution.cars.begin(); car_iter != solution.cars.end(); )
		{
			if (car_iter->car_trips.size() == 0)
				car_iter = solution.cars.erase(car_iter);
			else
				++car_iter;
		}
	}
	return solution;
}

// Mutation operator used by the evolutionary loop. It converts the selected
// objective m into a one-hot lambda vector and delegates to dynamic MDLS/AOS.
Chromosome GeneticAlgorithm::Mutation_Greedy(Chromosome& chrome, int m, vector<Chromosome>& EPxx)
{
	int dy_lamda[FUNC_NUM] = { 0, 0, 0, 0 };
	dy_lamda[m] = 1;
	Dynamic_mdls(chrome, dy_lamda, -1, EPxx);
	return chrome;
}

// Parent-selection entry point retained for compatibility with the original
// code. Despite the name, the current implementation samples one individual
// uniformly at random from the parent population.
Chromosome GeneticAlgorithm::Binary_tournament_selection(Population& parent)
{
	int ran = rand() % parent.individualSet.size();
	Chromosome BTS = parent.individualSet[ran].genes[0];
	return BTS;
}

bool GeneticAlgorithm::Unique_population(Population& Px, Chromosome X)
{
	if (Px.individualSet.size() == 0)
		return true;
	vector<Individual>::iterator I_iter = Px.individualSet.begin();
	for (; I_iter != Px.individualSet.end(); I_iter++) {
		//存在个体四个目标值均相等，说明不是独有的
		if ((fabs((*I_iter).genes[0].f[0] - X.f[0]) < EPS) &&
			(fabs((*I_iter).genes[0].f[1] - X.f[1]) < EPS) &&
			(fabs((*I_iter).genes[0].f[2] - X.f[2]) < EPS) &&
			(fabs((*I_iter).genes[0].f[3] - X.f[3]) < EPS))
			return false;
	}
	return true;
}

vector<Front> GeneticAlgorithm::fastNonDominatedSort(Population* r)
{
	vector<Front> front;
	front.resize(1);
	int real_size = r->populationSize;
	if (r->individualSet.size() < r->populationSize)
		real_size = r->individualSet.size();
	for (int i = 0; i < real_size; i++)
	{
		Individual* p = &r->individualSet[i];
		p->dominatedSet.clear();
		p->dominatedCount = 0;

		for (int j = 0; j < real_size; j++)
		{
			if (i == j) { continue; }
			Individual* q = &r->individualSet[j];
			if (p->dominate(*q))
			{
				p->dominatedSet.push_back(q);
			}
			else if (q->dominate(*p))
			{
				p->dominatedCount++;
			}
		}

		if (p->dominatedCount == 0)
		{
			p->rank = 0;
			front[0].push_back(p);
		}
	}

	int i = 0;
	while (front[i].size() != 0)
	{
		vector<Individual*> _tmpSet;
		_tmpSet.resize(0);
		for (int j = 0; j < front[i].size(); j++)
		{
			Individual* p = front[i][j];
			for (int k = 0; k < p->dominatedSet.size(); k++)
			{
				Individual* q = p->dominatedSet[k];
				q->dominatedCount--;
				if (q->dominatedCount == 0)
				{
					q->rank = i + 1;
					_tmpSet.push_back(q);
				}
			}
		}
		i++;
		front.push_back(_tmpSet);
	}

	return front;
}

bool comparator_0(Individual* a, Individual* b)
{
	return a->objectiveSet[0] < b->objectiveSet[0];
}

bool comparator_1(Individual* a, Individual* b)
{
	return a->objectiveSet[1] < b->objectiveSet[1];
}

bool comparator_2(Individual* a, Individual* b)
{
	return a->objectiveSet[2] < b->objectiveSet[2];
}

bool comparator_3(Individual* a, Individual* b)
{
	return a->objectiveSet[3] < b->objectiveSet[3];
}


void GeneticAlgorithm::crowdingDistanceAssignment(Front f, float max, float min)
{
	Chromosome chrome;
	int geneSize = 1;
	int objectiveSize = 4;
	Individual f_max(chrome, geneSize);
	Individual f_min(chrome, geneSize);
	f_max.maxevaluation();
	f_min.minevaluation();

	int l = f.size();
	for (int i = 0; i < l; i++)
	{
		f[i]->distance = 0;
	}
	for (int m = 0; m < objectiveSize; m++)
	{
		if (m == 0)
		{
			sort(f.begin(), f.end(), comparator_0);
		}
		else if (m == 1)
		{
			sort(f.begin(), f.end(), comparator_1);
		}
		else if (m == 2)
		{
			sort(f.begin(), f.end(), comparator_2);
		}
		else
		{
			sort(f.begin(), f.end(), comparator_3);
		}
		f[0]->distance = numeric_limits<float>::infinity();
		f[l - 1]->distance = numeric_limits<float>::infinity();
		for (int j = 2; j < l - 1; j++)
		{
			f[j]->distance = f[j]->distance + (f[j + 1]->objectiveSet[m] - f[j - 1]->objectiveSet[m]) / (f_max.objectiveSet[m] - f_min.objectiveSet[m]);
		}
	}
}
