#pragma once

#include <QVector>
class CCommand;
//�������У����ڼ�¼�������޸�ͼԪʵ��Ĳ�������
//����������ʱ���½����������˳�ʱ��ɾ�������ȫ���ڴ棩
// 
/*
MCommand����Ľ���
��Ա�������ӣ�ʵ��ָ�룬������������������Ҫ����
��Ա�������ӣ�Undo(), Redo();
*/

//ÿ�β���(�������޸Ļ�ɾ����ʱ�������m_curPos������ǰ���У�Ȼ�󽫲����������У�
// Undoʱ��m_curPos--����������Ԫ���ڵ�ʵ���Undo()��
// Redoʱ��m_curPos++����������Ԫ���ڵ�ʵ���Redo()��
//


/*
*  �����������      Undo                        Redo
*  ����ʵ��          �Ƴ�ʵ�����ʾ�б�          ��ʵ���ټ�����ʾ�б�
*  �ƶ�/��ת/����    �����������ԭ��λ��        ִ�в��������¼����������ʵ�弯��
*  ɾ��              ��ʵ���ټ�����ʾ            �Ƴ�ʵ�����ʾ�б�
*  �����������塢���͡���ɫ��  ����Ҫ������������������ʵ�弯����Ӧʵ��Undo/redo
*/

class CCommandQrue  //Ӧ�� queue
{
private:
	unsigned int m_curPos; //��¼��ǰλ�ã�����Undo/Redo
	QVector<CCommand*> m_vCommands;  //ȫ���Ĳ�������

public:
	CCommandQrue();
	~CCommandQrue();

public:
	void Undo(); //����
	void Redo(); //����
	
	void AddCommands(CCommand* pCmd);  //�����������
	void AdjustCommands();   //�������У������������ȥ����������Ҫɾ��ʵ��
	void DeleteAll();        //ɾ��ȫ���������У�����ɾ��ʵ���ڴ�
};
