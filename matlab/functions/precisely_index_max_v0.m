function [index] = precisely_index_max_v0 (sd1, sd2)
   fl_plot = true;

   size_w = length(sd1);
       
   K_norm = max(sd2)/max(sd1);
   sd1 = sd1.*K_norm;

   as1 = int32(abs(sd1));
   as2 = int32(abs(sd2));

   xssd = zeros(1,size_w);
   for i = 1:size_w
      xssd(i) = sum(abs(as1-as2(i:i+size_w-1)));
   end
   [~, index] = min(abs(xssd));
   index = index - 1;
   
   if (fl_plot)
      figure(888)
         subplot(2,1,1)
            plot(index+(1:size_w),abs(sd1),'.r-')
            hold on
            plot(abs(sd2),'.b-')
            hold off
            grid on
         subplot(2,1,2)
            plot(abs(xssd),'.g-')
            grid on
   end
end