数据集的特征指标分为三种订单数量规模、三种时间窗口类型和五种出行需求比例，
共计45个数据集，记为“alpha1-num1-num2-num3”。
其中alpha1表示路网规模，有S、M、L三种类型，
num1、num2分别表示从城市a到城市b的订单数量和从城市b到城市a的订单数量，结合起来可表示两城市之间的出行需求比例。
num3表示时间窗口类型，有1、2、3三种类型。

data文件夹内为测试算例
data/数据集(raw)的时间为北京时间，未经处理
data/数据集(时间处理版)的时间为处理过的时间，可直接进行使用，不需要再对其进行预处理
time-distance-matrix文件夹内为订单地点之间的时间矩阵和距离矩阵
其下有八个子文件夹，分别为src(出发点)/dest(目的地) - src(出发点)/dest(目的地) -dis(距离)/time(时间)
如dest-dest-dis矩阵表示为 所有订单地点的目的地 到 所有订单地点的目的地 的 距离矩阵