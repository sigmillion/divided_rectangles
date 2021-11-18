function r = new_rectangle(fun, c, e)
r.c = c; % Keep a copy of the center of the rectangle
r.e = e; % Keep a copy of the edge lengths of the rectangle, l=(1/3)^e
if isa(fun,'double')
    r.fc = fun;
else
    r.fc = fun(c); % Evaluate function at the center of the rectangle
end
return
