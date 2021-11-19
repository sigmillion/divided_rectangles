function y = branin_function(x)
x1 = 15*x(1) - 5;  % [0, 1] => [-5, 10]
x2 = 15*x(2);      % [0, 1] => [ 0, 15]
y = branin([x1;x2]);
return

function y = branin(x)
a = 1;
b = 5.1 / (4*pi*pi);
c = 5 / pi;
r = 6;
s = 10;
t = 1 / (8*pi);
y = a*(x(2) - b*x(1)*x(1) + c*x(1) - r)^2 + s*(1-t)*cos(x(1)) + s;
return
