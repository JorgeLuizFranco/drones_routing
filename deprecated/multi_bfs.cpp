#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <set>
#include <map>


using point_ii = std::pair<int, int>;

using ask_pair = std::pair<point_ii, point_ii>;

struct Drone{
    int flight_time_begin=0;
    int flight_time_end;
    std::vector<point_ii> path;
    ask_pair ask;  
    int id;
    double heuristic;

	Drone(int id, const ask_pair& ask) : id(id), ask(ask) {
        heuristic = std::hypot(ask.first.first - ask.second.first, ask.first.second - ask.second.second);
    }
};

const std::vector<point_ii> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};



void read_drones(int k, std::vector<Drone> &drones) {
	for (int i = 0; i < k; i++) {
		int i_begin, j_begin, i_end, j_end;
		std::cin >> i_begin >> j_begin >> i_end >> j_end;
		ask_pair ask = ask_pair({i_begin, j_begin}, {i_end, j_end});

		drones.emplace_back(i, ask);
	}
}

inline void path_update(std::vector<point_ii>& path, int& t, int &i, int &j, Drone& drone, std::map<std::pair<point_ii, int>, int>& scheduled, std::map<std::pair<point_ii, int>, point_ii>& parent) {

    path.push_back({i, j});
	auto pos_time = std::make_pair(point_ii(i, j), t);

	//printf("i: %d , j: %d , t: %d\n",i,j,t);

	scheduled[pos_time]=drone.id;

	std::tie(i, j) = parent[pos_time];
}

std::vector<point_ii> retrieve_path(std::map<std::pair<point_ii, int>, point_ii>& parent, point_ii drone_end, Drone &drone, std::map<std::pair<point_ii, int>, int>& scheduled) {
    std::vector<point_ii> path;

    auto [i, j] = drone_end;

    int t = drone.flight_time_end;
	
    while (parent.find(std::make_pair(point_ii(i, j), t)) != parent.end()) {

		path_update(path, t, i,j, drone, scheduled, parent);
        

		t--;

		if(parent.find(std::make_pair(point_ii(i, j), t)) == parent.end()){
			path_update(path, t, i,j, drone, scheduled, parent);
			break;
		}
    }

    std::reverse(path.begin(), path.end());

    return path;
}



inline bool valid_position(int x,int y, int n, int m){
	return (x>=0 && x<n) && (y>=0 && y<m);
}


std::vector<point_ii> bfs_min_path(int n, int m, Drone &drone, std::map<std::pair<point_ii, int>, int>& scheduled) {
    std::queue<std::pair<point_ii, int>> bfs_queue;
    auto [drone_begin, drone_end] = drone.ask;

    bool const path_found = false;

    drone.flight_time_begin = -1; // just for the first sum be 0

    std::map<std::pair<point_ii, int>, point_ii> parent;  // Change to a map structure

    std::map<std::pair<point_ii, int>, bool> visited;

    while (!path_found) {
        drone.flight_time_begin++;

        auto pos_time_begin = std::make_pair(drone_begin, drone.flight_time_begin);

        if (scheduled.find(pos_time_begin) != scheduled.end()) {
            continue;
        }

        bfs_queue.push({drone_begin, drone.flight_time_begin});

        visited[pos_time_begin] = true;

        while (!bfs_queue.empty()) {
            auto [position, flight_time] = bfs_queue.front();
            bfs_queue.pop();

            auto [pos_i, pos_j] = position;

            if (drone_end == point_ii(pos_i, pos_j)) {
                //std::cout << "Entrou\n\n\n\n";
                drone.flight_time_end = flight_time;
                return retrieve_path(parent, drone_end, drone, scheduled);
            }

            if (scheduled.find(std::make_pair(position, flight_time)) != scheduled.end()) {
                continue;
            }

            //printf("i: %d j: %d , t: %d , objective : (%d,%d) \n\n", pos_i, pos_j, flight_time, drone_end.first, drone_end.second);

            for (auto [d_i, d_j] : directions) {
                bool no_schedule = true;

                int next_pos_i = pos_i + d_i;
                int next_pos_j = pos_j + d_j;
                std::pair<point_ii, int> next_pos_time = {{next_pos_i, next_pos_j}, flight_time + 1};

                if (valid_position(next_pos_i, next_pos_j, n, m) && !visited[next_pos_time]) {
                    if (scheduled.find(next_pos_time) != scheduled.end()) {
                        if (no_schedule) {
                            no_schedule = false;
                            bfs_queue.push({position, flight_time + 1});
                        }
                        continue;
                    }
                    //printf("visit %d,%d\n", pos_i + d_i, pos_j + d_j);
                    visited[next_pos_time] = true;

                    // Store the parent based on (pos_i, pos_j, t)
                    parent[next_pos_time] = {pos_i, pos_j};

                    bfs_queue.push(next_pos_time);
                }
            }
        }
    }

    return {};
}


