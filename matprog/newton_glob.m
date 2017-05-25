clc;
clear all;
close all;


x=7.7; % initial point
N=100;
epsilon=.000000001;  % tolerance
dummy=1;  
counter=1;
sol=[];  % solution vector 
cnt=[];  %iteration counter

while(dummy==1||abs(val1)>epsilon) %  tolerance condition
    dummy=2;
    [val1,dval]=def_fn(x);  % function call  of f and df
    delta=-val1/dval;       
    dummy1=1;    
    dummy1=2;    
      y=x+delta;  
      [val,dval]=def_fn(y);
 
    x=y;
  sol(counter)=x;   
  disp(x)  
  
%%  plot function  
   sol(counter)=x;
    res(counter)=val1;
    cnt(counter)=counter;
    counter=counter+1;

    disp(x)
  % f=figure(1);
 %  plot(sol,'--k');
  % hold on
  % plot(res,'--r');
 %  hold on
 %  xlabel('iteration');
 %  ylabel('solution/residue');
 %  legend('solution','residue');
end
tbl(:,2)=sol(1,:);
tbl(:,1)=cnt(1,:);
tbl(:,3)=res(1,:)
cnames={'iteration','x-value','function-value' };
%uitable(f,'Data',tbl,'ColumnName',cnames,'Position',[800 400 300 150])