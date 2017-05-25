function y=GaussSeidel(A,b,x,NumIters)
% Runs the Gauss-Seidel method for solving Ax=b, starting with x and


% The output y will be the whole sequence of outputs 
% value (if x is in R^n, then y will be n x NumIters

D=diag(A);

A=A-diag(D);
D=1./D; %We need the inverses
n=length(x);
x=x(:); %Make sure x is a column vector
y=zeros(n,NumIters);
for j=1:NumIters
for k=1:n
x(k)=(b(k)-A(k,:)*x)*D(k);
end
y(:,j)=x;
end
