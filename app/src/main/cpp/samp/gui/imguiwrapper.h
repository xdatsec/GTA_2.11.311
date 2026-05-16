#pragma once

#include "imguirenderer.h"
#include "game/RW/RenderWare.h"

/*
	�����-��������� ����� imgui � ������-�������
*/

class ImGuiWrapper
{
public:
	ImGuiWrapper(const ImVec2& display_size, const std::string& font_path);

	/*
		�������������� imgui
		- �������� ��������� imgui
		- ��������� ������� ������
		- �������� ����� ������
	*/
	virtual bool initialize();

	/*
		�������� ����������� � drawList ���������
	*/
	virtual void render();

	/*
		����������� ��� ��������� ������� � ������
	*/
	virtual void shutdown();

	ImGuiRenderer* renderer() const { return m_renderer; }
	/*
		������� ������ ��������� ���������� API ������ �������
	*/
	void renderDrawData(ImDrawData* draw_data);

protected:
	virtual ~ImGuiWrapper();

	/*
		����� ������ ���� ����������. ����� �������� ��� ���������
	*/
	virtual void drawList() = 0;

	const ImVec2& displaySize() { return m_displaySize; }

private:
	/*
		������������� renderstate ��� ������ 2D �������
	*/
	void setupRenderState(ImDrawData* draw_data);
	/*
		������ �� �������� ������� � ������������ ������ ��� �������������
	*/
	void checkVertexBuffer(ImDrawData* draw_data);
	/*
		������� �������� ������
	*/
	bool createFontTexture();
	/*
		������� �������� ������
	*/
	void destroyFontTexture();

private:
	ImVec2 m_displaySize;
	std::string m_fontPath;

	ImGuiRenderer* m_renderer;
	RwRaster* m_fontRaster;

	RwIm2DVertex* m_vertexBuffer;
	int m_vertexBufferSize;
};