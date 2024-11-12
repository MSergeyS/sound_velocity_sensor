function [index] = precisely_index_max_v1 (sd1, sd2)
   fl_plot = true;

   size_w = length(sd1);

   % ищем абсолютное значение
   sd1 = abs(sd1);
   sd2 = abs(sd2);
   % нормируем
   sd1 = sd1/max(sd1);
   sd2 = sd2/max(sd2);

   [~, inx_sd1_05] = min(abs(sd1-0.6));
   [~, inx_sd2_05] = min(abs(sd2-0.6));
   abs_sd1_05 = sd1(inx_sd1_05);
   abs_sd2_05 = sd2(inx_sd2_05);
   abs_sd1_01 = abs_sd1_05;
   abs_sd2_01 = abs_sd1_05;
   inx_sd1_01 = 0;
   inx_sd2_01 = 0;
   while(abs_sd1_01 > 0.2)
      inx_sd1_01 = inx_sd1_01 - 1;
      abs_sd1_01 = sd1(inx_sd1_05 + inx_sd1_01);
   end 
   while(abs_sd2_01 > 0.2)
      inx_sd2_01 = inx_sd2_01 - 1;
      abs_sd2_01 = sd2(inx_sd2_05 + inx_sd2_01);
   end

   inx_sd1_0 = inx_sd1_01 + abs_sd1_01*inx_sd1_01/(abs_sd1_05 - abs_sd1_01);
   inx_sd2_0 = inx_sd2_01 + abs_sd2_01*inx_sd2_01/(abs_sd2_05 - abs_sd2_01);
   index = (inx_sd2_0 - inx_sd1_0) + (inx_sd2_05 - inx_sd1_05);
   
   if (fl_plot)
      figure(999)
         plot(index+(1:size_w),sd1,'.r-')
         hold on
         plot(index + inx_sd1_05 + [inx_sd1_0 inx_sd1_01 0],[0 abs_sd1_01 abs_sd1_05],'or--')
         plot(sd2,'.b-')
         plot(inx_sd2_05 + [inx_sd2_0 inx_sd2_01 0],[0 abs_sd2_01 abs_sd2_05],'ob--')
         hold off
         grid on
   end

   index = round(index);
end