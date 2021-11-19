function r = new_rectangle(fun, c, e)
r.emin = min(e); % Smallest exponent/longest side length/index of rectangle set
r.dmax = (1/3)^r.emin;
r.index = 0; % Index of this rectangle in the array
r.c = c; % Keep a copy of the center of the rectangle
r.e = e; % Keep a copy of the edge lengths of the rectangle, l=(1/3)^e
if isa(fun,'double')
    r.fc = fun;
else
    r.fc = fun(c); % Evaluate function at the center of the rectangle
end
return
