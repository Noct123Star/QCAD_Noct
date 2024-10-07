#pragma once
#include <QWidget>

class MEntity;
class MCommand;

//图形视窗
class QCADView :
    public QWidget
{
private:
    QList<MEntity*> m_EntityList;
    QList<MEntity*> m_SelectEntityList;
    MCommand* m_pCmd;
    int	m_nStartOperateNum; // 起始操作位置

public:
    QCADView();
    ~QCADView();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* mouseEvent);
    void mouseMoveEvent(QMouseEvent* mouseEvent);
    void mouseReleaseEvent(QMouseEvent* mouseEvent);
    void mouseDoubleClickEvent(QMouseEvent* mouseEvent);

public:
    void addEntity(MEntity* pEnt);
    void clearList();
    void drawLine();
    void selectEntity();

private:
    bool m_isModified; // 修改标记

public:
    bool m_bEditable;
    int  m_nOperations;
    int	 m_nCurrentOperation;
    bool isModified()const;
    void SetModifiedFlag(bool bModified = true);

private:
    QColor m_lineColor;
    Qt::PenStyle m_penStyle;
    qreal  m_penWidth;
    QColor m_brushColor;

public:
    void setLineColor(QColor color) { m_lineColor = color; }
    void setPenStyle(Qt::PenStyle style) { m_penStyle = style; }
    void setPenWidth(qreal width) { m_penWidth = width; }
    void setBrushColor(QColor color) { m_brushColor = color; }

    QColor lineColor() { return m_lineColor; }
    Qt::PenStyle penStyle() { return m_penStyle; }
    qreal penWidth() { return m_penWidth; }
    QColor brushColor() { return m_brushColor; }

public:
    void AddSelection(MEntity* pEnt);
    void ClearSelections() { m_SelectEntityList.clear(); }
    QList<MEntity*> GetEntityList() const { return m_EntityList; }
};

