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
start_line = 1;
start_col = 2;
length_line = 1800;
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

% test_data = readmatrix('../../data/28.04.2025/adc_025000_13_10COM6.txt');
% test_data = readmatrix('../../data/30.04.2025/adc_025000_9_42COM6.txt');
% test_data = readmatrix('../../data/30.04.2025/adc_025001_15_45COM1bo1.txt');
% test_data = readmatrix('../../data/30.04.2025/adc_025002_15_41COM2.txt');
% test_data = readmatrix('../../data/30.04.2025/adc_025002_15_45COM2.txt');
% test_data = readmatrix('../../data/06.05.2025/test#1/adc_025001_11_39COM12bo1(#6).txt');
% test_data = readmatrix('../../data/06.05.2025/test#1/adc_025002_11_38COM15(#4).txt');
% test_data = readmatrix('../../data/06.05.2025/adc_025001_17_29COM12#6.txt');
% test_data = readmatrix('../../data/06.05.2025/adc_025002_17_29COM15#4.txt');
% test_data = readmatrix('../../data/15.05.2025/adc_01_15_53COM1.txt');
% test_data = readmatrix('../../data/15.05.2025/adc_04_12_17COM1.txt');
% test_data = readmatrix('../../data/15.05.2025/adc_04_12_29COM1.txt');
% test_data = readmatrix('../../data/15.05.2025/adc_04_12_31COM1.txt');
% test_data = readmatrix('../../data/15.05.2025/adc_06_12_18COM2.txt');
% test_data = readmatrix('../../data/15.05.2025/adc_06_12_29COM2.txt');
% test_data = readmatrix('../../data/15.05.2025/adc_06_12_31COM2.txt');
% test_data = readmatrix('../../data/15.05.2025/adc_025000_15_53COM2.txt');
% test_data = readmatrix('../../data/16.05.2025/adc_025053_14_10COM7.txt');
% test_data = readmatrix('../../data/19.05.2025/adc_00_14_4COM6.txt');
% test_data = readmatrix('../../data/20.05.2025/adc_00_9_10COM6.txt');
% test_data = readmatrix('../../data/20.05.2025/adc_025049_16_59COM7.txt');
test_data = readmatrix('../../data/22.05.2025/adc_025049_15_57COM7.txt');
% test_data = readmatrix('../../data/22.05.2025/adc_025054_16_42COM7.txt');
number_line = size(test_data,1);

test_data = [test_data(start_line:1:size(test_data,1),start_col + (0:length_line-1))];
% test_data = [NaN(number_line-start_line+1,num_blank) test_data(start_line:1:size(test_data,1),start_col + (0:length_line-1))];

% r_data = readcell('../../data/28.04.2025/adc_025000_15_12COM6.txt');
% number_line = length(r_data);
% h = waitbar(0, 'Cчитываем гидролокационные данные...');
% i = 0;
% for a_line = 1 : number_line
%     i = i+1;
%     array_str = split(string(r_data{a_line}),' ');
%     test_data(i, :) = [NaN(num_blank,1); ...
%            str2double(array_str(start_col + (1:length_line-num_blank)))];
%    waitbar(a_line / number_line)
% end
% close(h)

[number_line, length_line] = size(test_data);

figure(321)
    plot(test_data(1,:),'.r');
    grid on
    drawnow
    ylim(2^(ADC_WIDTH)*[0 1])

if ~libisloaded('hydra_svm_math')
    [notfound, warnings] = loadlibrary('bin//hydra_svm_math/hydra_svm_math.dll', ... 
                           'bin/hydra_svm_math/hydra_svm_math.h', 'alias', 'hydra_svm_math');
    % libfunctionsview hydra_svm_math
end
% libfunctions hydra_svm-math

if ~libisloaded('hydra-svm-math')
    [notfound, warnings] = loadlibrary('bin/hydra-svm-math.dll', ... 
                           'bin/hydra-svm-math.h', 'alias', 'hydra-svm-math');
    % libfunctionsview hydra-svm-math
end
% libfunctions hydra-svm-math

%test_fir

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
    status = calllib('hydra-svm-math', 'hydra_svm_measure_normalization', ...
                      out_data_ptr, in_data_ptr, length_line);

    % figure(123)
    %     plot(out_data_ptr.Value,'.r');
    %     grid on
    %     drawnow
    %     ylim(2^(ADC_WIDTH-1)*[-1 1])

    test_data(a_line,:) = out_data_ptr.Value;
    waitbar(a_line / number_line)
end
close(h)

