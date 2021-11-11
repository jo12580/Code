#include<stdio.h>
#include<stdlib.h>
void main()
{float x,y,z;
char a;
printf("我用了这么久的时间\n");
printf("为的就是现在\nFor you\n");

a=getchar();

for(y=1.5f;y>-1.5f;y-=0.1f)											    
{
for(x=-1.5f;x<1.5f;x+=0.05f)
{z=x*x+y*y-1;
putchar(z*z*z-x*x*y*y*y<=0.0f?'f':' ');}
putchar('\n');
system("color 4");
}
printf("我始终坚信科学，不相信生辰八字，但唯有你，我希望有来世。\n");
printf("满心都是你，希望你能接受\n");
}