int find_max_time(std::vector<Drone>& drones){
	int max_time=-1;

	for(auto drone: drones){
		max_time=std::max(max_time,drone.flight_time_end);
	}

	return max_time;
}

void print_grid(std::vector<std::vector<char>> &grid,  std::map< std::pair<point_ii,int>, int>& scheduled, int flight_time) {
    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[0].size(); ++j) {
            char cell = grid[i][j];

            // Check if the cell is scheduled
			std::pair<point_ii,int> pos_time= {point_ii(i,j),flight_time};

            if (cell=='#' && scheduled.find(pos_time) != scheduled.end()) {

				int drone_scheduled=scheduled[pos_time];

                std::cout << 'S' << drone_scheduled<<' ';

            }else{
				std::cout << cell << cell<<' ';
			}
            
            
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

void print_paths(std::vector<Drone>& drones, std::map< std::pair<point_ii,int>, int>& scheduled, int n, int m){
	int t_max= find_max_time(drones);



	for(Drone& drone: drones){

		std::vector<std::vector<char>> grid(n, std::vector<char>(m, '#'));

		printf("Drone %d: \n\n",drone.id);

		int flight_time= drone.flight_time_begin;

		for(const point_ii& pos: drone.path  ){
			auto[pos_i,pos_j]= pos;
			grid[pos_i][pos_j]= static_cast<char>(drone.id + '0');

			//print_grid(grid,scheduled,flight_time);

			grid[pos_i][pos_j]= '#';

			//printf("flight time: %d\n\n",flight_time);
			++flight_time;
		}
		//printf("\n\n\n");
	}

	//printf("\n\n\n\n");

}

void solve(int n, int m, std::vector<Drone> &drones){

	std::map< std::pair<point_ii,int>, int> scheduled; // maps position,time to drone.id


	for(size_t i=0; i<drones.size(); i++){
		drones[i].path=bfs_min_path(n, m, drones[i], scheduled);
	}

	print_paths(drones, scheduled,n,m);
}



int main() {
	int N, M, K;
	std::cin >> N >> M >> K;

	 // Create an array of Drone objects
    std::vector<Drone> drones;
	drones.reserve(K);

	read_drones(K, drones);

    // Sort the drones by heuristic
    std::sort(drones.begin(), drones.end(), [](const Drone& a, const Drone& b) {
        return a.heuristic < b.heuristic;
    });

    for (const Drone& drone : drones) {
        //std::cout << "Drone ID: " << drone.id << " Heuristic: " << drone.heuristic << std::endl;
        // Output path or other information here
    }


	solve(N,M,drones);
    

    double average_time=0;
    double average_dist=0;

    for (const Drone& drone : drones) {
        //std::cout << "Drone ID: " << drone.id << " Heuristic: " << drone.heuristic << std::endl;
        average_time+= abs(drone.flight_time_begin-drone.flight_time_end);
        average_dist+= int(drone.path.size());
    }

    printf("Average Time: %.2lf \n\n",average_time/K);
    printf("Average Dist: %.2lf\n\n",average_dist/K);

	printf("\n\n");

	
	



	return 0;
}
