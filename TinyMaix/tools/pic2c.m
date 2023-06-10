pic = zeros(224,224,3);
pic = uint8(pic);
pic(50:176,1:224,1:3) = imread('test.jpg');

drawRectangleFrame(pic,[64,64],[80,104]);

fid=fopen('picture.c','w');
fprintf(fid,"const unsigned char pic[%d*%d*%d]={\\\n",size(pic,1),size(pic,2),size(pic,3));
for y=1:size(pic,1)
    for x=1:size(pic,2)
        for c=1:size(pic,3)
            fprintf(fid,'%3d,',pic(y,x,c));
        end
        fprintf(fid,' ');
    end
   fprintf(fid,'\n');
end
fprintf(fid,'};\n');
fclose(fid);
function [drawRectangleImage] = drawRectangleFrame(image,windowLocation,windowSize)
[row,col] = size(image); % 输入图像尺寸
x = windowLocation(1);
y = windowLocation(2);
height = windowSize(2);
width = windowSize(1);
if((x<=row && y<=col)&&(height<=row && width<=col))
    disp('矩形框合法！');
    figure;imshow(image);
    hold on
    drawRectangleImage = rectangle('Position',[x,y,width,height],'LineWidth',4,'EdgeColor','r');
    hold off
    

else
    disp('矩形框不合法！');
    
end
end