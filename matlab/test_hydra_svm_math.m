clear
close all
clc
addpath([cd '/functions/'])
global HYDRA_SVM_ADC_OUT_BUFF_SIZE
global ADC_WIDTH
global FREQUENCY_CENTRAL
global DATA_RATE

fl_plot_animation = false;

HYDRA_SVM_ADC_OUT_BUFF_SIZE = 1320;
ADC_WIDTH = 12;
FREQUENCY_CENTRAL = single(3e6);
DATA_RATE = single(4 * FREQUENCY_CENTRAL);
base = single([24; 58] / 1000);
reserve = 17; % запас
start_index = 0;
start_line = 2550;%1275;
start_col = 2;
length_line = 1600;
num_blank = 150;

% новый вариант оцифровки ГЛ данных
% АЦП не останавливается, работает на 6МГц, сдвиг на 1/4 периода несущей - засчёт
% сдвига формирования второго ЗИ.
% т.е. порядок действий теперь такой
% 1. Запускается АЦП на 6МГц с ДМА
% 2. Запускается по таймеру ШИМ - формирование первого ЗИ - получаем re отсчёты
% 3. Интервал накопления ГЛ данных (re)
% 4. Запускается по таймеру ШИМ - формирование второго ЗИ - получаем im отсчёты
%    (ЗИ сдвинут на 1/4 периода несущей)
% 5. Интервал накопления ГЛ данных (im)
%
% test_data = readmatrix('../../data/28.04.2025/adc_025000_13_10COM6.txt');
% number_line = size(test_data,1);
% 
% % в комплексном виде
% tmp_data = test_data;
% test_data(:,1:2:2000) = tmp_data(:,1:1000);
% test_data(:,2:2:2000) = tmp_data(:,1801:2800);
% 
% test_data = [NaN(number_line-start_line+1,num_blank) test_data(start_line:1:size(test_data,1),start_col + (0:length_line-1))];

r_data = readcell('../../data/28.04.2025/adc_025000_15_12COM6.txt');
number_line = length(r_data);
h = waitbar(0, 'Cчитываем гидролокационные данные...');
i = 0;
for a_line = 1 : number_line
    i = i+1;
    array_str = split(string(r_data{a_line}),' ');
    test_data(i, :) = [NaN(num_blank,1); ...
           str2double(array_str(start_col + (1:length_line-num_blank)))];
   waitbar(a_line / number_line)
end
close(h)

% number_line = length(tmp_data);
% length_line = 2000;
% test_data = NaN(number_line, length_line);
% num_blank = 300;
% start_col = 2;

% h = waitbar(0, 'Cчитываем гидролокационные данные...');
% for a_line = 1 : number_line
%     array_str = split(string(tmp_data{a_line}),' ');
%     if ( (length(array_str) - 1) <= length_line)
%       continue
%     end
%     % figure(321)
%     %    plot(str2double(array_str(2:end)),'.r');
%     %    grid on
%     %    drawnow
%     %    ylim(2^(ADC_WIDTH)*[0 1])

%     test_data(a_line, :) = str2double(array_str(start_col - 1 + num_blank + (1:length_line)));
%     waitbar(a_line / number_line)
% end
% close(h)

[number_line, length_line] = size(test_data);

figure(321)
    plot(test_data(1,:),'.r');
    grid on
    drawnow
    ylim(2^(ADC_WIDTH)*[0 1])

if ~libisloaded('hydra_svm_math')
    [notfound, warnings] = loadlibrary('bin/hydra_svm_math.dll', ... 
                           'bin/hydra_svm_math.h', 'alias', 'hydra_svm_math');
    libfunctionsview hydra_svm_math
end

% libfunctionsview hydra_svm_math
libfunctions hydra_svm_math

h = waitbar(0, 'Обрабатываем гидролокационные данные...');
for a_line = 1 : number_line

    in_data_ptr = libpointer('uint16Ptr', uint16(zeros(1,length_line)));
    in_data = uint16(test_data(a_line, :));
    in_data_ptr.Value = uint16(in_data);

    % figure(321)
    %     plot(in_data,'.r');
    %     grid on
    %     drawnow
    %     ylim(2^(ADC_WIDTH)*[0 1])

    out_data_ptr = libpointer('int16Ptr', int16(zeros(1,length_line)));
    status = calllib('hydra_svm_math', 'hydra_svm_measure_normalization', ...
                      in_data_ptr, out_data_ptr, length_line);

    % figure(123)
    %     plot(out_data_ptr.Value,'.r');
    %     grid on
    %     drawnow
    %     ylim(2^(ADC_WIDTH-1)*[-1 1])


