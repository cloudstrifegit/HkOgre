#ifndef OGRE_WIDGET_H
#define OGRE_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <Ogre.h>
#include <XkTimer.h>

class OgreWidget : public QWidget
{
	Q_OBJECT

public:
	OgreWidget(QWidget* parent = 0);
	~OgreWidget();

	Ogre::RenderWindow* getRenderWindow()
	{
		return m_pWindow;
	}

	void setNode(Ogre::SceneNode* pNode)
	{
		m_pNode = pNode;
	}

    void setLightNode(Ogre::SceneNode* pNode)
    {
        m_pLightNode = pNode;
    }

    void updatePhysics();
    void updateRender();

    void shootBox();
    void createStack();
    void createTriangle();

protected:
	void resizeEvent(QResizeEvent* evt);
	void timerEvent(QTimerEvent* evt);
	void paintEvent(QPaintEvent* evt);

	void keyPressEvent(QKeyEvent* evt);
	void keyReleaseEvent(QKeyEvent* evt);
	void mousePressEvent(QMouseEvent* evt);
	void mouseReleaseEvent(QMouseEvent* evt);
	void mouseMoveEvent(QMouseEvent* evt);
	void wheelEvent(QWheelEvent* evt);

private:
	Ogre::Camera* m_pCamera;
    Ogre::RenderWindow* m_pWindow;
    Ogre::Viewport* m_pVp;
	Ogre::SceneNode* m_pNode;
    Ogre::SceneNode* m_pLightNode;
    int m_nLastMouseX;
    int m_nLastMouseY;
    bool m_bFirstPressed;
	bool m_bResizing;
    bool m_bCtrlPressed;

    Xk::Timer m_timer;
    Ogre::Real m_fTime;
};

#endif