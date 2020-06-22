__kernel void vecadd(int num, __global int *arr )
    {
  
        int i = get_global_id(0);
        arr[i] = (i*num) + 1;
    
    
    }

