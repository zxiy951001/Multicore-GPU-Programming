__kernel void part1 (global int4 *arr1, global int *arr2, global int *out)
{
	local int8 v;
	v.lo=arr1[0];
	v.hi=arr1[1];

	local int8 v1,v2;
	v1=vload8(0,arr2);
	v2=vload8(1,arr2);

	int8 results;
	int8 ref={15,15,15,15,15,15,15,15};
	int8 cmp= ref-v;

	if(any(cmp))
	{
		//some element is >15
		results=select(v2,v1,cmp);
	}
	else
	{
		results.lo=v1.lo;
		results.hi=v2.lo;
	}

	vstore8(v,0,out);
	vstore8(v1,1,out);
	vstore8(v2,2,out);
	vstore8(results,3,out);

}