figure(321)
    plot(test_data(1,:),'.r');
    grid on
    drawnow
    ylim(2^(ADC_WIDTH)*[-1 1])

% делаем количество точек в строке кратное 4
mod_4 = mod(size(test_data,2), 4);
if ( mod_4 ~= 0 )
    test_data = int16([test_data zeros(number_line, 4-mod_4)]);
end
% количество считанных строк
[number_line,length_line] = size(test_data);

%% массивы скоростей звука
% референсный
% sound_velocity = readmatrix('../../data/20.05.2025/adc_025049_16_59COM7stab.txt');
% sound_velocity = sound_velocity(:,4)';

sound_velocity_ini = 1479.75;
sound_velocity_tgt = 1479;
sound_velocity = sound_velocity_ini: ...
                 (sound_velocity_tgt-sound_velocity_ini)/(number_line-1): ...
                 sound_velocity_tgt;

sound_velocity_estimation_my   = NaN(1,number_line);
sound_velocity_estimation      = NaN(1,number_line);

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
    h_sv_my        = plot(1:number_line,NaN(1,number_line), '.g');
    h_sv_matlab    = plot(1:number_line,NaN(1,number_line), 'or');
    legend('xcoor\_my', 'matlab', 'vkf4')
    hold off
    grid on
    ylabel('скорость звука [м/с]')
  h_axx(2) = subplot(3,1,3);
    h_err_sv_my        = plot(1:number_line,NaN(1,number_line), '.g');
    hold on   
    h_err_sv_matlab    = plot(1:number_line,NaN(1,number_line), 'or');
    legend('xcoor\_my', 'matlab')
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
    test_plots.h_abs  = plot(1:length_line,NaN(1,length_line),'.g');
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

xcorr_structure.index_time_xcorr = uint16(0);
xcorr_structure.index_time_propagation = uint16(0);
xcorr_structure.abs = single(1000);
xcorr_structure.phase = single(0);
xcorr_ptr = libstruct('Hydra_out_xcorr_t',xcorr_structure);

xcorr_structure_1.index_time_xcorr = uint16(0);
xcorr_structure_1.index_time_propagation = uint16(0);
xcorr_structure_1.abs = single(1000);
xcorr_structure_1.phase = single(0);
xcorr_ptr_1 = libstruct('Hydra_out_xcorr_t',xcorr_structure_1);

test_workmass_ptr = libpointer('int16Ptr', int16(zeros(1,length_line)));

data_complex.Re = int32(0);
data_complex.Im = int32(0);
data_complex_ptr = libstruct('Hydra_Svm_Complex32_t',data_complex);

otr1t = NaN;
otr2t = NaN;
otr1_ptr.Value = otr1t;
otr2_ptr.Value = otr2t;

svm_work_data.time1 = single(0);
svm_work_data.time2 = single(0);
svm_work_data.tost = double(0);
svm_work_data.L2 = single(0);
svm_work_data.temp = single(0);
% svm_work_data.freq = single(0);
svm_work_data.Vcurrent = single(0);
svm_work_data.Vaverage = single(0);
svm_work_data.Vlastgood = single(0);
svm_work_data.otr1 = uint16(0);
svm_work_data.otr2 = uint16(0);
svm_work_data.nDataCount = int32(0);
svm_work_data.good_measure = uint8(1);
svm_work_data.water_exist = uint8(1);
svm_work_data_ptr = libstruct('hydra_svm_work_data_t',svm_work_data);

for inx_line = 1:1:number_line
    test_mass = int16(test_data(inx_line, :));
    test_workmass_ptr.Value = test_mass;


%% считает код dll
    otr1_ptr.Value = otr1t;
    otr2_ptr.Value = otr2t;

