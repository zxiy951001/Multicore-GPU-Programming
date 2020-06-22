
//create global variable for filter
__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;



__constant float filterWindow7x7[49] = { 0.000036, 0.000363, 0.001446, 0.002291, 0.001446, 0.000363, 0.000036,
										 0.000363, 0.003676, 0.014662, 0.023226, 0.014662, 0.003676, 0.000363,
										 0.001446, 0.014662, 0.058488, 0.092651, 0.058488, 0.014662, 0.001446,
										 0.002291, 0.023226, 0.092651, 0.146768, 0.092651, 0.023226, 0.002291,
										 0.001446, 0.014662, 0.058488, 0.092651, 0.058488, 0.014662, 0.001446,
										 0.000363, 0.003676, 0.014662, 0.023226, 0.014662, 0.003676, 0.000363,
										 0.000036, 0.000363, 0.001446, 0.002291, 0.001446, 0.000363, 0.000036 };



__constant float filterPass7x7[7] = { 0.00598, 0.060626, 0.241843, 0.383103, 0.241843, 0.060626, 0.00598 };

__kernel void blur_image(__global int* option, read_only image2d_t src_image,
	write_only image2d_t dst_image)
{
	//get option chosen by user
	int getOption = option[0];
	int minValue = 0;
	int maxValue = 0;

	// Accumulated pixel value
	float4 sum = (float4)(0.0);

	//Set filter to 0
	int index = 0;

	int2 coord;
	float4 pixel;

		minValue = -3;
		maxValue = 3;

		//Get one direction for pass
		int column = get_global_id(0);
		int row = get_global_id(1);
		coord.y = row;
		index = 0;
		//Iterate over the columns
		for (int j = minValue; j <= maxValue; j++)
		{
			coord.x = column + j;

			//Read pixel from the image
			pixel = read_imagef(src_image, sampler, coord);
			//Acculumate weighted sum

				sum.xyz += pixel.xyz * filterPass7x7[index++];

		}
		//Write pixel to output
		coord = (int2)(column, row);
		write_imagef(dst_image, coord, sum);
	}

__kernel void blur_vertical(__global int* option, read_only image2d_t src_image,
	write_only image2d_t dst_image)
{
	//get option chosen by user
	int getOption = option[0];
	int minValue = 0;
	int maxValue = 0;

	// Accumulated pixel value
	float4 sum = (float4)(0.0);

	//Set filter to 0
	int index = 0;

	int2 coord;
	float4 pixel;



		minValue = -3;
		maxValue = 3;

	//Get one direction for pass
	int column = get_global_id(0);
	int row = get_global_id(1);
	
	coord.x = column;
	index = 0;
	//Iterate over the columns
	for (int j = minValue; j <= maxValue; j++)
	{
		coord.y = row + j;

		//Read pixel from the image
		pixel = read_imagef(src_image, sampler, coord);
		//Acculumate weighted sum

		//Write pixel to output
		coord = (int2)(column, row);
		write_imagef(dst_image, coord, sum);
	}
}
