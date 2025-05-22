function [index] = precisely_index_max_v1 (s1, s2, trh_1, trh_2)
   fl_plot = true;

   size_w = length(s1);

   % ищем абсолютное значение
   s1 = movmean(floor(abs(s1)),[1 1]);
   s2 = movmean(floor(abs(s2)),[1 1]);

   s1 = floor(abs(s1));
   s2 = floor(abs(s2));

   % fprintf('s1_abs = [');   fprintf('%d ' ,s1);   fprintf(']\n');
   % fprintf('s1_abs = [');   fprintf('%d ' ,s2);   fprintf(']\n');

   % значение порогов
   max_s1 = max(s1);
   max_s2 = max(s2);
   trh_1_s1 = trh_1*max_s1;
   trh_2_s1 = trh_2*max_s1;
   trh_1_s2 = trh_1*max_s2;
   trh_2_s2 = trh_2*max_s2;

   % ищем индекс для порога trh_2
   [~, inx_s1_trh2] = min(abs(s1-trh_2_s1));
   [~, inx_s2_trh2] = min(abs(s2-trh_2_s2));
   abs_s1_trh2 = s1(inx_s1_trh2);
   abs_s2_trh2 = s2(inx_s2_trh2);

   % ищем индекс для порога trh_1
   abs_s1_trh1 = abs_s1_trh2;
   abs_s2_trh1 = abs_s2_trh2;
   inx_s1_trh1 = 0;
   inx_s2_trh1 = 0;
   while(abs_s1_trh1 > trh_1_s1)
      inx_s1_trh1 = inx_s1_trh1 - 1;
      cur_inx = inx_s1_trh1 + inx_s1_trh2;
      if (cur_inx < 1)
          break;
      end
      abs_s1_trh1 = s1(cur_inx);
   end 
   while(abs_s2_trh1 > trh_1_s2)
      inx_s2_trh1 = inx_s2_trh1 - 1;
      cur_inx = inx_s2_trh1 + inx_s2_trh2;
      if (cur_inx < 1)
          break;
      end
      abs_s2_trh1 = s2(cur_inx);
   end

   % находим начало фрона (решаем уравнение)
   % уравнение прямой
   % если известны 2 точки прямой (x1,y1) и (x2,y2)
   %      x - x1      y - y1
   %     --------- = ---------
   %      x2 - x1     y2 - y1
   % для y = 0 получим
   %      x = x1 - y1*(x2-x1)/(y2 - y1)
   inx_s1_0 = inx_s1_trh1 + abs_s1_trh1*inx_s1_trh1/(abs_s1_trh2 - abs_s1_trh1);
   inx_s2_0 = inx_s2_trh1 + abs_s2_trh1*inx_s2_trh1/(abs_s2_trh2 - abs_s2_trh1);
   index = (inx_s2_0 - inx_s1_0) + (inx_s2_trh2 - inx_s1_trh2);

   % fprintf('%d %d %d %d %.2f\n',inx_s1_trh1, inx_s1_trh2, abs_s1_trh1, abs_s1_trh2, inx_s1_0)
   % fprintf('%d %d %d %d %.2f\n',inx_s2_trh1, inx_s2_trh2, abs_s2_trh1, abs_s2_trh2, inx_s2_0)
   
   if (fl_plot) % строим график
      figure(999)
         plot(index+(1:size_w),s1/max_s1,'.r-')
         hold on
         plot(index + inx_s1_trh2 + [inx_s1_0 inx_s1_trh1 0],[0 abs_s1_trh1 abs_s1_trh2]/max_s1,'or--')
         plot(s2/max_s2,'.b-')
         plot(inx_s2_trh2 + [inx_s2_0 inx_s2_trh1 0],[0 abs_s2_trh1 abs_s2_trh2]/max_s2,'ob--')
         hold off
         grid on
   end
   index = round(index);
   % fprintf('%d\n',index);
end