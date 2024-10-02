#pragma once

#include <QPoint>

class QMouseEvent;
class QCADView;
//class QRubberBand;

enum ECommandType	// ������
{
	ctSelect = 0,

	//  ��������������
	ctCreateLine = 1,		// ����ֱ�߶�
	ctCreateRectangle = 2,	// ��������
	ctCreateCircle = 3,		// ����Բ
	ctCreateArc = 4,		// ����Բ��
	ctCreateEllipse = 5,    //������Բ
	ctCreatePolygon = 6,    //����� 
	ctCreateText = 7,       //�����ı�

	//  �޸�������
	ctMove = 11,			// �ƶ�
	ctRotate = 12,			// ��ת
	ctMirror = 13,			// ����

	//�޸���ͼ������
	ctZoomPan = 20,
	ctZoomWindow = 21
};


class MCommand
{
protected:
	int		m_nStep; // ���������
	bool    m_bCaptured; //��׽
	QPointF m_ptCapture; //��׽��
	//QRubberBand* rubberBand; //��Ƥ��

	void     ShowCapturePoint(const QPointF& pos, bool bShow = false);
	bool     GetNearKeyPoint(QPointF& pt);
	QCADView* m_pDC;
public:
	MCommand() {
		m_nStep = 0;
		m_bCaptured = false;
	}
	MCommand(QCADView* pScene) {
		m_nStep = 0;
		m_bCaptured = false;
		m_pDC = pScene;
	}
	~MCommand() {}

	virtual int GetType() = 0; // ������������ ECommandType
	virtual int OnLButtonDown(QMouseEvent* mouseEvent) = 0;
	virtual int OnMouseMove(QMouseEvent* mouseEvent);
	virtual int OnRButtonDown(QMouseEvent* mouseEvent) = 0;
	virtual int OnLButtonDblClk(QMouseEvent* mouseEvent) { return 0; }
	virtual int Cancel() = 0;

};


