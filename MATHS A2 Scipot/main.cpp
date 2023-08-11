#include <sciplot/sciplot.hpp>
#include <stdlib.h>
#include <vector>
#include <iostream>
using namespace sciplot;

//Initialising a vector function.
std::vector<double> MultiplyMatrix(std::vector<std::vector<double>> _matrix, std::vector<double> _vector);

//Defining the main function passing in its required parameters to run the sciplot library.
int main(int argc, char** argv)
{
    // Creating vectors.
    std::vector<int> xplot{};//A vector of all the x value plots
    std::vector<int> yplot{};//the y plots
	std::vector<int> zplot{};//the z plots
	std::vector<int> points{};//holds all the points entered by the user.

	//User input and inputting multiple points...
	//initalising variables.
	int point;
	int lines = 0;
	while (lines < 2)
	{
		std::cout << "How many lines do you want to do plot (more than 1): ";
		std::cin >> lines; //Assigning the user input to lines.
		if (lines < 2)
		{
			std::cout << "ERROR: Only " << lines << " Entered!" << std::endl;
		}
	}
	std::cout << "Input the following: "<<std::endl;
	//Depending on how many lines the user has chosen to enter, the user will be asked in input
	//their x and y values.
	for (int i = 0; i < lines; i++)
	{
		std::cout << "point x: ";
		std::cin >> point;
		points.push_back(point);
		std::cout << "point y: ";
		std::cin >> point;
		points.push_back(point);
	}
	//This statement just pushes the first point the user has entered also to the back of the points vector
	//so that after all the other lines have been plotted this last position would be plotted as a line with its end point being the last point the user has entered.
	if (lines > 1)
	{
		points.push_back(points[0]);
		points.push_back(points[1]);
	}

	// Creating a 2D Plot object which creates a graph.
	Plot2D plot;
	//labeling the plot objects axis.
	plot.xlabel("x");
	plot.ylabel("y");
	//setting the range values of the plot object
	plot.xrange(0.0, 60);
	plot.yrange(0.0, 60);
	// This disables the deletion of the created gnuplot script and data file.
	plot.autoclean(false);

	// Creating a 3D Plot object and the similar is all setup for the 3D plot object.
	//with the adition of the z axis.
	Plot3D plot2;
	plot2.xlabel("x");
	plot2.ylabel("y");
	plot2.zlabel("z");
	plot2.xrange(0.0, 60);
	plot2.yrange(0.0, 60);
	plot2.zrange(0.0, 60);
	plot2.autoclean(false);

	// Creating a 3D Plot object
	Plot3D plot3;
	plot3.xlabel("x");
	plot3.ylabel("y");
	plot3.zlabel("z");
	plot3.xrange(-60, 60);
	plot3.yrange(-60, 60);
	plot3.zrange(-60, 60);
	plot3.autoclean(false);


	//Bresenham's line drawing algorithm========================

	//slope of a line = point2 y - point1 y = ydiff(rise), point2 x - point1 x = xdiff(run), rise/run = Slope e.g. 300/500(y/x) = 0.6(slope)
	//every pixel the y is incremented by 0.6 e.g. 0.6, 1.2, 1.8, 2.4 etc
	//we then round each incrementation to the nearest integer which then dictates where the pixel goes.

	//assigning the points to there x's and y's
	//loops through each set of points in the points vector.
	for (int l = 0; l < (lines*2); l=l+2)
	{
		int x1 = points[l];
		int y1 = points[l+1];
		int x2 = points[l+2];
		int y2 = points[l+3];

		//if the first point entered is greater than the other they are changed to comply with the algorithm:
		if (x1 > x2 && y1 != y2)
		{
			x1 = points[l + 2];
			x2 = points[l];
			y1 = points[l + 3];
			y2 = points[l + 1];
		}

		//initalises the position values that have been previously assigned to new x y and z variables.
		//z if equal to 0 as this algorithm only deals with 2D line drawing rather than 3D.
		int x = x1;
		int y = y1;
		int z = 0;

		//Determines if the line is rise over run or run over rise: key(run = x, rise = y)
		//determines if the x increases by the slope or y increases by the slope.
		bool rise = false;
		bool run = false;
		//calculating the differece between both x points and y points, then the difference between the difference of the x and y points.
		float ydiff = y2 - y1;
		float xdiff = x2 - x1;
		float diff = ydiff / xdiff;
		int dist;
		float threshold = 0.5;
		//if the slope is greater than 1 and less than -1, line is rise over run and the distance is defined by the difference in the y values
		//else the line is determined to run over rise and the distance is defined as the difference in x values.
		if ((diff > 1 || diff < -1))
		{
			diff = xdiff / ydiff;
			dist = (ydiff)+1;
			if (diff != 0)
			{
				rise = true;
			}
		}
		else
		{
			dist = (xdiff)+1;
			if (diff != 0)
			{
				run = true;
			}
		}
		float slope = diff;//initalisng a variable that is assigned the difference between points. 

		//Determining the threshold, if slope is negative then the threshold will start as negative also.
		if (slope < 0)
		{
			threshold = -0.5;//threshold represent half a pixel.
		}

		//if run is true...
		if (run == true)
		{
			for (int i = 0; i < dist; i++)//loops until the line distance has been met.
			{
				//Pushes the every x y and z value that has either been changed or stayed the same to their corrosponding vector.
				xplot.push_back(x);
				yplot.push_back(y);
				zplot.push_back(z);
				//calculating pixel position:
				//as this is run over rise, the y value will only increase once it has met or exceeded the threshold.
				//once this happens, the y value is increase or decrease depending if the point value associated with y1 is greater or less than y2.
				if (y1 > y2)
				{
					if (slope <= threshold)
					{
						y = y - 1;
						threshold -= 1;
					}
				}
				else
				{
					if (slope >= threshold)
					{
						y = y + 1;
						threshold += 1;
					}
				}
				x = x + 1;//x is increase every loop regardless.

				//the slope is also incremented by the difference:
				slope = slope + diff;
			}
		}
		//The following for rise over run is identical to the previous run over rise however this only increments or decrements the x value once the threshaold has been met or exceeded...
		//and the y value increments regardless.
		if (rise == true)
		{
			for (int i = 0; i < dist; i++)
			{
				// plotting polt
				xplot.push_back(x);
				yplot.push_back(y);
				zplot.push_back(z);
				//calculating pixel position:
				if (x1 > x2)
				{
					if (slope <= threshold)
					{
						x = x - 1;
						threshold -= 1;
					}
				}
				else
				{
					if (slope >= threshold)
					{
						x = x + 1;
						threshold += 1;
					}
				}
				y = y + 1;//x is increase every loop regardless.
				//increment the slope:
				slope = slope + diff;
			}
		}
		else if(diff == 0)//if the slope is 0 neither rise over run or run over rise then the program just plots all the points.
		{
			xplot.push_back(x1);
			yplot.push_back(y1);
			zplot.push_back(z);
			xplot.push_back(x2);
			yplot.push_back(y2);
			zplot.push_back(z);
		}
	}

	//matrix rotations, initalise a vector of vectors that are of the double data type.
	std::vector<std::vector<double>> matrix(3, std::vector<double>(3));

	//initalising the vector holding position values.
	std::vector<double> xyz{ };
	double theta = 0;//initalising variable theta holding the angle value.

	std::cout << "Rotating a 3D Point using Matrices:" << std::endl;
	std::cout << "Input the following point you would like to rotate..." << std::endl;
	std::cout << "point x: ";
	std::cin >> point;
	xyz.push_back(point);
	std::cout << "point y: ";
	std::cin >> point;
	xyz.push_back(point);
	std::cout << "point z: ";
	std::cin >> point;
	xyz.push_back(point);
	std::cout << "At what angle would you like to rotate the point on the x axis: ";
	std::cin >> theta;

	//creating vectors to hold the points due to sciplot's draw funtions only accepting indiviual vectors of values.
	std::vector<double> xPoint = { xyz[0] };
	std::vector<double> yPoint = { xyz[1] };
	std::vector<double> zPoint = { xyz[2] };

	plot2.drawPoints(xPoint, yPoint, zPoint).label("Before Rotation");//draws the intial rotation point

	//Ouputing the inital values.
	std::cout << "Original position: ";
	std::cout << xyz[0] << ", " << xyz[1] << ", " << xyz[2] << std::endl;

	//assigning the matrix x rotation values:
	//row 1
	matrix[0][0] = 1;
	matrix[0][1] = 0;
	matrix[0][2] = 0;
	//row 2
	matrix[1][0] = 0;
	matrix[1][1] = std::cos(theta);
	matrix[1][2] = -std::sin(theta);
	//row 3
	matrix[2][0] = 0;
	matrix[2][1] = std::sin(theta);
	matrix[2][2] = std::cos(theta);

	//mutliplying the matrix by the vector of positions
	xyz = MultiplyMatrix(matrix, xyz);
	std::cout << "Rotated x poisiton: ";
	std::cout << xyz[0] << ", " << xyz[1] << ", " << xyz[2] << std::endl;

	std::cout << "At what angle would you like to rotate the point on the y axis: ";
	std::cin >> theta;

	//assigning the matrix y rotation values:
	//row 1
	matrix[0][0] = std::cos(theta);
	matrix[0][1] = 0;
	matrix[0][2] = std::sin(theta);
	//row 2
	matrix[1][0] = 0;
	matrix[1][1] = 1;
	matrix[1][2] = 0;
	//row 3
	matrix[2][0] = -std::sin(theta);
	matrix[2][1] = 0;
	matrix[2][2] = std::cos(theta);

	xyz = MultiplyMatrix(matrix, xyz);
	std::cout << "Rotated y poisiton: ";
	std::cout << xyz[0] << ", " << xyz[1] << ", " << xyz[2] << std::endl;

	std::cout << "At what angle would you like to rotate the point on the z axis: ";
	std::cin >> theta;

	//assigning the matrix z rotation values:
	//row 1
	matrix[0][0] = std::cos(theta);
	matrix[0][1] = -std::sin(theta);
	matrix[0][2] = 0;
	//row 2
	matrix[1][0] = std::sin(theta);
	matrix[1][1] = std::cos(theta);
	matrix[1][2] = 0;
	//row 3
	matrix[2][0] = 0;
	matrix[2][1] = 0;
	matrix[2][2] = 1;

	xyz = MultiplyMatrix(matrix, xyz);
	std::cout << "Rotated z poisiton: ";
	std::cout << xyz[0] << ", " << xyz[1] << ", " << xyz[2] << std::endl;
	//end of x rotation and multiplying matrices with vectors.

	//Plotting the data on to the previously initalised plot objects.
	plot.drawCurveWithPoints(xplot, yplot).label("2D");//draws the 2D lines that the user has input and have been through the bresenham algorithm.

	//assigns the new rotation to their corresponding vectors.
	xPoint[0] = xyz[0];
	yPoint[0] = xyz[1];
	zPoint[0] = xyz[2];

	plot3.drawPoints(xPoint, yPoint, zPoint).label("After Rotation");//draws the new point that has been rotated

    // Create figure to hold the plot objects
	Figure fig1 = { {plot},
		{plot2, plot3 } };

    // Create canvas to hold figure and setting the size of the canvas when it is shown.
    Canvas canvas1 = { {fig1} };
	canvas1.size(1000, 1000);

    // Shows the plots/figure in a pop-up window.
    canvas1.show();

	return 0;
}

std::vector<double> MultiplyMatrix(std::vector<std::vector<double>> _matrix, std::vector <double> _vector)
{
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			_matrix[row][col] = _matrix[row][col] * _vector[col];
		}
	}
	_vector[0] = (_matrix[0][0] + _matrix[0][1] + _matrix[0][2]);
	_vector[1] = (_matrix[1][0] + _matrix[1][1] + _matrix[1][2]);
	_vector[2] = (_matrix[2][0] + _matrix[2][1] + _matrix[2][2]);
	return _vector;
}