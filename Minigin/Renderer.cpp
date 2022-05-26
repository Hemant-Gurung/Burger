#include "MiniginPCH.h"
#include "Renderer.h"

#include <chrono>

#include "SceneManager.h"
#include "Texture2D.h"
#include "imgui.h"
#include "imgui_impl_opengl2.h"
#include "imgui_impl_sdl.h"
#include "imgui_plot.h"

constexpr size_t bufSize = 512;
float x_data[12];
static float y_data[12];
float f{ 0.4f };



int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window * window)
{
	
	m_Window = window;
	m_Renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	//initialize imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//ImGuiIO & io = ImGui::GetIO();

	//set up render platform
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
	
	
}

void dae::Renderer::Render() 
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);
	SceneManager::GetInstance().Render();

	
		//
		//ImGui_ImplOpenGL2_NewFrame();
		//ImGui_ImplSDL2_NewFrame(m_Window);
		//ImGui::NewFrame();

		//if (ImGui::Button("Trash The Cache"))
		//{
		//	TrashTheCache();
		//	m_showTrash = true;
		//	m_showGame3D = false;
		//}
		//if (m_showTrash)
		//{
		//	//ImGui::SetNextWindowPos(ImVec2{ 10,10 });
		//	Plot();
		//}
		//

		//
		//ImGui::Begin("Exercise-3");
		//ImGui::Text("%s", "GameObject3D");

		//if (ImGui::Button("Trash The Cache with Gameobject3d"))
		//{
		//	TrashTheCacheWithGameObject3D();
		//	m_showGame3D = true;
		//}

		//if (m_showGame3D)
		//{
		//	m_showTrash = false;
		//	//ImGui::SetNextWindowPos(ImVec2{ 80,10 });
		//	Plot();

		//}
		//ImGui::End();
		//
		//	//ImGui::ShowDemoWindow();
		//
		//
		//ImGui::Render();
		//ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
		//ImGui::End();

	SDL_RenderPresent(m_Renderer);
}


void dae::Renderer::Plot()
{
	ImU32 colors[3] = { ImColor(0, 255, 0), ImColor(255, 0, 0), ImColor(0, 0, 255) };
	uint32_t selection_start = 0, selection_length = 0;

	

	ImGui::PlotConfig conf;

	conf.values.xs = x_data;
	conf.values.count = 12;
	conf.values.ys = y_data;
	conf.values.ys_count = 12;
	conf.values.colors = colors;
	conf.scale.min = 0;
	conf.scale.max = 2000;
	conf.tooltip.show = true;

	conf.grid_x.size = 100;
	conf.grid_x.subticks = 100;
	conf.grid_x.show = true;

	conf.grid_y.size = 100;
	conf.grid_y.subticks = 100;
	conf.grid_y.show = true;
	conf.selection.show = true;
	conf.selection.start = &selection_start;
	conf.selection.length = &selection_length;
	conf.frame_size = ImVec2(200, 200);
	ImGui::Plot("plot1", conf);
}

void dae::Renderer::TrashTheCache()
{
	std::vector<int>arr(100000);
	int index{};
	for (int stepSize = 1; stepSize <= 1024; stepSize *= 2)
	{
		auto startTime = std::chrono::high_resolution_clock().now();
		for (size_t i = 0; i < arr.size(); i += stepSize)
		{
			arr[i] *= 2;
		}

		auto endTime = std::chrono::high_resolution_clock().now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

		//std::cout << duration << std::endl;
		y_data[index] = float(duration);
		++index;
	}

	float j{ 1 };
	for (int i = 1; i <= 1024; ++i) {
		
		j = float(j * 2);
		x_data[i] = j;
		if (j >= 1024)
		{
			break;
		}

	}
}

void dae::Renderer::TrashTheCacheWithGameObject3D()
{
	std::vector<GameObject3D> objects(100000);
	int index{};
	for (int stepSize = 1; stepSize <= 1024; stepSize *= 2)
	{
		int j{};
		auto startTime = std::chrono::high_resolution_clock().now();
		for (size_t i = 0; i < objects.size(); i += stepSize)
		{
			objects[j].transform.matrix[1] =float( rand());
			j++;
		}

		auto endTime = std::chrono::high_resolution_clock().now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

		
		y_data[index] = float(duration);
		++index;
	}
}


void dae::Renderer::Destroy()
{
	
	//Destroy sdl and opengl

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}

}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(Texture2D& texture, const float x, const float y, const float width, const float height,const float startLeft, const float startBottom,bool horizontal) const
{
	SDL_RendererFlip flip=SDL_FLIP_NONE;
	const float scale = 1.f;
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width* scale);
	dst.h = static_cast<int>(height* scale);

	SDL_Rect sourceRect;
	sourceRect.x = static_cast<int>(startLeft);
	sourceRect.y = static_cast<int>(startBottom);
	sourceRect.w = 16;
	sourceRect.h = 16;
	
	
	SDL_Point center;
	center.x = (sourceRect.x + sourceRect.w) / 2;
	center.y = (sourceRect.y + sourceRect.h) / 2;
	if (horizontal)
	{
		flip = SDL_FLIP_HORIZONTAL;
	}
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &sourceRect, &dst, 0, &center, flip);

	//SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &sourceRect, &dst);
	//SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderLine(const float posx1, const float posy1, const float posx2, const float posy2) const
{
	SDL_SetRenderDrawColor(GetSDLRenderer(), 255, 255, 255, 1);
	SDL_RenderDrawLine(GetSDLRenderer(), (int)posx1, (int)posy1, (int)posx2, (int)posy2);
}

void dae::Renderer::RenderBox(const Rectf& box,int size) const
{
	RenderLine(box.left, box.bottom, box.left + size, box.bottom);
	RenderLine(box.left + size, box.bottom, box.left + size, box.bottom + size);
	RenderLine(box.left + size, box.bottom + size, box.left, box.bottom + size);
	RenderLine(box.left, box.bottom + size, box.left, box.bottom);
}

SDL_Texture* dae::Renderer::FlipTexture(Texture2D& texture, const float x, const float y, const float width, const float height, const float startLeft, const float startBottom,bool vertical) const
{
	SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width * 1.5f);
	dst.h = static_cast<int>(height * 1.5f);

	SDL_Rect sourceRect;
	sourceRect.x = static_cast<int>(startLeft);
	sourceRect.y = static_cast<int>(startBottom);
	sourceRect.w = 16;
	sourceRect.h = 16;

	SDL_Point center;
	center.x = (dst.x + dst.w) / 2;
	center.y = (dst.y + dst.h) / 2;
	if (!vertical)
	{
		flip = SDL_FLIP_VERTICAL;
	}
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &sourceRect, &dst, 180,&center, flip);
	return texture.GetSDLTexture();
}
