function r = new_rectangle(fun, c, e)
r.f = fun; % Optimization function
r.c = c; % Keep a copy of the center of the rectangle
r.e = e; % Keep a copy of the edge lengths of the rectangle
r.fc = fun(c); % Evaluate function at the center of the rectangle
return
