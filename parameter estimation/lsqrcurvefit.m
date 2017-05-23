clc
clear
close all

run('/Users/madhurinannaware/Documents/Natasha_sharma/lab2/lv_data.m');
p0=[1.2028 0.0324 0.2739 0.0044];
years=[1862:2:1910];
for i=1:25
[p(:,i),error]=lsqcurvefit(@lv_rhs1,p0,years,[H(i);L(i)]);
end

k1=p(1,:);
index=find(k1>1);
k2=k1(index);

l=p(2,:);
index=find(l>0);
m=l(index);

disp(['Parameter values with lsqcurvefit Method :'])
disp(['a=' num2str(mean(k2))])
disp(['b=' num2str(mean(m))])
disp(['r=' num2str(abs(p(3,1)))])
disp(['c=' num2str(abs(p(4,1)))])