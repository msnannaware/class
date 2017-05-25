
diff = 0;
%count = 0;
for n = 1:100     %approx no. of iteration
    x = 1/8^n;       %input value

    f = sqrt(x^2 + 1) -1 ;         % defined function
    g = x^2/ (sqrt(x^2 + 1) + 1);
    count = count + 1;
   % if (f==0)                       %display f when computer stops calculating
   %     disp(count)
        f
        g
  %      break;
        
   % end
    
end



