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
    
end
