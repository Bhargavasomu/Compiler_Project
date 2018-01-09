declblock
{
	int a,b;
	int c;
	int d;
	int pp[100],bbq[1000];
	int i,j;
	int sum1;
	int sum2;
}
codeblock
{
	a=2;
	b=3;
	pp[i]=99;
	for i = 1, 100
	{
		for j=1,100,2
		{
			pp[i] = i ;
			bbq[i] = i+1;
		}
	}
	sum1=0;
	sum2=0;
	for i = 1, 100,2
	{
		sum1 = sum1 + pp[i];
		sum2 = sum2 + bbq[i];
 	}

	while( (a>(3+2*(b*c)) and b<c) or b+c<0 )
	{
		while(a>0)
		{
			while(b>0)
			{
				if c > 0
				{
					println "Sum1 value: ",sum1;
				}
				else
				{
					println "NOT OK";
				}
				if(d>100)
				{
					print "ok1";
				}
				if(d<105)
				{
					print "ok2";
				}
				else
				{
					print "No1";
				}
				if((a=2))
				{
					print "plese";
				}
				if((a)=2)
				{	print "ahaa"; }
				if(((a))=2)
				{	print "adjustable"; }
			}
		}
	}

	print "Sum1 value: ",sum1;
	print "Sum2 value: ",sum2;
}
