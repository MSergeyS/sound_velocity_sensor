clear
close all
clc
addpath([cd '/functions/'])
global HYDRA_SVM_ADC_OUT_BUFF_SIZE
global ADC_WIDTH
global FREQUENCY_CENTRAL
global DATA_RATE

fl_plot_animation = true;

HYDRA_SVM_ADC_OUT_BUFF_SIZE = 1320;
ADC_WIDTH = 12;
FREQUENCY_CENTRAL = single(3e6);
DATA_RATE = single(4 * FREQUENCY_CENTRAL);

base = single([24; 58] / 1000);

% test_data = uint16(readmatrix('./data/test_data_lfm_jitter.txt'));
% test_data = uint16(readmatrix('./data/test_data_lfm.txt'));
% % sound_velocity_ini = 1450;%1400;
% % sound_velocity_tgt = 1520;%1500;
% test_data = uint16(readmatrix('./data/test_data_fkm_jitter.txt'));
% test_data = uint16(readmatrix('./data/test_data_lfm_jitter_1476_1.txt'));
% num_blank = 0;
% start_col = 1;
% test_data = readmatrix('../data/28.08.2024/1/adc_565535_9_53COM7.txt');
% num_blank = 330;
% start_col = 6;
% test_data = readmatrix('../../data/20241002/adc_565535_17_40COM21.txt');
% test_data = readmatrix('../../data/20241002/adc_565535_18_5COM21.txt');
% test_data = int16(readmatrix('../../data/08.10.2024/adc_565535_15_32COM21.txt'));
% test_data = readmatrix('../../data/11.10.2024/adc_565535_15_51COM5.txt');   %+++++++++++++++++++++++++++++++
% test_data = readmatrix('../../data/17.10.2024/adc_565535_16_26COM5.txt');
% test_data = readmatrix('../../data/18.10.2024/adc_565535_16_28COM21.txt');  %+++++++++++++++++++++++++++++++
% test_data = readmatrix('../../data/31.10.2024/23051/adc_565535_10_12COM2.txt');
% test_data = readmatrix('../../data/31.10.2024/23054/adc_565535_10_12COM5.txt');
% test_data = readmatrix('../../data/05.11.2024/old/adc_523047_15_28COM3.txt');
% test_data = readmatrix('../../data/05.11.2024/old/adc_523050_15_21COM3.txt');
% test_data = readmatrix('../../data/05.11.2024/old/adc_523051_15_36COM3.txt');
% test_data = readmatrix('../../data/05.11.2024/old/adc_523052_15_24COM3.txt');
% test_data = readmatrix('../../data/05.11.2024/old/adc_523054_15_15COM3.txt');
% test_data = readmatrix('../../data/05.11.2024/new/adc_523047_16_36COM3.txt');
% test_data = readmatrix('../../data/05.11.2024/new/adc_523047_16_39COM3.txt');
% test_data = readmatrix('../../data/05.11.2024/new/adc_523050_16_18COM3.txt');
% test_data = readmatrix('../../data/05.11.2024/new/adc_523050_16_28COM3.txt');
% test_data = readmatrix('../../data/05.11.2024/new/adc_523051_15_59COM3.txt');
% test_data = readmatrix('../../data/05.11.2024/new/adc_523052_16_32COM3.txt');
% test_data = readmatrix('../../data/05.11.2024/new/adc_523054_16_14COM3.txt');

% test_data = readmatrix('../../data/06.11.2024/23047/adc_523047_13_55COM3.txt');
% test_data = readmatrix('../../data/06.11.2024/23052/adc_523052_15_23COM3.txt');
% test_data = readmatrix('../../data/06.11.2024/23054/adc_523054_16_38COM3.txt');

% test_data = readmatrix('../../data/08.11.2024/47_1/adc_023047_12_17COM3.txt');
% test_data = readmatrix('../../data/08.11.2024/47_2/adc_023047_12_46COM3.txt');
% test_data = readmatrix('../../data/08.11.2024/47_2/adc_023047_12_59COM3.txt');
% test_data = readmatrix('../../data/08.11.2024/47_3/adc_023047_13_30COM3.txt');
test_data = readmatrix('../../data/14.11.2024/adc_023047_15_30COM3.txt');

num_blank = 330;
start_col = 2;
% sound_velocity_ini = 1475;
% sound_velocity_tgt = 1477;
sound_velocity_ini = 1421;
sound_velocity_tgt = 1505;

% количество считанных строк
[number_line,~] = size(test_data);
% добавляем 330 пустых точек в начало
test_data = [nan(number_line, num_blank) test_data(:,start_col:end)];

% делаем количество точек в строке кратное 4
mod_4 = mod(size(test_data,2), 4);
if ( mod_4 ~= 0 )
    test_data = [test_data zeros(number_line, 4-mod_4)];
end
% количество считанных строк
[number_line,length_line] = size(test_data);

% убираем постоянную составляющую в сигнале
% % offset = 2^(ADC_WIDTH-1);
% offset = double(mean(mean(test_data,2,"omitnan"),"omitnan"));
% % test_data = uint16(double(test_data) - offset + 2^(ADC_WIDTH-1));
% % test_data(:,2:2:end) = uint16(double(test_data(:,2:2:end)) + 15);
% test_data = int16(test_data) - offset;
% test_data(:,1:num_blank) = NaN;

