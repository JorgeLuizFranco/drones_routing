#include <algorithm>
#include <bits/stdc++.h>
#include <queue>

// using namespace std;

using point_ii = std::pair<int, int>;

using ask_pair = std::pair<point_ii, point_ii>;

const std::vector<point_ii> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

std::pair<double, int> heuristic(int drone_id, point_ii begin_point,
																 point_ii end_point) {
																		
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

std::vector<point_ii> retrieve_path(std::vector<std::vector<point_ii>>& parent, point_ii drone_end){

	std::vector<point_ii> path;
	path.reserve((int)parent[0].size() + parent.size());


	auto [i,j]= drone_end;

	while(parent[i][j]!=point_ii(-1,-1)){
		path.push_back({i,j});

		std::tie(i,j)= parent[i][j];
		
	}

	std::reverse(path.begin(),path.end());

	return path;
}
	
std::vector<point_ii> bfs_min_path(int n, int m, int drone_id, std::vector<ask_pair> &asks) {

	std::vector<std::vector<bool>> visited(n, std::vector<bool>(m, false));

	std::vector<std::vector<point_ii>> parent(n, std::vector<point_ii>(m, {-1,-1}));

	std::queue<point_ii> bfs_queue;

	auto[drone_begin, drone_end]= asks[drone_id];

	bfs_queue.push(drone_begin);

	while(!bfs_queue.empty()){
		auto[pos_i,pos_j]= bfs_queue.front();

		if(drone_end == point_ii(pos_i,pos_j)){
			return retrieve_path(parent,drone_end);
		}

		for(auto [d_i, d_j]: directions){
				if(!visited[pos_i+d_i][pos_j+d_j]){
						visited[pos_i+d_i][pos_j+d_j]=true;
						
						parent[pos_i+d_i][pos_j+d_j]={pos_i,pos_j};

						bfs_queue.push({pos_i,pos_j});
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

	return 0;
}
