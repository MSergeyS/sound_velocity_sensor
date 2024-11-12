function [time_propagation] = crosscorrelation_fi (scope_real, otr1, otr2)

global HYDRA_SVM_ADC_OUT_BUFF_SIZE
global ADC_WIDTH
global FREQUENCY_CENTRAL
global DATA_RATE

scope_real = double(scope_real);
otr1 = double(otr1);
otr2 = double(otr2);

    reserve = 12; % запас
    signal_size = 27 * (DATA_RATE / FREQUENCY_CENTRAL);
    window_size = (signal_size + 2*reserve); % сигнал с хвостом
    if(otr1 == 0)
        otr1 = 336;             % минимальный индекс начала первого окна
        otr2 = 811;             % минимальный индекс начала первого окна
        window_size = 370;      % максимальный размер окна
        reserve = 0; % запас

    end

    massiv_size = HYDRA_SVM_ADC_OUT_BUFF_SIZE;
    
    % проверки на всякий случай
    if (otr1 < 12 || otr2 < 24)
        return
        % error('отражения не найдены!');
    elseif (otr2 + window_size + 12 > massiv_size)
        return
        % error('слишком маленькая скорость');
    elseif (otr1 > otr2)
        return
        % error('inx1 > inx2 !!!');
    end
        
    % получаем комплексные отсчёты
    scope_complex = [ (      scope_real(1:4:end) + 1i*scope_real(2:4:end) ); ...
                      (     -scope_real(3:4:end) + 1i*scope_real(2:4:end) ); ...
                      (     -scope_real(3:4:end) - 1i*scope_real(4:4:end) ); ...
                      ( [ scope_real(5:4:end) 0] - 1i*scope_real(4:4:end) ) ];

    % scope_complex = [ (     -scope_real(1:4:end) - 1i*scope_real(2:4:end) ); ...
    %                   (      scope_real(3:4:end) - 1i*scope_real(2:4:end) ); ...
    %                   (      scope_real(3:4:end) + 1i*scope_real(4:4:end) ); ...
    %                   ( [-scope_real(5:4:end) 0] + 1i*scope_real(4:4:end) ) ];

    % scope_complex = [ (      scope_real(2:4:end) + 1i*scope_real(3:4:end) ); ...
    %                   (     -scope_real(4:4:end) + 1i*scope_real(3:4:end) ); ...
    %                   (     -scope_real(4:4:end) - 1i*[scope_real(5:4:end) 0] ); ...
    %                   ( [ scope_real(6:4:end) 0] - 1i*[scope_real(5:4:end) 0] ) ];

    % scope_complex = [ (     -scope_real(2:4:end) - 1i*scope_real(3:4:end) ); ...
    %                   (      scope_real(4:4:end) - 1i*scope_real(3:4:end) ); ...
    %                   (      scope_real(4:4:end) + 1i*[scope_real(5:4:end) 0] ); ...
    %                   ( [-scope_real(6:4:end) 0] + 1i*[scope_real(5:4:end) 0] ) ];

    % scope_complex = [ ( [0 -scope_real(2:4:end-4)] - 1i*scope_real(1:4:end) ); ...
    %                   (        scope_real(2:4:end) - 1i*scope_real(1:4:end) ); ...
    %                   (        scope_real(2:4:end) + 1i*scope_real(3:4:end) ); ...
    %                   (       -scope_real(4:4:end) + 1i*scope_real(3:4:end) ) ];
    

    scope_complex = reshape(scope_complex,1, size(scope_real,2));

    %% Вычисление корреляционной функции
    % сигнал в первом окне
    inx_window_1 = otr1 - reserve;
    s1 = scope_complex(inx_window_1:inx_window_1 + window_size - 1);
    s1(225:end) = 0 + 1i*0;
    % сигнал во втором окне
    s2 = scope_complex(otr2 - reserve:otr2 - reserve + window_size - 1); 
    
    length_s = length(s1);
    complex_zero = struct('re',int16(0),'im',int16(0));
    s1_fi = repelem(complex_zero,1,length_s);
    s2_fi = repelem(complex_zero,1,length_s);

    datatype = 'int32';
    number_digits = 20;
    for inx = 1:length_s
       s1_fi(inx).re = eval([datatype '(' num2str(real(s1(inx)),number_digits) ');']);
       s1_fi(inx).im = eval([datatype '(' num2str(imag(s1(inx)),number_digits) ');']);
       s2_fi(inx).re = eval([datatype '(' num2str(real(s2(inx)),number_digits) ');']);
       s2_fi(inx).im = eval([datatype '(' num2str(imag(s2(inx)),number_digits) ');']);
    end

    % взаимная корреляция
    xss = xcorr(s2,s1);
    % берём только правую часть
    xss = xss(size(s2,2):end);
    
    % length_s = length(s1);
    % xss = NaN(1,length_s);
    % for inx = 1:length_s
    %    xss(inx) = xcorr_time(s2, s1, inx);
    % end
    
    % нормируем
    xss = xss / (signal_size*2^(2*ADC_WIDTH-1));
    % ищем максимум
    [~, inx_max_xss] = max(abs(xss));
    
    abs_xss = abs(xss(inx_max_xss));    % амплитуда ВКФ
    phase = angle(xss(inx_max_xss)); % фаза ВКФ

    % inx_max_xss = 0;
    % max_xss = 0;
    % max_xss_complex = struct('re',single(0),'im',single(0));
    % datatype_result = 'int64';
    % for inx = 1:length_s
    %    xss = xcorr_time_fi(s2_fi, s1_fi, inx);
    %    re = eval([datatype_result '(' num2str(xss.re,number_digits) ');']);
    %    im = eval([datatype_result '(' num2str(xss.im,number_digits) ');']);
    %    abs_xss = eval(['u' datatype_result '(' num2str(re*re + im*im,number_digits) ');']);    % амплитуда ВКФ
    %    if (abs_xss > max_xss) % ищем максимум
    %        inx_max_xss = inx;
    %        max_xss = abs_xss;
    %        max_xss_complex.re = re;
    %        max_xss_complex.im = im;
    %    end
    % end
    % phase = atan2(double(max_xss_complex.im),double(max_xss_complex.re)); % фаза ВКФ

    %% измеренное время
    t1 = (otr2 - otr1 + (inx_max_xss-1))/DATA_RATE;
    fix_num_periods = floor(t1*FREQUENCY_CENTRAL);
    if (phase < -pi/2)
        fix_num_periods = fix_num_periods + 1;
    end

    time_propagation = (fix_num_periods + phase/(2*pi))/FREQUENCY_CENTRAL;

    time = 1e6*(otr2 - otr1 + (0:window_size-1))./DATA_RATE;

    [inx_max_xss fix_num_periods phase time_propagation*1e6]

    figure(777)
      axc(1) = subplot(2,1,1);
        plot(time,abs(s1),'.r')
        hold on
        plot(time,abs(s2),'.b')
        hold off
        ylim([0 2^(ADC_WIDTH-1)])
        grid on
      axc(2) = subplot(2,1,2);
        plot(time,abs(xss),'.g')
        hold on
        line((fix_num_periods/FREQUENCY_CENTRAL)*1e6*[1 1],[0 1], ...
             'Color','r','LineStyle','--')
        line(t1*1e6*[1 1],[0 1], ...
             'Color','b','LineStyle','--')
        line(time_propagation*1e6*[1 1],[0 1], ...
             'Color','k','LineStyle','--')
        hold off
        grid on
        % xlim((t1 + [-10 10]./DATA_RATE)*1e6)
        xlim([44 47])
        % ylim(max_abs_xss + [-0.02 0.005])  
        % xlim([min(time) max(time)])
        ylim([0 0.15])
      linkaxes(axc,'x')
      drawnow
      pause(0.5)
end