%     okno = int16(4);
%     normalized_fullsize = int16(zeros(1,1600));
%     for imas = int16(okno*2:2:mas_size-1)
%         srednee_okno1 = single(0.);
%         srednee_okno2 = single(0.);
%         for iokno = int16(0:okno-1)
%             if (imas > 360)
%               inx_1 = iokno * 2 + (imas - okno);
%               inx_2 = iokno * 2 + (imas + 1 - okno);
%               [inx_1 inx_2]
%               [single(in_data(inx_1)) single(in_data(inx_2))]
%             end
%             srednee_okno1 = srednee_okno1 + single(in_data(iokno * 2 + (imas - okno)));
%             srednee_okno2 = srednee_okno2 + single(in_data(iokno * 2 + (imas + 1 - okno)));
%         end
%         srednee_okno1 = srednee_okno1 / single(okno);
%         srednee_okno2 = srednee_okno2 / single(okno);
% 
%         normalized_fullsize(imas) = (int16(in_data(imas)) - int16(srednee_okno1));
%         normalized_fullsize(imas + 1) = (int16(in_data(imas + 1)) - int16(srednee_okno2));
% 
%         figure (111)
%            plot(in_data(1:imas), 'xr')
%            hold on
%            plot(imas,srednee_okno1, 'ob')
%            plot(imas,srednee_okno2, 'om')
%            plot(normalized_fullsize(1:imas), '.b')
%            hold off
%            grid on
%            %ylim([2^(ADC_WIDTH-1)*[-1 1]])
%            drawnow
%     end
% 
% figure(223)
%     plot(normalized_fullsize,'.r');
%     grid on
%     drawnow
%     ylim(2^(ADC_WIDTH-1)*[-1 1])

    test_data(a_line,:) = out_data_ptr.Value;
    waitbar(a_line / number_line)
end
close(h)

% sound_velocity = readmatrix('../../data/09.04.2025/adc_00_14_6COM7stab.txt');
% sound_velocity = readmatrix('../../data/15.04.2025/adc_025000_14_36COM6stab.txt');
% sound_velocity = sound_velocity(:,4)';

sound_velocity_ini = 1479.75;
sound_velocity_tgt = 1479;

% количество считанных строк
[number_line,~] = size(test_data);
% добавляем 330 пустых точек в начало
% test_data = [nan(number_line, num_blank) test_data(:,start_col:end)];
% sound_velocity = sound_velocity(1:number_line);

% test_data = readmatrix('../../data/DATA3.20.11.24.txt')';
% num_blank = 330;
% test_data = test_data(2,:);
% test_data(1:num_blank) = 0;
% max_data = 0.65;
% test_data = (2^(ADC_WIDTH-1))*0.7*test_data/max_data;
% 
% sound_velocity = 1470;
% 
% [number_line,~] = size(test_data);


% делаем количество точек в строке кратное 4
mod_4 = mod(size(test_data,2), 4);
if ( mod_4 ~= 0 )
    test_data = int16([test_data zeros(number_line, 4-mod_4)]);
end
% количество считанных строк
[number_line,length_line] = size(test_data);

% % % убираем постоянную составляющую в сигнале
% offset = 2^(ADC_WIDTH-1);
% % offset = double(mean(mean(test_data,2,"omitnan"),"omitnan"));
% % % % test_data = uint16(double(test_data) - offset + 2^(ADC_WIDTH-1));
% % % % test_data(:,2:2:end) = uint16(double(test_data(:,2:2:end)) + 15);
% test_data = int16(test_data) - offset;
% % % test_data(:,1:num_blank) = NaN;
% 
% % figure
% for kk = 1:number_line
%   % % % offset_1 = int16(round(123*sin(2*pi*2*(1:2:length_line)/length_line)));
%   % % % offset_2 = int16(round(123*sin(2*pi*2*(2:2:length_line)/length_line)));
%   offset_1 = int16(movmean(test_data(kk,1:2:end),[0 3]));
%   % % offset_1 = int16(movmean(offset_1,[15 15]));
%   offset_2 = int16(movmean(test_data(kk,2:2:end),[0 3]));
%   % % offset_2 = int16(movmean(offset_2,[15 15]));
%   test_data(kk,1:2:end) = test_data(kk,1:2:end) - offset_1;
%   test_data(kk,2:2:end) = test_data(kk,2:2:end) - offset_2;
% 
%   % % plot(abs(test_data(kk,1:2:end)),'.r')
%   % plot(test_data(kk,1:2:end),'.r')
%   % % % plot(offset_1,'.r')
%   % hold on
%   % % plot(abs(test_data(kk,2:2:end)),'.b')
%   % plot(test_data(kk,2:2:end),'.b')
%   % % % plot(offset_2,'.b')
%   % hold off
%   % grid on
%   % xlim([1 length_line/2])
%   % ylim([0 2^ADC_WIDTH])
%   % drawnow
%   % pause(0.5)
% end
% % test_data = int16(test_data);
% test_data(:,1:num_blank) = NaN;

