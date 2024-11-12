function xS  = xcorr_time( S1, S2,  n) 
  length_S = size(S1,2); % длина сигнала
  if (abs(n)>=length_S) % если сдвиг больше длины сигнала
     xS = 0;
     return
  else 
    if (n < 0) % сдвиг влево
      S2 = S2(1+n:end);
      S1 = S1(1:end-n);
    else % сдвиг вправо
      S1 = S1(1+n:end);
      S2 = S2(1:end-n);
    end
  end
  % перемножение комплексных чисел
  re = sum( real(S1).*real(S2) + imag(S1).*imag(S2));
  im = sum(-real(S1).*imag(S2) + real(S2).*imag(S1));
  xS = re + 1i*im;
end