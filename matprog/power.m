A=[ 6 5 -5; 2 6 -2; 2 5 -2]
eig(A)
x = [ 1 ;2 ; 3]
for iter=1:1
    
    y = A*x
    iter
    lambda = dot(x,y)/dot(x,x)
    %normalization of x
    x= y/sqrt(dot (y,y));
end
    