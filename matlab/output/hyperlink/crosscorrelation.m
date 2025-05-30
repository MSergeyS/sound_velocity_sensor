function [time_propagation, otr1, otr2 ] = crosscorrelation (scope_real, base, otr1, otr2)

global HYDRA_SVM_ADC_OUT_BUFF_SIZE
global ADC_WIDTH
global FREQUENCY_CENTRAL
global DATA_RATE

scope_real = double(scope_real);
otr1 = double(otr1);
otr2 = double(otr2);

    reserve = 12; % запас
    signal_size = 30 * (DATA_RATE / FREQUENCY_CENTRAL);
    window_size = (signal_size + 4*reserve); % сигнал с хвостом
    if(otr1 == 0)
        otr1 = 336;             % минимальный индекс начала первого окна
        otr2 = 811;             % минимальный индекс начала первого окна
        window_size = 370;      % максимальный размер окна
        reserve = 0; % запас

    end

    otr2 = otr2 - reserve;
   
    % получаем комплексные отсчёты
    scope_complex = [ (      scope_real(1:4:end) + 1i*scope_real(2:4:end) ); ...
                      (     -scope_real(3:4:end) + 1i*scope_real(2:4:end) ); ...
                      (     -scope_real(3:4:end) - 1i*scope_real(4:4:end) ); ...
                      ( [ scope_real(5:4:end) 0] - 1i*scope_real(4:4:end) ) ];

    scope_complex = reshape(scope_complex,1, size(scope_real,2));

    % figure(555)
    % subplot(2,1,1)
    %    plot(real(scope_complex),'.r')
    %    hold on
    %    plot(imag(scope_complex),'.b')
    %    plot(abs(scope_complex),'.g')
    %    fill(otr1 - reserve + window_size*[0 0 1 1],2^(ADC_WIDTH-1)*[1 -1 -1 1],'red','FaceAlpha',0.2)
    %    fill(otr2 - reserve + window_size*[0 0 1 1],2^(ADC_WIDTH-1)*[1 -1 -1 1],'red','FaceAlpha',0.2)
    %    hold off
    %    grid on
    %    % xlim([370 420])
    %    ylim(2^(ADC_WIDTH-1)*[-1 1])
    % subplot(2,1,2)
    %    plot(angle(scope_complex),'.g')
    %    hold on
    %    fill(otr1 - reserve + window_size*[0 0 1 1],pi*[1 -1 -1 1],'red','FaceAlpha',0.2)
    %    fill(otr2 - reserve + window_size*[0 0 1 1],pi*[1 -1 -1 1],'red','FaceAlpha',0.2)
    %    hold off
    %    grid on
    %    ylim(pi*[-1 1])

    
    %% Вычисление корреляционной функции
    % сигнал в первом окне
    inx_window_1 = otr1 - reserve;
    s1 = scope_complex(inx_window_1:inx_window_1 + window_size-1);
    s1(225:end) = 0 + 1i*0;
    % сигнал во втором окне
    inx_window_2 = otr2 - reserve;
    s2 = scope_complex(inx_window_2:inx_window_2 + window_size-1);

    % взаимная корреляция
    xss = xcorr(s2,s1);
    % берём только правую часть
    xss = xss(size(s2,2):end);
    
    % нормируем
    xss = xss / (signal_size*2^(2*ADC_WIDTH-1));
    % ищем максимум
    [max_abs_xss, ~] = max(abs(xss));
    % phase = angle(xss(inx_max_xss_x)); % фаза ВКФ
    
    if (reserve ~= 0)    
        % уточняем индек начала фронта
        sd1 = scope_complex(otr1-reserve:otr1+reserve);
        sd2 = scope_complex(otr2-reserve:otr2+3*reserve);
        size_w = length(sd1);
        
        K_norm = max(sd2)/max(sd1);
        sd1 = sd1.*K_norm;

        as1 = int32(abs(sd1));
        as2 = int32(abs(sd2));

        xssd = zeros(1,size_w);
        for i = 1:size_w
           xssd(i) = sum(abs(as1-as2(i:i+size_w-1)));

        end
        [~, inx_max_xss] = min(abs(xssd));
        inx_max_xss = inx_max_xss - 1;

        % figure(888)
        %   subplot(2,1,1)
        %      plot(inx_max_xss+(1:size_w),abs(sd1),'.r-')
        %      hold on
        %      plot(abs(sd2),'.b-')
        %      hold off
        %      grid on
        %   subplot(2,1,2)
        %      plot(abs(xssd),'.g-')
        %      grid on
    end
    phase = angle(xss(inx_max_xss)); % фаза ВКФ

    %% измеренное время
    inx_max = otr2 - otr1 + (inx_max_xss-1);
    time_xcorr = inx_max/DATA_RATE;

    fix_num_periods = floor(time_xcorr*FREQUENCY_CENTRAL);
    if ( (phase < 0) && ( inx_max - fix_num_periods*4 > 1) ) || ...
       ( (phase < -pi/2) && ( inx_max - fix_num_periods*4 == 1) ) || ...
       ( (phase >= 0) && (phase < pi/2) && (inx_max - fix_num_periods * 4 == 3) )
        fix_num_periods = fix_num_periods + 1;
    end

    time_propagation = (fix_num_periods + phase/(2*pi))/FREQUENCY_CENTRAL;
    
    % time = 1e6*(otr2 - otr1 + (0:window_size-1))./DATA_RATE;
    %
    % figure(777)
    %   axc(1) = subplot(3,1,1);
    %     plot(time,abs(s1),'.r')
    %     hold on
    %     plot(time,abs(s2),'.b')
    %     % plot(time,angle(s1),'.g')
    %     hold off
    %     % ylim([0 2^(ADC_WIDTH-1)])
    %     % ylim([-pi pi])
    %     grid on
    %   axc(2) = subplot(3,1,2);
    %     plot(time,abs(xss),'.g')
    %     hold on
    %     line((fix_num_periods/FREQUENCY_CENTRAL)*1e6*[1 1],[0 1], ...
    %          'Color','r','LineStyle','--')
    %     line(time_xcorr*1e6*[1 1],[0 1], ...
    %          'Color','b','LineStyle','--')
    %     line(time_propagation*1e6*[1 1],[0 1], ...
    %          'Color','k','LineStyle','--')
    %     hold off
    %     grid on
    %     % xlim((t1 + [-10 10]./DATA_RATE)*1e6)
    %     % xlim([44 47])
    %     % ylim(max_abs_xss *[0.9 1.05])  
    %     % xlim([min(time) max(time)])
    %     % ylim([0.02 0.035])
    %   axc(3) = subplot(3,1,3);
    %     plot(time,angle(xss),'.g')
    %     hold on 
    %     hold off
    %     grid on
    %     % xlim((t1 + [-10 10]./DATA_RATE)*1e6)
    %     % xlim([44 47])
    %     % ylim(max_abs_xss + [-0.02 0.005])  
    %   %   xlim([min(time) max(time)])
    %     ylim(pi*[-1 1])
    %   % linkaxes(axc,'x')
    %   drawnow
    %   % pause(0.5)

    % sound_velocity = 2 * (base(2) - base(1)) ./ time_propagation; 
    % otr1_x = uint16(round((2*base(1)/sound_velocity)*DATA_RATE));
    % otr2_x = uint16(round((2*base(2)/sound_velocity)*DATA_RATE));
    % 
      % figure(17)
      %  plot (abs(scope_complex(otr1_x-2*reserve:otr1_x-2*reserve+window_size)),'.r-')
      %  hold on
      %  plot (abs(scope_complex(otr2_x-2*reserve:otr2_x-2*reserve+window_size)),'.b-')
      %  hold off
      %  grid on
      %  drawnow
    
    end