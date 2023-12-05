#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <SDL.h>

int main()
{
	//��ʼ��
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("Init failed: %s", SDL_GetError());
		return -1;
	}

	SDL_Log("Init successed!");

	//��������
	SDL_Window* Window = SDL_CreateWindow("PI Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, 0);

	//��ȡsurface
	SDL_Surface* Surface = SDL_GetWindowSurface(Window);

	//�޸��ڴ���ɫ
	uint32_t* px = (uint32_t*)Surface->pixels;

	for(int i = 0; i < 200; i++)
		for (int j = 0; j < 200; j++)
		{
			px[i * 600 + j] = 0xffff0000; //ARGB
		}
	
	//���������ݸ��µ���Ļ��
	SDL_UpdateWindowSurface(Window);

	//SDL_Delay(3000);
	SDL_Event event;
	while (true)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				break;
			}
		}
	}

	//����surface
	SDL_FreeSurface(Surface);

	//���ٴ���
	SDL_DestroyWindow(Window);

	//�˳�����
	SDL_Quit();
	SDL_Log("Quit!");

	return 0;
}