sound_velocity = sound_velocity_ini: ...
                 (sound_velocity_tgt-sound_velocity_ini)/(number_line-1): ...
                 sound_velocity_tgt;
sound_velocity_estimation_vkf4 = NaN(1,number_line);
sound_velocity_estimation_my   = NaN(1,number_line);
sound_velocity_estimation      = NaN(1,number_line);
sound_velocity_estimation_fi   = NaN(1,number_line);

figure;
  h_ax(1) = subplot(2, 1, 1);
    h_input_data = plot(1:length_line,NaN(1,length_line), '.g');
    grid on
    ylim(2 ^ ADC_WIDTH * [0 1])
    ylabel('входные данные')
  h_ax(2) = subplot(2, 1, 2);
    h_work_data = plot(1:length_line,NaN(1,length_line), '.g');
    hold on
    h_otr1 = line(NaN.* [1 1], 2 ^ (ADC_WIDTH - 1) .* [-1 1]);
    h_otr2 = line(NaN.* [1 1], 2 ^ (ADC_WIDTH - 1) .* [-1 1]);
    hold off
    grid on
    ylim(2 ^ (ADC_WIDTH - 1) * [-1 1])
    ylabel('данные после подготовки')
    xlim([1 length_line])
linkaxes(h_ax, 'x')

figure;
  h_axx(1) = subplot(3,1,[1 2]);
    h_sv_etalon    = plot(1:number_line,sound_velocity, '.k');
    hold on
    h_sv_my        = plot(1:number_line,NaN(1,number_line), 'og');
    h_sv_matlab    = plot(1:number_line,NaN(1,number_line), 'xb');
    h_sv_vkf4      = plot(1:number_line,NaN(1,number_line), '.r-');
    legend('xcoor\_my', 'matlab', 'vkf4')
    hold off
    grid on
    ylabel('скорость звука [м/с]')
  h_axx(2) = subplot(3,1,3);
    h_err_sv_my        = plot(1:number_line,NaN(1,number_line), '.g');
    hold on   
    h_err_sv_matlab    = plot(1:number_line,NaN(1,number_line), 'xb');
    h_err_sv_vkf4      = plot(1:number_line,NaN(1,number_line), '.r');
    legend('xcoor\_my', 'matlab', 'vkf4')
    hold off
    grid on
    ylabel('ошибка [см/с]')

linkaxes(h_axx, 'x')
% xlim([1 number_line])


figure;
  h_ax_scl(1) = subplot(2,1,1);
    test_plots.h_real = plot(1:length_line,NaN(1,length_line),'.r');
    hold on
    test_plots.h_imag = plot(1:length_line,NaN(1,length_line),'.b');
    test_plots.h_abs = plot(1:length_line,NaN(1,length_line),'.g');
    hold off
    grid on
    ylim(2^(ADC_WIDTH-1)*[-1 1])
  h_ax_scl(2) = subplot(2,1,2);
    test_plots.h_angle = plot(1:length_line,NaN(1,length_line),'.g');
    grid on
    xlim([1 length_line])
    ylim(pi*[-1 1])
linkaxes(h_ax_scl, 'x')

signal_size = 30 * (DATA_RATE / FREQUENCY_CENTRAL);
window_size = (signal_size + 4*reserve); % сигнал с хвостом
figure;
    test_plots.h_s1 = plot(0:window_size,NaN(1,window_size+1),'.r-');
    hold on
    test_plots.h_s2 = plot(0:window_size,NaN(1,window_size+1),'.b-');
    hold off
    grid on
    xlim([1 window_size])
    ylim([0 2^(ADC_WIDTH-1)])
    drawnow

otr1_ptr = libpointer('uint16Ptr', 0);
otr2_ptr = libpointer('uint16Ptr', 0);
xcorr_structure.index_time_propagation = uint16(0);
xcorr_structure.abs = single(1000);
xcorr_structure.phase = single(0);
xcorr_ptr = libstruct('Hydra_out_xcorr_t',xcorr_structure);
test_workmass_ptr = libpointer('int16Ptr', int16(zeros(1,length_line)));

otr1t = NaN;
otr2t = NaN;
otr1_ptr.Value = otr1t;
otr2_ptr.Value = otr2t;

for inx_line = 1:1:number_line
    test_mass = int16(test_data(inx_line, :));
    test_workmass_ptr.Value = test_mass;


%% считает код dll
    otr1_ptr.Value = otr1t;
    otr2_ptr.Value = otr2t;

