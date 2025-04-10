function test_otr (scope_real, otr1, otr2, test_plots)

    global FREQUENCY_CENTRAL
    global DATA_RATE

    scope_real = double(scope_real);
    otr1 = double(otr1);
    otr2 = double(otr2);

    reserve = 12; % запас
    signal_size = 30 * (DATA_RATE / FREQUENCY_CENTRAL);
    window_size = (signal_size + 4*reserve); % сигнал с хвостом
        
    % получаем комплексные отсчёты
    scope_complex = [ (      scope_real(1:4:end) + 1i*scope_real(2:4:end) ); ...
                      (     -scope_real(3:4:end) + 1i*scope_real(2:4:end) ); ...
                      (     -scope_real(3:4:end) - 1i*scope_real(4:4:end) ); ...
                      ( [ scope_real(5:4:end) 0] - 1i*scope_real(4:4:end) ) ];
    
    scope_complex = reshape(scope_complex,1, size(scope_real,2));

    test_plots.h_real.YData = real(scope_complex);
    test_plots.h_imag.YData = imag(scope_complex);
    test_plots.h_abs.YData = abs(scope_complex);
    test_plots.h_angle.YData = angle(scope_complex);
    test_plots.h_s1.YData = abs(scope_complex(otr1-2*reserve:otr1-2*reserve+window_size));
    test_plots.h_s2.YData = abs(scope_complex(otr2-2*reserve:otr2-2*reserve+window_size));
    drawnow
 end