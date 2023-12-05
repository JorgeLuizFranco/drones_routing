#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <queue>
#include <set>

// using namespace std;

using point_ii = std::pair<int, int>;

using ask_pair = std::pair<point_ii, point_ii>;

const std::vector<point_ii> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

std::pair<double, int> heuristic(int drone_id, ask_pair ask) {

	auto[begin_point,end_point]= ask;
																		
	auto [x_begin, y_begin] = begin_point;
	auto [x_end, y_end] = end_point;

	return {std::hypot(x_begin - x_end, y_begin - y_end), drone_id};

}

void read_asks(int k, std::vector<ask_pair> &asks) {
	for (int i = 0; i < k; i++) {
		int i_begin, j_begin, i_end, j_end;
		std::cin >> i_begin >> j_begin >> i_end >> j_end;
		asks[i] = ask_pair({i_begin, j_begin}, {i_end, j_end});
	}
}

std::set<point_ii> retrieve_path(std::vector<std::vector<point_ii>>& parent, point_ii drone_end){

	std::set<point_ii> path;


	auto [i,j]= drone_end;

	while(parent[i][j]!=point_ii(-1,-1)){
		path.insert({i,j});

		printf("i:%d , j:%d ",i,j);
		std::tie(i,j)= parent[i][j];
		
	}

	std::reverse(path.begin(),path.end());

	return path;
}


inline bool valid_position(int x,int y, int n, int m){
	return (x>=0 && x<n) && (y>=0 && y<m);
}

std::set<point_ii> bfs_min_path(int n, int m, int drone_id, std::vector<ask_pair> &asks) {

	std::vector<std::vector<bool>> visited(n, std::vector<bool>(m, false));

	std::vector<std::vector<point_ii>> parent(n, std::vector<point_ii>(m, {-1,-1}));

	std::queue<point_ii> bfs_queue;

	auto[drone_begin, drone_end]= asks[drone_id];

	bfs_queue.push(drone_begin);

	visited[drone_begin.first][drone_begin.second]=true;	
	//printf("ou\n");

	while(!bfs_queue.empty()){
		auto[pos_i,pos_j]= bfs_queue.front(); bfs_queue.pop();
		

		if(drone_end == point_ii(pos_i,pos_j)){
			return retrieve_path(parent,drone_end);
		}

		printf("i: %d j: %d\n", pos_i,pos_j);

		for(auto [d_i, d_j]: directions){
				if(valid_position(pos_i+d_i,pos_j+d_j,n,m) && !visited[pos_i+d_i][pos_j+d_j]){
						printf("visit %d,%d\n",pos_i+d_i,pos_j+d_j);
						visited[pos_i+d_i][pos_j+d_j]=true;
						
						parent[pos_i+d_i][pos_j+d_j]={pos_i,pos_j};

						bfs_queue.push({pos_i+d_i,pos_j+d_j});
				}
		}
	}

	return {};
}

int main() {
	int N, M, K;
	std::cin >> N >> M >> K;
	std::vector<ask_pair> asks(K);
	read_asks(K, asks);

	std::vector<std::pair<double,int>> heuristic_order_drones;

	heuristic_order_drones.reserve(K);

	std::vector<std::set<point_ii>> min_path(K);

	for(int drone_id=0; drone_id<K; drone_id++){
		heuristic_order_drones.push_back(heuristic(drone_id, asks[drone_id]));
		min_path[drone_id]= bfs_min_path(N, M, drone_id,asks);
	}

	//std::sort(heuristic_order_drones.begin(),heuristic_order_drones.end());



	for(auto [h,drone_id]: heuristic_order_drones){
		std::cout<<" Path Drone "<<drone_id<<": \n";
		for(auto [pos_i,pos_j]: min_path[drone_id]){
			std::cout<< "[ "<<pos_i<<" , "<<pos_j<<" ]\n";
		}
		std::cout<<"\n\n";
	}


	return 0;
}
