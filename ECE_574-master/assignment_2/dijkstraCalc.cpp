 double dijkstra(int** edges, int n)
 {
	 int * distance = new int[n];
	 bool * visited = new bool[n];
	 
	 for(int i = 0; i < n; i++)
	 {
		 distance[i] = 100; //set to max distance
		 visited[i] = false; // set all to unvisited
	 }
	 
	 distance[0] = 0;
	 
	 //Decide on 2-D array method from synthesize.c and https://www.youtube.com/watch?v=Zd27SysrXUc
	 //or method from https://www.youtube.com/watch?v=wQIb1NonMIM
	 
	 //find longest path via chosen algorithm implementation 
	 
	 //find the set of strings (operations) and their data widths and calculate time with getValue.cpp
	 
	 
 }