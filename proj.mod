# Declare the sets and parameters
reset;
param N;  # Number of rows in the grid 2D
param M;  # Number of columns in the grid 2D
param K;  # Number of drones
param x_max;  # Index maximum for rows
param y_max;  # Index maximum for columns
param a;  # Constant for time weighting
param b;  # Constant for distance weighting
param T;  # Upper limit for final time
param i_start{1..K};
param j_start{1..K};
param i_end{1..K};
param j_end{1..K};

set DroneSet := 1..K;

# Decision variables
var x{1..N, 1..M, DroneSet, 1..T} binary;
var t_end{k in DroneSet} integer;
var d_end{k in DroneSet} integer;

# Objective function
minimize TotalCost:
    sum {k in DroneSet} (a * t_end[k] + b * d_end[k]) / K;

# Constraints
subject to DronePosition{k in DroneSet, t in 1..T}:
    sum {i in 1..N, j in 1..M} x[i, j, k, t] = 1;

    
subject to StartPosition {k in DroneSet}:
	x[i_start[k], j_start[k], k, 1] = 1;   # t0=1
    

subject to EndPosition{k in DroneSet}:
  sum {t in 1..T}  x[i_end[k], j_end[k], k, t] = 1;



subject to EndTime{k in DroneSet}:
   t_end[k]= sum {t in 1..T} t* x[i_end[k], j_end[k], k, t]  ;
 
subject to TimeEnd {i in 1..N, j in 1..M, k in DroneSet, t in 1..T}:
    if t > t_end[k]+1 then x[i,j,k,t] = 0;



subject to MovementConstraint{i in 2..N-1, j in 2..M-1, k in DroneSet, t in 1..T-1}:
    x[i, j, k, t+1] <= x[i+1, j, k, t] + x[i-1, j, k, t] + x[i, j+1, k, t] + x[i, j-1, k, t];

subject to DeltaCalculation{ k in DroneSet}:
    d_end[k] = sum {i in 1..N, j in 1..M, t in 2..T} abs(x[i, j, k, t] - x[i, j, k, t-1]);

subject to TimeLimit{k in DroneSet}: 1 <= t_end[k] <= T;
#subject to IndexBound_x{i in 1..N}:  1 <= i <= x_max;
#subject to IndexBound_y{j in 1..M}:  1 <= j <= y_max;    
subject to IndexBound_t{t in 1..T}:  1 <= t <= T;      
    
option solver gurobi;

#option solver cbc;

data proj.dat;
display i_start,j_start,i_end,j_end;  
solve;

display TotalCost, t_end, d_end;#,x;

display _total_solve_time;
    