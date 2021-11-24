if dim==2 % Only draw rectangles for 2D problems
    % Draw the rectangles we have so far
    figure(); % New figure window
    subplot(121);
    plot([0 1 1 0 0],[0 0 1 1 0],'k');
    bdr = (1/3)^i * 0.1;
    hold on;
    for j=1:length(R)
        for k=1:length(R{j})
            r = R{j}(k);
            plot(r.c(1),r.c(2),'.b');
            d = (1/3).^r.e;
            x1min = r.c(1) - 0.5*d(1) + bdr;
            x1max = r.c(1) + 0.5*d(1) - bdr;
            x2min = r.c(2) - 0.5*d(2) + bdr;
            x2max = r.c(2) + 0.5*d(2) - bdr;
            plot([x1min x1max x1max x1min x1min],...
                 [x2min x2min x2max x2max x2min],'r');
        end
    end
    title(['Iteration = ',int2str(i)]);
    axis([-0.1 1.1 -0.1 1.1]);
    axis square;

    % Show global minima
    l=[-1,0,1];
    x1 = (1+2*l)*pi;
    x2 = 6+(5.1)/(4*pi*pi) * x1.^2 - 5/pi *x1;
    x1 = (x1+5)/15;
    x2 = x2/15;
    for l=1:length(x1)
        plot(x1(l),x2(l),'mx');
    end
    hold off;
    xlabel('x_1');
    ylabel('x_2');
    
    subplot(122);
    num_rect = zeros(size(R));
    for l=1:length(R)
        num_rect(l) = length(R{l});
    end
    bar([0:length(R)-1],num_rect);
    title(['Number Rectangles = ',int2str(sum(num_rect))]);
    xlabel('Exponent of (1/3)');
    ylabel('Number of Rectangles Having Longest Side Length');
    drawnow;
end