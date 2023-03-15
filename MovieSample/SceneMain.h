#pragma once
class SceneMain
{
public:
	SceneMain();
	virtual ~SceneMain();

	void init();
	void update();
	void draw();

private:
	void updateNormal();
	void updateDemoMovie();
private:
	void (SceneMain::*m_updateFunc)();

	int m_wallPaper;

	int	m_movieHandle;
	int	m_titleFontHandle;
	int	m_waitFontHandle;

	int m_wallOffset;
	int m_waitFrame;
	int m_demoAlpha;
};

