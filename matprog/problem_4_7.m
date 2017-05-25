m=50;
for i=1:3;
M=m-10*i
bases= (factorial(3*M))/((factorial(M))*(factorial(2*M)))
N=2*M;
A=rand(M,N);
b=zeros(M,1)+1;
c=rand(N,1);
[P,X,Y] = DLPRG(A,b,c,N,M);
end

X, Y