% % figure
% for kk = 1:number_line
%   % % % % offset_1 = int16(round(123*sin(2*pi*2*(1:2:length_line)/length_line)));
%   % % % % offset_2 = int16(round(123*sin(2*pi*2*(2:2:length_line)/length_line)));
%   % offset_1 = int16(movmean(test_data(kk,1:2:end),[0 3]));
%   % % % offset_1 = int16(movmean(offset_1,[15 15]));
%   % offset_2 = int16(movmean(test_data(kk,2:2:end),[0 3]));
%   % % % offset_2 = int16(movmean(offset_2,[15 15]));
%   % test_data(kk,1:2:end) = test_data(kk,1:2:end) - offset_1;
%   % test_data(kk,2:2:end) = test_data(kk,2:2:end) - offset_2;
% 
%   plot(abs(test_data(kk,1:2:end)),'.r')
%   % plot(test_data(kk,1:2:end),'.r')
%   % plot(offset_1,'.r')
%   hold on
%   plot(abs(test_data(kk,2:2:end)),'.b')
%   % plot(test_data(kk,2:2:end),'.b')
%   % plot(offset_2,'.b')
%   hold off
%   grid on
%   xlim([1 length_line/2])
%   % ylim([0 2^ADC_WIDTH])
%   drawnow
%   pause(0.5)
% end
test_data = int16(test_data);
test_data(:,1:num_blank) = NaN;

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
    %h_sv_etalon    = plot(1:number_line,sound_velocity, '.k');
    hold on
    h_sv_my        = plot(1:number_line,NaN(1,number_line), 'og');
    h_sv_matlab    = plot(1:number_line,NaN(1,number_line), '.b');
    h_sv_vkf4      = plot(1:number_line,NaN(1,number_line), '.r-');
    legend('xcoor\_my', 'matlab', 'vkf4')
    hold off
    grid on
    ylabel('скорость звука [м/с]')
  h_axx(2) = subplot(3,1,3);
    h_err_sv_my        = plot(1:number_line,NaN(1,number_line), '.g');
    hold on   
    h_err_sv_matlab    = plot(1:number_line,NaN(1,number_line), '.b');
    h_err_sv_vkf4      = plot(1:number_line,NaN(1,number_line), '.r');
    legend('xcoor\_my', 'matlab', 'vkf4')
    hold off
    grid on
    ylabel('ошибка [см/с]')

linkaxes(h_axx, 'x')
xlim([1 number_line])


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

reserve = 12; % запас
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

if ~libisloaded('hydra_svm_math')
    [notfound, warnings] = loadlibrary('bin/hydra_svm_math.dll', ... 
                           'bin/hydra_svm_math.h', 'alias', 'hydra_svm_math');
end

% libfunctionsview hydra_svm_math
libfunctions hydra_svm_math

% otr1_ptr = libpointer('uint16Ptr', 380);
% otr2_ptr = libpointer('uint16Ptr', 920);
otr1_ptr = libpointer('uint16Ptr', 0);
otr2_ptr = libpointer('uint16Ptr', 0);
xcorr_structure.index_time_propagation = uint16(0);
xcorr_structure.abs = single(1000);
xcorr_structure.phase = single(0);
xcorr_ptr = libstruct('Hydra_out_xcorr_t',xcorr_structure);
test_workmass_ptr = libpointer('int16Ptr', int16(zeros(1,length_line)));

for inx_line = 1:1:number_line
    % offset = double(mean(test_data(inx_line, :),2,"omitnan"));
    % test_mass = int16(test_data(inx_line, :) ) - offset - 0*round(xcorr_ptr.abs*0.03);
    % % test_mass(1:num_blank) = NaN;

    test_mass = int16(test_data(inx_line, :));
    test_workmass_ptr.Value = int16(test_mass);


%% считает код dll
tic 
    otr1t = NaN;
    otr2t = NaN;
    if (otr1_ptr.Value ~= 0)
       otr1t = otr1_ptr.Value;
       otr2t = otr2_ptr.Value;
    end
    sound_velocity_estimation_my(inx_line) = calllib('hydra_svm_math', 'hydra_svm', ...
                                test_workmass_ptr, base(1), base(2), otr1_ptr, otr2_ptr, xcorr_ptr);
    % % double([otr1_ptr.Value otr2_ptr.Value])
    % test_otr (test_workmass_ptr.Value, otr1_ptr.Value, otr2_ptr.Value, test_plots);
    otr1_dll = otr1_ptr.Value;
    otr2_dll = otr2_ptr.Value;
toc

% расчёт в matlab
    if (~isnan(otr1t))
       otr1_ptr.Value = otr1t;
       otr2_ptr.Value = otr2t;
    end
    [time_propagation, otr1_matlab, otr2_matlab] = ...
                        crosscorrelation (test_workmass_ptr.Value, base, ...
                                           otr1_ptr.Value, otr2_ptr.Value);
    test_otr (test_workmass_ptr.Value, otr1_ptr.Value, otr2_ptr.Value, test_plots);
    sound_velocity_estimation(inx_line) = 2 * (base(2) - base(1)) ./ time_propagation;

    % % fprintf('время распространения = %f [мкс]\n', time_propagation_fi * 1e6);
    % fprintf('скорость распространения звука = %8.2f [м/с]\n', sound_velocity_estimation_fi);

    otr1_ptr.Value = otr1_dll;
    otr2_ptr.Value = otr2_dll;

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

h_sv_vkf4.YData = movmedian(sound_velocity_estimation_my,[ 10 10 ]);

h_err_sv_vkf4.YData      = (sound_velocity - sound_velocity_estimation_vkf4)*100;
h_err_sv_my.YData        = (sound_velocity - sound_velocity_estimation_my)*100;
h_err_sv_matlab.YData    = (sound_velocity - sound_velocity_estimation)*100;
h_err_sv_matlab_fi.YData = (sound_velocity - sound_velocity_estimation_fi)*100;

drawnow

clear all
unloadlibrary hydra_svm_math;