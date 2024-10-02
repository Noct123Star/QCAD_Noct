#include "QCADView.h"
#include "ENTITY.H"
#include "MCommand.H"
#include "mainwindow.h"
#include "MCreateLine.h"
#include "MSelectCmd.h"

#include <QPainter>
#include <QStatusBar>

QCADView::QCADView()
{
	m_pCmd = NULL;
	m_lineColor = QColor(0, 0, 0);
	m_penStyle = Qt::SolidLine;
	m_penWidth = 1;
	m_brushColor = QColor(255, 255, 255);
}

QCADView::~QCADView()
{
	//���ʵ����������
	
	//�����������ظ�
	delete m_pCmd;
}

void QCADView::paintEvent(QPaintEvent* event)
{
	QPainter dc(this);
	//���ƻ���
	
	//����ϵͳ
	dc.setPen(QPen(Qt::black, 1));
	dc.drawLine(0, 0, 1000, 0); 
	dc.drawLine(0, 0, 0, 1000);

	//����ȫ��ʵ��
	foreach (MEntity* pEnt, m_EntityList)
	{
		//���ʵ����ѡ���У�����ʾ״̬
		if( !m_SelectEntityList.contains(pEnt) )
		{
			dc.setPen(pEnt->GetPen());
			dc.setBrush(pEnt->GetBrush());
		}
		else {
			QPen pen(Qt::DashLine);
			pen.setColor(Qt::green);
			pen.setWidth(1);
			QBrush brush(Qt::white);
			dc.setPen(pen);
			dc.setBrush(brush);
		}

		pEnt->Draw(&dc);
	}
}

void QCADView::mousePressEvent(QMouseEvent* mouseEvent)
{
	if (mouseEvent->button() == Qt::LeftButton && m_pCmd)
	{
		m_pCmd->OnLButtonDown(mouseEvent);
	}
	if (mouseEvent->button() == Qt::RightButton && m_pCmd)
	{
		m_pCmd->OnRButtonDown(mouseEvent);
	}
}

void QCADView::mouseMoveEvent(QMouseEvent* mouseEvent)
{
	if (m_pCmd)
	{
		m_pCmd->OnMouseMove(mouseEvent);
		return;
	}

	//��ʾ��Ļ�������������
	MainWindow* pMain = g_pMainWnd;
	QPointF scnPos = mouseEvent->pos();
	QString sScreenPosX = QString::number(scnPos.x(), 'f', 2);
	QString sScreenPosY = QString::number(scnPos.y(), 'f', 2);
	QString sScnPos = QStringLiteral("��ǰ���꣺");
	sScnPos += sScreenPosX;
	sScnPos += ", ";
	sScnPos += sScreenPosY;

	if (m_pCmd == NULL)
		pMain->statusBar()->showMessage(sScnPos);
}

void QCADView::mouseReleaseEvent(QMouseEvent* mouseEvent)
{

}

void QCADView::mouseDoubleClickEvent(QMouseEvent* mouseEvent)
{
	if (mouseEvent->button() != Qt::LeftButton)
	{
		return;
	}

	if (m_pCmd)
	{
		m_pCmd->OnLButtonDblClk(mouseEvent);
	}
}

void QCADView::addEntity(MEntity* pEnt)
{
	m_EntityList.push_back(pEnt);
}

void QCADView::drawLine()
{
	//QPainter pDC(this);

	if (m_pCmd && m_pCmd->GetType() == ctCreateLine)
		return;

	delete m_pCmd;
	m_pCmd = new MCreateLine(this);
}

void QCADView::selectEntity()
{
	if (m_pCmd && m_pCmd->GetType() == ctSelect)
		return;

	delete m_pCmd;
	m_pCmd = new MSelectCmd(this);
}

void QCADView::AddSelection(MEntity* pEnt)
{
	m_SelectEntityList.push_back(pEnt);
}