tic 
    if (otr1_ptr.Value ~= 0)
       otr1t = otr1_ptr.Value;
       otr2t = otr2_ptr.Value;
    end
    sound_velocity_estimation_my(inx_line) = calllib('hydra_svm_math', 'hydra_svm', ...
                                test_workmass_ptr, base(1), base(2), otr1_ptr, otr2_ptr, xcorr_ptr);
    % % double([otr1_ptr.Value otr2_ptr.Value])
    % test_otr (test_workmass_ptr.Value, otr1_ptr.Value, otr2_ptr.Value, reserve, test_plots);
    otr1t = otr1_ptr.Value;
    otr2t = otr2_ptr.Value;
    % inx_shift = randi([-10,10],1,1);
    % otr1_ptr.Value = otr1_dll + inx_shift;
    % inx_shift = randi([-10,10],1,1);
    % otr2_ptr.Value = otr2_dll + inx_shift;
toc

% %% расчёт в matlab
%     otr1_ptr.Value = otr1t;
%     otr2_ptr.Value = otr2t;
%     [time_propagation, otr1_matlab, otr2_matlab] = ...
%                         crosscorrelation (test_workmass_ptr.Value, base, ...
%                                           otr1_ptr.Value, otr2_ptr.Value, reserve);
%     otr1t = otr1_matlab;  %   
%     otr2t = otr2_matlab;  %   
% 
%     test_otr (test_workmass_ptr.Value, otr1_matlab, otr2_matlab, reserve, test_plots);
%     sound_velocity_estimation(inx_line) = 2 * (base(2) - base(1)) ./ time_propagation;
% 
%     % fprintf('время распространения = %f [мкс]\n', time_propagation * 1e6);
%     % fprintf('скорость распространения звука = %8.2f [м/c]\n', sound_velocity_estimation(inx_line));
%     % fprintf('индексы начала эхо-сигналов = [%u   %u]\n', otr1t, otr2t);

    if (fl_plot_animation)
        h_input_data.YData = test_mass;
        h_work_data.YData = get(test_workmass_ptr).Value;

        h_otr1.XData = double(otr1_ptr.Value) .* [1 1];
        h_otr2.XData = double(otr2_ptr.Value) .* [1 1];

        h_sv_vkf4.YData      = sound_velocity_estimation_vkf4;
        h_sv_my.YData        = sound_velocity_estimation_my;
        h_sv_matlab.YData    = sound_velocity_estimation;

        h_err_sv_vkf4.YData      = (sound_velocity - sound_velocity_estimation_vkf4)*100;
        h_err_sv_my.YData        = (sound_velocity - sound_velocity_estimation_my)*100;
        h_err_sv_matlab.YData    = (sound_velocity - sound_velocity_estimation)*100;

        drawnow
        pause(0.5)
    end

    % fprintf('скорость распространения звука, [м/с]: %8.2f; %8.2f; %8.2f; %8.2f; %8.2f\n', ...
    %                                     sound_velocity(inx_line), ...
    %                                     sound_velocity_estimation_vkf4(inx_line), ...
    %                                     sound_velocity_estimation_my(inx_line), ...
    %                                     sound_velocity_estimation(inx_line), ...
    %                                     sound_velocity_estimation_fi(inx_line));

end

h_input_data.YData = test_mass;
h_work_data.YData = get(test_workmass_ptr).Value;

h_otr1.XData = double(otr1_ptr.Value) .* [1 1];
h_otr2.XData = double(otr2_ptr.Value) .* [1 1];

h_sv_vkf4.YData      = sound_velocity_estimation_vkf4;
h_sv_my.YData        = sound_velocity_estimation_my;
h_sv_matlab.YData    = sound_velocity_estimation;
h_sv_matlab_fi.YData = sound_velocity_estimation_fi;

sound_velocity_flt = movmedian(sound_velocity_estimation_my,[ 10 10 ]);
h_sv_vkf4.YData = sound_velocity_flt;

h_err_sv_vkf4.YData      = (sound_velocity_flt - sound_velocity_estimation_vkf4)*100;
h_err_sv_my.YData        = (sound_velocity_flt - sound_velocity_estimation_my)*100;
h_err_sv_matlab.YData    = (sound_velocity_flt - sound_velocity_estimation)*100;
h_err_sv_matlab_fi.YData = (sound_velocity_flt - sound_velocity_estimation_fi)*100;

drawnow

sum(abs(sound_velocity_flt - sound_velocity_estimation)>3)
sum(abs(sound_velocity_flt - sound_velocity_estimation_my)>3)

figure;
  histogram(sound_velocity_flt - sound_velocity_estimation_my,'Normalization','probability')
  grid on

clear all
unloadlibrary hydra_svm_math;