% tic 
    if (otr1_ptr.Value ~= 0)
       otr1t = otr1_ptr.Value;
       otr2t = otr2_ptr.Value;
    end

    
    % sound_velocity_estimation_my(inx_line) 


    % calllib('hydra-svm-math', 'hydra_svm_vkf_my', test_workmass_ptr,  otr1_ptr.Value, otr2_ptr.Value, xcorr_ptr, data_complex_ptr);
    % tost = calllib('hydra-svm-math', 'hydra_time_propagation_calculation', xcorr_ptr);
    % % fprintf('%d  %f\n' ,xcorr_ptr.index_time_xcorr, 2 * (base(2) - base(1)) / tost);
    % fprintf('%d  %d  %f  %f\n', ...
    %          xcorr_ptr.index_time_xcorr, ...
    %          xcorr_ptr.index_time_propagation, ...
    %          xcorr_ptr.abs, ...
    %          xcorr_ptr.phase);
    
    % calllib('hydra_svm_math', 'hydra_svm_vkf_my', test_workmass_ptr,  otr1_ptr.Value, otr2_ptr.Value, xcorr_ptr_1);
    % sound_velocity_estimation_my(inx_line) = calllib('hydra_svm_math', 'hydra_svm', ...
    %                               test_workmass_ptr, base(1), base(2), otr1_ptr, otr2_ptr, xcorr_ptr);
    % otr1t_dll = otr1_ptr.Value;
    % otr2t_dll = otr2_ptr.Value;

    % % fprintf('%d  %f\n', xcorr_ptr_1.index_time_xcorr, sound_velocity_estimation_my(inx_line));
    % fprintf('%d  %d  %f  %f\n', ...
    %          xcorr_ptr_1.index_time_xcorr, ...
    %          xcorr_ptr_1.index_time_propagation, ...
    %          xcorr_ptr_1.abs, ...
    %          xcorr_ptr_1.phase);
    
    calllib('hydra-svm-math', 'hydra_svm_calc_v', test_workmass_ptr, svm_work_data_ptr, xcorr_ptr);
    tost = calllib('hydra-svm-math', 'hydra_time_propagation_calculation', xcorr_ptr);
    sound_velocity_estimation_my(inx_line) = svm_work_data_ptr.Vcurrent;

    otr1t_dll = svm_work_data_ptr.otr1;
    otr2t_dll = svm_work_data_ptr.otr2;

    % test_otr (test_workmass_ptr.Value, otr1t_dll, otr2t_dll, reserve, test_plots);
    
    

% toc

%% расчёт в matlab
    % otr1_ptr.Value = otr1t;
    % otr2_ptr.Value = otr2t;
    % [time_propagation, otr1_matlab, otr2_matlab] = ...
    %                     crosscorrelation (test_workmass_ptr.Value, base, ...
    %                                       otr1_ptr.Value, otr2_ptr.Value, reserve);
    % 
    % sound_velocity_estimation(inx_line) = 2 * (base(2) - base(1)) ./ time_propagation;
    % test_otr (test_workmass_ptr.Value, otr1_matlab, otr2_matlab, reserve, test_plots);
    % 
    % [otr1_matlab otr2_matlab]
    % [otr1t_dll otr2t_dll]
    % 
    % % fprintf('время распространения = %f [мкс]\n', time_propagation * 1e6);
    % % fprintf('скорость распространения звука = %8.2f [м/c]\n', sound_velocity_estimation(inx_line));
    % % fprintf('индексы начала эхо-сигналов = [%u   %u]\n', otr1t, otr2t);

    if (fl_plot_animation)
        h_input_data.YData = test_mass;
        h_work_data.YData = get(test_workmass_ptr).Value;

        h_otr1.XData = double(otr1_ptr.Value) .* [1 1];
        h_otr2.XData = double(otr2_ptr.Value) .* [1 1];

        h_sv_my.YData        = sound_velocity_estimation_my;
        h_sv_matlab.YData    = sound_velocity_estimation;

        h_err_sv_my.YData        = (sound_velocity - sound_velocity_estimation_my)*100;
        h_err_sv_matlab.YData    = (sound_velocity - sound_velocity_estimation)*100;

        drawnow
        % pause(0.5)
    end

    otr1t = otr1t_dll;  %otr1_matlab; % 
    otr2t = otr2t_dll;  %otr2_matlab; %  

end

h_input_data.YData = test_mass;
h_work_data.YData = get(test_workmass_ptr).Value;

h_otr1.XData = double(otr1_ptr.Value) .* [1 1];
h_otr2.XData = double(otr2_ptr.Value) .* [1 1];

h_sv_my.YData        = sound_velocity_estimation_my;
h_sv_matlab.YData    = sound_velocity_estimation;

sound_velocity_flt = movmedian(sound_velocity_estimation_my,[ 10 10 ]);
h_sv_vkf4.YData = sound_velocity_flt;

h_err_sv_my.YData        = (sound_velocity_flt - sound_velocity_estimation_my)*100;
h_err_sv_matlab.YData    = (sound_velocity_flt - sound_velocity_estimation)*100;

drawnow

sum(abs(sound_velocity_flt - sound_velocity_estimation)>3)
sum(abs(sound_velocity_flt - sound_velocity_estimation_my)>3)

figure;
  histogram(sound_velocity_flt - sound_velocity_estimation_my,'Normalization','probability')
  grid on

clear all
unloadlibrary hydra-svm-math;