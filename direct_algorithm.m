% Set up the variable space and the function to be optimized
dim = 2; % Dimension of optimization vector
opt_fun = @(x) branin_function(x); % Set the function to be optimized

% Create initial rectangle
c = 0.5*ones(dim,1); % Center of dim-dimensional unit cube
e = zeros(dim,1); % List of exponents (3^e) of rectangle edge lengths
r = new_rectangle(opt_fun, c, e); % Make a rectangle

% Create list of lists of rectangles (using nested cell arrays)
epsilon = 1e-4;
emax = ceil(log(epsilon) / log(1/3));
enum = emax + 1;
elist = [0:emax];
dlist = (1/3).^elist;
fprintf('epsilon = %g, emax = %d, (1/3)^emax = %g\n',epsilon,emax,(1/3)^emax);
R = cell(enum,1);
R{r.emin+1} = [r];

% Main loop for divided rectangles method
iter = 200; % Number of iterations in divided rectangles method
for i=1:iter
    % Find set of potentially optimal rectangles
    subpoints = {};
    for j=1:length(R)
        if(length(R{j}) > 0)
            [themin,minind] = min([R{j}.fc]);
            R{j}(minind).index = minind;
            R{j}(minind).emin = j-1; % emin uses 0's based indexing
            subpoints{end+1} = R{j}(minind);
        end
    end
    
    % Draw lines to find out which points will be kept in the set of potentially optimal rectangles.
    optimal = [subpoints{1}];
    for j=2:length(subpoints)
        jbest = j;
        best = 2 * (optimal(end).fc - subpoints{j}.fc) / ...
               (optimal(end).dmax - subpoints{j}.dmax);
        for k=j+1:length(subpoints)
            tbest = 2 * (optimal(end).fc - subpoints{k}.fc) / ...
               (optimal(end).dmax - subpoints{k}.dmax);
            if(tbest > best)
                best = tbest;
                jbest = j;
            end
        end
        if(best > 0 && jbest == j) % Include other check (epsilon) here.
            optimal(end+1) = subpoints{j};
        end
    end
    
    % Process each optimal rectangle
    delete_list = cell(enum,1);
    for j=1:length(optimal)
        % Divide the rectangle and add new rectangles to the list
        Rt = divide(optimal(j),opt_fun);
        index = optimal(j).index;
        emin = optimal(j).emin;
        delete_list{emin+1}(end+1) = index;
        % Process list of new rectangles
        for k=1:length(Rt)
            emin = Rt(k).emin;
            R{emin+1}(end+1) = Rt(k);
        end
    end
    % Delete rectangles
    for j=1:length(delete_list)
        if(length(delete_list{j}) > 0)
            R{j}(delete_list{j}) = [];
        end
    end

    keyboard;
end
