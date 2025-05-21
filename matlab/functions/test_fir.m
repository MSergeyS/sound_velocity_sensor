a_line = 1;

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
scope_real = double(out_data_ptr.Value);

% делаем количество точек в строке кратное 4
mod_4 = mod(length(scope_real), 4);
if ( mod_4 ~= 0 )
    scope_real = int16([scope_real zeros(1, 4-mod_4)]);
end

% получаем комплексные отсчёты
scope_complex = [ (      scope_real(1:4:end) + 1i*scope_real(2:4:end) ); ...
                  (     -scope_real(3:4:end) + 1i*scope_real(2:4:end) ); ...
                  (     -scope_real(3:4:end) - 1i*scope_real(4:4:end) ); ...
                  ( [ scope_real(5:4:end) 0] - 1i*scope_real(4:4:end) ) ];
scope_complex = reshape(scope_complex,1, size(scope_real,2));
scope_complex = conj(scope_complex);

figure
   plot(real(scope_complex),'.r')
   hold on
   plot(imag(scope_complex),'.b')
   plot(abs(scope_complex),'.g')
   hold off
   grid on

filt = fir;
scope_complex = filt(real(scope_complex)) + 1i*filt(imag(scope_complex));

figure
   plot(real(scope_complex),'.r')
   hold on
   plot(imag(scope_complex),'.b')
   plot(abs(scope_complex),'.g')
   hold off
   grid on