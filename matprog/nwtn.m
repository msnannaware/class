 clc;
clear all;
close all;
%% The implementation uses line serch global newton algorithm
% the function for which the algorithm has been implemented is stores in
% Def_fn

x=0.1; % initial point
N=100;
epsilon=.000000001;  % tolerance
dummy=1;  % variable used to implement a do-while loop
counter=1;
sol=[];  % stores the solution vector till it converges
cnt=[];  % stores the number of iteration
%% algorithm begins
while(dummy==1||abs(val1)>epsilon) % second condition is the tolerance
    dummy=2;
    [val1,dval]=def_fn(x);  % function call that retrieves value of f and df
    delta=-val1/dval;       % newton's direction
    dummy1=1;  
  sol(counter)=x; % stores the evaluted value of x
   
  disp(x)  
  
%% this part of the code is used to make the dynamic plot function  
   sol(counter)=x;
    res(counter)=val1;
    cnt(counter)=counter;
    counter=counter+1;

    disp(x)
   f=figure(1);
   plot(sol,'--k');
   hold on
   plot(res,'--r');
   hold on
   xlabel('iteration');
   ylabel('solution/residue');
   legend('solution','residue');
end
tbl(:,2)=sol(1,:);
tbl(:,1)=cnt(1,:);
tbl(:,3)=res(1,:)
cnames={'iteration','x-value','function-value' };
uitable(f,'Data',tbl,'ColumnName',cnames,'Position',[800 400 300 150])