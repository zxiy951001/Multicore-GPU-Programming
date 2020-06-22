
__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;


__constant float BlurringFilter7x7[49] = { 0.000036, 0.000363, 0.001446, 0.002291, 0.001446, 0.000363, 0.000036,
										 0.000363, 0.003676, 0.014662, 0.023226, 0.014662, 0.003676, 0.000363,
										 0.001446, 0.014662, 0.058488, 0.092651, 0.058488, 0.014662, 0.001446,
										 0.002291, 0.023226, 0.092651, 0.146768, 0.092651, 0.023226, 0.002291,
										 0.001446, 0.014662, 0.058488, 0.092651, 0.058488, 0.014662, 0.001446,
										 0.000363, 0.003676, 0.014662, 0.023226, 0.014662, 0.003676, 0.000363,
										 0.000036, 0.000363, 0.001446, 0.002291, 0.001446, 0.000363, 0.000036 };


__constant float GaussianBlur2PassFilter7x7[7] = { 0.00598, 0.060626, 0.241843, 0.383103, 0.241843, 0.060626, 0.00598 };

__kernel void blur_image(__global int* choices, read_only image2d_t src_image, 
						write_only image2d_t dst_image)
{
	//get option chosen by user
	int UserChoices = choices[0];
	int min = 0;
	int max = 0;

	// Accumulated pixel value
	float4 sum = (float4)(0.0);

	//Set filter to 0
	int index = 0;

	int2 coord;
	float4 pixel;

	//if user chose option window pass approach
	
	//set range value in for loop based on choices by user

		min = -3;
		max = 3;

		//Iterate over the rows
	if (UserChoices < 4)
	{
		//Get row and column position
		int column = get_global_id(0);
		int row = get_global_id(1);

		for (int i = min; i <= max; i++) {
			coord.y = row + i;

			//Iterate over the columns
			for (int j = min; j <= max; j++) {
				coord.x = column + j;

				//Read pixel from the image
				pixel = read_imagef(src_image, sampler, coord);
				//Acculumate weighted sum

					sum.xyz += pixel.xyz * BlurringFilter7x7[index++];


			}
		}
		//Write pixel to output
		coord = (int2)(column, row);
		write_imagef(dst_image, coord, sum);

	}
	//if user chose option window pass approach
	else

	{
		
		int column = get_global_id(0);
		int row = get_global_id(1);
		coord.y = row;
		index = 0;
		//Iterate over the columns
		for (int j = min; j <= max; j++) 
		{
			coord.x = column + j;

			//Read pixel from the image
			pixel = read_imagef(src_image, sampler, coord);
			//Acculumate weighted sum

				sum.xyz += pixel.xyz * GaussianBlur2PassFilter7x7[index++];

		}
		coord.x = column;
		index = 0;
		//Iterate over the columns
		for (int j = min; j <= max; j++)
		{
			coord.y = row + j;

			//Read pixel from the image
			pixel = read_imagef(src_image, sampler, coord);
			
		}
		//Write pixel to output
		coord = (int2)(column, row);
		sum.xyz /= 2;
		write_imagef(dst_image, coord, sum);
	}
}
