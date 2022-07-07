#ifndef DESKTOPSPRITE_H
#define DESKTOPSPRITE_H

#include "ui_desktopsprite.h"
#include "luaScript.h"
#include "IFrame.h"
#include "mersenne_twister.h"

typedef std::vector<QPixmap *> PICLIST;


struct RenderData
{
	bool bMirror;
	QPixmap* pic;
	int x;
	int y;
};

typedef std::vector<RenderData> RENDERDATA_LIST;

class DeskTopSprite : public QWidget, public IFrame
{
	Q_OBJECT

public:
	DeskTopSprite(QWidget *parent = 0, Qt::WFlags flags =Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::Tool);
	~DeskTopSprite();



private:
	Ui::DeskTopSpriteClass ui;
	QPushButton DebugCloseBtn;
	QPoint oldPos;
	bool press;
	bool bShowMenu;
	float deltaTime;
	float lastTime;
	QTime timer;
	QTimer *updateTimer ;

	PICLIST picList;
	RENDERDATA_LIST renderQueue;
	
	QAction* act_about;
	QAction* act_exit;
	QAction* act_autoRun;
	QAction* act_systemTray;
	QMenu *menu;
	QSettings *reg;
	QSystemTrayIcon* trayIcon;

	QString currentDir;

	MersenneTwister* mt;

	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event = 0);
	void contextMenuEvent(QContextMenuEvent *event);
	void FrameUpdate();

	virtual void RenderPic( int x, int y, QPixmap* pic , bool bMirror);
	virtual QPixmap* LoadPic( const QString& fn );

	virtual float GetTime()const;

	virtual void SetDebugMode();

	virtual void Resize( int x, int y );

	virtual  QPoint GetWinPos() const;

	virtual int GetScreenWidth() const;

	virtual int GetScreenHeight() const;

	virtual void Move( int x, int y );

	virtual int Random_Int(int min, int max);

	virtual double Random_Double( double min,double max );

	std::string GetCerrentDir();
private slots:
	void About();
	void AboutToHide ();
	void AboutToShow ();
	void Close();
	void AutoRunCheck(bool bCheck);
	void ShowTrayCheck(bool bCheck);
};



#endif // DESKTOPSPRITE_H
