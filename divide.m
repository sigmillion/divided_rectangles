function Rt = divide(r, fun)
% Algorithm to divide the rectangle.
delta = r.dmax / 3;
% Find all dimensions having the longest side length.
% Evaluate the function at the two centers in this dimension.
vals = {};
valsrt = [];
for i=1:length(r.e)
    if(r.e(i) == r.emin)
        v.dim = i;
        
        v.x1 = c;
        v.x1(i) = c(i) - delta;
        v.f1 = fun(v.x1);

        v.x2 = c;
        v.x2(i) = c(i) + delta;
        v.f2 = fun(v.x2);
        
        v.f1 = f1;
        v.x1 = x1;
        vals{end+1} = v;
        valsrt[end+1] = min([f1,f2]);
    end
end
[~,ind]=sort(valsrt,'ascend');
vals = vals{ind};

Rt = [];
for i=1:length(vals)
    dim = vals{i}.dim;
    r.e(dim) = r.e(dim) + 1; % Shrink rectangle along this dimension
    r = new_rectangle(vals{i}.f1,vals{i}.x1,r.e);
    Rt = [Rt, r];
    r = new_rectangle(vals{i}.f2,vals{i}.x2,r.e);
    Rt = [Rt, r];
end
