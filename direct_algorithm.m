% Set up the variable space and the function to be optimized
dim = 2; % Dimension of optimization vector
opt_fun = @(x) branin_function(x); % Set the function to be optimized

% Create initial rectangle
c = 0.5*ones(dim,1); % Center of dim-dimensional unit cube
e = zeros(dim,1); % List of exponents (3^e) of rectangle edge lengths
r = new_rectangle(opt_fun, c, e); % Make a rectangle
R = [r]; % Add rectangle to the list

% Main loop for divided rectangles method
iter = 200; % Number of iterations in divided rectangles method
for i=1:iter
    % Find set of potentially optimal rectangles
    points = cell(30,1); % Potentially optimal rectangles
    for j=1:length(R) % Loop over the list of rectangles
        % 1. Find the maximum side length = minimum exponent for each rectangle
        e = min(r.e); % exponent (1/3)^e
        r.emin = e;
        r.dmax = (1/3)^e;
        if(length(points{e+1}) > 0)
            if(R(j).fc < points{e+1}.fc)
                points{e+1} = R(j);
            end
        else
            points{e+1} = R(j);
        end
    end
    subpoints = {};
    isp = 0;
    for j=1:length(points)
        if(length(points{j}) > 0)
            isp = isp + 1;
            subpoints{isp} = points{j};
        end
    end
    
    % Draw lines to find out which points will be kept in the set of potentially optimal rectangles.
    optimal = [subpoints(1)];
    io = 1;
    for j=2:length(subpoints)
        jbest = j;
        best = 2 * (optimal(end).fc - subpoints(j).fc) / ...
               (optimal(end).dmax - subpoints(j).dmax);
        for k=j+1:length(subpoints)
            tbest = 2 * (optimal(end).fc - subpoints(k).fc) / ...
               (optimal(end).dmax - subpoints(k).dmax);
            if(tbest > best)
                best = tbest;
                jbest = j;
            end
        end
        if(best > 0 && jbest == j) % Include other check (epsilon) here.
            io = io + 1;
            optimal(io) = subpoints(j);
        end
    end
    
    % Process each optimal rectangle
    for j=1:length(optimal)
        % Divide the rectangle and add new rectangles to the list
        Rt = divide(optimal(j));
        R = [R, Rt];
    end
    

end
