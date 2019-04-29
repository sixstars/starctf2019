The time limit is 60s,so it's easy to pass this challenge with BFS and pruning  
But actually we can use a number  to represent the positions of human and boxes, and the number will never repeat.  
for example :  
if the map is 10 * 10.   
the popsition of human is x and y  
the popsitions of box is x1,y1,x2,y2...  
then the number shoule be:   
10*x+y+(10*x1+y1)*100+(10*x2+y1)*10000+...

