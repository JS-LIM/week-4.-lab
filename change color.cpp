#include <GLFW/glfw3.h>
#include <cstring>
#include <stdlib.h>        // srand, rand
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include "math.h"
#include <stdbool.h>

const int width = 900;
const int height = 400;

double xpos, ypos;
float* pixels = new float[width*height * 3];

void drawPixel(const int& i, const int& j, const float& red, const float& green, const float& blue)
{
	pixels[(i + width* j) * 3 + 0] = red;
	pixels[(i + width* j) * 3 + 1] = green;
	pixels[(i + width* j) * 3 + 2] = blue;
}

// scratched from https://courses.engr.illinois.edu/ece390/archive/archive-f2000/mp/mp4/anti.html
// see 'Rasterization' part.

void drawrowLine(const int& i0, const int& j0, const int& i1, const int& j1, const float& red, const float& green, const float& blue)
{
	for (int i = i0; i <= i1; i++)
	{
		const int j = (j1 - j0)*(i - i0) / (i1 - i0) + j0;

		drawPixel(i, j, red, green, blue);
	}
}


void drawcircle(const double &x_c, const double &y_c, const double &r, const float& red, const float& green, const float& blue)
{
	for (double i = 0; i <= (x_c + r); i++)
	{
		for (double j = 0; j <= (y_c + r); j++)
		{
			double f1 = ((i - x_c)*(i - x_c)) + ((j - y_c)*(j - y_c)) - (r*r);
			double f2 = ((i - x_c)*(i - x_c)) + ((j - y_c)*(j - y_c)) - ((r - 3)*(r - 3));

			if (f1 <= 0 && f2>0)
			{
				drawPixel(i, j, red, green, blue);
			}
		}
	}
}

void drawcolLine(const int& i0, const int& j0, const int& i1, const int& j1, const float& red, const float& green, const float& blue) // 세로줄을 생성하는 함수
{
	for (int j = j0; j <= j1; j++)
	{
		const int i = (i1 - i0)*(j - j0) / (j1 - j0) + i0; //위의 가로줄 생성 함수에서 i,j를 바꿔 분모가 0일때 생성안되는 경우를 보완

		drawPixel(i, j, red, green, blue);
	}
}


void drawOnPixelBuffer()
{
	//std::memset(pixels, 1.0f, sizeof(float)*width*height * 3); // doesn't work
	std::fill_n(pixels, width*height * 3, 1.0f);   // white background

												   //for (int i = 0; i<width*height; i++) {
												   //   pixels[i * 3 + 0] = 1.0f; // red
												   //   pixels[i * 3 + 1] = 1.0f; // green
												   //   pixels[i * 3 + 2] = 1.0f; // blue
												   //}

	const int i = rand() % width, j = rand() % height;
	drawPixel(i, j, 0.0f, 0.0f, 0.0f);

	// drawing a line
	//TODO: anti-aliasing
	const int i0 = 100, i1 = 200;
	const int j0 = 50, j1 = 80;
	//drawLine(i0, j0, i1, j1, 1.0f, 0.0f, 0.0f);

	//TODO: try moving object
}


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Style Programming", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glClearColor(1, 1, 1, 1); // while background

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			pixels[(i + width* j) * 3 + 0] = 1.0f;
			pixels[(i + width* j) * 3 + 1] = 1.0f;
			pixels[(i + width* j) * 3 + 2] = 1.0f; //2중 for문을 통해 흰색 배경 설정
		}
	}
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		int r = 50;
		std::fill_n(pixels, width*height * 3, 1.0f);

		glfwGetCursorPos(window, &xpos, &ypos);
		ypos = height - ypos;
		for (int i = width / 6 - 50; i < width - 150; i += 150)
		{
			for (int j = height / 4; j < height; j += 200)
			{
				drawcircle(i, j, r, 1.0f, 0.0f, 0.0f);
				if ((xpos - i)*(xpos - i) + (ypos - j)*(ypos - j) < (r*r))
				{
					drawcircle(i, j, r, 0.0f, 0.0f, 1.0f);
				}
			}
		}

		drawrowLine(70, 100, 130, 100, 1.0f, 0.0f, 0.0f);
		drawrowLine(110, 120, 130, 100, 1.0f, 0.0f, 0.0f);
		drawrowLine(110, 80, 130, 100, 1.0f, 0.0f, 0.0f);

		drawrowLine(240, 100, 260, 100, 1.0f, 0.0f, 0.0f);
		drawrowLine(230, 80, 250, 120, 1.0f, 0.0f, 0.0f);
		drawrowLine(250, 120, 270, 80, 1.0f, 0.0f, 0.0f);

		drawcolLine(400, 70, 400, 130, 1.0f, 0.0f, 0.0f);

		drawrowLine(520, 100, 580, 100, 1.0f, 0.0f, 0.0f);
		drawrowLine(520, 100, 540, 120, 1.0f, 0.0f, 0.0f);
		drawrowLine(520, 100, 540, 80, 1.0f, 0.0f, 0.0f);

		drawcolLine(700, 70, 700, 130, 1.0f, 0.0f, 0.0f);
		drawrowLine(680, 110, 700, 130, 1.0f, 0.0f, 0.0f);
		drawrowLine(700, 130, 720, 110, 1.0f, 0.0f, 0.0f);

		drawrowLine(70, 280, 130, 320, 1.0f, 0.0f, 0.0f);

		drawcircle(250, 300, 20, 1.0f, 0.0f, 0.0f);

		drawrowLine(370, 270, 430, 270, 1.0f, 0.0f, 0.0f);
		drawrowLine(370, 330, 430, 330, 1.0f, 0.0f, 0.0f);
		drawcolLine(370, 270, 370, 330, 1.0f, 0.0f, 0.0f);
		drawcolLine(430, 270, 430, 330, 1.0f, 0.0f, 0.0f);

		drawrowLine(520, 270, 580, 330, 1.0f, 0.0f, 0.0f);
		drawrowLine(520, 330, 580, 270, 1.0f, 0.0f, 0.0f);

		drawcolLine(700, 270, 700, 330, 1.0f, 0.0f, 0.0f);
		drawrowLine(680, 290, 700, 270, 1.0f, 0.0f, 0.0f);
		drawrowLine(700, 270, 720, 290, 1.0f, 0.0f, 0.0f);

		/* Loop until the user closes the window */
			/* Render here */
			//glClear(GL_COLOR_BUFFER_BIT);

			//TODO: RGB struct
			//Make a pixel drawing function
			//Make a line drawing function

		glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
		glfwTerminate();
	delete[] pixels; // or you may reuse pixels array 

	return 0;
}