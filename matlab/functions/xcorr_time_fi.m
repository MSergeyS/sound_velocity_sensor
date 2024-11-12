function xS  = xcorr_time_fi( S1, S2,  n)

  length_S = size(S1,2); % длина сигнала
  datatype = underlyingType(S1(1).re);
  zeros_element.re = eval([datatype '(0)']);
  zeros_element.im = eval([datatype '(0)']);
  
  if (abs(n) >= length_S) % если сдвиг больше длины сигнала
     xS = zeros_element;
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
  xS = zeros_element;
  for inx = 1:length_S-n
    % перемножение комплексных чисел
    xS.re = xS.re + S1(inx).re*S2(inx).re + S1(inx).im*S2(inx).im;
    xS.im = xS.im - S1(inx).re*S2(inx).im + S2(inx).re*S1(inx).im;
  end

end