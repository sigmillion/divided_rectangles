function Rt = divide(r, fun)
% Algorithm to divide the rectangle.
emin = min(r.e); % 0-based
if(emin+1 ~= r.emin)
    fprintf('Error on emin.!\n');
end
delta = (1/3)^(emin+1); % Divide the longest side length (using +1)
% Find all dimensions having the longest side length.
ind = find(r.e == emin);
% Evaluate the function at the two centers in these dimensions.
vals = {};
valsrt = [];
for k=1:length(ind)
    i = ind(k); % Get the index
    v.dim = i;
        
    v.x1 = r.c;
    v.x1(i) = r.c(i) - delta;
    v.f1 = fun(v.x1);

    v.x2 = r.c;
    v.x2(i) = r.c(i) + delta;
    v.f2 = fun(v.x2);
        
    vals{end+1} = v;
    valsrt(end+1) = min([v.f1,v.f2]);
end

% Sort by function valuation
[~,ind]=sort(valsrt,'ascend');
vals = vals(ind);

% Process the rectangles in order of their function values (smallest first)
Rt = [];
for i=1:length(vals)
    dim = vals{i}.dim;
    r.e(dim) = r.e(dim) + 1; % Shrink rectangle along this dimension
    r1 = new_rectangle(vals{i}.f1,vals{i}.x1,r.e);
    r1.index = r.index;
    r2 = new_rectangle(vals{i}.f2,vals{i}.x2,r.e);
    r2.index = r.index;
    Rt = [Rt, r1, r2];
end
r.emin = min(r.e);
r.dmax = (1/3)^r.emin;
Rt = [Rt, r];
