function [time_propagation, otr1, otr2 ] = crosscorrelation (scope_real, base, otr1, otr2, reserve)

global HYDRA_SVM_ADC_OUT_BUFF_SIZE
global ADC_WIDTH
global FREQUENCY_CENTRAL
global DATA_RATE

scope_real = double(scope_real);
otr1 = double(otr1);
otr2 = double(otr2);

    signal_size = 30 * (DATA_RATE / FREQUENCY_CENTRAL);
    window_size = (signal_size + 4*reserve); % сигнал с хвостом
    if(otr1 == 0)
        otr1 = 300;             % минимальный индекс начала первого окна
        otr2 = 730;             % минимальный индекс начала первого окна
        window_size = 433;      % максимальный размер окна
        reserve = 0; % запас

    end

    otr2 = otr2 - reserve; %!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    % получаем комплексные отсчёты
    scope_complex = [ (      scope_real(1:4:end) + 1i*scope_real(2:4:end) ); ...
                      (     -scope_real(3:4:end) + 1i*scope_real(2:4:end) ); ...
                      (     -scope_real(3:4:end) - 1i*scope_real(4:4:end) ); ...
                      ( [ scope_real(5:4:end) 0] - 1i*scope_real(4:4:end) ) ];

    scope_complex = reshape(scope_complex,1, size(scope_real,2));
    % scope_complex = conj(scope_complex);

    % % фильтрация
    % filt = fir;
    % scope_complex = filt(real(scope_complex'))' - 1i*filt(imag(scope_complex'))';
    % scope_complex = circshift(scope_complex, -ceil(length(filt.Numerator)/2)+1);

    figure(555)
    subplot(2,1,1)
       plot(real(scope_complex),'.r')
       hold on
       plot(imag(scope_complex),'.b')
       plot(abs(scope_complex),'.g')
       fill(otr1 - reserve + window_size*[0 0 1 1],2^(ADC_WIDTH-1)*[1 -1 -1 1],'red','FaceAlpha',0.2)
       fill(otr2 - reserve + window_size*[0 0 1 1],2^(ADC_WIDTH-1)*[1 -1 -1 1],'red','FaceAlpha',0.2)
       hold off
       grid on
       % xlim([370 420])
       ylim(2^(ADC_WIDTH-1)*[-1 1])
    subplot(2,1,2)
       plot(angle(scope_complex),'.g')
       hold on
       fill(otr1 - reserve + window_size*[0 0 1 1],pi*[1 -1 -1 1],'red','FaceAlpha',0.2)
       fill(otr2 - reserve + window_size*[0 0 1 1],pi*[1 -1 -1 1],'red','FaceAlpha',0.2)
       hold off
       grid on
       ylim(pi*[-1 1])

   % persistent aa;
   %  if isempty(aa)
   %      figure(666)
   %      aa = plot(mean(abs(scope_complex(310:end))),'.g');
   %      grid on
   %  else
   %      aa.YData = [aa.YData mean(abs(scope_complex(310:end)))];
   %  end


    
    %% Вычисление корреляционной функции
    % сигнал в первом окне
    inx_window_1 = otr1 - reserve-1;
    s1 = scope_complex(inx_window_1:inx_window_1 + window_size-1);
    s1(225:end) = 0 + 1i*0;
    % сигнал во втором окне
    inx_window_2 = otr2 - reserve-1;
    s2 = scope_complex(inx_window_2:inx_window_2 + window_size-1);

    % взаимная корреляция
    xss = xcorr(s2,s1);
    % берём только правую часть
    xss = xss(size(s2,2):end);
    xss = floor(real(xss)) + 1i*floor(imag(xss));
    
    % нормируем
    %xss = xss / (signal_size*2^(2*ADC_WIDTH-1));
    % ищем максимум
    [max_abs_xss, inx_max_xss] = max(abs(xss));
    % phase = angle(xss(inx_max_xss_x)); % фаза ВКФ
    phase = single(atan2(single(imag(xss(inx_max_xss))),single(real(xss(inx_max_xss)))));
    
    inx_max_xss_f = NaN;
    otr2 = otr2 + reserve;
    if (reserve ~= 0)    
        % уточняем индек начала фронта
        % inx_max_xss_f(1) = precisely_index_max_v1 ( ...
        %     scope_complex(otr1+reserve:otr1+3*reserve), ...
        %     scope_complex(otr2+reserve:otr2+3*reserve), ...
        %     0.1, 0.6);
        % inx_max_xss_f(2) = precisely_index_max_v1 ( ...
        %     scope_complex(otr1+reserve:otr1+3*reserve), ...
        %     scope_complex(otr2+reserve:otr2+3*reserve), ...
        %     0.15, 0.65);
        % inx_max_xss_f(3) = precisely_index_max_v1 ( ...
        %     scope_complex(otr1+reserve:otr1+3*reserve), ...
        %     scope_complex(otr2+reserve:otr2+3*reserve), ...
        %     0.2, 0.6);
        % inx_max_xss_f(4) = precisely_index_max_v1 ( ...
        %     scope_complex(otr1+reserve:otr1+3*reserve), ...
        %     scope_complex(otr2+reserve:otr2+3*reserve), ...
        %     0.2, 0.7);
        inx_max_xss_f = precisely_index_max_v1 ( ...
            scope_complex(otr1+reserve:otr1+3*reserve), ...
            scope_complex(otr2+reserve:otr2+3*reserve), ...
            0.2, 0.6);
        % inx_max_xss_f = mean(inx_max_xss_f);
        if (inx_max_xss < 1)
            inx_max_xss = 1;
        end
    end
    


    % phase = angle(single(xss(inx_max_xss))); % фаза ВКФ
    % phase = single(atan2(single(imag(xss(inx_max_xss))),single(real(xss(inx_max_xss)))));

    % diff_xss = diff([abs(xss) 0]);
    % I = find(diff_xss(inx_max_xss + (-5:5)) > 0, 1, 'last');
    % 
    % inx_max_xss = inx_max_xss + I - 6;

    %% измеренное время
    inx_max_xcorr = otr2 - otr1 + (inx_max_xss-reserve-1);
    time_xcorr = inx_max_xcorr/DATA_RATE;
    time_front = NaN;
    if (~isinf(inx_max_xss_f) && ~isnan(inx_max_xss_f)) % && inx_max_xss_f>1)
        inx_max_f = otr2 - otr1 + (inx_max_xss_f-1);
        time_front = inx_max_f/DATA_RATE;
        fix_num_periods = floor(time_front*FREQUENCY_CENTRAL);
        if  ((inx_max_xcorr - inx_max_f) < -7) || ...
            ((inx_max_xcorr - inx_max_f) > 7)
            inx_max = inx_max_xcorr;
        else
            inx_max = inx_max_f;
        end
        
    else
        fix_num_periods = floor(time_xcorr*FREQUENCY_CENTRAL);
        inx_max = inx_max_xcorr;
    end
    

    if ( (phase < 0) && ( inx_max - fix_num_periods*4 > 1) ) || ...
       ( (phase < -pi/2) && ( inx_max - fix_num_periods*4 == 1) ) || ...
       ( (phase >= 0) && (phase < pi/2) && (inx_max - fix_num_periods * 4 == 3) )
        fix_num_periods = single(fix_num_periods + 1);
    end
    if (inx_max_xcorr - 4*(fix_num_periods + phase/single(2*pi)) > 2)
        fix_num_periods = fix_num_periods + 1;
    end
    if (inx_max_xcorr - 4*(fix_num_periods + phase/single(2*pi)) < -2)
        fix_num_periods = fix_num_periods - 1;
    end

    time_propagation = (fix_num_periods + phase/single(2*pi))/FREQUENCY_CENTRAL;
    
    time = 1e6*(otr2 - reserve - otr1 + (0:window_size-1))./DATA_RATE;

    figure(777)
      axc(1) = subplot(3,1,1);
        plot(time,abs(s1),'.r')
        hold on
        plot(time,abs(s2),'.b')
        hold off
        ylim([0 2^(ADC_WIDTH-1)])
        grid on
      axc(2) = subplot(3,1,2);
        plot(time,abs(xss),'.g')
        hold on
        % plot(time,diff_xss,'.k')
        line((fix_num_periods/FREQUENCY_CENTRAL)*1e6*[1 1],[0 1], ...
             'Color','r','LineStyle','--')
        line(time_xcorr*1e6*[1 1],max(abs(xss))*[0 1], ...
             'Color','g','LineStyle','--')
        line(time_front*1e6*[1 1],max(abs(xss))*[0 1], ...
             'Color','b','LineStyle','--')
        line(time_propagation*1e6*[1 1],max(abs(xss))*[0 1], ...
             'Color','r','LineStyle','--')
        hold off
        grid on
        % xlim((t1 + [-10 10]./DATA_RATE)*1e6)
        xlim([44 47])
        ylim(max_abs_xss *[0.9 1.05])  
        % xlim([min(time) max(time)])
        % ylim([0.02 0.035])
      axc(3) = subplot(3,1,3);
        plot(time,angle(xss),'.g')
        hold on 
        hold off
        grid on
        % xlim((t1 + [-10 10]./DATA_RATE)*1e6)
        % xlim([44 47])
        % ylim(max_abs_xss + [-0.02 0.005])  
      %   xlim([min(time) max(time)])
        ylim(pi*[-1 1])
      % linkaxes(axc,'x')
      drawnow
      % pause(0.5)

    sound_velocity = 2 * (base(2) - base(1)) ./ time_propagation; 
    otr1 = uint16(round((2*base(1)/sound_velocity)*DATA_RATE)) - reserve;
    otr2 = uint16(round((2*base(2)/sound_velocity)*DATA_RATE)) - reserve;

      % figure(17)
      %  plot (abs(scope_complex(otr1-2*reserve-1:otr1-2*reserve+window_size-1)),'.r-')
      %  hold on
      %  plot (abs(scope_complex(otr2-2*reserve:otr2-2*reserve+window_size)),'.b-')
      %  hold off
      %  grid on
      %  drawnow
    
    end