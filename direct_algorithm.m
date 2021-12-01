% Author: Jake Gunther
% Date: 11/24/2021
% Contact: jake.gunther@usu.edu
%
% History
% 11/24/2021 - Initial version.
%

clear all;
close all;

% Set up the variable space and the function to be optimized
dim = 2; % Dimension of optimization vector
opt_fun = @(x) branin_function(x); % Set the function to be optimized.
% Note that the code below optimizes over the unit square/cube in
% "dim" dimensions.  Therefore, the user must write their
% optimization function calls with internal variable scaling.  For
% example, say the variable x lies in the interval [a,b].  Then
% create a wrapper for the function to be optimized that scales the
% input variables as follows: xnew = a + (b-a)*x.  This
% transformation linearly maps the [0,1] interval to [a,b].  The
% code below optimizes over x in [0,1].

% Create initial rectangle
c = 0.5*ones(dim,1); % Center of dim-dimensional unit cube
e = zeros(dim,1); % List of exponents (3^e) of rectangle edge lengths
r = new_rectangle(opt_fun, c, e); % Make a rectangle

% Create list of lists of rectangles (using nested cell arrays)
max_num_rectangles = 1000; % Maximum number of rectangles to make
epsilon = 1e-10; % Set precision of final rectangle
emax = ceil(log(epsilon) / log(1/3));
enum = emax + 1;
elist = [0:emax];
dlist = (1/3).^elist;
fprintf('epsilon = %g, emax = %d, (1/3)^emax = %g\n',epsilon,emax,(1/3)^emax);
R = cell(enum,1);
R{r.emin+1} = [r];

% Draw initial rectangles.
i=0;
draw_rectangles_2d;

% Main loop for divided rectangles method
iter = 200; % Maximum number of iterations in divided rectangles method
for i=1:iter
    if length(R) > enum
        break;
    end
    
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
            if emin==length(R)
                R{emin+1} = Rt(k);
            else
                R{emin+1}(end+1) = Rt(k);
            end
        end
    end
    % Delete rectangles
    for j=1:length(delete_list)
        if(length(delete_list{j}) > 0)
            R{j}(delete_list{j}) = [];
        end
    end

    % Count rectangles
    numr = 0;
    for j=1:length(R)
        numr = numr + length(R{j});
    end
    fprintf('Number of rectangesl = %d\n',numr);
    if(numr > max_num_rectangles)
        break;
    end
    
    % Draw rectangles created so far.
    draw_rectangles_2d;
    
    fprintf('Press a ENTER to continue\n');
    pause;
end

% Find the best rectangle.  There's probably better ways to keep
% track of the best rectangle while the algorithm is running.  This
% code is lazy and finds the best rectangle after the fact.
fmin = inf;
jmin = 0;
kmin = 0;
for j=1:length(R)
    for k=1:length(R{j})
        if(R{j}(k).fc < fmin)
            jmin = j;
            kmin = k;
            fmin = R{j}(k).fc;
        end
    end
end
r = R{jmin}(kmin);
fprintf('The minimizing rectangle has center = [\n');
for j=1:dim
    fprintf('%g\n',r.c(j));
end
fprintf('].\n');
fprintf('Minimum cost = %g\n',fmin);
fprintf('Number of iterations = %d\n',i);