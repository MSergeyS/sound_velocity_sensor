clear
close all
clc

n = int32(3);
a = [2 4 1;
     5 2 1;
     2 3 4];
 y = [36 47 37];
 x = zeros(n,1);

if ~libisloaded('mgauss')
    [notfound,warnings] = loadlibrary('bin/mgauss.dll', 'bin/mgauss.h', 'alias', 'mgauss');
end

libfunctionsview mgauss

Aptr = libpointer('doublePtrPtr',a);
Yptr = libpointer('doublePtr',y);
Xptr = libpointer('doublePtr',x);

% pv = libpointer('doublePtrPtr',zeros(4,3));
% calllib('mgauss','Foo',4,3,pv)
% pv.value

calllib('mgauss','gauss', Aptr, Yptr, Xptr, n);

Xptr.Value

unloadlibrary('mgauss');

%codegen mex_gauss -args {a, y, n} gauss.cpp gauss.h -report